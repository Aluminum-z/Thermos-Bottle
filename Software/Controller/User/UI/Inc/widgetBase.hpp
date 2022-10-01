/**************************************************************
 * @file    widgetBase.hpp
 * @brief   Base functions and classes for widget.
 * @author  Alzn
 * @date    2022-07-10
 **************************************************************/
#pragma once

#include "deviceManager.hpp"
#include <cstdint>
#include <vector>

class WidgetBase {
protected:
  WidgetBase *mParentPtr;

  uint16_t mX;
  uint16_t mY;
  uint16_t mWidth;
  uint16_t mHeight;

  bool mIsVisible;

private:
  void init(void) {
    mParentPtr = nullptr;
    mX = 0;
    mY = 0;
    mWidth = 0;
    mHeight = 0;
    mIsVisible = false;
  }

protected:
  void mSetVisible(bool isVisible) {
    if (mIsVisible == isVisible) {
      return;
    }
    mIsVisible = isVisible;
    if (isVisible) {
      repaint();
    }
  }

  Lcd::LcdDriver &getLcd(void) {
    DeviceManager &deviceMng = DeviceManager::getInstance();
    return deviceMng.getLcd();
  }

public:
  WidgetBase(void) {
    init();
  }

  virtual ~WidgetBase(void) {
  }

  void setParent(WidgetBase *pParent) {
    mParentPtr = pParent;
  }

  virtual void setVisible(bool isVisiable) {
    mSetVisible(isVisiable);
  }

  virtual void setPosition(uint16_t x, uint16_t y) {
    if (nullptr != mParentPtr) {
      x += mParentPtr->getX();
      y += mParentPtr->getY();
    }
    mX = x;
    mY = y;
  }

  virtual void setSize(uint16_t width, uint16_t height) {
    mWidth = width;
    mHeight = height;
  }

  virtual void repaint(void) {
  }

  virtual void flash(bool flashState, uint16_t cfgVal) {
  }

  bool getVisible(void) {
    return mIsVisible;
  }

  uint16_t getX(void) {
    return mX;
  }

  uint16_t getY(void) {
    return mY;
  }

  uint16_t getWidth(void) {
    return mWidth;
  }

  uint16_t getHeight(void) {
    return mHeight;
  }
};

/* END OF FILE */