/**************************************************************
 * @file    widgetTemperature.hpp
 * @brief   Widget for temperature.
 * @author  Alzn
 * @date    2022-09-10
 **************************************************************/
#pragma once

#include "comFunc.hpp"
#include "comType.hpp"
#include "widgetBase.hpp"
#include "widgetText.hpp"

class WidgetTemperature : public WidgetBase {
private:
  static const uint16_t mBackColor = 0x0000;
  static const uint16_t mHeatColor = ComFunc::rgb565(255, 0, 0);
  static const uint16_t mCoolColor = ComFunc::rgb565(0, 162, 232);
  static const uint16_t mMidTriColor = 0xFFFF;

  WidgetText mTextTopTemp;
  WidgetText mTextBtmTemp;

  TecMode mTecMode;

private:
  WidgetTemperature(void);
  WidgetTemperature(const WidgetTemperature &);
  WidgetTemperature &operator=(const WidgetTemperature &);

  void repaint(void);
  void drawTecImages(void);
  uint16_t getBtmColor(void);
  uint16_t getTopColor(void);
  void setTempValue(WidgetText &textWidget, float temp);

public:
  static WidgetTemperature &getInstance(void);

  void setVisible(bool isVisiable);
  void setTecMode(TecMode mode);
  void setTempTop(float temp);
  void setTempBtm(float temp);
};

/* END OF FILE */