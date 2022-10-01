/**************************************************************
 * @file    lcdChip.hpp
 * @brief   LCD Chip Manager
 * @author  Alzn
 * @date    2022-03-28
 **************************************************************/
#pragma once

#include "gpio.hpp"
#include "main.h"
#include "pwm.hpp"

namespace Lcd {
/* Typedef */
enum class DispOrientation : uint32_t {
  Horizontal,
  Vertical,
  HoriWithFlip,
  VertWithFlip,
};

/* Class */
class LcdChipBase {
public:
  /* Basic Operation */
  virtual void init(void){};
  virtual void setRange(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2){};
  virtual void setOrientation(DispOrientation newOrientation){};
  virtual void writeColor(uint16_t color){};
  virtual void fillColor(uint16_t color, size_t num){};
};

} // namespace Lcd

/* END OF FILE */