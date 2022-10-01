/**************************************************************
 * @file    mainMessage.cpp
 * @brief   Message manager for main thread.
 * @author  Alzn
 * @date    2022-09-04
 *************************************************************/

#include "FreeRTOS.h"
#include "timers.h"

#include "deviceManager.hpp"
#include "mainMessage.hpp"
#include "pid.hpp"

#include <cmath>

/**
 * @brief   Callback function for main timer.
 * @author  Alzn
 * @date    2022-09-04
 */
static void mainTimerCallback(TimerHandle_t xTimer) {
  MainMessage &mainMsg = MainMessage::getInstance();
  mainMsg.send(MainMessage::MsgType::MainTimer, 0);
}

class MainMsgHandler {
private:
  enum TickDiv {
    TickTemp,
    TickPwrOut = 5,
    TickPwrIn = 9,
    TickMax = 10,
  };

private:
  TimerHandle_t mTimerHandle;

  uint16_t mTick;

  float mVolIn;
  float mCurTe;

  float mTempBtm;
  float mTempTop;

  bool mIsPwrEnabled;
  WorkMode mWorkMode;
  float mCfgTemp;

  Pid<float> mPidTec;
  TecMode mTecMode;

  bool mIsUiSynced;

private:
  MainMsgHandler &operator=(const MainMsgHandler &) = delete;
  MainMsgHandler(const MainMsgHandler &) = delete;
  MainMsgHandler() {
    mIsPwrEnabled = false;
    mTick = 0;
    mIsUiSynced = false;
  }

  void restartMainTimer(void) {
    xTimerStart(mTimerHandle, 0);
  }

  void sendValueToUI(UiMessage::MsgType msgType, float value1, float value2) {
    UiMessage &uiMsg = UiMessage::getInstance();
    UiMessage::MsgDataFloat msgData;
    msgData.data1 = value1;
    msgData.data2 = value2;
    uiMsg.send(msgType, msgData);
  }

  void setTecMode(TecMode mode) {
    if (mTecMode == mode) {
      return;
    }
    mTecMode = mode;
    DeviceManager &deviceManager = DeviceManager::getInstance();
    Tec &tec = deviceManager.getTec();
    tec.setMode(mTecMode);
    UiMessage &uiMsg = UiMessage::getInstance();
    uiMsg.send(UiMessage::MsgType::TecMode, mTecMode);
  }

public:
  static MainMsgHandler &getInstance(void) {
    static MainMsgHandler instance;
    return instance;
  }

  void init(void) {
    /* Variable */
    mVolIn = 0.0f;
    mCurTe = 0.0f;
    mTempBtm = 0.0f;
    mTempTop = 0.0f;

    mPidTec.setFactor(0.05f, 0.0005f, 0.02f);
    mPidTec.setCtrlValLimit(-1.0f, 1.0f);
    mPidTec.reset(0.0f);

    mTecMode = TecMode::BtmCoolTopHeat;
    mWorkMode = WorkMode::Thermostat;
    mCfgTemp = 20.0f;
    mIsUiSynced = false;
    /* Hardware */

    /* Timer */
    mTimerHandle = xTimerCreate("Main", pdMS_TO_TICKS(100), pdFALSE, nullptr, mainTimerCallback);
    restartMainTimer();
  }

  void mainTimerHandler(void) {
    mTick++;
    if (mTick >= TickMax) {
      mTick = 0;
    }

    restartMainTimer();

    if (!mIsUiSynced) {
      UiMessage &uiMsg = UiMessage::getInstance();
      uiMsg.send<UiMessage::MsgDataModeAndValue>(UiMessage::MsgType::WorkModeAndValue, {mCfgTemp, mWorkMode});
    }

    DeviceManager &deviceManager = DeviceManager::getInstance();
    Adc &adc = deviceManager.getAdc();
    if (!adc.getIsConverting()) {
      adc.start();
    }
    if (mTick == TickPwrIn) {
      sendValueToUI(UiMessage::MsgType::PowerIn, mVolIn, mCurTe);
    }

    Ds18b20 &tempsenTop = deviceManager.getTempSenTop();
    Ds18b20 &tempSenBtm = deviceManager.getTempSenBtm();
    mTempTop = tempsenTop.readTemp();
    mTempBtm = tempSenBtm.readTemp();
    if (mTick == TickTemp) {
      sendValueToUI(UiMessage::MsgType::Temperatures, mTempTop, mTempBtm);
    }

    float fanPwr = 0.0f;
    float tecPwr = 0.0f;
    if (!mIsPwrEnabled) {
      tecPwr = 0.0f;
      fanPwr = 0.0f;
    } else {
      mPidTec.setMeaVal(mTempBtm);
      if (mTecMode == TecMode::BtmCoolTopHeat) {
        if (mPidTec.getCtrlVal() > 0.01f) {
          setTecMode(TecMode::BtmHeatTopCool);
        }
      } else {
        if (mPidTec.getCtrlVal() < -0.01f) {
          setTecMode(TecMode::BtmCoolTopHeat);
        }
      }
      tecPwr = fabsf(mPidTec.getCtrlVal());
      if (mTecMode == TecMode::BtmCoolTopHeat) {
        const float maxTemperature = 60.0f;
        const float fanThreshold = 30.0f;
        if (mTempTop < fanThreshold) {
          fanPwr = 0.0f;
        } else {
          fanPwr = (mTempTop - fanThreshold) * (1.0f / (maxTemperature - fanThreshold));
          if (fanPwr > 1.0f) {
            fanPwr = 1.0f;
          }
        }
      } else {
        const float minTemperature = 0.0f;
        const float fanThreshold = 20.0f;
        if (mTempTop > fanThreshold) {
          fanPwr = 0.0f;
        } else {
          fanPwr = (fanThreshold - mTempTop) * (1.0f / (fanThreshold - minTemperature));
          if (fanPwr > 1.0f) {
            fanPwr = 1.0f;
          }
        }
      }
    }
    Tec &tec = deviceManager.getTec();
    tec.setDutyCycle(tecPwr);
    Fan &fan = deviceManager.getFan();
    fan.setDutyCycle(fanPwr);

    if (mTick == TickPwrOut) {
      sendValueToUI(UiMessage::MsgType::PowerOut, tecPwr, fanPwr);
    }
  }

