#include "utils.h"
#include "avr/io.h"
#include "avr/iom88.h"
#include "avr/interrupt.h"
#include "seg.h"

long milliseconds;

void utilsInit(void) {
    TCCR2B |= (1<<CS22); //prescaler/64
    TCCR2A |= (1<<WGM21); //CTC mode
    TIMSK2 |= (1<<OCIE2A);
    OCR2A = 250;
    milliseconds = 0;
}

ISR(TIMER2_COMPA_vect) {
    milliseconds++;
}

char* intToStr(int val, char* buf) {
    buf[2] = val % 10 + 0x30;
    buf[1] = (val / 10) % 10 + 0x30;
    buf[0] = (val / 100) % 10 + 0x30;

    return buf;
}

uint16_t getAdjustTempFromRaw(uint16_t raw) {
    return (uint16_t)(raw * ADJUST_TEMP_COEF + MIN_TEMP);
}

uint16_t getHeatTempFromRaw(uint16_t raw) {
    return (uint16_t)((raw - ZERO_HEAT_TEMP_COEF) * HEAT_TEMP_COEF) + ROOM_TEMP;
}

int absi(int val) {
    if (val < 0) {
        return -val;
    }

    return val;
}

float absf(float val) {
    if (val < 0) {
        return -val;
    }

    return val;
}
