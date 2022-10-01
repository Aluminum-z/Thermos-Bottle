/**************************************************************
 * @file    widgetPowerOutput.cpp
 * @brief   Widget for output power.
 * @author  Alzn
 * @date    2022-09-12
 *************************************************************/

#include "comFunc.hpp"
#include "widgetPowerOut.hpp"
#include <cstring>

/**
 * @brief   Constructor
 * @author  Alzn
 * @date    2022-09-12
 */
WidgetPowerOut::WidgetPowerOut(void) {
  setPosition(70, 4);
  setSize(47, 73);

  mBarTec.setParent(this);
  mBarTec.setPosition(7, 1);
  mBarTec.setSize(7, 60);
  mBarTec.setProcess(0);
  mBarTec.setDirection(WidgetProcessBar::Direction::DownToUp);

  mBarFan.setParent(this);
  mBarFan.setPosition(31, 1);
  mBarFan.setSize(7, 60);
  mBarFan.setProcess(0);
  mBarFan.setDirection(WidgetProcessBar::Direction::DownToUp);

  mTextTec.setParent(this);
  mTextTec.setPosition(0, 63);
  mTextTec.setSize(24, 10);
  mTextTec.setAlign(WidgetText::AlignVertCenter | WidgetText::AlignHoriCenter);
  mTextTec.setFont(&Lcd::Font::straightLine7x10);
  mTextTec.setTextStr("---");

  mTextFan.setParent(this);
  mTextFan.setPosition(24, 63);
  mTextFan.setSize(24, 10);
  mTextFan.setAlign(WidgetText::AlignVertCenter | WidgetText::AlignHoriCenter);
  mTextFan.setFont(&Lcd::Font::straightLine7x10);
  mTextFan.setTextStr("---");
}

/**
 * @brief   Repaint all of the widget.
 * @author  Alzn
 * @date    2022-09-12
 */
void WidgetPowerOut::repaint(void) {
  if (!getVisible()) {
    return;
  }
  Lcd::LcdDriver &lcd = getLcd();
  lcd.drawImage(mX + 2, mY + 56, Lcd::Image::letter3x5T, mColor, mBackColor);
  lcd.drawImage(mX + 17, mY + 56, Lcd::Image::letter3x5E, mColor, mBackColor);
  lcd.drawImage(mX + 26, mY + 56, Lcd::Image::letter3x5F, mColor, mBackColor);
  lcd.drawImage(mX + 41, mY + 56, Lcd::Image::letter3x5N, mColor, mBackColor);

  mBarFan.repaint();
  mBarTec.repaint();

  mTextFan.repaint();
  mTextTec.repaint();
}

/**
 * @brief   Set the text of the widget.
 * @author  Alzn
 * @date    2022-09-12
 */
void WidgetPowerOut::setPwrStr(WidgetText &textWidget, float pwr) {
  const size_t strBufLen = 8;
  const size_t strTextMaxLen = 4;
  char strBuf[strBufLen];
  if (pwr > 1.0f) {
    pwr = 1.0f;
  }
  if (pwr < 0.0f) {
    pwr = 0.0f;
  }
  ComFunc::fixPointPrintf(strBuf, strBufLen, 0, 0, pwr * 100.0f);
  size_t strLen = strlen(strBuf);
  if (strLen + 1 < strTextMaxLen) {
    strBuf[strLen] = '%';
    strBuf[strLen + 1] = '\0';
  }
  textWidget.setTextStr(strBuf);
  textWidget.repaintIfNeed();
}

/**
 * @brief   Get the unique instance
 * @author  Alzn
 * @date    2022-09-12
 */
WidgetPowerOut &WidgetPowerOut::getInstance(void) {
  static WidgetPowerOut instance;
  return instance;
}

/**
 * @brief   Set the visibility of this widget.
 * @author  Alzn
 * @date    2022-09-12
 */
void WidgetPowerOut::setVisible(bool isVisible) {
  mSetVisible(isVisible);
  mBarFan.setVisible(isVisible);
  mBarTec.setVisible(isVisible);
  mTextFan.setVisible(isVisible);
  mTextTec.setVisible(isVisible);
}

/**
 * @brief   Set the power percentages.
 * @author  Alzn
 * @date    2022-09-12
 */
void WidgetPowerOut::setPwrFan(float pwr) {
  mBarFan.setProcess(pwr);
  setPwrStr(mTextFan, pwr);
}
void WidgetPowerOut::setPwrTec(float pwr) {
  mBarTec.setProcess(pwr);
  setPwrStr(mTextTec, pwr);
}

/* END OF FILE */