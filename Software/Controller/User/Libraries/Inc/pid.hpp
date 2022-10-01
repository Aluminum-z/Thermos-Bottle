/**************************************************************
 * @file    pid.hpp
 * @brief   Pid controller
 * @author  Alzn
 * @date    2022-09-16
 **************************************************************/
#pragma once

template <typename TValue>
class Pid {
private:
  TValue mKp;
  TValue mKi;
  TValue mKd;
  TValue mI;
  TValue mPreErr;
  TValue mCfgMeaVal;
  TValue mCtrlVal;
  TValue mCtrlValMin;
  TValue mCtrlValMax;

public:
  Pid(void) {
    reset(0.0f);
    mKp = 0.0f;
    mKi = 0.0f;
    mKd = 0.0f;
    mCfgMeaVal = 0.0f;
    mCtrlVal = 0.0f;
    mCtrlValMin = 0.0f;
    mCtrlValMax = 0.0f;
  }

  void setFactor(TValue kp, TValue ki, TValue kd) {
    mKp = kp;
    mKi = ki;
    mKd = kd;
  }

  void reset(float ctrlVal) {
    mI = 0.0f;
    mPreErr = 0.0f;
    mCtrlVal = ctrlVal;
  }

  void setCtrlValLimit(TValue min, TValue max) {
    mCtrlValMin = min;
    mCtrlValMax = max;
    if (mCtrlVal < mCtrlValMin) {
      mCtrlVal = mCtrlValMin;
    }
    if (mCtrlVal > mCtrlValMax) {
      mCtrlVal = mCtrlValMax;
    }
  }

  void setCfgMeaVal(TValue val) {
    mCfgMeaVal = val;
  }

  TValue setMeaVal(TValue meaVal) {
    TValue err = mCfgMeaVal - meaVal;
    mPreErr = err;
    mI += err;
    float valI = mKi * mI;
    if (valI > mCtrlValMax) {
      mI = mCtrlValMax / mKi;
    } else if (valI < mCtrlValMin) {
      mI = mCtrlValMin / mKi;
    }
    mCtrlVal = (mKp)*err + (mKi) * (mI) + (mKd) * (err - mPreErr);
    if (mCtrlVal < mCtrlValMin) {
      mCtrlVal = mCtrlValMin;
    } else if (mCtrlVal > mCtrlValMax) {
      mCtrlVal = mCtrlValMax;
    }
    return mCtrlVal;
  }

  TValue getCtrlVal(void) {
    return mCtrlVal;
  }
};

/* END OF FILE */