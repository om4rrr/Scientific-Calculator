/*
 * Keypad_prog.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: DELL
 */

#include "../../SERVICE/stdTypes.h"
#include "../../SERVICE/ErrorStates.h"

#include "../../MCAL/DIO/DIO_int.h"

#include "Keypad_priv.h"
#include "Keypad_config.h"


ES_t KPD_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	static u64 Local_u64Check = 0;

	/* Set Column Pins as Output */
	Local_u64Check |= (DIO_enuSetPinDirection(KPD_COLUMN0_PORT, KPD_COLUMN0_PIN, DIO_u8OUTPUT) << 0);
	Local_u64Check |= (DIO_enuSetPinDirection(KPD_COLUMN1_PORT, KPD_COLUMN1_PIN, DIO_u8OUTPUT) << 3);
	Local_u64Check |= (DIO_enuSetPinDirection(KPD_COLUMN2_PORT, KPD_COLUMN2_PIN, DIO_u8OUTPUT) << 6);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(KPD_COLUMN3_PORT, KPD_COLUMN3_PIN, DIO_u8OUTPUT) << 9);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(KPD_COLUMN4_PORT, KPD_COLUMN4_PIN, DIO_u8OUTPUT) << 12);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(KPD_COLUMN5_PORT, KPD_COLUMN5_PIN, DIO_u8OUTPUT) << 15);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(KPD_COLUMN6_PORT, KPD_COLUMN6_PIN, DIO_u8OUTPUT) << 18);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(KPD_COLUMN7_PORT, KPD_COLUMN7_PIN, DIO_u8OUTPUT) << 21);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(KPD_COLUMN8_PORT, KPD_COLUMN8_PIN, DIO_u8OUTPUT) << 24);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(KPD_COLUMN9_PORT, KPD_COLUMN9_PIN, DIO_u8OUTPUT) << 27);

	/* Set Column Pins Value as High */
	DIO_enuSetPinValue(KPD_COLUMN0_PORT, KPD_COLUMN0_PIN, DIO_u8HIGH);
	DIO_enuSetPinValue(KPD_COLUMN1_PORT, KPD_COLUMN1_PIN, DIO_u8HIGH);
	DIO_enuSetPinValue(KPD_COLUMN2_PORT, KPD_COLUMN2_PIN, DIO_u8HIGH);
	DIO_enuSetPinValue(KPD_COLUMN3_PORT, KPD_COLUMN3_PIN, DIO_u8HIGH);
	DIO_enuSetPinValue(KPD_COLUMN4_PORT, KPD_COLUMN4_PIN, DIO_u8HIGH);
	DIO_enuSetPinValue(KPD_COLUMN5_PORT, KPD_COLUMN5_PIN, DIO_u8HIGH);
	DIO_enuSetPinValue(KPD_COLUMN6_PORT, KPD_COLUMN6_PIN, DIO_u8HIGH);
	DIO_enuSetPinValue(KPD_COLUMN7_PORT, KPD_COLUMN7_PIN, DIO_u8HIGH);
	DIO_enuSetPinValue(KPD_COLUMN8_PORT, KPD_COLUMN8_PIN, DIO_u8HIGH);
	DIO_enuSetPinValue(KPD_COLUMN9_PORT, KPD_COLUMN9_PIN, DIO_u8HIGH);


	/* Set Row Pins as Input */

	Local_u64Check |= ((u64)DIO_enuSetPinDirection(KPD_ROW0_PORT, KPD_ROW0_PIN, DIO_u8INPUT) << 30);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(KPD_ROW1_PORT, KPD_ROW1_PIN, DIO_u8INPUT) << 33);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(KPD_ROW2_PORT, KPD_ROW2_PIN, DIO_u8INPUT) << 36);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(KPD_ROW3_PORT, KPD_ROW3_PIN, DIO_u8INPUT) << 39);

	/* Set Row Pins Value as Pulled_up */
	DIO_enuSetPinValue(KPD_ROW0_PORT, KPD_ROW0_PIN, DIO_u8PULL_UP);
	DIO_enuSetPinValue(KPD_ROW1_PORT, KPD_ROW1_PIN, DIO_u8PULL_UP);
	DIO_enuSetPinValue(KPD_ROW2_PORT, KPD_ROW2_PIN, DIO_u8PULL_UP);
	DIO_enuSetPinValue(KPD_ROW3_PORT, KPD_ROW3_PIN, DIO_u8PULL_UP);

	u8 Local_u8Iterator;
	for(Local_u8Iterator = 0; Local_u8Iterator <= 39; Local_u8Iterator += 3)
	{
		if(( (Local_u64Check >> Local_u8Iterator) & (0b111) ) != ES_OK)
		{
			return ES_NOK;
		}
	}

	Local_enuErrorState = ES_OK;
	return Local_enuErrorState;
}



ES_t KPD_enuGetPressedKey(u8 * Copy_Pu8PinState)
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8PressedKey = NOT_PRESSED_KEY;

	static u8 Local_Au8KPD[KPD_ROW_NUM][KPD_COLUMN_NUM] = KPD_BUTTONS;

	static u8 Local_Au8Rows [KPD_ROW_NUM][2] = KPD_ROWS;

	static u8 Local_Au8Columns [KPD_COLUMN_NUM][2] = KPD_COLUMNS;

	static u8 Local_u8Iterator1, Local_u8Iterator2;
	for(Local_u8Iterator1 = 0; Local_u8Iterator1 < KPD_COLUMN_NUM ; ++Local_u8Iterator1)
	{
		if(DIO_enuSetPinValue(Local_Au8Columns[Local_u8Iterator1][0], Local_Au8Columns[Local_u8Iterator1][1], DIO_u8LOW) != ES_OK)
		{
			return Local_enuErrorState;
		}
		for(Local_u8Iterator2 = 0; Local_u8Iterator2 < KPD_ROW_NUM; ++Local_u8Iterator2)
		{
			if(DIO_enuGetPinValue(Local_Au8Rows[Local_u8Iterator2][0], Local_Au8Rows[Local_u8Iterator2][1], Copy_Pu8PinState) != ES_OK)
			{
				return Local_enuErrorState;
			}

			if(DIO_u8LOW == *Copy_Pu8PinState)
			{
				while(DIO_u8LOW == *Copy_Pu8PinState)
				{
					DIO_enuGetPinValue(Local_Au8Rows[Local_u8Iterator2][0], Local_Au8Rows[Local_u8Iterator2][1], Copy_Pu8PinState);
				}
				*Copy_Pu8PinState = Local_Au8KPD[Local_u8Iterator2][Local_u8Iterator1];
				return ES_OK;
			}
		}
		DIO_enuSetPinValue(Local_Au8Columns[Local_u8Iterator1][0], Local_Au8Columns[Local_u8Iterator1][1], DIO_u8HIGH);
	}

	*Copy_Pu8PinState = Local_u8PressedKey;
	Local_enuErrorState = ES_OK;
	return Local_enuErrorState;
}
