/**************************************************************
 * @file    comType.hpp
 * @brief   Common typedef.
 * @author  Alzn
 * @date    2022-09-11
 **************************************************************/
#pragma once
#include <cstdint>

enum class TecMode : uint16_t {
  BtmCoolTopHeat = 0,
  BtmHeatTopCool,
};

enum class WorkMode : uint16_t {
  Thermostat = 0,
  MaxPwrCool,
  MaxPwrHeat,
  ModeMax,
};

/* END OF FILE */