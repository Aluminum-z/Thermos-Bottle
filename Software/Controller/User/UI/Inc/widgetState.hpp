/**************************************************************
 * @file    widgetState.hpp
 * @brief   State widgets.
 * @author  Alzn
 * @date    2022-09-12
 **************************************************************/
#pragma once

#include "widgetBase.hpp"
#include "widgetText.hpp"

class WidgetState : public WidgetBase {
public:
  enum FlashWidget {
    FlashModeText,
  };

private:
  WidgetText mTextSwitch;
  WidgetText mTextMode;
  WidgetText mTextCfgVal;
  WidgetText mTextVin;
  WidgetText mTextPwr;

  bool mFlashState;
  uint16_t mModeTextColor;

private:
  WidgetState(void);
  WidgetState(const WidgetState &);
  WidgetState &operator=(const WidgetState &);

  void setStr(WidgetText &textWidget, float value, uint16_t decNum, char unit);

public:
  static WidgetState &getInstance(void);

  void flash(bool flashState, uint16_t cfgVal);

  void setVisible(bool isVisiable);
  void setIsEnable(bool isEnabled);
  void setVin(float vin);
  void setPwr(float pwr);
  void setWorkMode(WorkMode mode);
  void setCtrlValue(float value);
};

/* END OF FILE */