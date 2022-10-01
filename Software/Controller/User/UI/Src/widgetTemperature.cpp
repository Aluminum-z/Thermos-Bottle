/**************************************************************
 * @file    widgetTemperature.cpp
 * @brief   Widget for temperature display.
 * @author  Alzn
 * @date    2022-09-10
 *************************************************************/

#include "widgetTemperature.hpp"
#include <cstring>

/**
 * @brief   Constructor
 * @author  Alzn
 * @date    2022-09-10
 */
WidgetTemperature::WidgetTemperature(void) {
  setPosition(0, 0);
  setSize(69, 80);

  mTecMode = TecMode::BtmCoolTopHeat;

  mTextTopTemp.setParent(this);
  mTextTopTemp.setPosition(0, 4);
  mTextTopTemp.setSize(mWidth, 20);
  mTextTopTemp.setAlign(WidgetText::AlignHoriCenter | WidgetText::AlignVertCenter);
  mTextTopTemp.setFont(&Lcd::Font::straightLine12x20);
  mTextTopTemp.setTextStr("----");
  mTextTopTemp.setTextColor(getTopColor());

  mTextBtmTemp.setParent(this);
  mTextBtmTemp.setPosition(0, 56);
  mTextBtmTemp.setSize(mWidth, 20);
  mTextBtmTemp.setAlign(WidgetText::AlignHoriCenter | WidgetText::AlignVertCenter);
  mTextBtmTemp.setFont(&Lcd::Font::straightLine12x20);
  mTextBtmTemp.setTextStr("----");
  mTextBtmTemp.setTextColor(getBtmColor());
}

/**
 * @brief   Repaint all of the widget.
 * @author  Alzn
 * @date    2022-09-11
 */
void WidgetTemperature::repaint(void) {
  if (!getVisible()) {
    return;
  }
  Lcd::LcdDriver &lcd = getLcd();
  lcd.drawBlock(mX, mY, mWidth, mHeight, mBackColor);
  mTextTopTemp.repaint();
  mTextBtmTemp.repaint();
  drawTecImages();
}

/**
 * @brief   Repaint the image of the thermoelectric coller.
 * @author  Alzn
 * @date    2022-09-11
 */
void WidgetTemperature::drawTecImages(void) {
  uint16_t imageX = mX + 1;
  uint16_t imageY = mY + 25;
  const uint16_t imageW = 67;
  const uint16_t imageH = 27;
  Lcd::LcdDriver &lcd = getLcd();
  lcd.drawBlock(imageX, imageY, imageW, imageH, mBackColor);

  uint16_t x = imageX;
  uint16_t y = imageY;
  uint16_t imageOper = 0;
  /* Heat */
  if (mTecMode == TecMode::BtmCoolTopHeat) {
    y = imageY + 3;
    imageOper = 0;
  } else {
    y = imageY + 20;
    imageOper = Lcd::ImageOper::VertFlip;
  }
  for (uint16_t i = 0; i < 6; i++) {
    x = imageX + 1 + i * 11;
    lcd.drawImage(x, y, Lcd::Image::heat10x6, mHeatColor, mBackColor, imageOper);
  }
  /* Cool */
  if (mTecMode == TecMode::BtmCoolTopHeat) {
    y = imageY + 18;
  } else {
    y = imageY + 3;
  }
  for (uint16_t i = 0; i < 4; i++) {
    x = imageX + 2 + i * 19;
    lcd.drawImage(x, y, Lcd::Image::snow6x6, mCoolColor, mBackColor);
  }
  for (uint16_t i = 0; i < 3; i++) {
    x = imageX + 11 + i * 19;
    lcd.drawImage(x, y + 2, Lcd::Image::snow6x6, mCoolColor, mBackColor);
  }
  /* Tec */
  if (mTecMode == TecMode::BtmCoolTopHeat) {
    y = imageY + 9;
  } else {
    y = imageY + 11;
  }
  lcd.drawBlock(imageX, y, imageW, 2, getTopColor());
  lcd.drawBlock(imageX, y + 7, imageW, 2, getBtmColor());
  imageOper = 0;
  if (mTecMode == TecMode::BtmCoolTopHeat) {
    y = imageY + 11;
  } else {
    imageOper = Lcd::ImageOper::VertFlip;
    y = imageY + 13;
  }
  for (uint16_t i = 0; i < 6; i++) {
    x = imageX + 1 + i * 11;
    lcd.drawImage(x, y, Lcd::Image::triangle9x5, mMidTriColor, mBackColor, imageOper);
  }
}

