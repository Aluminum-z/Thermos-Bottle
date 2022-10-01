/**************************************************************
 * @file    lcdChipSpi.cpp
 * @brief   Lcd chip libraries for SPI interface.
 * @author  Alzn
 * @date    2022-09-04
 *************************************************************/

#include "lcdChipSpi.hpp"

namespace Lcd {
const uint32_t gSpiTimeOut = 32;

/**
 * @brief   Constructor for ST7735 (LCD Module: ZJY096T)
 * @author  Alzn
 * @date    2022-09-04
 */
LcdChipSt7735s::LcdChipSt7735s(const ChipInfoSpi &deviceInfo) {
  mChipSelectPin = deviceInfo.chipSelectPin;
  mDataCmdSelectPin = deviceInfo.dataCmdSelectPin;
  mResetPin = deviceInfo.resetPin;
  mSpi = deviceInfo.hSpi;
  mOffsetX = 0;
  mOffsetY = 0;
}

/**
 * @brief   Write command to lcd.
 * @author  Alzn
 * @date    2022-09-04
 */
void LcdChipSt7735s::writeCom(uint8_t cmd) {
  mDataCmdSelectPin.write(Gpio::Reset);
  mChipSelectPin.write(Gpio::Reset);
  HAL_SPI_Transmit(mSpi, &cmd, 1, gSpiTimeOut);
  mChipSelectPin.write(Gpio::Set);
}

/**
 * @brief   Write data to lcd.
 * @author  Alzn
 * @date    2022-09-04
 */
void LcdChipSt7735s::writeDataByte(uint8_t data) {
  mDataCmdSelectPin.write(Gpio::Set);
  mChipSelectPin.write(Gpio::Reset);
  HAL_SPI_Transmit(mSpi, &data, 1, gSpiTimeOut);
  mChipSelectPin.write(Gpio::Set);
}

/**
 * @brief   Write data to lcd.
 * @author  Alzn
 * @date    2022-09-04
 */
void LcdChipSt7735s::writeDataWord(uint16_t data) {
  uint8_t dataBuf[2];
  dataBuf[0] = (data >> 8) & 0xFF;
  dataBuf[1] = data & 0xFF;
  mDataCmdSelectPin.write(Gpio::Set);
  mChipSelectPin.write(Gpio::Reset);
  HAL_SPI_Transmit(mSpi, dataBuf, 2, gSpiTimeOut);
  mChipSelectPin.write(Gpio::Set);
}

/**
 * @brief   Fast write color data.
 * @author  Alzn
 * @date    2022-09-04
 */
void LcdChipSt7735s::fillColor(uint16_t color, size_t num) {
  const uint16_t bufColorNum = 8;
  uint8_t dataBuf[bufColorNum * 2];
  dataBuf[0] = (color >> 8) & 0xFF;
  dataBuf[1] = color & 0xFF;
  uint16_t writeNum = (num < bufColorNum) ? num : bufColorNum;
  for (uint16_t i = 1; i < writeNum; i++) {
    dataBuf[i * 2] = dataBuf[0];
    dataBuf[i * 2 + 1] = dataBuf[1];
  }
  mDataCmdSelectPin.write(Gpio::Set);
  mChipSelectPin.write(Gpio::Reset);
  while (num > 0) {
    writeNum = (num < bufColorNum) ? num : bufColorNum;
    HAL_SPI_Transmit(mSpi, dataBuf, writeNum * 2, gSpiTimeOut);
    num -= writeNum;
  }
  mChipSelectPin.write(Gpio::Set);
}

/**
 * @brief   Initialize the lcd
 * @author  Alzn
 * @date    2022-09-04
 */
void LcdChipSt7735s::init(void) {
  if (mResetPin.isValid()) {
    mResetPin.write(Gpio::Reset);
    HAL_Delay(1);
    mResetPin.write(Gpio::Set);
  }
  HAL_Delay(50);

  writeCom(0x11);
  HAL_Delay(120);
  writeCom(0xB1);
  writeDataByte(0x05);
  writeDataByte(0x3C);
  writeDataByte(0x3C);
  writeCom(0xB2);
  writeDataByte(0x05);
  writeDataByte(0x3C);
  writeDataByte(0x3C);
  writeCom(0xB3);
  writeDataByte(0x05);
  writeDataByte(0x3C);
  writeDataByte(0x3C);
  writeDataByte(0x05);
  writeDataByte(0x3C);
  writeDataByte(0x3C);
  writeCom(0xB4);
  writeDataByte(0x03);
  writeCom(0xC0);
  writeDataByte(0xAB);
  writeDataByte(0x0B);
  writeDataByte(0x04);
  writeCom(0xC1);
  writeDataByte(0xC5);
  writeCom(0xC2);
  writeDataByte(0x0D);
  writeDataByte(0x00);
  writeCom(0xC3);
  writeDataByte(0x8D);
  writeDataByte(0x6A);
  writeCom(0xC4);
  writeDataByte(0x8D);
  writeDataByte(0xEE);
  writeCom(0xC5);
  writeDataByte(0x0F);
  writeCom(0xE0);
  writeDataByte(0x07);
  writeDataByte(0x0E);
  writeDataByte(0x08);
  writeDataByte(0x07);
  writeDataByte(0x10);
  writeDataByte(0x07);
  writeDataByte(0x02);
  writeDataByte(0x07);
  writeDataByte(0x09);
  writeDataByte(0x0F);
  writeDataByte(0x25);
  writeDataByte(0x36);
  writeDataByte(0x00);
  writeDataByte(0x08);
  writeDataByte(0x04);
  writeDataByte(0x10);
  writeCom(0xE1);
  writeDataByte(0x0A);
  writeDataByte(0x0D);
  writeDataByte(0x08);
  writeDataByte(0x07);
  writeDataByte(0x0F);
  writeDataByte(0x07);
  writeDataByte(0x02);
  writeDataByte(0x07);
  writeDataByte(0x09);
  writeDataByte(0x0F);
  writeDataByte(0x25);
  writeDataByte(0x35);
  writeDataByte(0x00);
  writeDataByte(0x09);
  writeDataByte(0x04);
  writeDataByte(0x10);

  writeCom(0xFC);
  writeDataByte(0x80);

  writeCom(0x3A);
  writeDataByte(0x05);
  writeCom(0x36);
  writeDataByte(0x08);
  writeCom(0x21);
  writeCom(0x29);
  writeCom(0x2A);
  writeDataByte(0x00);
  writeDataByte(0x1A);
  writeDataByte(0x00);
  writeDataByte(0x69);
  writeCom(0x2B);
  writeDataByte(0x00);
  writeDataByte(0x01);
  writeDataByte(0x00);
  writeDataByte(0xA0);
  writeCom(0x2C);
}

/**
 * @brief   Set the range for writting color.
 * @author  Alzn
 * @date    2022-09-04
 */
void LcdChipSt7735s::setRange(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2) {
  writeCom(0x2A);
  writeDataWord(x1 + mOffsetX);
  writeDataWord(x2 + mOffsetX);
  writeCom(0x2B);
  writeDataWord(y1 + mOffsetY);
  writeDataWord(y2 + mOffsetY);
  writeCom(0x2C);
}

/**
 * @brief   Set the orientation for display.
 * @author  Alzn
 * @date    2022-09-04
 */
void LcdChipSt7735s::setOrientation(DispOrientation newOrientation) {
  writeCom(0x36);
  switch (newOrientation) {
    case DispOrientation::Horizontal:
      writeDataByte(0x78);
      mOffsetX = 1;
      mOffsetY = 26;
      break;

    case DispOrientation::Vertical:
      writeDataByte(0x08);
      mOffsetX = 26;
      mOffsetY = 1;
      break;

    case DispOrientation::HoriWithFlip:
      writeDataByte(0xA8);
      mOffsetX = 1;
      mOffsetY = 26;
      break;

    case DispOrientation::VertWithFlip:
      writeDataByte(0xC8);
      mOffsetX = 26;
      mOffsetY = 1;
      break;

    default:
      writeDataByte(0x78);
      mOffsetX = 1;
      mOffsetY = 26;
      break;
  }
}
} // namespace Lcd

/* END OF FILE */