#include<ServoCtrl.hpp>
#include<cstring>

//ServoCtrl::ServoCtrl(){
//
//}

uint32_t ServoCtrl::getBaseID(){
	return baseID;
}

bool ServoCtrl::setDuty(TIM_HandleTypeDef &htim, uint8_t data[8]){
	if (htim.Instance == TIM2){
		std::memcpy(&duty[0],data + 0,sizeof(uint16_t));
		std::memcpy(&duty[1],data + 2,sizeof(uint16_t));
		std::memcpy(&duty[2],data + 4,sizeof(uint16_t));
		std::memcpy(&duty[3],data + 6,sizeof(uint16_t));
		__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_1, duty[0]);
		__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_2, duty[1]);
		__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_3, duty[2]);
		__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_4, duty[3]);
		return true;
	}else if(htim.Instance == TIM3){
		std::memcpy(&duty[4],data + 0,sizeof(uint16_t));
		std::memcpy(&duty[5],data + 2,sizeof(uint16_t));
		std::memcpy(&duty[6],data + 4,sizeof(uint16_t));
		std::memcpy(&duty[7],data + 6,sizeof(uint16_t));
		__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_1, duty[4]);
		__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_2, duty[5]);
		__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_3, duty[6]);
		__HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_4, duty[7]);
		return true;
	}else{
		return false;
	}
}

//bool ServoCtrl::setTIM2Duty(uint8_t data[8]){
//	std::memcpy(&duty[0],data + 0,sizeof(uint16_t));
//	std::memcpy(&duty[1],data + 2,sizeof(uint16_t));
//	std::memcpy(&duty[2],data + 4,sizeof(uint16_t));
//	std::memcpy(&duty[3],data + 6,sizeof(uint16_t));
//	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty[0]);
//	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, duty[1]);
//	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, duty[2]);
//	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, duty[3]);
//	return true;
//}
//
//bool ServoCtrl::setTIM3Duty(uint8_t data[8]){
//	std::memcpy(&duty[4],data + 0,sizeof(uint16_t));
//	std::memcpy(&duty[5],data + 2,sizeof(uint16_t));
//	std::memcpy(&duty[6],data + 4,sizeof(uint16_t));
//	std::memcpy(&duty[7],data + 6,sizeof(uint16_t));
//	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty[4]);
//	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, duty[5]);
//	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, duty[6]);
//	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, duty[7]);
//	return true;
//}

uint8_t TIM_ERROR(/*TIM_HandleTypeDef &htim, uint32_t Channel*/){
	return 0;
}

bool ServoCtrl::setMode(uint8_t mode){
	if(HAL_GPIO_ReadPin(EMS_GPIO_Port,EMS_Pin) == GPIO_PIN_SET){
		switch(mode){
		case 0:
			ServoCtrl::mode = Mode::dis;
			break;
		case 1:
			ServoCtrl::mode = Mode::pos;
			break;
		default:
			break;
		}
		return true;
	}else{
		return false;
	}
}

Mode ServoCtrl::getMode(){
	return mode;
}

bool ServoCtrl::reset(TIM_HandleTypeDef &htim2, TIM_HandleTypeDef &htim3){
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);

	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
	mode = Mode::dis;
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,LED_YELLOW_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
	return true;
}

void ServoCtrl::startPWM_(TIM_HandleTypeDef &htim, uint32_t Channel){
	if(htim.ChannelState[Channel/4] == HAL_TIM_CHANNEL_STATE_READY){
		if(HAL_TIM_PWM_Start(&htim, Channel) == HAL_ERROR){
			TIM_ERROR(/*htim, Channel*/);
		}
	}
}

void ServoCtrl::startPWM(TIM_HandleTypeDef &htim2, TIM_HandleTypeDef &htim3){
	if((mode == Mode::pos)){
		startPWM_(htim2,TIM_CHANNEL_1);
		startPWM_(htim2,TIM_CHANNEL_2);
		startPWM_(htim2,TIM_CHANNEL_3);
		startPWM_(htim2,TIM_CHANNEL_4);

		startPWM_(htim3,TIM_CHANNEL_1);
		startPWM_(htim3,TIM_CHANNEL_2);
		startPWM_(htim3,TIM_CHANNEL_3);
		startPWM_(htim3,TIM_CHANNEL_4);

		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,LED_YELLOW_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_RESET);
	}
}

void ServoCtrl::stopPWM_(TIM_HandleTypeDef &htim, uint32_t Channel){
	if(htim.ChannelState[Channel/4] == HAL_TIM_CHANNEL_STATE_BUSY){
		if(HAL_TIM_PWM_Stop(&htim, Channel) == HAL_ERROR){
			TIM_ERROR(/*htim, Channel*/);
		}
	}
}

void ServoCtrl::stopPWM(TIM_HandleTypeDef &htim2, TIM_HandleTypeDef &htim3){
	if((HAL_GPIO_ReadPin(EMS_GPIO_Port,EMS_Pin) == GPIO_PIN_RESET)||(mode == Mode::dis)){
		if(mode != Mode::dis){
			mode = Mode::dis;
		}
		stopPWM_(htim2, TIM_CHANNEL_1);
		stopPWM_(htim2, TIM_CHANNEL_2);
		stopPWM_(htim2, TIM_CHANNEL_3);
		stopPWM_(htim2, TIM_CHANNEL_4);

		stopPWM_(htim3, TIM_CHANNEL_1);
		stopPWM_(htim3, TIM_CHANNEL_2);
		stopPWM_(htim3, TIM_CHANNEL_3);
		stopPWM_(htim3, TIM_CHANNEL_4);

		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,LED_YELLOW_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
	}
}
