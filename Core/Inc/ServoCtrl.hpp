#include<main.h>

enum class Mode{
	dis,
	pos,
};


class ServoCtrl{
private:
	uint32_t baseID = 0x300;
	uint16_t duty[8] = {0,0,0,0,0,0,0,0};
	Mode mode = Mode::dis;
	void startPWM_(TIM_HandleTypeDef &htim, uint32_t Channel);
	void stopPWM_(TIM_HandleTypeDef &htim, uint32_t Channel);
public:
//	ServoCtrl();
	uint32_t getBaseID();
	bool setDuty(TIM_HandleTypeDef &htim ,uint8_t data[8]);
//	bool setTIM2Duty(uint8_t data[8]);
//	bool setTIM3Duty(uint8_t data[8]);
	bool setMode(uint8_t mode);
	Mode getPreMode();
	Mode getMode();
	bool reset(TIM_HandleTypeDef &htim2, TIM_HandleTypeDef &htim3);
	void startPWM(TIM_HandleTypeDef &htim2, TIM_HandleTypeDef &htim3);
	void stopPWM(TIM_HandleTypeDef &htim2, TIM_HandleTypeDef &htim3);
};