  void adcConvCpltHandler(void) {
    DeviceManager &deviceManager = DeviceManager::getInstance();
    Adc &adc = deviceManager.getAdc();
    adc.convCpltCallBack();
    mVolIn = adc.getValue(Adc::AdcValVin);
    mCurTe = adc.getValue(Adc::AdcValPwr);
  }

  void setIsPwrEnabled(bool isEnabled) {
    if (mIsPwrEnabled == isEnabled) {
      return;
    }
    mIsPwrEnabled = isEnabled;
    if (isEnabled) {
      switch (mWorkMode) {
        case WorkMode::Thermostat:
          mPidTec.setCfgMeaVal(mCfgTemp);
          break;

        case WorkMode::MaxPwrCool:
          mPidTec.setCfgMeaVal(0.0f);
          break;

        case WorkMode::MaxPwrHeat:
          mPidTec.setCfgMeaVal(80.0f);
          break;

        default:
          break;
      }
    }
    mPidTec.reset(0.0f);
    UiMessage &uiMsg = UiMessage::getInstance();
    uiMsg.send(UiMessage::MsgType::PowerIsEnable, isEnabled);
  }

  void setWorkMode(WorkMode mode) {
    if (mWorkMode == mode) {
      return;
    }
    mWorkMode = mode;
    setIsPwrEnabled(false);
    UiMessage &uiMsg = UiMessage::getInstance();
    uiMsg.send<UiMessage::MsgDataModeAndValue>(UiMessage::MsgType::WorkModeAndValue, {mCfgTemp, mWorkMode});
  }

  void setCtrlValue(float value) {
    if (mWorkMode != WorkMode::Thermostat) {
      return;
    }
    const float maxTemp = 80.0f;
    const float minTemp = 0.0f;
    if (value > maxTemp) {
      value = maxTemp;
    }
    if (value < minTemp) {
      value = minTemp;
    }
    if (mCfgTemp == value) {
      return;
    }
    mCfgTemp = value;
    mPidTec.setCfgMeaVal(value);
    UiMessage &uiMsg = UiMessage::getInstance();
    uiMsg.send<UiMessage::MsgDataModeAndValue>(UiMessage::MsgType::WorkModeAndValue, {mCfgTemp, mWorkMode});
  }
};

/**
 * @brief   Get the unique instance.
 * @author  Alzn
 * @date    2022-04-07
 */
MainMessage &MainMessage::getInstance(void) {
  static MainMessage instance;
  return instance;
}

/**
 * @brief   Initialize the message meanager.
 * @author  Alzn
 * @date    2022-04-07
 */
void MainMessage::init(void) {
  initQueue(8, sizeof(MsgStruct));
  MainMsgHandler &mainMsgHandler = MainMsgHandler::getInstance();
  mainMsgHandler.init();
}

/**
 * @brief   Waitting for messages and handle one.
 * @author  Alzn
 * @date    2022-04-07
 */
void MainMessage::waitAndHandle(void) {
  MsgStruct msgStruct;
  receiveElement(&msgStruct);
  MainMsgHandler &mainMsgHandler = MainMsgHandler::getInstance();
  switch (msgStruct.msgType) {
    case MsgType::MainTimer:
      mainMsgHandler.mainTimerHandler();
      break;

    case MsgType::AdcConvCplt:
      mainMsgHandler.adcConvCpltHandler();
      break;

    case MsgType::IsPwrEnabled:
      mainMsgHandler.setIsPwrEnabled(getMsgData<bool>(msgStruct));
      break;

    case MsgType::WorkMode:
      mainMsgHandler.setWorkMode(getMsgData<WorkMode>(msgStruct));
      break;

    case MsgType::CtrlValue:
      mainMsgHandler.setCtrlValue(getMsgData<float>(msgStruct));
      break;

    default:
      break;
  }
}

/* END OF FILE */