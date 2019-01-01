#include "adc.h"
#include "avr/io.h"
#include "avr/iom88.h"

void adcInit(void) {
    ADMUX |= (1<<REFS0) | (1<<REFS1); // internal 1.1v ref
    ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
}

uint16_t read() {
    ADCSRA |= (1<<ADEN) | (1<<ADSC);

    while(ADCSRA & (1<<ADSC)) {
    }

    return ADC;
}

uint16_t readPotenciometer() {
    ADMUX &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3));
    ADMUX |= (1<<MUX1);
    return read();
}

uint16_t readHeat() {
    ADMUX &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3));
    return read();
}
