#include <CanCtrl.hpp>

CanCtrl::CanCtrl(/*uint32_t baseID*/){
	txHeader_.RTR = CAN_RTR_DATA;            // フレームタイプはデータフレーム
	txHeader_.IDE = CAN_ID_STD;              // 標準ID(11ﾋﾞｯﾄ)
	txHeader_.DLC = 8;                       // データ長は8バイトに
	txHeader_.TransmitGlobalTime = DISABLE;  // ???
	filter_.FilterIdHigh         = 0x300/*baseID*/ << 5;              // フィルターIDの上位16ビット
	filter_.FilterIdLow          = 0;                        // フィルターIDの下位16ビット
	filter_.FilterMaskIdHigh     = 0x7f8 << 5;               // フィルターマスクの上位16ビット
	filter_.FilterMaskIdLow      = 0b110;                    // フィルターマスクの下位16ビット
	//フィルターマスクは1が確定させたいところで0が無視したいところ
	filter_.FilterScale          = CAN_FILTERSCALE_32BIT;    // フィルタースケール
	filter_.FilterFIFOAssignment = CAN_FILTER_FIFO0;         // フィルターに割り当てるFIFO
	filter_.FilterBank           = 0;                        // フィルターバンクNo
	filter_.FilterMode           = CAN_FILTERMODE_IDMASK;    // フィルターモード
	filter_.SlaveStartFilterBank = 14;                       // スレーブCANの開始フィルターバンクNo
	filter_.FilterActivation     = ENABLE;                   // フィルター無効／有効
}

void CanCtrl::init(CAN_HandleTypeDef &hcan, uint32_t baseID){
	setBaseID(baseID);
	filter_.FilterIdHigh = this->baseID_ << 5;
	HAL_CAN_ConfigFilter(&hcan, &filter_);
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

bool CanCtrl::setBaseID(uint32_t baseID){
	this->baseID_ = baseID;
	return true;
}

uint32_t CanCtrl::getBaseID(){
	return baseID_;
}

bool CanCtrl::receive(CAN_HandleTypeDef &hcan ,uint32_t& RID,uint8_t (&data)[8]){
	if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxHeader_, data) == HAL_OK){
		RID = rxHeader_.StdId;
		for(uint8_t i = 0;i<8;i++){
			rxData_[i] = data[i];
		}
		canFlug_ = CanLed::received;
		return true;
	}
	else{
		return false;
	}
}

bool CanCtrl::send(CAN_HandleTypeDef &hcan ,uint32_t TID/*送信用ID*/,uint8_t (&data)[8]/*送信内容*/,uint8_t dlc){
	txHeader_.StdId = TID;
	txHeader_.DLC = dlc;
	if(0 < HAL_CAN_GetTxMailboxesFreeLevel(&hcan)){
		uint32_t TxMailbox;//ボックス番号(ここに返ってくる)
		if (HAL_CAN_AddTxMessage(&hcan,&txHeader_,data,&TxMailbox) == HAL_OK){
			canFlug_ = CanLed::sended;
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}

void CanCtrl::updateLed(){
	if(canFlug_ == CanLed::received){
		HAL_GPIO_WritePin(LED_CAN_GPIO_Port,LED_CAN_Pin,GPIO_PIN_SET);
		canTick_ = HAL_GetTick();
		canFlug_ = CanLed::none;
	}else if(canFlug_ == CanLed::sended){
		HAL_GPIO_WritePin(LED_CAN_GPIO_Port,LED_CAN_Pin,GPIO_PIN_SET);
		canTick_ = HAL_GetTick();
		canFlug_ = CanLed::none;
	}
	else if(HAL_GetTick() > canTick_+150){
		HAL_GPIO_WritePin(LED_CAN_GPIO_Port,LED_CAN_Pin,GPIO_PIN_RESET);
	}
}
