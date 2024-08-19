/*
 * Keypad_int.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: DELL
 */

#ifndef KEYPAD_INT_H_
#define KEYPAD_INT_H_

#define NOT_PRESSED_KEY        0xff

ES_t KPD_enuInit(void);

ES_t KPD_enuGetPressedKey(u8 * Copy_Pu8PinState);

#endif /* KEYPAD_INT_H_ */
