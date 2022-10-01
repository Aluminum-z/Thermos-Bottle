/**************************************************************
 * @file    widgetState.cpp
 * @brief   State widgets.
 * @author  Alzn
 * @date    2022-09-12
 *************************************************************/

#include "widgetState.hpp"
#include "comFunc.hpp"
#include <cstring>

/**
 * @brief   Constructor
 * @author  Alzn
 * @date    2022-09-12
 */
WidgetState::WidgetState(void) {
  setPosition(120, 3);
  setSize(40, 80 - 3);

  mTextSwitch.setParent(this);
  mTextMode.setParent(this);
  mTextCfgVal.setParent(this);
  mTextVin.setParent(this);
  mTextPwr.setParent(this);

  uint16_t textH = mHeight / 5;
  mTextSwitch.setPosition(0, 0);
  mTextMode.setPosition(0, textH);
  mTextCfgVal.setPosition(0, textH * 2);
  mTextVin.setPosition(0, textH * 3);
  mTextPwr.setPosition(0, textH * 4);

  mTextSwitch.setSize(mWidth, textH);
  mTextMode.setSize(mWidth, textH);
  mTextCfgVal.setSize(mWidth, textH);
  mTextVin.setSize(mWidth, textH);
  mTextPwr.setSize(mWidth, textH);

  mTextSwitch.setAlign(WidgetText::AlignHoriCenter | WidgetText::AlignVertCenter);
  mTextMode.setAlign(WidgetText::AlignHoriCenter | WidgetText::AlignVertCenter);
  mTextCfgVal.setAlign(WidgetText::AlignHoriCenter | WidgetText::AlignVertCenter);
  mTextVin.setAlign(WidgetText::AlignHoriCenter | WidgetText::AlignVertCenter);
  mTextPwr.setAlign(WidgetText::AlignHoriCenter | WidgetText::AlignVertCenter);

  mTextSwitch.setFont(&Lcd::Font::straightLine7x10);
  mTextMode.setFont(&Lcd::Font::straightLine7x10);
  mTextCfgVal.setFont(&Lcd::Font::straightLine7x10);
  mTextVin.setFont(&Lcd::Font::straightLine7x10);
  mTextPwr.setFont(&Lcd::Font::straightLine7x10);

  mTextSwitch.setTextColor(ComFunc::rgb565(255, 0, 0));

  mTextSwitch.setTextStr("OFF");
  mTextMode.setTextStr("STAT");
  mTextCfgVal.setTextStr("");
  mTextVin.setTextStr("---");
  mTextPwr.setTextStr("---");

  mFlashState = false;
  mModeTextColor = 0xFFFF;
}

/**
 * @brief   Get the unique instance.
 * @author  Alzn
 * @date    2022-09-12
 */
WidgetState &WidgetState::getInstance(void) {
  static WidgetState instance;
  return instance;
}

/**
 * @brief   Flash the widget.
 * @author  Alzn
 * @date    2022-09-18
 */
void WidgetState::flash(bool flashState, uint16_t cfgVal) {
  switch (cfgVal) {
    case FlashModeText:
      if (flashState) {
        mTextMode.setTextColor(0x0000);
        mTextMode.setBackColor(0xFFFF);
        mTextMode.repaintIfNeed();
      } else {
        mTextMode.setTextColor(mModeTextColor);
        mTextMode.setBackColor(0x0000);
        mTextMode.repaintIfNeed();
      }
      break;

    default:
      break;
  }
}

/**
 * @brief   Set the visiable of the temperature widget.
 * @author  Alzn
 * @date    2022-09-11
 */
void WidgetState::setVisible(bool isVisiable) {
  mSetVisible(isVisiable);
  mTextSwitch.setVisible(isVisiable);
  mTextMode.setVisible(isVisiable);
  mTextCfgVal.setVisible(isVisiable);
  mTextVin.setVisible(isVisiable);
  mTextPwr.setVisible(isVisiable);
}

/**
 * @brief   Set the display value to widget.
 * @author  Alzn
 * @date    2022-09-11
 */
void WidgetState::setStr(WidgetText &textWidget, float value, uint16_t decNum, char unit) {
  const size_t strBufLen = 8;
  char strBuf[strBufLen];
  ComFunc::fixPointPrintf(strBuf, strBufLen, 0, decNum, value);
  size_t strLen = strlen(strBuf);
  if (strLen + 1 < strBufLen) {
    strBuf[strLen] = unit;
    strBuf[strLen + 1] = '\0';
  }
  textWidget.setTextStr(strBuf);
  textWidget.repaintIfNeed();
}

/**
 * @brief   Set the power is enabled.
 * @author  Alzn
 * @date    2022-09-15
 */
void WidgetState::setIsEnable(bool isEnabled) {
  if (isEnabled) {
    mTextSwitch.setTextStr("ON");
    mTextSwitch.setTextColor(ComFunc::rgb565(0, 255, 0));
  } else {
    mTextSwitch.setTextStr("OFF");
    mTextSwitch.setTextColor(ComFunc::rgb565(255, 0, 0));
  }
  mTextSwitch.repaintIfNeed();
}

/**
 * @brief   Set the input voltage.
 * @author  Alzn
 * @date    2022-09-15
 */
void WidgetState::setVin(float vin) {
  setStr(mTextVin, vin, 1, 'v');
}

/**
 * @brief   Set the power of tec.
 * @author  Alzn
 * @date    2022-09-15
 */
void WidgetState::setPwr(float pwr) {
  setStr(mTextPwr, pwr, 1, 'w');
}

/**
 * @brief   Set the work mode.
 * @author  Alzn
 * @date    2022-09-18
 */
void WidgetState::setWorkMode(WorkMode mode) {
  auto setModeTextColor = [&](uint16_t color) {
    mModeTextColor = color;
    mTextMode.setTextColor(color);
  };
  switch (mode) {
    case WorkMode::Thermostat:
      mTextMode.setTextStr("STAT");
      setModeTextColor(0xFFFF);
      mTextCfgVal.setVisible(true);
      break;

    case WorkMode::MaxPwrCool:
      mTextMode.setTextStr("MXC");
      setModeTextColor(ComFunc::rgb565(0, 162, 232));
      mTextCfgVal.setTextStr("");
      mTextCfgVal.repaintIfNeed();
      mTextCfgVal.setVisible(false);
      break;

    case WorkMode::MaxPwrHeat:
      mTextMode.setTextStr("MXH");
      setModeTextColor(ComFunc::rgb565(255, 0, 0));
      mTextCfgVal.setTextStr("");
      mTextCfgVal.repaintIfNeed();
      mTextCfgVal.setVisible(false);
      break;

    default:
      break;
  }
  mTextMode.repaintIfNeed();
}

/**
 * @brief   Set the value set by user.
 * @author  Alzn
 * @date    2022-09-18
 */
void WidgetState::setCtrlValue(float value) {
  setStr(mTextCfgVal, value, 0, 32);
}

/* END OF FILE */