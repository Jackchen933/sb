#ifndef __DATASAVE_H__
#define __DATASAVE_H__

#include "BL24C02.h"

void EEPROM_Init(void);
uint8_t EEPROM_Check(void);
uint8_t SettingSave(uint8_t *buf, uint8_t addr, uint8_t lenth);
uint8_t SettingGet(uint8_t *buf, uint8_t addr, uint8_t lenth);

// WatchFace相关保存
#define EEPROM_ADDR_WATCHFACE 0x12  // 表盘样式保存地址

void DataSave_SetWatchFace(uint8_t face_style);
uint8_t DataSave_GetWatchFace(void);

#endif
