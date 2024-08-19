/*
 * LCD_int.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: DELL
 */

#ifndef LCD_INT_H_
#define LCD_INT_H_

#include "../../SERVICE/stdTypes.h"
#include "../../SERVICE/ErrorStates.h"


ES_t LCD_enuInit(void);

ES_t LCD_enuDisplayString(u8 *Copy_Pu8Data);

ES_t LCD_enuClearDisplay(void);

ES_t LCD_enuSendCommand(u8 Copy_u8Command);

ES_t LCD_enuDisplayChar(u8  Copy_u8Char);

ES_t LCD_enuSendExtraChar(u8 * Copy_Pu8ExtraChar, u8 Copy_u8NumOfChar, u8 Copy_u8WritrMode);

ES_t LCD_enuWriteNumber(f32 Copy_u8num);


#endif /* LCD_INT_H_ */
