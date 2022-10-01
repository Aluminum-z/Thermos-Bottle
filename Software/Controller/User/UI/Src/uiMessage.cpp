/**************************************************************
 * @file    uiMessage.cpp
 * @brief   Message manager for UI module.
 * @author  Alzn
 * @date    2022-04-07
 *************************************************************/

#include "FreeRTOS.h"
#include "timers.h"

#include "comFunc.hpp"
#include "deviceManager.hpp"
#include "key.hpp"
#include "mainMessage.hpp"
#include "uiMessage.hpp"
#include "widgetPowerOut.hpp"
#include "widgetState.hpp"
#include "widgetTemperature.hpp"

/**
 * @brief   Callback function for ui timer.
 * @author  Alzn
 * @date    2022-09-04
 */
static void uiFlashTimerCallBack(TimerHandle_t xTimer) {
  UiMessage &uiMessage = UiMessage::getInstance();
  uiMessage.send(UiMessage::MsgType::Flash, 0);
}

class UiMsgHandler {
private:
  enum class Menu : uint16_t {
    Main = 0,
    CfgMode
  };

private:
  TimerHandle_t mTimerHandle;
  bool mFlashState;
  WidgetBase *mFlashWidgetPtr;
  uint16_t mFlashCfgVal;

  bool mIsPwrEnabled;
  Menu mMenu;

  WorkMode mWorkMode;
  float mCtrlValue;

private:
  UiMsgHandler(const UiMsgHandler &);
  UiMsgHandler &operator=(const UiMsgHandler &);
  UiMsgHandler(void) {
    mIsPwrEnabled = false;
    mMenu = Menu::Main;
    mFlashWidgetPtr = nullptr;
    mFlashCfgVal = 0;
  }

  void setFlashWidget(WidgetBase *pWidget, uint16_t cfgVal) {
    if (nullptr != mFlashWidgetPtr) {
      mFlashWidgetPtr->flash(false, mFlashCfgVal);
    }
    mFlashWidgetPtr = pWidget;
    mFlashCfgVal = cfgVal;
    handleFlashTimer();
  }

public:
  static UiMsgHandler &getInstance(void) {
    static UiMsgHandler instance;
    return instance;
  }

  void init(void) {
    mTimerHandle = xTimerCreate("Ui", pdMS_TO_TICKS(500), pdTRUE, nullptr, uiFlashTimerCallBack);
    xTimerStart(mTimerHandle, portMAX_DELAY);
  }

  void setIsPwrEnabled(bool isEnabled) {
    if (mIsPwrEnabled == isEnabled) {
      return;
    }
    mIsPwrEnabled = isEnabled;
    WidgetState &stateWidget = WidgetState::getInstance();
    stateWidget.setIsEnable(isEnabled);
  }

  void setWorkModeAndValue(WorkMode mode, float value) {
    if ((mWorkMode == mode) && (mCtrlValue == value)) {
      return;
    }
    mWorkMode = mode;
    mCtrlValue = value;
    WidgetState &stateWidget = WidgetState::getInstance();
    stateWidget.setWorkMode(mode);
    stateWidget.setCtrlValue(value);
    stateWidget.flash(false, WidgetState::FlashModeText);
  }

