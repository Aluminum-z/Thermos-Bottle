/**************************************************************
 * @file    ds18b20.hpp
 * @brief   Driver for ds18b20
 * @author  Alzn
 * @date    2022-09-05
 **************************************************************/
#pragma once

#include "gpio.hpp"

class Ds18b20 {
private:
  Gpio mGpio;

  void delayUs(uint16_t us);

  void resetBus(void);
  void send(uint8_t data);
  uint8_t receive(void);

public:
  Ds18b20() {
  }
  Ds18b20(const Gpio &gpio) {
    mGpio = gpio;
  }
  void init(void);
  float readTemp(void);
};

/* END OF FILE */