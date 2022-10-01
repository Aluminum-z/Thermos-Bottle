/**************************************************************
 * @file    message.hpp
 * @brief   Message library.
 * @author  Alzn
 * @date    2022-09-12
 **************************************************************/
#pragma once

#include "FreeRTOS.h"
#include "queue.h"

class Message {
protected:
  QueueHandle_t mQueueHandle;

protected:
  Message() {
    mQueueHandle = nullptr;
  }

  void initQueue(size_t elementNum, size_t elementSize) {
    mQueueHandle = xQueueCreate(elementNum, elementSize);
  }

  void sendElement(const void *pElement, bool isISR = false, TickType_t delay = portMAX_DELAY) {
    if ((nullptr == mQueueHandle) || (nullptr == pElement)) {
      return;
    }
    if (isISR) {
      xQueueSendFromISR(mQueueHandle, pElement, nullptr);
    } else {
      xQueueSend(mQueueHandle, pElement, delay);
    }
  }

  void receiveElement(void *pBuf) {
    if ((nullptr == mQueueHandle)) {
      return;
    }
    xQueueReceive(mQueueHandle, pBuf, portMAX_DELAY);
  }
};

/* END OF FILE */