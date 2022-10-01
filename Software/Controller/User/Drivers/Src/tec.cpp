/**************************************************************
 * @file    tec.cpp
 * @brief   Driver for thermoelectric cooler.
 * @author  Alzn
 * @date    2022-09-15
 *************************************************************/

#include "tec.hpp"

/**
 * @brief   Initialize the thermoelectric cooler.
 * @author  Alzn
 * @date    2022-09-15
 */
void Tec::init(void){
  mIoSleep.write(Gpio::Reset);
  mPwmPos.init();
  mPwmNeg.init();
  mPwmPos.setDutyCycle(1.0f);
  mPwmNeg.setDutyCycle(1.0f);
}

/**
 * @brief   Set the mode of thermoelectric cooler.
 * @author  Alzn
 * @date    2022-09-15
 */
void Tec::setMode(TecMode mode) {
  mMode = mode;
  if (mode == TecMode::BtmCoolTopHeat) {
    mPwmPos.setDutyCycle(1.0f);
    mPwmNeg.setDutyCycle(1.0f - mDutyCycle);
  } else {
    mPwmPos.setDutyCycle(1.0f - mDutyCycle);
    mPwmNeg.setDutyCycle(1.0f);
  }
}

/**
 * @brief   Set the duty cycle of the output current.
 * @author  Alzn
 * @date    2022-09-15
 */
void Tec::setDutyCycle(float dutyCycle) {
  if (mDutyCycle == dutyCycle) {
    return;
  }
  mDutyCycle = dutyCycle;
  if (mMode == TecMode::BtmCoolTopHeat) {
    mPwmNeg.setDutyCycle(1.0 - mDutyCycle);
  } else {
    mPwmPos.setDutyCycle(1.0 - mDutyCycle);
  }
  if (dutyCycle <= 0.0f) {
    mIoSleep.write(Gpio::Reset);
    mPwmNeg.setEnable(false);
    mPwmPos.setEnable(false);
  } else {
    mPwmNeg.setEnable(true);
    mPwmPos.setEnable(true);
    mIoSleep.write(Gpio::Set);
  }
}

/* END OF FILE */