/**************************************************************
 * @file    widgetProcessBar.cpp
 * @brief   Process bar widget.
 * @author  Alzn
 * @date    2022-09-08
 *************************************************************/

#include "widgetProcessBar.hpp"
#include "logging.hpp"

/**
 * @brief   Get the maximum length of the bar.
 * @author  Alzn
 * @date    2022-09-08
 */
uint16_t WidgetProcessBar::getMaxLength(void) {
  switch (mDirection) {
    case Direction::LeftToRight:
    case Direction::RightToLeft:
      return mWidth - mLengthGap + 1;
      break;

    case Direction::DownToUp:
    case Direction::UpToDown:
      return mHeight - mLengthGap + 1;
      break;

    default:
      break;
  }
  return mWidth - mLengthGap + 1;
}

/**
 * @brief   Check the size of process bar is valid for draw.
 * @author  Alzn
 * @date    2022-09-10
 */
bool WidgetProcessBar::checkSizeIsValid(void) {
  if ((mWidth <= mLengthGap) || (mHeight <= mLengthGap)) {
    LOG("Size of process bar is invalid");
    return false;
  }
  return true;
}

/**
 * @brief   Draw block in bar area.
 * @author  Alzn
 * @date    2022-09-08
 */
void WidgetProcessBar::drawBar(uint16_t start, uint16_t end, uint16_t color) {
  Lcd::LcdDriver &lcd = getLcd();
  switch (mDirection) {
    case Direction::LeftToRight:
      lcd.drawBlock(mX + mBarPosInc + start, mY + mBarPosInc, end - start, mHeight - mLengthGap + 1, color);
      break;
    case Direction::RightToLeft:
      lcd.drawBlock(mX + mWidth - mBarPosInc - end + 1, mY + mBarPosInc, end - start, mHeight - mLengthGap + 1, color);
      break;

    case Direction::DownToUp:
      lcd.drawBlock(mX + mBarPosInc, mY + mHeight - mBarPosInc - end + 1, mWidth - mLengthGap + 1, end - start, color);
      break;
    case Direction::UpToDown:
      lcd.drawBlock(mX + mBarPosInc, mY + mBarPosInc + start, mWidth - mLengthGap + 1, end - start, color);
      break;

    default:
      break;
  }
}

/**
 * @brief   Refresh the bar without repaint the border.
 * @author  Alzn
 * @date    2022-09-08
 */
void WidgetProcessBar::refresh(uint16_t newLength) {
  if (newLength > mBarLength) {
    drawBar(mBarLength, newLength, mBarColor);
  } else {
    drawBar(newLength, mBarLength, mBackColor);
  }
  mBarLength = newLength;
}

/**
 * @brief   Repaint the process bar.
 * @author  Alzn
 * @date    2022-09-08
 */
void WidgetProcessBar::repaint(void) {
  if (!checkSizeIsValid()) {
    return;
  }
  if (!getVisible()) {
    return;
  }
  Lcd::LcdDriver &lcd = getLcd();
  /* Border */
  lcd.drawLine(mX, mY, mX, mY + mHeight, mBorderColor);
  lcd.drawLine(mX, mY, mX + mWidth, mY, mBorderColor);
  lcd.drawLine(mX + mWidth, mY, mX + mWidth, mY + mHeight, mBorderColor);
  lcd.drawLine(mX, mY + mHeight, mX + mWidth, mY + mHeight, mBorderColor);
  /* Bar */
  lcd.drawBlock(mX + 1, mY + 1, mWidth - 2, mHeight - 2, mBackColor);
  drawBar(0, mBarLength, mBarColor);
}

/**
 * @brief   Set the size of widget.
 * @author  Alzn
 * @date    2022-09-10
 */
void WidgetProcessBar::setSize(uint16_t width, uint16_t height) {
  mWidth = width;
  mHeight = height;
  checkSizeIsValid();
}

/**
 * @brief   Set the direction of the bar.
 * @author  Alzn
 * @date    2022-09-12
 */
void WidgetProcessBar::setDirection(Direction dir) {
  if (mDirection == dir) {
    return;
  }
  mDirection = dir;
  repaint();
}

/**
 * @brief   Set the process in the range of 0.0. to 1.0f.
 * @author  Alzn
 * @date    2022-09-08
 */
void WidgetProcessBar::setProcess(float process) {
  uint16_t newLength = getMaxLength();
  if (process > 1.0f) {
    process = 1.0f;
  }
  if (process < 0.0f) {
    process = 0.0f;
  }
  newLength = (static_cast<float>(newLength) * (process) + 0.5f);
  if (newLength == mBarLength) {
    return;
  }
  refresh(newLength);
}

/* END OF FILE */