/**************************************************************
 * @file    widgetProcessBar.hpp
 * @brief   Process bar widget.
 * @author  Alzn
 * @date    2022-09-08
 **************************************************************/
#pragma once

#include "widgetBase.hpp"

class WidgetProcessBar : public WidgetBase {
public:
  enum class Direction : uint16_t {
    LeftToRight = 0,
    RightToLeft,
    DownToUp,
    UpToDown,
  };

private:
  static const uint16_t mBarPosInc = 2;
  static const uint16_t mLengthGap = mBarPosInc * 2;

  uint16_t mBackColor;
  uint16_t mBorderColor;
  uint16_t mBarColor;

  Direction mDirection;

  float mProcess;
  uint16_t mBarLength;

private:
  void init(void) {
    mBackColor = 0x0000;
    mBorderColor = 0xFFFF;
    mBarColor = 0xFFFF;
    mDirection = Direction::LeftToRight;
    mProcess = 0.0f;
  }

  uint16_t getMaxLength(void);
  bool checkSizeIsValid(void);
  void drawBar(uint16_t start, uint16_t end, uint16_t color);
  void refresh(uint16_t newLength);

public:
  WidgetProcessBar(void) {
    init();
  }
  void repaint(void);
  void setSize(uint16_t width, uint16_t height);
  void setDirection(Direction dir);

  void setProcess(float process);
};

/* END OF FILE */