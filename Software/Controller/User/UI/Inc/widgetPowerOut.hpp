/**************************************************************
 * @file    widgetPowerOut.hpp
 * @brief   Widget for output power.
 * @author  Alzn
 * @date    2022-09-12
 **************************************************************/
#pragma once

#include "widgetBase.hpp"
#include "widgetProcessBar.hpp"
#include "widgetText.hpp"

class WidgetPowerOut : public WidgetBase {
private:
  const uint16_t mBackColor = 0x0000;
  const uint16_t mColor = 0xFFFF;

  WidgetProcessBar mBarFan;
  WidgetProcessBar mBarTec;

  WidgetText mTextFan;
  WidgetText mTextTec;
private:
  WidgetPowerOut(void);
  WidgetPowerOut(const WidgetPowerOut &);
  WidgetPowerOut &operator=(const WidgetPowerOut &);

  void repaint(void);
  void setPwrStr(WidgetText &textWidget, float pwr);

public:
  static WidgetPowerOut &getInstance(void);

  void setVisible(bool isVisible);
  void setPwrFan(float pwr);
  void setPwrTec(float pwr);
};

/* END OF FILE */