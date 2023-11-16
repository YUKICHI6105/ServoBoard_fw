/*
 *  CanCtrl.hpp
 *
 *  Created on: 2023/05/07
 *      Author: ykc
 */
#pragma once

#include <main.h>
#include <stm32f1xx_hal_can.h>
#include <stm32f1xx_hal_gpio.h>

extern "C"{
	extern CAN_HandleTypeDef hcan;
}

class CanCtrl {
private:
	uint8_t rxData[8];
	CAN_RxHeaderTypeDef rxHeader;//受信用フレーム設定
	CAN_FilterTypeDef filter;//受信時に中身を仕分けるためのパラメーター設定
	int canFlug = 0;
	uint32_t canTick = 0.0f;
public:
	CanCtrl();
	void canInit();
	bool receive(uint32_t& RID,uint8_t data[8]);//受信関数(エラー判定のみ)内容は引数に入れ込む。
	void ledCan();
};

inline CanCtrl::CanCtrl(){
	filter.FilterIdHigh         = 0x300 << 5;               // フィルターIDの上位16ビット
	filter.FilterIdLow          = 0;                        // フィルターIDの下位16ビット
	filter.FilterMaskIdHigh     = 0x7f8 << 5;               // フィルターマスクの上位16ビット
	filter.FilterMaskIdLow      = 0b110;                    // フィルターマスクの下位16ビット
	//フィルターマスクは1が確定させたいところで0が無視したいところ
	filter.FilterScale          = CAN_FILTERSCALE_32BIT;    // フィルタースケール
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;         // フィルターに割り当てるFIFO
	filter.FilterBank           = 0;                        // フィルターバンクNo
	filter.FilterMode           = CAN_FILTERMODE_IDMASK;    // フィルターモード
	filter.SlaveStartFilterBank = 14;                       // スレーブCANの開始フィルターバンクNo
	filter.FilterActivation     = ENABLE;                   // フィルター無効／有効
}

inline void CanCtrl::canInit(){
	HAL_CAN_ConfigFilter(&hcan, &filter);
}

inline bool CanCtrl::receive(uint32_t& RID,uint8_t data[8]){
	if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxHeader, data) == HAL_OK){
		RID = rxHeader.StdId;
		for(uint8_t i = 0;i<8;i++){
			rxData[i] = data[i];
		}
		canFlug =1;
		return true;
	}
	else{
		return false;
	}
}

inline void CanCtrl::ledCan(){
	if(canFlug == 1){
	HAL_GPIO_WritePin(LED_CAN_GPIO_Port,LED_CAN_Pin,GPIO_PIN_SET);
	canTick = HAL_GetTick();
	canFlug = 0;
	}
	else if(HAL_GetTick() > canTick+150){
		HAL_GPIO_WritePin(LED_CAN_GPIO_Port,LED_CAN_Pin,GPIO_PIN_RESET);
	}
}
