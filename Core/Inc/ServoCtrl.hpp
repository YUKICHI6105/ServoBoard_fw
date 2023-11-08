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
	Mode preMode =Mode::dis;
public:
//	ServoCtrl();
	uint32_t getBaseID();
	bool setTIM2Duty(uint8_t data[8]);
	bool setTIM3Duty(uint8_t data[8]);
	bool setMode(uint8_t mode);
	Mode getPreMode();
	Mode getMode();
	bool reset();
	void startPWM();
	void stopPWM();
};
