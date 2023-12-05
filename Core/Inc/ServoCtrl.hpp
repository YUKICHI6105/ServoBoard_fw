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
	bool setBaseID(uint32_t baseID);
	uint32_t getBaseID();
	bool setDuty(TIM_HandleTypeDef &htim ,uint8_t data[8]);
	uint8_t& getDuty(TIM_HandleTypeDef &htim);
	bool setMode(uint8_t mode);
	Mode getMode();
	bool reset(TIM_HandleTypeDef &htim2, TIM_HandleTypeDef &htim3);
	void updateEMS();
	void updatePWM(TIM_HandleTypeDef &htim2, TIM_HandleTypeDef &htim3);
};
