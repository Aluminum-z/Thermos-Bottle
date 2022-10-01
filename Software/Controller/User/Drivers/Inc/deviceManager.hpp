/**************************************************************
 * @file    deviceManager.hpp
 * @brief   Device manager.
 * @author  Alzn
 * @date    2022-04-03
 **************************************************************/
#pragma once

#include "adc.hpp"
#include "ds18b20.hpp"
#include "fan.hpp"
#include "gpio.hpp"
#include "key.hpp"
#include "lcdDriver.hpp"
#include "main.h"
#include "tec.hpp"

class DeviceManager {
private:
  Key mKeyMod;
  Key mKeyAdd;
  Key mKeySub;
  Lcd::LcdDriver mLcd;
  Adc mAdc;
  Ds18b20 mTempSenBtm;
  Ds18b20 mTempSenTop;
  Tec mTec;
  Fan mFan;

  DeviceManager(void);
  DeviceManager &operator=(const DeviceManager &) = delete;
  DeviceManager(const DeviceManager &) = delete;

public:
  static DeviceManager &getInstance(void);
  void init(void);
  Lcd::LcdDriver &getLcd(void) {
    return mLcd;
  };

  Adc &getAdc(void) {
    return mAdc;
  }

  Key &getKeyMod(void) {
    return mKeyMod;
  }
  Key &getKeyAdd(void) {
    return mKeyAdd;
  }
  Key &getKeySub(void) {
    return mKeySub;
  }

  Ds18b20 &getTempSenBtm(void) {
    return mTempSenBtm;
  }
  Ds18b20 &getTempSenTop(void) {
    return mTempSenTop;
  }

  Tec &getTec(void) {
    return mTec;
  }

  Fan &getFan(void) {
    return mFan;
  }
};

/* END OF FILE */