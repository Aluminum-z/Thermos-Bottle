/**************************************************************
 * @file    lcdChipFsmc.hpp
 * @brief   Lcd chip libraries for FSMC interface.
 * @author  Alzn
 * @date    2022-09-04
 **************************************************************/
#pragma once

#include "gpio.hpp"
#include "lcdChip.hpp"

namespace Lcd {
/* Typedef */
enum class RegSelectAddr : uint32_t {
  A00,
  A01,
  A02,
  A03,
  A04,
  A05,
  A06,
  A07,
  A08,
  A09,
  A10,
  A11,
  A12,
  A13,
  A14,
  A15,
  A16,
  A17,
  A18,
  A19,
  A20,
  A21,
  A22,
  A23,
  A24,
  A25,
  MAX,
};

enum class ChipSelectPin : uint32_t {
  NE1,
  NE2,
  NE3,
  NE4,
  MAX,
};

typedef struct {
  ChipSelectPin chipSelect;
  RegSelectAddr regSelect;
  Gpio resetPin;
} ChipInfoFsmc;

/* Class */
class LcdChipNT35510 : public LcdChipBase {
private:
  uint16_t *mAddrCom;
  uint16_t *mAddrData;
  Gpio mResetPin;

private:
  /* Register Control */
  void writeCom(uint16_t command) {
    *(mAddrCom) = command;
  }
  void writeData(uint16_t data) {
    *(mAddrData) = data;
  }
  uint16_t readData(void) const {
    return *(mAddrData);
  }
  void writeReg(uint16_t reg, uint16_t data) {
    writeCom(reg);
    writeData(data);
  }
  uint16_t readReg(uint16_t reg) {
    writeCom(reg);
    return readData();
  }

public:
  LcdChipNT35510(const ChipInfoFsmc &deviceInfo);

  /* Basic Operation */
  void init(void);
  void setRange(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
  void setOrientation(DispOrientation newOrientation);
  void writeColor(uint16_t color) {
    writeData(color);
  }
  void fillColor(uint16_t color, size_t num);
};

} // namespace Lcd

/* END OF FILE */