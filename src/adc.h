#ifndef ADC_H
#define ADC_H

#include "stdint.h"

extern void adcInit(void);
extern uint16_t readHeat(void);
extern uint16_t readPotenciometer(void);

#endif // ADC_H
