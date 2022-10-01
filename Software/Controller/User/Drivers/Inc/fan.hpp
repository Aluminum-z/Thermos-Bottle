/**************************************************************
 * @file    fan.hpp
 * @brief   Driver for fan.
 * @author  Alzn
 * @date    2022-09-18
 **************************************************************/
#pragma once

#include "gpio.hpp"
#include "pwm.hpp"

class Fan {
private:
  Pwm mPwm;
  Gpio mIoSleep;

public:
  Fan(void) {
  }
  Fan(const Pwm &pwm, const Gpio &ioSleep)
      : mPwm(pwm), mIoSleep(ioSleep) {
  }
  void init(void);
  void setDutyCycle(float dutyCycle);
};

/* END OF FILE */