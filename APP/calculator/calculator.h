/*
 * calculator.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: DELL
 */

#ifndef APP_CALCULATOR_CALCULATOR_H_
#define APP_CALCULATOR_CALCULATOR_H_


#define  NUM_OF_BITS               16
#define  NUM_OF_OPERANDS           12
#define  NUM_OF_OPERATORS          11

#define LEFT_SHIFT                 0x18
#define RIGHT_SHIFT                0x1c

void Local_voidCalCulator(void);

void Local_voidNumberRepresentation(void);

void Local_voidClrArray(u8 * Copy_Pu8Array, u8 Copy_u8Length);

void Local_voidCalculateFunctions(void);

void Local_voidDisplayFunction(void);

void Local_voidGetOperand(void);

void Local_voidDisplayOutput(void);

void Local_ClearDiplayedText(void);

void Local_voidMathError(void);

void Local_voidSyntaxError(void);

void Local_voidShiftText(void);

void Local_SetAnswer(void);

void Local_CalculateShiftIterator(void);


#endif /* APP_CALCULATOR_CALCULATOR_H_ */
