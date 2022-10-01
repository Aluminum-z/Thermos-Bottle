/**************************************************************
 * @file    key.hpp
 * @brief   Key manager
 * @author  Alzn
 * @date    2022-09-04
 **************************************************************/
#pragma once

#include "gpio.hpp"
#include "uiMessage.hpp"

enum class KeyId : uint16_t {
  Mod = 0,
  Add,
  Sub,
};

enum class KeyType : uint16_t {
  Click = 0,
  LongPress,
  Repeat
};

typedef struct {
  KeyId id;
  KeyType type;
} KeyValue;

class Key {
private:
  enum class KeyState : uint16_t {
    Release = 0,
    PrePress,
    Press,
    LongPress,
  };

private:
  Gpio mGpio;
  KeyId mId;
  uint32_t mPrePressTick;
  KeyState mKeyState;

  void sendKey(KeyType type) {
    UiMessage &uiMsg = UiMessage::getInstance();
    uiMsg.send<KeyValue>(UiMessage::MsgType::Key, {mId, type}, 0);
  }

public:
  Key(void) {
    mKeyState = KeyState::Release;
    mPrePressTick = 0;
  }

  Key(const Gpio &gpio, KeyId id) {
    mGpio = gpio;
    mId = id;
    mKeyState = KeyState::Release;
    mPrePressTick = 0;
  }

  void check(void) {
    const uint32_t longPressTickLimit = 1500;
    const uint32_t RepeatTickLimit = 300;
    const uint32_t filterTickLimit = 50;
    bool isPressed = (mGpio.read() == Gpio::Reset);
    switch (mKeyState) {
      case KeyState::Release:
        if (isPressed) {
          mKeyState = KeyState::PrePress;
          mPrePressTick = HAL_GetTick();
        }
        break;

      case KeyState::PrePress:
        if (!isPressed) {
          mKeyState = KeyState::Release;
          break;
        }
        if ((HAL_GetTick() > (mPrePressTick + filterTickLimit))) {
          mKeyState = KeyState::Press;
          mPrePressTick = HAL_GetTick();
        }
        break;

      case KeyState::Press:
        if (!isPressed) {
          mKeyState = KeyState::Release;
          sendKey(KeyType::Click);
          break;
        }
        if (HAL_GetTick() > (mPrePressTick + longPressTickLimit)) {
          mKeyState = KeyState::LongPress;
          mPrePressTick = HAL_GetTick();
          sendKey(KeyType::LongPress);
        }
        break;

      case KeyState::LongPress:
        if (!isPressed) {
          mKeyState = KeyState::Release;
          break;
        }
        if (HAL_GetTick() > (mPrePressTick + RepeatTickLimit)) {
          mPrePressTick = HAL_GetTick();
          sendKey(KeyType::Repeat);
        }
        break;

      default:
        break;
    }
  }
};

/* END OF FILE */