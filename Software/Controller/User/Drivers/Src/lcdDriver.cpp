/**************************************************************
 * @file    lcdDriver.cpp
 * @brief   Lcd driver.
 * @author  Alzn
 * @date    2022-03-28
 *************************************************************/

#include "lcdDriver.hpp"

namespace Lcd {

/**
 * @brief   Constructor
 * @author  Alzn
 * @date    2022-03-28
 */
LcdDriver::LcdDriver(void) {
  mPtrChip = nullptr;
}

/**
 * @brief   Constructor with device information.
 * @author  Alzn
 * @date    2022-03-28
 * @note    This function mast be called befor initialization.
 */
LcdDriver::LcdDriver(const DeviceInfo &deviceInfo) {
  mPtrChip = deviceInfo.mPtrChip;
  mSizeX = deviceInfo.sizeX;
  mSizeY = deviceInfo.sizeY;
  mDispOrientation = deviceInfo.dispOrientation;
  mBacklightPwm = deviceInfo.backlightPwm;
}

/**
 * @brief   Initialize lcd module.
 * @author  Alzn
 * @date    2022-03-28
 */
void LcdDriver::init(void) {
  if (mBacklightPwm.isValid()) {
    mBacklightPwm.init();
    mBacklightPwm.setEnable(true);
    mBacklightPwm.setDutyCycle(0.0f);
  }
  mPtrChip->init();
  setDispOrientation(mDispOrientation);
  clear(0x0000);
  clear(0x0000); /* Make sure the LCD is updated before turning on the backlight. */
  if (mBacklightPwm.isValid()) {
    mBacklightPwm.setDutyCycle(0.5f);
  }
}

/**
 * @brief   Clear screen
 * @author  Alzn
 * @date    2022-03-28
 */
void LcdDriver::clear(uint16_t color) {
  drawBlock(0, 0, mSizeX, mSizeY, color);
}

/**
 * @brief   Set backlight in percentage.
 * @author  Alzn
 * @date    2022-03-29
 */
void LcdDriver::setBacklight(float lightPct) {
  if (mBacklightPwm.isValid()) {
    mBacklightPwm.setDutyCycle(lightPct);
  }
}

/**
 * @brief   Set backlight in percentage.
 * @author  Alzn
 * @date    2022-03-29
 * @param   newOrientation The new orientation you want to change to. @ref DispOrientation
 */
void LcdDriver::setDispOrientation(DispOrientation newOrientation) {
  mPtrChip->setOrientation(newOrientation);
  if ((mDispOrientation == DispOrientation::Horizontal || mDispOrientation == DispOrientation::HoriWithFlip)
      != (newOrientation == DispOrientation::Horizontal || newOrientation == DispOrientation::HoriWithFlip)) {
    uint32_t tempSize = mSizeX;
    mSizeX = mSizeY;
    mSizeY = tempSize;
  }
  mDispOrientation = newOrientation;
}

/**
 * @brief   Get binary image of specified character from font.
 * @author  Alzn
 * @date    2022-04-09
 */
const uint8_t *LcdDriver::getCharFontImage(char value, const FontType &font) {
  if ((value >= font.charStart) && (value <= font.charEnd)) {
    return font.pData + (value - font.charStart) * font.bytePerChar;
  }
  return nullptr;
}

/**
 * @brief   Get specified bit in binary image.
 * @author  Alzn
 * @date    2022-04-09
 * @note    This function is not effective.
 */
bool LcdDriver::getBinaryImageBit(uint16_t x, uint16_t y, uint16_t widget, uint16_t height, const uint8_t *pImage) {
  pImage += y * widget / 8;
  if ((widget % 8) != 0) {
    pImage += y;
  }
  pImage += x / 8;
  x %= 8;
  return (*pImage & (0x80 >> x)) != 0;
}

/**
 * @brief   Fill a block.
 * @author  Alzn
 * @date    2022-03-29
 */
void LcdDriver::drawBlock(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
  mPtrChip->setRange(x, x + width - 1, y, y + height - 1);
  mPtrChip->fillColor(color, width * height);
}

/**
 * @brief   Connect line.
 * @author  Alzn
 * @date    2022-03-29
 * @param   x1    Start x
 * @param   y1    Start y
 * @param   x2    End x
 * @param   y2    End y
 * @param   color
 */
void LcdDriver::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
  int32_t incX, incY, d, dAddPos, dAddNeg;
  int32_t x, y, step;
  incX = x2 - x1;
  incY = y2 - y1;
  if (incX == 0) {
    if (incY > 0) {
      drawVertLine(x1, y1, 1 + incY, color);
    } else {
      drawVertLine(x1, y1 + incY, 1 - incY, color);
    }
    return;
  }
  if (incY == 0) {
    if (incX > 0) {
      drawHoriLine(x1, y1, 1 + incX, color);
    } else {
      drawHoriLine(x1 + incX, y1, 1 - incX, color);
    }
    return;
  }
  if (incX < 0) {
    incX = -incX;
  }
  if (incY < 0) {
    incY = -incY;
  }
  if (incX > incY) {
    if (x1 > x2) {
      x1 -= incX;
      x2 += incX;
      y = y2;
      y2 = y1;
      y1 = y;
    }
    d = -incX;
    dAddPos = 2 * (incY - incX);
    dAddNeg = 2 * incY;
    step = (y1 < y2) ? 1 : -1;
    x = x1;
    y = y1;
    mPtrChip->setRange(x, mSizeX, y, y);
    for (x = x1; x <= x2; x++) {
      mPtrChip->writeColor(color);
      if (d > 0) {
        y += step;
        d += dAddPos;
        mPtrChip->setRange(x, mSizeX, y, y);
      } else {
        d += dAddNeg;
      }
    }
  } else {
    if (y1 > y2) {
      y1 -= incY;
      y2 += incY;
      x = x2;
      x2 = x1;
      x1 = x;
    }
    d = -incY;
    dAddPos = 2 * (incX - incY);
    dAddNeg = 2 * incX;
    step = (x1 < x2) ? 1 : -1;
    x = x1;
    y = y1;
    mPtrChip->setRange(x, x, y, mSizeY);
    for (y = y1; y <= y2; y++) {
      mPtrChip->writeColor(color);
      if (d > 0) {
        x += step;
        d += dAddPos;
        mPtrChip->setRange(x, x, y, mSizeY);
      } else {
        d += dAddNeg;
      }
    }
  }
}

