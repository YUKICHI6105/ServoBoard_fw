#include<main.h>
#include<CanCtrl.hpp>
#include<ServoCtrl.hpp>

CanCtrl can;
ServoCtrl servo;

extern"C"{
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

	void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
		uint32_t receiveID = 0x110;
		uint8_t receiveData[8];
		if (can.receive(receiveID,receiveData) == false)return;
		switch(receiveID){
			case 0x300:
				servo.setMode(receiveData[0]);
				break;
			case 0x301:
				servo.setTIM2Duty(receiveData);
				break;
			case 0x302:
				servo.setTIM3Duty(receiveData);
				break;
			default:
				break;
			}
//		if (solenoid.update(receiveID,receiveData) == false)return;
	}

	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
		if (!(GPIO_Pin == EMS_Pin))return;
		if (servo.reset() == true){
			HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
		}
		else{
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		}
	}

	void main_cpp(){
		for(uint8_t i=0;i<3;i++){
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			HAL_Delay(100);
		}
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);

		HAL_CAN_Start(&hcan);
		HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

		while(true){
			servo.startPWM();
			servo.stopPWM();
			can.ledCan();
			uint16_t tick = HAL_GetTick()%2000;
			if(tick < 100) HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
			else HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		}
	}
}
