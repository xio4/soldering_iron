#include "seg.h"
#include "avr/io.h"
#include "avr/iom88.h"

// PORTD segments
// 0bGF.ABCDE
// 0bGC.DEFAB

#define SEGB 0
#define SEGA 1
#define SEGF 2
#define SEGE 3
#define SEGD 4
#define SEGDT 5
#define SEGC 6
#define SEGG 7

#define SEGMENTS_COUNT 3
#define HIDDEN_SYM 0x7F

static uint8_t cell;
static char segments[SEGMENTS_COUNT];

void setCell(uint8_t idx) {
    switch (idx) {
        case 0:
            PORTC &= ~(1<<PINC3);
            PORTC |= (1<<PINC4) | (1<<PINC5);
        break;
        case 1:
            PORTC &= ~(1<<PINC4);
            PORTC |= (1<<PINC3) | (1<<PINC5);
        break;
        case 2:
            PORTC &= ~(1<<PINC5);
            PORTC |= (1<<PINC3) | (1<<PINC4);
        break;
    }
}

void showSym(char sym) {
    switch (sym) {
        case 0x30: // 0
            PORTD=(1<<SEGA) | (1<<SEGB) | (1<<SEGC) | (1<<SEGD) | (1<<SEGE) | (1<<SEGF);
        break;
        case 0x31: // 1
            PORTD=(1<<SEGB) | (1<<SEGC);
        break;
        case 0x32: // 2
            PORTD=(1<<SEGA) | (1<<SEGB) | (1<<SEGD) | (1<<SEGE) | (1<<SEGG);
        break;
        case 0x33: // 3
            PORTD=(1<<SEGA) | (1<<SEGB) | (1<<SEGC) | (1<<SEGD) | (1<<SEGG);
        break;
        case 0x34: // 4
            PORTD= (1<<SEGB) | (1<<SEGC) | (1<<SEGF) | (1<<SEGG);
        break;
        case 0x35: // 5
            PORTD=(1<<SEGA) | (1<<SEGC) | (1<<SEGD) | (1<<SEGF) | (1<<SEGG);
        break;
        case 0x36: // 6
            PORTD=(1<<SEGA) | (1<<SEGC) | (1<<SEGD) | (1<<SEGE) | (1<<SEGF) | (1<<SEGG);
        break;
        case 0x37: // 7
            PORTD=(1<<SEGA) | (1<<SEGB) | (1<<SEGC);
        break;
        case 0x38: // 8
            PORTD=(1<<SEGA) | (1<<SEGB) | (1<<SEGC) | (1<<SEGD) | (1<<SEGE) | (1<<SEGF) | (1<<SEGG);
        break;
        case 0x39: // 9
            PORTD=(1<<SEGA) | (1<<SEGB) | (1<<SEGC) | (1<<SEGD) | (1<<SEGF) | (1<<SEGG);
        break;
        case 0x2D: // "-"
            PORTD=(1<<SEGG);
        break;
        case 0x45: // E
            PORTD=(1<<SEGA) | (1<<SEGD) | (1<<SEGE) | (1<<SEGF) | (1<<SEGG);
        break;
        case 0x62: // b
            PORTD=(1<<SEGA) | (1<<SEGB) | (1<<SEGC) | (1<<SEGD) | (1<<SEGE) | (1<<SEGF) | (1<<SEGG);
        case 0x6e: // n
            PORTD=(1<<SEGA) | (1<<SEGB) | (1<<SEGC) | (1<<SEGD) | (1<<SEGE) | (1<<SEGF) | (1<<SEGG);
        break;
        case 0x6f: // o
            PORTD=(1<<SEGA) | (1<<SEGB) | (1<<SEGC) | (1<<SEGD) | (1<<SEGE) | (1<<SEGF) | (1<<SEGG);
        break;
        case 0x72: // r
            PORTD= (1<<SEGE) | (1<<SEGG);
        break;
        default:
            PORTD = 0x00;
    }

}

void setText(char* text) {
    for (uint8_t i = 0; i < SEGMENTS_COUNT; ++i) {
        char val = text[i];

        if (val == '\0') {
            segments[i] = HIDDEN_SYM;
        } else {
            segments[i] = text[i];
        }
    }
}

void showNext() {
    showSym(HIDDEN_SYM);
    setCell(cell);
    showSym(segments[cell]);
    cell++;

    if (cell >= SEGMENTS_COUNT) {
        cell = 0;
    }
}

void segInit(void) {
    cell = 0;

    for (uint8_t i = 0; i < SEGMENTS_COUNT; ++i) {
        segments[i] = HIDDEN_SYM;
    }

    DDRD = 0xFF; // write to PORTD
    DDRC |= (1<<PINC3) | (1<<PINC4) | (1<<PINC5);
}

