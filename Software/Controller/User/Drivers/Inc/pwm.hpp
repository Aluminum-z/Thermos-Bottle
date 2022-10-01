/**************************************************************
 * @file    pwm.hpp
 * @brief   PWM Driver.
 * @author  Alzn
 * @date    2022-03-29
 **************************************************************/
#pragma once

#include "main.h"

class Pwm {
private:
  TIM_HandleTypeDef *mTimer;
  uint16_t mChannel;
  float mDutyCycle;
  bool mIsEnable;

public:
  typedef enum
  {
    Channel1 = TIM_CHANNEL_1,
    Channel2 = TIM_CHANNEL_2,
    Channel3 = TIM_CHANNEL_3,
    Channel4 = TIM_CHANNEL_4,
  } ChannelType;

  Pwm();
  Pwm(TIM_HandleTypeDef *hTim, ChannelType channel);

  void init(void);
  void setEnable(bool isEnable);
  void setDutyCycle(float dutyCycle);

  bool isValid(void) {
    return (nullptr != mTimer);
  }
  bool isEnable(void) {
    return mIsEnable;
  }
  float getDutyCycle(void) {
    return mDutyCycle;
  }
};

/* END OF FILE */