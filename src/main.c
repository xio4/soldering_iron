#include "avr/interrupt.h"
#include "utils.h"
#include "seg.h"
#include "adc.h"
#include "heat.h"

#define SHOW_HEAT_TEMP_COUNT 1000
#define SHOW_ADJUST_TEMP_COUNT 200
#define ADJUST_TEMP_COUNT 150
#define HEAT_TEMP_COUNT 100
#define HEAT_UPDATE_COUNT 50
#define ADJUST_TEMP_DELTA 3

extern long milliseconds;
static long showAdjustCounter;
static long showHeatCounter;
static long adjustCounter;
static long heatCounter;
static long heatUpdateCounter;

void init(void) {
    utilsInit();
    segInit();
    adcInit();
    heatInit();

    sei();
}

int main() {
    init();
    char buf[3];
    uint16_t adjustTemp = 0;
    uint16_t lastAdjustTemp = 0;
    uint16_t heatTemp = 0;

    showHeatCounter = milliseconds;
    showAdjustCounter = milliseconds;
    adjustCounter = milliseconds;
    heatCounter = milliseconds;
    heatUpdateCounter = milliseconds;

    while (1) {
        if ((milliseconds - showAdjustCounter >= SHOW_ADJUST_TEMP_COUNT)
            && absi((int)(adjustTemp - lastAdjustTemp)) > ADJUST_TEMP_DELTA) {
            setText(intToStr((int)adjustTemp, buf));

            showAdjustCounter = milliseconds;
            showHeatCounter = milliseconds;
            lastAdjustTemp = adjustTemp;
        }

        if (milliseconds - showHeatCounter >= SHOW_HEAT_TEMP_COUNT) {
            setText(intToStr((int)heatTemp, buf));
            showHeatCounter = milliseconds;
        }

        if (milliseconds - adjustCounter >= ADJUST_TEMP_COUNT) {
            adjustTemp = getAdjustTempFromRaw(readPotenciometer());

            adjustCounter = milliseconds;
        }

        if (milliseconds - heatCounter >= HEAT_TEMP_COUNT) {
            heatTemp = getHeatTempFromRaw(readHeat());

            heatCounter = milliseconds;
        }

        if (milliseconds - heatUpdateCounter >= HEAT_UPDATE_COUNT) {
            heatUpdate(heatTemp, adjustTemp);

            heatUpdateCounter = milliseconds;
        }

        showNext();
    }
}
