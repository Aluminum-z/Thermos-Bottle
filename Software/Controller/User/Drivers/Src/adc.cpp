/**************************************************************
 * @file    adc.cpp
 * @brief   Adc driver.
 * @author  Alzn
 * @date    2022-09-04
 *************************************************************/

#include "adc.hpp"
#include <cmath>

/**
 * @brief   Initialize the adc and dma.
 * @author  Alzn
 * @date    2022-09-04
 */
void Adc::init(void) {
  if ((nullptr == mAdc) || (nullptr == mDma)) {
    return;
  }
  mIsConverting = false;
}

/**
 * @brief   Start the converting.
 * @author  Alzn
 * @date    2022-09-04
 */
void Adc::start(void) {
  HAL_ADC_Start_DMA(mAdc, reinterpret_cast<uint32_t *>(mAdcBuf), AdcChNum);
  mIsConverting = true;
}

/**
 * @brief   Convert completed, calculate the value.
 * @author  Alzn
 * @date    2022-09-04
 */
void Adc::convCpltCallBack(void) {
  mIsConverting = false;
  const float factorVol = 3.3f / 4096.0f * 2.0f * 0.98f;
  mAdcValue[AdcValVin] = factorVol * static_cast<float>(mAdcBuf[AdcChVin]);
  mAdcValue[AdcValPwr] = factorVol * fabsf(static_cast<float>(mAdcBuf[AdcChTeSen1]) - static_cast<float>(mAdcBuf[AdcChTeSen2]));
  mAdcValue[AdcValPwr] = mAdcValue[AdcValPwr] * mAdcValue[AdcValPwr] * 0.4f;
  const uint16_t *const pTempCal30 = ((uint16_t *)((uint32_t)0x1FF8007A));
  const uint16_t *const pTempCal130 = ((uint16_t *)((uint32_t)0x1FF8007E));
  mAdcValue[AdcValTemp] = 30 + 100 * (mAdcBuf[AdcChTemp] - static_cast<int32_t>(*pTempCal30)) / (static_cast<int32_t>(*pTempCal130) - static_cast<int32_t>(*pTempCal30));
}

/* END OF FILE */