/**
 * @brief   Draw a binary image.
 * @author  Alzn
 * @date    2022-03-29
 * @note    Image is scanned in a group of 8 bits from left(MSB) to right(LSB),
 *          and then from top to buttom.
 * @param   x       x-coordinate of the top left point.
 * @param   y       y-coordinate of the top left point.
 * @param   width
 * @param   height
 * @param   pImage
 * @param   color
 * @param   backColor
 * @param   oper Flip horizontal or vertical.
 */
void LcdDriver::drawBinaryImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *pImage, uint16_t color, uint16_t backColor, uint16_t oper) {
  const uint16_t dataBits = 8;
  uint16_t xRoop = width / dataBits;
  bool currBit = false;
  if (nullptr == pImage) {
    return;
  }
  if (width % dataBits != 0) {
    xRoop += 1;
  }
  auto drawImageLine = [&](uint16_t iY, uint16_t drawY) {
    const uint8_t *pData = pImage + iY * xRoop;
    uint16_t drawBits;
    uint16_t sameColorNum = 0;
    uint16_t data = 0;
    mPtrChip->setRange(x, x + width - 1, drawY, drawY);
    if ((oper & HoriFlip) == HoriFlip) {
      for (uint16_t iX = 0; iX < xRoop; iX++) {
        drawBits = width - (xRoop - iX - 1) * dataBits;
        data = pData[xRoop - iX - 1];
        if (drawBits > dataBits) {
          drawBits = dataBits;
        }
        if (((data == 0xFF) && currBit) || ((data == 0x00) && !currBit)) {
          sameColorNum += drawBits;
        } else {
          uint8_t mask = 0x01 << (dataBits - drawBits);
          for (uint16_t bit = 0; bit < drawBits; bit++) {
            if (currBit == ((data & mask) == mask)) {
              sameColorNum++;
            } else {
              mPtrChip->fillColor(currBit ? color : backColor, sameColorNum);
              currBit = !currBit;
              sameColorNum = 1;
            }
            mask = mask << 1;
          }
        }
      }
    } else {
      for (uint16_t iX = 0; iX < xRoop; iX++) {
        drawBits = width - iX * dataBits;
        data = pData[iX];
        if (drawBits > dataBits) {
          drawBits = dataBits;
        }
        if (((data == 0xFF) && currBit) || ((data == 0x00) && !currBit)) {
          sameColorNum += drawBits;
        } else {
          uint8_t mask = 0x80;
          for (uint16_t bit = 0; bit < drawBits; bit++) {
            if (currBit == ((data & mask) == mask)) {
              sameColorNum++;
            } else {
              mPtrChip->fillColor(currBit ? color : backColor, sameColorNum);
              currBit = !currBit;
              sameColorNum = 1;
            }
            mask = mask >> 1;
          }
        }
      }
    }
    mPtrChip->fillColor(currBit ? color : backColor, sameColorNum);
  };
  if ((oper & VertFlip) == VertFlip) {
    for (uint16_t iY = 0; iY < height; iY++) {
      drawImageLine(height - iY - 1, y + iY);
    }
  } else {
    for (uint16_t iY = 0; iY < height; iY++) {
      drawImageLine(iY, y + iY);
    }
  }
}

/**
 * @brief   Draw a image.
 * @author  Alzn
 * @date    2022-04-09
 */
void LcdDriver::drawImage(uint16_t x, uint16_t y, const ImageType &image, uint16_t color, uint16_t backColor, uint16_t oper) {
  drawBinaryImage(x, y, image.width, image.height, image.pData, color, backColor, oper);
}

/**
 * @brief   Draw a string.
 * @author  Alzn
 * @date    2022-03-29
 * @note    Font is scanned in a group of 8 bits from left to right,
 *          and then from top to buttom.
 * @param   x       x-coordinate of the top left point.
 * @param   y       y-coordinate of the top left point.
 * @param   pStr
 * @param   font
 * @param   color
 * @param   backColor
 */
void LcdDriver::drawString(uint16_t x, uint16_t y, const char *pStr, const FontType &font, uint16_t color, uint16_t backColor) {
  if (nullptr == pStr) {
    return;
  }
  uint16_t drawX = x;
  for (const char *pChar = pStr; *pChar != '\0'; pChar++) {
    const uint8_t *pFontImage = getCharFontImage(*pChar, font);
    if (nullptr != pFontImage) {
      drawBinaryImage(drawX, y, font.width, font.height, pFontImage, color, backColor);
    } else {
      drawBlock(drawX, y, font.width, font.height, backColor);
    }
    drawX += font.width;
  }
}

} // namespace Lcd

/* END OF FILE */