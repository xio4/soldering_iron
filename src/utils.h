#ifndef UTILS_H
#define UTILS_H

#include "stdint.h"

#define MIN_TEMP 150
//#define MIN_TEMP 20
#define MAX_TEMP 350
#define MAX_ADC 1020
#define ADJUST_TEMP_COEF (float)(MAX_TEMP - MIN_TEMP) / MAX_ADC
// Use ZERO_HEAT_TEMP_COEF and CORRECT_COEF to calibrate temp
#define ZERO_HEAT_TEMP_COEF 250
#define ROOM_TEMP 23
#define CORRECT_COEF 185
#define HEAT_TEMP_COEF (float)(MAX_TEMP - ROOM_TEMP) / (MAX_ADC - ZERO_HEAT_TEMP_COEF - CORRECT_COEF)

extern long milliseconds;
extern void utilsInit(void);
extern char* intToStr(int val, char* buf);
extern uint16_t getAdjustTempFromRaw(uint16_t raw);
extern uint16_t getHeatTempFromRaw(uint16_t raw);
extern int absi(int val);
extern float absf(float val);

#endif // UTILS_H
