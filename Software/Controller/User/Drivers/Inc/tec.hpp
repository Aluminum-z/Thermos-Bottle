/**************************************************************
 * @file    tec.hpp
 * @brief   Driver for Thermoelectric coller.
 * @author  Alzn
 * @date    2022-09-15
 **************************************************************/
#pragma once

#include "comType.hpp"
#include "gpio.hpp"
#include "pwm.hpp"

class Tec {
private:
  TecMode mMode;
  Pwm mPwmPos;
  Pwm mPwmNeg;
  Gpio mIoSleep;
  float mDutyCycle;

public:
  Tec(void) {
    mMode = TecMode::BtmCoolTopHeat;
    mDutyCycle = 0.0f;
  }
  Tec(const Pwm &pwmPos, const Pwm &pwmNeg, const Gpio &ioSleep)
      : mPwmPos(pwmPos), mPwmNeg(pwmNeg), mIoSleep(ioSleep) {
    mMode = TecMode::BtmCoolTopHeat;
    mDutyCycle = 0.0f;
  }
  void init(void);
  void setMode(TecMode mode);
  void setDutyCycle(float dutyCycle);
};

/* END OF FILE */