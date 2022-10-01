/**************************************************************
 * @file    uiMain.cpp
 * @brief   User interface module.
 * @author  Alzn
 * @date    2022-04-07
 *************************************************************/
#include "FreeRTOS.h"
#include "task.h"

#include "uiMain.hpp"
#include "uiMessage.hpp"

#include "widgetTemperature.hpp"
#include "widgetPowerOut.hpp"
#include "widgetState.hpp"

/* Static */

/* Function */
void initWidget(void);

/**
 * @brief   The main function for user interface module.
 * @author  Alzn
 * @date    2022-04-07
 */
void uiThread(void *pParament) {
  UiMessage &uiMsg = UiMessage::getInstance();
  uiMsg.init();
  initWidget();
  while (true) {
    uiMsg.waitAndHandle();
  }
}

/**
 * @brief   Create thread for user interface module.
 * @author  Alzn
 * @date    2022-04-07
 */
void createUiThread(void) {
  xTaskCreate(uiThread, "UI", 256, nullptr, 0, nullptr);
}

/**
 * @brief   Initialize widget.
 * @author  Alzn
 * @date    2022-04-09
 */
void initWidget(void) {
  WidgetTemperature &widgetTemp = WidgetTemperature::getInstance();
  widgetTemp.setVisible(true);
  WidgetPowerOut &widgetPowerOut = WidgetPowerOut::getInstance();
  widgetPowerOut.setVisible(true);
  WidgetState &widgetState = WidgetState::getInstance();
  widgetState.setVisible(true);
}

/* END OF FILE */