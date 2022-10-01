/**************************************************************
 * @file    lcdFont.hpp
 * @brief   Font library.
 * @author  Alzn
 * @date    2022-03-29
 **************************************************************/
#pragma once

#include <cstdint>

namespace Lcd {
typedef struct {
  uint16_t width;
  uint16_t height;
  uint16_t bytePerChar;
  uint16_t byte;
  uint16_t charStart;
  uint16_t charEnd;
  const uint8_t *pData;
} FontType;

namespace Font {
extern const FontType consolas16n8x14;
extern const FontType straightLine7x10;
extern const FontType straightLine12x20;
} // namespace Font
} // namespace Lcd

/* END OF FILE */