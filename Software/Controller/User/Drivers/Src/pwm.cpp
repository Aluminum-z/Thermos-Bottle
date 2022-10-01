/**************************************************************
 * @file    pwm.cpp
 * @brief   PWM Driver
 * @author  Alzn
 * @date    2022-03-29
 *************************************************************/

#include "pwm.hpp"

/**
 * @brief   Constructor
 * @author  Alzn
 * @date    2022-03-29
 */
Pwm::Pwm(TIM_HandleTypeDef *hTim, ChannelType channel) {
  mTimer = hTim;
  mChannel = channel;
  mDutyCycle = 0.0f;
  mIsEnable = false;
}

/**
 * @brief   Default Constructor
 * @author  Alzn
 * @date    2022-03-29
 */
Pwm::Pwm() {
  mTimer = nullptr;
  mChannel = 0;
  mDutyCycle = 0.0f;
  mIsEnable = false;
}

/**
 * @brief   Initialize timer.
 * @author  Alzn
 * @date    2022-03-29
 */
void Pwm::init(void){
  setEnable(false);
  setDutyCycle(0.0f);
}

/**
 * @brief   Set whether the timer is enabled.
 * @author  Alzn
 * @date    2022-03-29
 */
void Pwm::setEnable(bool isEnable) {
  HAL_StatusTypeDef setRev = HAL_OK;
  if (isEnable) {
    setRev = HAL_TIM_PWM_Start(mTimer, mChannel);
  } else {
    setRev = HAL_TIM_PWM_Stop(mTimer, mChannel);
  }
  if (HAL_OK == setRev) {
    mIsEnable = isEnable;
  }
}

/**
 * @brief   Set duty cycle.
 * @author  Alzn
 * @date    2022-03-29
 */
void Pwm::setDutyCycle(float dutyCycle) {
  if (dutyCycle > 1.0f) {
    dutyCycle = 1.0f;
  }
  if (dutyCycle < 0.0f) {
    dutyCycle = 0.0f;
  }
  uint32_t compareVal = (float)__HAL_TIM_GET_AUTORELOAD(mTimer) * dutyCycle;
  __HAL_TIM_SET_COMPARE(mTimer, mChannel, compareVal);
  mDutyCycle = dutyCycle;
}

/* END OF FILE */