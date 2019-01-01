#include "avr/interrupt.h"
#include "avr/iom88.h"
#include "pid.h"
#include "heat.h"
#include "utils.h"
#include "stdint.h"

#define CONSERVATIVE_MODE 0
#define AGGRESSIVE_MODE 1

static struct PID_DATA pidData;

void heatInit(void) {
    TCCR1A = 0x00;
    TCCR1B = 0x00;
    TCCR1A |= (1<<COM1A1); //Clear OC1A on compare match
    TCCR1A |= (1<<WGM11); // Fast PWM 16bit ICR1 is TOP
    TCCR1B |= (1<<WGM12) | (1<<WGM13); // Fast PWM 16bit
    TCCR1B |= (1<<CS11); //prescaler/8 244.14/8Hz
    DDRB |= (1<<PINB1); // PB1 as output
    ICR1 = 40000;

    pid_Init(CONS_KP, CONS_KI, CONS_KD, &pidData);
}

void heatUpdate(uint16_t heatTemp, uint16_t adjustTemp) {
    int16_t coef = pid_Controller(adjustTemp, heatTemp, &pidData);

    if (coef < 0) {
        OCR1A = 1;
    } else {
        OCR1A = coef * PWM_COEF;
    }
}

ISR(TIMER1_COMPA_vect) {

}
