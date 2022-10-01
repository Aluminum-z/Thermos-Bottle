/**************************************************************
 * @file    ds18b20.cpp
 * @brief   Driver for ds18b20
 * @author  Alzn
 * @date    2022-09-05
 *************************************************************/

#include "ds18b20.hpp"

/**
 * @brief   Delay in microsecond.
 * @author  Alzn
 * @date    2022-09-05
 */
void Ds18b20::delayUs(uint16_t us) {
  while (us > 0) {
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    us--;
  }
}

/**
 * @brief   Reset the bus.
 * @author  Alzn
 * @date    2022-09-05
 */
void Ds18b20::resetBus(void) {
  mGpio.write(Gpio::Set);
  mGpio.write(Gpio::Reset);
  delayUs(480);
  mGpio.write(Gpio::Set);
  delayUs(480);
}

/**
 * @brief   Send data to ds18b20.
 * @author  Alzn
 * @date    2022-09-05
 */
void Ds18b20::send(uint8_t data) {
  for (uint16_t i = 0; i < 8; i++) {
    mGpio.write(Gpio::Set);
    delayUs(2);
    mGpio.write(Gpio::Reset);
    delayUs(2);
    if ((data & 0x01) == 0x01) {
      mGpio.write(Gpio::Set);
    }
    delayUs(59);
    data >>= 1;
  }
}

/**
 * @brief   Receive data from ds18b20.
 * @author  Alzn
 * @date    2022-09-05
 */
uint8_t Ds18b20::receive(void) {
  uint8_t data = 0;
  for (uint16_t i = 0; i < 8; i++) {
    mGpio.write(Gpio::Set);
    delayUs(2);
    mGpio.write(Gpio::Reset);
    delayUs(2);
    mGpio.write(Gpio::Set);
    delayUs(15);
    data >>= 1;
    if (mGpio.read() == Gpio::Set) {
      data |= 0x80;
    }
    delayUs(45);
  }
  return data;
}

/**
 * @brief   Initialize the ds18b20 sensor.
 * @author  Alzn
 * @date    2022-09-05
 */
void Ds18b20::init(void) {
  resetBus();
  send(0xCC);
  send(0x44);
  resetBus();
}

/**
 * @brief   Read the temperature from ds18b20.
 * @author  Alzn
 * @date    2022-09-05
 */
float Ds18b20::readTemp(void) {
  resetBus();
  send(0xCC);
  send(0x44);
  resetBus();
  send(0xCC);
  send(0xBE);
  uint16_t data = 0;
  data |= receive();
  data |= (static_cast<uint16_t>(receive()) << 8);
  return static_cast<float>(data) * 0.0625f;
}

/* END OF FILE */