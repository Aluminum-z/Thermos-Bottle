/**************************************************************
 * @file    mainMessage.hpp
 * @brief   Message manager for main thread.
 * @author  Alzn
 * @date    2022-09-04
 **************************************************************/
#pragma once

#include "message.hpp"

class MainMessage : Message {
public:
  static const size_t mMaxMsgSize = 4;

  enum class MsgType : uint32_t {
    MainTimer,
    AdcConvCplt,
    IsPwrEnabled,
    WorkMode,
    CtrlValue,
  };

  typedef struct
  {
    MsgType msgType;
    uint8_t data[mMaxMsgSize];
  } MsgStruct;

private:
  MainMessage(void){};
  MainMessage &operator=(const MainMessage &) = delete;
  MainMessage(const MainMessage &);

  template <typename tValue>
  const tValue &getMsgData(const MsgStruct &msgStruct) {
    return *(reinterpret_cast<const tValue *>(msgStruct.data));
  }

public:
  static MainMessage &getInstance(void);
  void init(void);
  void waitAndHandle(void);
  template <typename tValue>
  void send(MsgType msgType, tValue value, bool isISR = false) {
    static_assert(sizeof(value) <= mMaxMsgSize, "Size of value is bigger than buffer.");
    MsgStruct msgStruct;
    msgStruct.msgType = msgType;
    *(reinterpret_cast<tValue *>(msgStruct.data)) = value;
    sendElement(&msgStruct, isISR);
  }
};

/* END OF FILE */