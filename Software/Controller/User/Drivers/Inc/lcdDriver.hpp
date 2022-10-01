/**************************************************************
 * @file    lcdDriver.hpp
 * @brief   LCD Base Driver.
 * @author  Alzn
 * @date    2022-03-28
 **************************************************************/
#pragma once

/* Includes */
#include "lcdChip.hpp"
#include "lcdFont.hpp"
#include "lcdImage.hpp"
#include "main.h"

namespace Lcd {

/* Typedef */
typedef struct
{
  LcdChipBase *mPtrChip;
  uint32_t sizeX;
  uint32_t sizeY;
  DispOrientation dispOrientation;
  Pwm backlightPwm;
} DeviceInfo;

enum ImageOper{
  HoriFlip = 0x01,
  VertFlip = 0x02,
};

/* Class */
class LcdDriver {
private:
  LcdChipBase *mPtrChip;
  uint32_t mSizeX;
  uint32_t mSizeY;
  DispOrientation mDispOrientation;
  Pwm mBacklightPwm;

public:
  LcdDriver(void);
  LcdDriver(const DeviceInfo &deviceInfo);
  /* Basic */
  void init(void);
  void clear(uint16_t color);
  void setBacklight(float lightPct);
  void setDispOrientation(DispOrientation newOrientation);
  uint32_t getSizeX(void) {
    return mSizeX;
  }
  uint32_t getSizeY(void) {
    return mSizeY;
  }
  const uint8_t *getCharFontImage(char value, const FontType &font);
  bool getBinaryImageBit(uint16_t x, uint16_t y, uint16_t widget, uint16_t height, const uint8_t *pImage);
  /* Draw */
  void drawBlock(uint16_t x,
                 uint16_t y,
                 uint16_t width,
                 uint16_t height,
                 uint16_t color);
  inline void drawPoint(uint16_t x, uint16_t y, uint16_t color) {
    mPtrChip->setRange(x, x, y, y);
    mPtrChip->writeColor(color);
  }
  void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
  inline void drawHoriLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color) {
    drawBlock(x, y, width, 1, color);
  }
  inline void drawVertLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color) {
    drawBlock(x, y, 1, height, color);
  }
  void drawBinaryImage(uint16_t x,
                       uint16_t y,
                       uint16_t width,
                       uint16_t height,
                       const uint8_t *pImage,
                       uint16_t color,
                       uint16_t backColor,
                       uint16_t oper = 0x00);
  void drawImage(uint16_t x,
                 uint16_t y,
                 const ImageType &image,
                 uint16_t color,
                 uint16_t backColor,
                       uint16_t oper = 0x00);
  void drawString(uint16_t x,
                  uint16_t y,
                  const char *pStr,
                  const FontType &font,
                  uint16_t color,
                  uint16_t backColor);
};

} // namespace Lcd

/* END OF FILE */