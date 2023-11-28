#include<main.h>
#include<CanCtrl.hpp>
#include<ServoCtrl.hpp>

CanCtrl can;
ServoCtrl servo;

extern"C"{
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern CAN_HandleTypeDef hcan;

//volatile int debug_code = 0;

	void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
		uint32_t receiveID = 0x110;
		uint8_t receiveData[8] = {0,0,0,0,0,0,0,0};
		if (can.receive(*hcan ,receiveID,receiveData) == false)return;
		switch(receiveID){
			case 0x300:
				if(servo.setMode(receiveData[0]) == true){
//					debug_code = 1;
					uint8_t data[8];
					data[0] = 1;
					can.send(*hcan, 0x303, data, 1);
				}else{
					uint8_t data[8];
					data[0] = 0;
					can.send(*hcan, 0x303, data, 1);
				}
				break;
			case 0x301:
				if(servo.setDuty(htim2 ,receiveData) == true){
					uint8_t data[8];
					data[0] = 3;
					can.send(*hcan, 0x303, data, 1);
				}else{
					uint8_t data[8];
					data[0] = 2;
					can.send(*hcan, 0x303, data, 1);
				}
				break;
			case 0x302:
				if(servo.setDuty(htim3 ,receiveData) == true){
					uint8_t data[8];
					data[0] = 5;
					can.send(*hcan, 0x303, data, 1);
				}else{
					uint8_t data[8];
					data[0] = 4;
					can.send(*hcan, 0x303, data, 1);
				}
				break;
			default:
				break;
			}
//		if (solenoid.update(receiveID,receiveData) == false)return;
	}

	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
		if (!(GPIO_Pin == EMS_Pin))return;
		if (servo.reset(htim2 ,htim3) == true){
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
		can.init(hcan);



		while(true){
			servo.startPWM(htim2 ,htim3);
			servo.stopPWM(htim2 ,htim3);
			can.ledCan();
			uint16_t tick = HAL_GetTick()%2000;
			if(tick < 100) HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
			else HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		}
	}
}
