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

enum CanLed{
	none,
	received,
	sended
};

class CanCtrl {
private:
	uint32_t baseID_ = 0x300;
	uint8_t rxData_[8];
	CAN_TxHeaderTypeDef txHeader_;//送信用フレーム設定
	CAN_RxHeaderTypeDef rxHeader_;//受信用フレーム設定
	CAN_FilterTypeDef filter_;//受信時に中身を仕分けるためのパラメーター設定
	CanLed canFlug_ = CanLed::none;
	uint32_t canTick_ = 0.0f;
public:
	CanCtrl(/*uint32_t baseID*/);
	void init(CAN_HandleTypeDef &hcan, uint32_t baseID);
	bool setBaseID(uint32_t baseID);
	uint32_t getBaseID();
	bool receive(CAN_HandleTypeDef &hcan ,uint32_t& RID,uint8_t (&data)[8]);//受信関数(エラー判定のみ)内容は引数に入れ込む。
	bool send(CAN_HandleTypeDef &hcan ,uint32_t TID ,uint8_t (&data)[8], uint8_t dlc);
	void updateLed();
};
//namespace Can{
//	struct CanParam{
//		CAN_TxHeaderTypeDef txHeader_;//送信用フレーム設定
//		CAN_RxHeaderTypeDef rxHeader_;//受信用フレーム設定
//		CAN_FilterTypeDef filter_;//受信時に中身を仕分けるためのパラメーター設定
//	};
//	void init(CanParam &canParam);
//	bool receive(CanParam &canParam ,uint32_t& RID,uint8_t (&data)[8], uint8_t dlc);
//	bool send(CanParam &canParam ,uint32_t TID ,uint8_t (&data)[8], uint8_t dlc);
//	void led();
//}
