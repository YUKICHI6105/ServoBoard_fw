#include<main.h>
#include<CanCtrl.hpp>
#include<ServoCtrl.hpp>
#include<LedCtrl.hpp>

ServoCtrl servo;
CanCtrl can;

extern"C"{
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern CAN_HandleTypeDef hcan;

//volatile int debug_code = 0;
	bool setMode(uint8_t mode){
			switch(mode){
			case 0:
				servo.setMode(Mode::dis);
				return true;
			case 1:
				servo.setMode(Mode::pos);
				return true;
			default:
				return false;
			}
	}

	void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
		uint32_t receiveID = 0x110;
		uint8_t receiveData[8] = {0,0,0,0,0,0,0,0};
		if (can.receive(*hcan ,receiveID,receiveData) == false)return;
		switch(receiveID-= can.getBaseID()){
			case 0x0:
				if(setMode(receiveData[0]) == true){
//					debug_code = 1;
					uint8_t data[8];
					data[0] = 1;
					can.send(*hcan, can.getBaseID()+3, data, 1);
				}else{
					uint8_t data[8];
					data[0] = 0;
					can.send(*hcan, can.getBaseID()+3, data, 1);
				}
				break;
			case 0x1:
				if(servo.setDuty(htim2 ,receiveData) == true){
					uint8_t data[8];
					data[0] = 3;
					can.send(*hcan, can.getBaseID()+3, data, 1);
				}else{
					uint8_t data[8];
					data[0] = 2;
					can.send(*hcan, can.getBaseID()+3, data, 1);
				}
				break;
			case 0x2:
				if(servo.setDuty(htim3 ,receiveData) == true){
					uint8_t data[8];
					data[0] = 5;
					can.send(*hcan, can.getBaseID()+3, data, 1);
				}else{
					uint8_t data[8];
					data[0] = 4;
					can.send(*hcan, can.getBaseID()+3, data, 1);
				}
				break;
			default:
				if(receiveID == 0x0){
					if(receiveData[0] == 0){
						servo.reset(htim2, htim3);
					}
				}
				break;
			}
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
		led::init();
		can.init(hcan,0x300);
		uint16_t tick = 0;

		while(true){
			servo.updateEMS();
			servo.updatePWM(htim2 ,htim3);
			can.updateLed();
			led::update(tick);
		}
	}
}
