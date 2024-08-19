#ifndef MACRO_TO_FUNCTIONS_H
#define MACRO_TO_FUNCTIONS_H

#define Get_Bit(reg, bit) ((reg >> (bit)) & 1)

#define Set_Bit(reg, bit) reg |= (1 << (bit))

#define Clr_Bit(reg, bit) reg &= ~(1 << (bit))

#define Toggle_Bit(reg, bit) reg ^= (1 << (bit))

#endif
