/**************************************************************
 * @file    lcdImage.hpp
 * @brief   Lcd image library.
 * @author  Alzn
 * @date    2022-04-09
 **************************************************************/
#pragma once

#include <cstdint>

namespace Lcd {
typedef struct {
  uint16_t width;
  uint16_t height;
  const uint8_t *pData;
} ImageType;

namespace Image {
extern const ImageType letter3x5T;
extern const ImageType letter3x5E;
extern const ImageType letter3x5F;
extern const ImageType letter3x5N;
extern const ImageType snow6x6;
extern const ImageType triangle9x5;
extern const ImageType heat10x6;
}
} // namespace Lcd

/* END OF FILE */