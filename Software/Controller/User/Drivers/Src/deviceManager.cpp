/**************************************************************
 * @file    deviceManager.cpp
 * @brief   Device manager.
 * @author  Alzn
 * @date    2022-04-03
 *************************************************************/

#include "deviceManager.hpp"
#include "lcdChipSpi.hpp"

/**
 * @brief   Constructor.
 * @author  Alzn
 * @date    2022-04-03
 */
DeviceManager::DeviceManager(void) {
  /* Gpio */
  /* Keys */
  mKeyMod = Key(Gpio(KEY_MODE_GPIO_Port, KEY_MODE_Pin), KeyId::Mod);
  mKeyAdd = Key(Gpio(KEY_ADD_GPIO_Port, KEY_ADD_Pin), KeyId::Add);
  mKeySub = Key(Gpio(KEY_SUB_GPIO_Port, KEY_SUB_Pin), KeyId::Sub);
  /* Lcd */
  Lcd::ChipInfoSpi lcdChipInfo;
  lcdChipInfo.chipSelectPin = Gpio(LCD_CS_GPIO_Port, LCD_CS_Pin);
  lcdChipInfo.dataCmdSelectPin = Gpio(LCD_RS_GPIO_Port, LCD_RS_Pin);
  lcdChipInfo.resetPin = Gpio(LCD_RST_GPIO_Port, LCD_RST_Pin);
  lcdChipInfo.hSpi = &hspi2;
  static Lcd::LcdChipSt7735s lcdChip(lcdChipInfo);
  Lcd::DeviceInfo lcdDeviceInfo;
  lcdDeviceInfo.mPtrChip = &lcdChip;
  lcdDeviceInfo.backlightPwm = Pwm(&htim22, Pwm::Channel1);
  lcdDeviceInfo.dispOrientation = Lcd::DispOrientation::Horizontal;
  lcdDeviceInfo.sizeX = 160;
  lcdDeviceInfo.sizeY = 80;
  mLcd = Lcd::LcdDriver(lcdDeviceInfo);
  /* Adc */
  mAdc = Adc(&hadc, &hdma_adc);
  /* Temperature */
  mTempSenBtm = Ds18b20(Gpio(TEMP1_GPIO_Port, TEMP1_Pin));
  mTempSenTop = Ds18b20(Gpio(TEMP2_GPIO_Port, TEMP2_Pin));
  /* Tec */
  mTec = Tec(Pwm(&htim2, Pwm::Channel1), Pwm(&htim2, Pwm::Channel2), Gpio(TE_SLEEP_GPIO_Port, TE_SLEEP_Pin));

  mFan = Fan(Pwm(&htim22, Pwm::Channel2), Gpio(FAN_EN_GPIO_Port, FAN_EN_Pin));
}

/**
 * @brief   Get the unique instance.
 * @author  Alzn
 * @date    2022-04-03
 */
DeviceManager &DeviceManager::getInstance(void) {
  static DeviceManager instance;
  return instance;
}

/**
 * @brief   Initialize devices.
 * @author  Alzn
 * @date    2022-04-03
 */
void DeviceManager::init(void) {
  mLcd.init();
  mAdc.init();
  mTempSenBtm.init();
  mTempSenTop.init();
  mTec.init();
  mFan.init();
}

/* END OF FILE */