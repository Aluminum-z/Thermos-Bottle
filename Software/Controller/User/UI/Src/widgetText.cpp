/**************************************************************
 * @file    widgetText.cpp
 * @brief   Text widget.
 * @author  Alzn
 * @date    2022-04-09
 *************************************************************/

#include "widgetText.hpp"
#include "comFunc.hpp"
#include "logging.hpp"
#include <cstring>

/**
 * @brief   Deconstructor
 * @author  Alzn
 * @date    2022-09-12
 */
WidgetText::~WidgetText() {
  if (nullptr != mStrBufPtr) {
    free(mStrBufPtr);
  }
}

/**
 * @brief   Repaint the text.
 * @author  Alzn
 * @date    2022-07-10
 */
void WidgetText::repaint(void) {
  mIsNeedRepaint = false;
  if (!getVisible()) {
    return;
  }
  uint16_t charHeight = mFontPtr->height;
  uint16_t lineNum = mGetTextLineNum(mStrBufPtr);
  uint16_t alignVertType = getVertAlignType(mTextAlign);
  uint16_t textY = 0;
  uint16_t textHeight = lineNum * charHeight;
  if (textHeight > mHeight) {
    lineNum = mHeight / charHeight;
    textHeight = lineNum * charHeight;
  }
  switch (alignVertType) {
    case AlignTop:
      textY = 0;
      break;

    case AlignBottom:
      textY = mHeight - textHeight;
      break;

    case AlignVertCenter:
      textY = (mHeight - textHeight) / 2;
      break;

    default:
      break;
  }

  uint16_t charWidth = mFontPtr->width;
  uint16_t alignHoriType = getHoriAlignType(mTextAlign);
  Lcd::LcdDriver &lcd = getLcd();
  const char *pStr = mStrBufPtr;
  if (nullptr == pStr) {
    lcd.drawBlock(mX, mY, mWidth, mHeight, mBackColor);
    return;
  }
  lcd.drawBlock(mX, mY, mWidth, textY, mBackColor);
  for (uint16_t line = 0; line < lineNum; line++) {
    uint16_t y = textY + mY + line * charHeight;
    uint16_t x = 0;
    uint16_t lineCharNum = mGetTextLineCharNum(pStr);
    uint16_t drawCharNum = lineCharNum;
    if (drawCharNum * charWidth > mWidth) {
      drawCharNum = mWidth / charWidth;
    }
    switch (alignHoriType) {
      case AlignLeft:
        x = 0;
        break;

      case AlignRight:
        x = mWidth - drawCharNum * charWidth;
        break;

      case AlignHoriCenter:
        x = (mWidth - drawCharNum * charWidth) / 2;
        break;

      default:
        break;
    }
    lcd.drawBlock(mX, y, x, charHeight, mBackColor);
    x += mX;
    for (uint16_t i = 0; i < drawCharNum; i++) {
      uint8_t charIndex = pStr[i];
      if ((charIndex < mFontPtr->charStart) || (charIndex > mFontPtr->charEnd)) {
        lcd.drawBlock(x, y, charWidth, charHeight, mTextColor);
      } else {
        const uint8_t *fontDataPtr = mFontPtr->pData + (charIndex - mFontPtr->charStart) * mFontPtr->bytePerChar;
        lcd.drawBinaryImage(x, y, charWidth, charHeight, fontDataPtr, mTextColor, mBackColor);
      }
      x += charWidth;
    }
    if (x - mX < mWidth) {
      lcd.drawBlock(x, y, mWidth - (x - mX), charHeight, mBackColor);
    }
    pStr += lineCharNum;
    if (*pStr == '\n') {
      pStr++;
    }
  }
  lcd.drawBlock(mX, mY + textY + textHeight, mWidth, mHeight - textY - textHeight, mBackColor);
}

/**
 * @brief   Get the number of characters in a string.
 * @author  Alzn
 * @date    2022-07-11
 */
uint16_t WidgetText::mGetTextLineCharNum(const char *pStr) {
  uint16_t num = 0;
  if (nullptr == pStr) {
    return 0;
  }
  while ((pStr[num] != '\n') && (pStr[num] != '\0')) {
    num++;
  }
  return num;
}

/**
 * @brief   Get the number of lines in a string.
 * @author  Alzn
 * @date    2022-07-11
 */
uint16_t WidgetText::mGetTextLineNum(const char *pStr) {
  uint16_t lineNum = 1;
  if (nullptr == pStr) {
    return 0;
  }
  for (uint16_t i = 0; pStr[i] != '\0'; i++) {
    if (pStr[i] == '\n') {
      lineNum++;
    }
  }
  return lineNum;
}

/**
 * @brief   Set the mode of align for text.
 * @author  Alzn
 * @date    2022-07-10
 */
void WidgetText::setAlign(uint16_t align) {
  if (mTextAlign == align) {
    return;
  }
  if (!ComFunc::isValueHasOnlyOne1(getVertAlignType(align))
      || !ComFunc::isValueHasOnlyOne1(getHoriAlignType(align))) {
    LOG("Invalid type of align : %d ", align);
    return;
  }
  mTextAlign = align;
  mIsNeedRepaint = true;
}

/**
 * @brief   Set font of text
 * @author  Alzn
 * @date    2022-07-10
 */
void WidgetText::setFont(const Lcd::FontType *pFont) {
  if (nullptr == pFont) {
    LOG("Null ptr.");
  }
  mFontPtr = pFont;
  mIsNeedRepaint = true;
}

/**
 * @brief   Set the text
 * @author  Alzn
 * @date    2022-07-10
 */
void WidgetText::setTextStr(const char *pStr) {
  if ((mStrBufPtr != nullptr) && (0 == strcmp(mStrBufPtr, pStr))) {
    return;
  }
  size_t inputStrLen = strlen(pStr);
  if (inputStrLen + 1 > mStrBufSize) {
    if (nullptr != mStrBufPtr) {
      free(mStrBufPtr);
    }
    mStrBufPtr = static_cast<char *>(malloc(inputStrLen + 1));
    if (nullptr != mStrBufPtr) {
      mStrBufSize = inputStrLen + 1;
    }
  }
  if (nullptr != mStrBufPtr) {
    strcpy(mStrBufPtr, pStr);
  }
  mIsNeedRepaint = true;
}

/**
 * @brief   Set the color.
 * @author  Alzn
 * @date    2022-07-11
 */
void WidgetText::setTextColor(uint16_t color) {
  mTextColor = color;
  mIsNeedRepaint = true;
}

void WidgetText::setBackColor(uint16_t color) {
  mBackColor = color;
  mIsNeedRepaint = true;
}

/* END OF FILE */