/**************************************************************
 * @file    uiMessage.hpp
 * @brief   Message meanager for UI module.
 * @author  Alzn
 * @date    2022-04-07
 *************************************************************/
#pragma once

#include "comType.hpp"
#include "message.hpp"

class UiMessage : Message {
public:
  static const size_t mMaxMsgSize = 8;

  enum class MsgType : uint32_t {
    Flash,
    Key,
    Temperatures,
    PowerOut,
    PowerIn,
    PowerIsEnable,
    TecMode,
    WorkModeAndValue,
  };

  typedef struct
  {
    MsgType msgType;
    uint8_t data[mMaxMsgSize];
  } MsgStruct;

  typedef struct
  {
    float data1;
    float data2;
  } MsgDataFloat;

  typedef struct
  {
    float value;
    WorkMode mode;
  } MsgDataModeAndValue;

private:
  UiMessage(void){};
  UiMessage &operator=(const UiMessage &) = delete;
  UiMessage(const UiMessage &);

  template <typename tValue>
  const tValue &getMsgData(const MsgStruct &msgStruct) {
    return *(reinterpret_cast<const tValue *>(msgStruct.data));
  }

public:
  static UiMessage &getInstance(void);
  void init(void);
  void waitAndHandle(void);
  template <typename tValue>
  void send(MsgType msgType, tValue value, TickType_t delay = portMAX_DELAY) {
    static_assert(sizeof(value) <= mMaxMsgSize, "Size of value is bigger than buffer.");
    MsgStruct msgStruct;
    msgStruct.msgType = msgType;
    *(reinterpret_cast<tValue *>(msgStruct.data)) = value;
    sendElement(&msgStruct, false, delay);
  }
};

/* END OF FILE */