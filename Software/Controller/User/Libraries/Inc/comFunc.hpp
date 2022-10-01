/**************************************************************
 * @file    comFunc.hpp
 * @brief   Common functions.
 * @author  Alzn
 * @date    2022-07-03
 **************************************************************/
#pragma once

#include <cmath>
#include <cstdint>
#include <type_traits>

class ComFunc {
public:
  static void decimalToFraction(float decimal, int32_t &numerator, int32_t &denominator) {
    const float error = 0.0001;
    float a = decimal;
    float b = 1.0f;
    float mod = std::fmod(a, b);
    while (mod > error) {
      a = b;
      b = mod;
      mod = std::fmod(a, b);
    }
    numerator = std::roundf(decimal / b);
    denominator = std::roundf(1.0 / b);
  }

  template <typename T>
  static void limitValue(T &value, const T &min, const T &max) {
    if (value < min) {
      value = min;
    }
    if (value > max) {
      value = max;
    }
  }

  template <typename T>
  static bool isValueHasOnlyOne1(T value) {
    static_assert(std::is_integral<T>::value, "Integral required.");
    if (value == 0) {
      return false;
    }
    if (value < 0) {
      value = -value;
      if (value == -value) {
        return true;
      }
    }
    return ((value & (value - 1)) == 0);
  }

  static constexpr uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return (((static_cast<uint16_t>(r) >> 3) & 0x1F) << 11)
           + (((static_cast<uint16_t>(g) >> 2) & 0x3F) << 5)
           + ((static_cast<uint16_t>(b) >> 3) & 0x1F);
  }

  static void fixPointPrintf(char *pBuf, size_t bufLen, uint16_t intNum, uint16_t decNum, float value) {
    if (nullptr == pBuf) {
      return;
    }
    if (!std::isfinite(value)) {
      pBuf[0] = '\0';
    }
    pBuf[bufLen - 1] = '\0';
    bool isNeg = (value < 0);
    if (isNeg) {
      value = -value;
    }
    int32_t intPart = 1;
    for (uint16_t i = 0; i < decNum; i++) {
      intPart *= 10;
    }
    value += 0.5f / static_cast<float>(intPart);
    intPart = static_cast<int32_t>(value);
    uint16_t intPartLen = 0;
    while (intPart > 0) {
      intPart /= 10;
      intPartLen++;
    }
    if (intPartLen < intNum) {
      for (int32_t i = 0; i < (intNum - intPartLen); i++) {
        pBuf[i] = ' ';
      }
      if (isNeg) {
        pBuf[intNum - intPartLen - 1] = '-';
      }
      intPartLen = intNum;
    } else if (isNeg) {
      pBuf[0] = '-';
      intPartLen++;
    }
    if (intPartLen == 0) {
      intPartLen = 1;
    }
    intPart = static_cast<int32_t>(value);
    if ((intPart == 0) && (static_cast<uint16_t>(intPartLen - 1) < (bufLen - 1))) {
      pBuf[intPartLen - 1] = '0';
    }
    for (int32_t i = intPartLen - 1; (i >= 0) && (intPart > 0); i--) {
      if (i > static_cast<int32_t>(bufLen - 1)) {
        continue;
      }
      pBuf[i] = '0' + intPart % 10;
      intPart /= 10;
    }
    if (intPartLen >= (bufLen - 1)) {
      return;
    }
    if (decNum > 0) {
      pBuf[intPartLen] = '.';
    } else {
      pBuf[intPartLen] = '\0';
      return;
    }
    if (intPartLen >= bufLen) {
      return;
    }
    intPartLen++; /* For '.' */
    intPart = static_cast<int32_t>(value);
    value -= static_cast<float>(intPart);
    for (uint32_t i = 0; (i < decNum) && ((i + intPartLen) < (bufLen - 1)); i++) {
      value *= 10.0f;
      pBuf[i + intPartLen] = '0' + (static_cast<int32_t>(value) % 10);
    }
    if (intPartLen + decNum >= bufLen) {
      return;
    }
    pBuf[intPartLen + decNum] = '\0';
  }
};

/* END OF FILE */