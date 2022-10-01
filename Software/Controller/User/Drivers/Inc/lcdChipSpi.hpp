/**************************************************************
 * @file    lcdChipSpi.hpp
 * @brief   Lcd chip libraries for spi interface.
 * @author  Alzn
 * @date    2022-09-04
 **************************************************************/
#pragma once

#include "gpio.hpp"
#include "lcdChip.hpp"

/* Typedef */
namespace Lcd {
typedef struct {
  Gpio chipSelectPin;
  Gpio dataCmdSelectPin;
  Gpio resetPin;
  SPI_HandleTypeDef *hSpi;
} ChipInfoSpi;

/* Class */
class LcdChipSt7735s : public LcdChipBase {
private:
  Gpio mChipSelectPin;
  Gpio mDataCmdSelectPin;
  Gpio mResetPin;
  SPI_HandleTypeDef *mSpi;

  uint16_t mOffsetX;
  uint16_t mOffsetY;

private:
  void writeCom(uint8_t cmd);
  void writeDataByte(uint8_t data);
  void writeDataWord(uint16_t data);

public:
  LcdChipSt7735s(const ChipInfoSpi &deviceInfo);

  void init(void);
  void setRange(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
  void setOrientation(DispOrientation newOrientation);
  void writeColor(uint16_t color) {
    writeDataWord(color);
  }
  void fillColor(uint16_t color, size_t num);
};
} // namespace Lcd

/* END OF FILE */