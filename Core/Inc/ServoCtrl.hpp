#include<main.h>

enum class Mode{
	dis,
	pos,
};


class ServoCtrl{
private:
	Mode mode = Mode::dis;
	uint16_t duty[8] = {0,0,0,0,0,0,0,0};
	void _startPWM(TIM_HandleTypeDef &htim, uint32_t Channel);
	void _stopPWM(TIM_HandleTypeDef &htim, uint32_t Channel);
public:
//	ServoCtrl();
	bool setMode(uint8_t mode);
	Mode getMode();
	bool setDuty(TIM_HandleTypeDef &htim ,uint8_t data[8]);
	uint8_t& getDuty(TIM_HandleTypeDef &htim);
	bool reset(TIM_HandleTypeDef &htim2, TIM_HandleTypeDef &htim3);
	void updateEMS();
	void updatePWM(TIM_HandleTypeDef &htim2, TIM_HandleTypeDef &htim3);
};
