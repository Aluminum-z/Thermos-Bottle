/**************************************************************
 * @file    adc.hpp
 * @brief   Adc driver
 * @author  Alzn
 * @date    2022-09-04
 **************************************************************/
#pragma once

#include "main.h"

class Adc {
public:
  enum Value : uint16_t {
    AdcValVin = 0,
    AdcValPwr,
    AdcValTemp,
    AdcValNum,
  };

private:
  enum Channel : uint16_t {
    AdcChTeSen1 = 0,
    AdcChTeSen2,
    AdcChVin,
    AdcChTemp,
    AdcChNum,
  };

private:
  ADC_HandleTypeDef *mAdc;
  DMA_HandleTypeDef *mDma;

  bool mIsConverting;

  uint16_t mAdcBuf[AdcChNum];
  float mAdcValue[AdcValNum];

public:
  Adc() {
    mAdc = nullptr;
    mDma = nullptr;
    mIsConverting = false;
  };
  Adc(ADC_HandleTypeDef *hAdc, DMA_HandleTypeDef *hDma) {
    mAdc = hAdc;
    mDma = hDma;
    mIsConverting = false;
  }

  void init(void);
  void start(void);
  void convCpltCallBack(void);

  float getValue(Value ch) {
    return mAdcValue[ch];
  }

  bool getIsConverting(void) {
    return mIsConverting;
  }
};

/* END OF FILE */