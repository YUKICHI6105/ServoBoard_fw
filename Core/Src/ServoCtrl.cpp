#include<ServoCtrl.hpp>
#include<cstring>

extern"C"{
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
}

//ServoCtrl::ServoCtrl(){
//
//}

uint32_t ServoCtrl::getBaseID(){
	return baseID;
}

bool ServoCtrl::setTIM2Duty(uint8_t data[8]){
	std::memcpy(&duty[0],&data + 0,sizeof(uint16_t));
	std::memcpy(&duty[1],&data + 2,sizeof(uint16_t));
	std::memcpy(&duty[2],&data + 4,sizeof(uint16_t));
	std::memcpy(&duty[3],&data + 6,sizeof(uint16_t));
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty[0]);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, duty[1]);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, duty[2]);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, duty[3]);
	return true;
}

bool ServoCtrl::setTIM3Duty(uint8_t data[8]){
	std::memcpy(&duty[4],&data + 0,sizeof(uint16_t));
	std::memcpy(&duty[5],&data + 2,sizeof(uint16_t));
	std::memcpy(&duty[6],&data + 4,sizeof(uint16_t));
	std::memcpy(&duty[7],&data + 6,sizeof(uint16_t));
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty[4]);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, duty[5]);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, duty[6]);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, duty[7]);
	return true;
}

bool ServoCtrl::setMode(uint8_t mode){
	switch(mode){
	case 0:
		ServoCtrl::mode = Mode::dis;
	case 1:
		ServoCtrl::mode = Mode::pos;
	}
	return true;
}

Mode ServoCtrl::getMode(){
	return mode;
}

bool ServoCtrl::reset(){
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);

	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
	preMode = Mode::dis;
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,LED_YELLOW_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
	return true;
}

void ServoCtrl::startPWM(){
	if((mode == Mode::pos)&&(preMode == Mode::dis)){
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
		preMode = Mode::pos;
		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,LED_YELLOW_Pin,GPIO_PIN_SET);
	}
}

void ServoCtrl::stopPWM(){
	if(HAL_GPIO_ReadPin(EMS_GPIO_Port,EMS_Pin) == GPIO_PIN_RESET){
		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);

		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
		preMode = Mode::dis;
		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port,LED_YELLOW_Pin,GPIO_PIN_RESET);
	}
}
