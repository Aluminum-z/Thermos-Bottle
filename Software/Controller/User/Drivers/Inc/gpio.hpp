/**************************************************************
 * @file    gpio.hpp
 * @brief   Gpio control library.
 * @author  Alzn
 * @date    2022-03-29
 **************************************************************/
#pragma once

#include "main.h"

class Gpio {
private:
  GPIO_TypeDef *mGpioX;
  uint16_t mGpioPin;

public:
  typedef enum
  {
    Reset = GPIO_PIN_RESET,
    Set = GPIO_PIN_SET,
  } GpioState;

  Gpio() {
    mGpioX = nullptr;
    mGpioPin = 0;
  };

  Gpio(GPIO_TypeDef *pGpioX, uint16_t gpioPin)
      : mGpioX(pGpioX), mGpioPin(gpioPin) {
  }

  inline void write(GpioState status) const {
    HAL_GPIO_WritePin(mGpioX, mGpioPin, static_cast<GPIO_PinState>(status));
  }
  inline GpioState read(void) const {
    return static_cast<GpioState>(HAL_GPIO_ReadPin(mGpioX, mGpioPin));
  }
  inline bool isValid(void) const {
    return (mGpioX != nullptr);
  }
};

/* END OF FILE */