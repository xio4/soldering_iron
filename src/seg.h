#ifndef SEG_H
#define SEG_H

#include "stdint.h"

extern void setCell(uint8_t idx);
extern void segInit(void);
extern void showSym(char sym);
extern void setText(char* text);
extern void showNext(void);

#endif // SEG_H
