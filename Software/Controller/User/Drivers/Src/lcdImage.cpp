/**************************************************************
 * @file    lcdImage.cpp
 * @brief   Lcd image library.
 * @author  Alzn
 * @date    2022-04-09
 *************************************************************/
#include "lcdImage.hpp"

namespace Lcd {
namespace Image {
static const uint8_t imageLetter3x5T[] = {
  0xE0, 0x40, 0x40, 0x40, 0x40
};

const ImageType letter3x5T = {
  .width = 3,
  .height = 5,
  .pData = imageLetter3x5T,
};

static const uint8_t imageLetter3x5E[] = {
  0xE0, 0x80, 0xE0, 0x80, 0xE0
};

const ImageType letter3x5E = {
  .width = 3,
  .height = 5,
  .pData = imageLetter3x5E,
};

static const uint8_t imageLetter3x5F[] = {
  0xE0, 0x80, 0xE0, 0x80, 0x80
};

const ImageType letter3x5F = {
  .width = 3,
  .height = 5,
  .pData = imageLetter3x5F,
};

static const uint8_t imageLetter3x5N[] = {
  0xA0, 0xE0, 0xE0, 0xE0, 0xA0
};

const ImageType letter3x5N = {
  .width = 3,
  .height = 5,
  .pData = imageLetter3x5N,
};

static const uint8_t imageSnow6x6[] = {
  0xB4, 0x48, 0xB4, 0xB4, 0x48, 0xB4
};

const ImageType snow6x6 = {
  .width = 6,
  .height = 6,
  .pData = imageSnow6x6,
};

static const uint8_t imageTriangle9x5[] = {
  0x08,0x00, 0x1C,0x00, 0x3E,0x00, 0x7F,0x00, 0xFF,0x80
};

const ImageType triangle9x5 = {
  .width = 9,
  .height = 5,
  .pData = imageTriangle9x5,
};

static const uint8_t imageHeat10x6[] = {
  0x04,0x00, 0x08,0x00, 0x04,0x00, 0x88,0x80, 0x44,0x40, 0x88,0x80
};

const ImageType heat10x6 = {
  .width = 10,
  .height = 6,
  .pData = imageHeat10x6,
};

} // namespace Image
} // namespace Lcd

/* END OF FILE */