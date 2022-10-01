/**************************************************************
 * @file    cout.cpp
 * @brief   Retargeting std::cout
 * @author  Alzn
 * @date    2022-06-26
 *************************************************************/

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

int _write(int fd, char *ptr, int len){
  // HAL_UART_Transmit(&huart5, (uint8_t *)(ptr), len, HAL_MAX_DELAY);
  return len;
}

#ifdef __cplusplus
 extern "C" {
#endif
/* END OF FILE */