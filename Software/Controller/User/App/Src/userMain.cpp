/**************************************************************
 * @file    userMain.cpp
 * @brief   User main task.
 * @author  Alzn
 * @date    2022-03-28
 *************************************************************/

/* Include */
#include "FreeRTOS.h"
#include "task.h"

#include "deviceManager.hpp"
#include "logging.hpp"
#include "mainMessage.hpp"
#include "uiMain.hpp"
#include "uiMessage.hpp"

extern "C" {
/**
 * @brief   Main thread function.
 * @author  Alzn
 * @date    2022-09-12
 */
void mainThread(void *pPara) {
  MainMessage &mainMsg = MainMessage::getInstance();
  mainMsg.init();

  createUiThread();

  while (true) {
    mainMsg.waitAndHandle();
  }
}

/**
 * @brief   User main function.
 * @author  Alzn
 * @date    2022-03-28
 */
void userMain(void) {
  DeviceManager &deviceManager = DeviceManager::getInstance();
  deviceManager.init();

  LOG("Init done, create threads and start kernel...");

  xTaskCreate(mainThread, "Main", 164, nullptr, 2, nullptr);
  __HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
  __HAL_TIM_ENABLE_IT(&htim6, TIM_IT_UPDATE);
  __HAL_TIM_ENABLE(&htim6);
  vTaskStartScheduler();
}

/**
 * @brief   Timer 6 update interrupt callback function.
 * @author  Alzn
 * @date    2022-09-12
 */
void TIM6_DAC_IRQHandler(void) {
  __HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
  xPortSysTickHandler();
}

/**
 * @brief   Adc convert complete callback function.
 * @author  Alzn
 * @date    2022-09-04
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  MainMessage &mainMsg = MainMessage::getInstance();
  mainMsg.send(MainMessage::MsgType::AdcConvCplt, 0, true);
}

/**
 * @brief   Hook function for idle thread.
 * @author  Alzn
 * @date    2022-09-14
 */
void vApplicationIdleHook(void) {
  DeviceManager &deviceManager = DeviceManager::getInstance();
  deviceManager.getKeyMod().check();
  deviceManager.getKeyAdd().check();
  deviceManager.getKeySub().check();
}
}

/* END OF FILE */