  void handleKey(const KeyValue &keyValue) {
    MainMessage &mainMsg = MainMessage::getInstance();
    switch (mMenu) {
      case Menu::Main:
        switch (keyValue.id) {
          case KeyId::Mod: {
            if (keyValue.type == KeyType::Click) {
              mainMsg.send(MainMessage::MsgType::IsPwrEnabled, !mIsPwrEnabled);
            }
            if (keyValue.type == KeyType::LongPress) {
              mMenu = Menu::CfgMode;
              setFlashWidget(&(WidgetState::getInstance()), WidgetState::FlashModeText);
            }
          } break;

          case KeyId::Add:
          case KeyId::Sub:
            if (mWorkMode == WorkMode::Thermostat) {
              float addValue = 1.0f;
              if ((keyValue.type == KeyType::LongPress) || (keyValue.type == KeyType::Repeat)) {
                addValue = 5.0f;
              }
              if (keyValue.id == KeyId::Sub) {
                addValue = -addValue;
              }
              mainMsg.send(MainMessage::MsgType::CtrlValue, mCtrlValue + addValue);
            }
            break;

          default:
            break;
        }
        break;

      case Menu::CfgMode:
        switch (keyValue.id) {
          case KeyId::Mod:
            if (keyValue.type == KeyType::Click) {
              mMenu = Menu::Main;
              setFlashWidget(nullptr, 0);
            }
            break;

          case KeyId::Add:
            if (keyValue.type == KeyType::Click) {
              int32_t newMode = static_cast<uint16_t>(mWorkMode);
              newMode++;
              if (newMode >= static_cast<int32_t>(WorkMode::ModeMax)) {
                newMode = 0;
              }
              mainMsg.send(MainMessage::MsgType::WorkMode, static_cast<WorkMode>(newMode));
            }
            break;

          case KeyId::Sub:
            if (keyValue.type == KeyType::Click) {
              int32_t newMode = static_cast<uint16_t>(mWorkMode);
              newMode--;
              if (newMode < 0) {
                newMode = static_cast<int32_t>(WorkMode::ModeMax) - 1;
              }
              mainMsg.send(MainMessage::MsgType::WorkMode, static_cast<WorkMode>(newMode));
            }
            break;

          default:
            break;
        }
        break;

      default:
        break;
    }
  }

  void handleFlashTimer(void) {
    if (nullptr == mFlashWidgetPtr) {
      return;
    }
    mFlashState = !mFlashState;
    mFlashWidgetPtr->flash(mFlashState, mFlashCfgVal);
  }
};

/**
 * @brief   Waitting for messages and handle one.
 * @author  Alzn
 * @date    2022-04-07
 */
void UiMessage::waitAndHandle(void) {
  UiMsgHandler &uiMsgHandler = UiMsgHandler::getInstance();
  MsgStruct msgStruct;
  receiveElement(&msgStruct);
  switch (msgStruct.msgType) {
    case MsgType::Temperatures: {
      WidgetTemperature &tempWidget = WidgetTemperature::getInstance();
      tempWidget.setTempTop(getMsgData<MsgDataFloat>(msgStruct).data1);
      tempWidget.setTempBtm(getMsgData<MsgDataFloat>(msgStruct).data2);
    } break;

    case MsgType::PowerOut: {
      WidgetPowerOut &pwrOutWidget = WidgetPowerOut::getInstance();
      pwrOutWidget.setPwrTec(getMsgData<MsgDataFloat>(msgStruct).data1);
      pwrOutWidget.setPwrFan(getMsgData<MsgDataFloat>(msgStruct).data2);
    } break;

    case MsgType::PowerIn: {
      WidgetState &stateWidget = WidgetState::getInstance();
      stateWidget.setVin(getMsgData<MsgDataFloat>(msgStruct).data1);
      stateWidget.setPwr(getMsgData<MsgDataFloat>(msgStruct).data2);
    } break;

    case MsgType::PowerIsEnable:
      uiMsgHandler.setIsPwrEnabled(getMsgData<bool>(msgStruct));
      break;

    case MsgType::Key:
      uiMsgHandler.handleKey(getMsgData<KeyValue>(msgStruct));
      break;

    case MsgType::Flash:
      uiMsgHandler.handleFlashTimer();
      break;

    case MsgType::TecMode: {
      WidgetTemperature &tempWidget = WidgetTemperature::getInstance();
      tempWidget.setTecMode(getMsgData<TecMode>(msgStruct));
    } break;

    case MsgType::WorkModeAndValue: {
      const MsgDataModeAndValue msgData = getMsgData<MsgDataModeAndValue>(msgStruct);
      uiMsgHandler.setWorkModeAndValue(msgData.mode, msgData.value);
    } break;

    default:
      break;
  }
}

/**
 * @brief   Get the unique instance.
 * @author  Alzn
 * @date    2022-04-07
 */
UiMessage &UiMessage::getInstance(void) {
  static UiMessage instance;
  return instance;
}

/**
 * @brief   Initialize the message meanager.
 * @author  Alzn
 * @date    2022-04-07
 */
void UiMessage::init(void) {
  initQueue(8, sizeof(MsgStruct));
  UiMsgHandler &uiMsgHandler = UiMsgHandler::getInstance();
  uiMsgHandler.init();
}

/* END OF FILE */