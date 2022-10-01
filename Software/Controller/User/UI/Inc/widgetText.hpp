/**************************************************************
 * @file    widgetText.hpp
 * @brief   Text widget.
 * @author  Alzn
 * @date    2022-04-09
 **************************************************************/
#pragma once

#include "lcdFont.hpp"
#include "widgetBase.hpp"

class WidgetText : public WidgetBase {
public:
  enum Align : uint16_t {
    AlignLeft = 0x0001,
    AlignRight = 0x0002,
    AlignHoriCenter = 0x0004,
    AlignTop = 0x0010,
    AlignBottom = 0x0020,
    AlignVertCenter = 0x0040,
  };

private:
  char *mStrBufPtr;
  size_t mStrBufSize;

  uint16_t mBackColor;
  uint16_t mTextColor;

  uint16_t mTextAlign;

  bool mIsNeedRepaint;

  const Lcd::FontType *mFontPtr;

private:
  void init(void) {
    mStrBufPtr = nullptr;
    mStrBufSize = 0;
    mBackColor = 0x0000;
    mTextColor = 0xFFFF;
    mFontPtr = &Lcd::Font::consolas16n8x14;
    mIsNeedRepaint = true;
  }

  uint16_t getHoriAlignType(uint16_t align) {
    return (align & 0x0F);
  }

  uint16_t getVertAlignType(uint16_t align) {
    return (align & 0xF0);
  }

  uint16_t mGetTextLineCharNum(const char *pStr);
  uint16_t mGetTextLineNum(const char *pStr);

public:
  WidgetText(void) {
    init();
  }

  ~WidgetText();

  void repaint(void);
  void repaintIfNeed(void) {
    if (mIsNeedRepaint) {
      repaint();
    }
  }
  void setAlign(uint16_t align);
  void setFont(const Lcd::FontType *pFont);
  void setTextStr(const char *pStr);
  void setTextColor(uint16_t color);
  void setBackColor(uint16_t color);
};

/* END OF FILE */