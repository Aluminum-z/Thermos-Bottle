/**************************************************************
 * @file    fan.cpp
 * @brief   Fan driver.
 * @author  Alzn
 * @date    2022-09-18
 *************************************************************/

#include "fan.hpp"

/**
 * @brief   Initializate the fan.
 * @author  Alzn
 * @date    2022-09-18
 */
void Fan::init(void) {
  mPwm.setDutyCycle(0.99f);
  mPwm.setEnable(true);
  mIoSleep.write(Gpio::Reset);
}

/**
 * @brief   Set the power of the fan.
 * @author  Alzn
 * @date    2022-09-18
 */
void Fan::setDutyCycle(float dutyCycle) {
  if (dutyCycle < 0.001f) {
    mIoSleep.write(Gpio::Reset);
    mPwm.setDutyCycle(0.99f);
    return;
  }
  mPwm.setDutyCycle(1.0f - dutyCycle);
  mIoSleep.write(Gpio::Set);
}

/* END OF FILE */