/**
 * @brief   Get the color at the buttom or top.
 * @author  Alzn
 * @date    2022-09-11
 */
uint16_t WidgetTemperature::getBtmColor(void) {
  if (mTecMode == TecMode::BtmCoolTopHeat) {
    return mCoolColor;
  }
  return mHeatColor;
}
uint16_t WidgetTemperature::getTopColor(void) {
  if (mTecMode == TecMode::BtmCoolTopHeat) {
    return mHeatColor;
  }
  return mCoolColor;
}

/**
 * @brief   Set the string of temperature to widget.
 * @author  Alzn
 * @date    2022-09-11
 */
void WidgetTemperature::setTempValue(WidgetText &textWidget, float temp) {
  const size_t strBufLen = 8;
  char strBuf[strBufLen];
  uint16_t decNum = 1;
  if ((temp > 100.0f) || (temp < -9.9f)) {
    decNum = 0;
  }
  ComFunc::fixPointPrintf(strBuf, strBufLen, 0, decNum, temp);
  size_t strLen = strlen(strBuf);
  if (strLen + 1 < strBufLen) {
    strBuf[strLen] = '/'; /* '\' = '℃' */
    strBuf[strLen + 1] = '\0';
  }
  textWidget.setTextStr(strBuf);
  textWidget.setTextColor([&]() -> uint16_t {
    const float minTemp = 0.0f;
    const float midTemp = 20.0f;
    const float maxTemp = 80.0f;
    if (temp < minTemp) {
      return mCoolColor;
    } else if (temp < midTemp) {
      uint8_t g = 162 + (temp - minTemp) * ((1.0f - 0.6353f) * 255.0f / (midTemp - minTemp));
      uint8_t b = 232 + (temp - minTemp) * ((1.0f - 0.9098f) * 255.0f / (midTemp - minTemp));
      return ComFunc::rgb565(0, g, b);
    } else if (temp < maxTemp) {
      int16_t r = (temp - midTemp) * (255.0f * 3.0f / (maxTemp - midTemp));
      if (r > 255) {
        r = 255;
      }
      int16_t g = 255 - (temp - midTemp) * (255.0f * 1.5f / (maxTemp - midTemp));
      if (g < 0) {
        g = 0;
      }
      int16_t b = 255 - (temp - midTemp) * (255.0f * 3.0f / (maxTemp - midTemp));
      if (b < 0) {
        b = 0;
      }
      return ComFunc::rgb565(r, g, b);
    }
    return mHeatColor;
  }());
  textWidget.repaintIfNeed();
}

/**
 * @brief   Get the unique instance for temperature widget.
 * @author  Alzn
 * @date    2022-09-10
 */
WidgetTemperature &WidgetTemperature::getInstance(void) {
  static WidgetTemperature instance;
  return instance;
}

/**
 * @brief   Set the visiable of the temperature widget.
 * @author  Alzn
 * @date    2022-09-11
 */
void WidgetTemperature::setVisible(bool isVisiable) {
  mSetVisible(isVisiable);
  mTextTopTemp.setVisible(isVisiable);
  mTextBtmTemp.setVisible(isVisiable);
}

/**
 * @brief   Set the mode of thermoelectric coller.
 * @author  Alzn
 * @date    2022-09-11
 */
void WidgetTemperature::setTecMode(TecMode mode) {
  if (mTecMode == mode) {
    return;
  }
  mTecMode = mode;
  drawTecImages();
}

/**
 * @brief   Set the temperatures.
 * @author  Alzn
 * @date    2022-09-11
 */
void WidgetTemperature::setTempTop(float temp) {
  setTempValue(mTextTopTemp, temp);
}
void WidgetTemperature::setTempBtm(float temp) {
  setTempValue(mTextBtmTemp, temp);
}

/* END OF FILE */