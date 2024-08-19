/*
 * LCD_prog.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: DELL
 */

#include "../../SERVICE/stdTypes.h"
#include "../../SERVICE/ErrorStates.h"
#include "util/delay.h"
#include "string.h"

#include "../../MCAL/DIO/DIO_int.h"

#include "LCD_priv.h"
#include "LCD_config.h"


u8 Global_u8Flag = 0;

ES_t LCD_enuInit(void)
{
	ES_t Local_ErrorState = ES_NOK;

	_delay_ms(40);

	u64 Local_u64Check = 0;

	Local_u64Check |= (DIO_enuSetPinDirection(RS_PORT, RS_PIN, DIO_u8OUTPUT) << 0);
	Local_u64Check |= (DIO_enuSetPinDirection(RW_PORT, RW_PIN, DIO_u8OUTPUT) << 3);
	Local_u64Check |= (DIO_enuSetPinDirection(EN_PORT, EN_PIN, DIO_u8OUTPUT) << 6);

	Local_u64Check |= ((u64)DIO_enuSetPinDirection(D4_PORT, D4_PIN, DIO_u8OUTPUT) << 9 );
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(D5_PORT, D5_PIN, DIO_u8OUTPUT) << 12);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(D6_PORT, D6_PIN, DIO_u8OUTPUT) << 15);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(D7_PORT, D7_PIN, DIO_u8OUTPUT) << 18);

#if LCD_MODE == EIGHT_BIT

	Local_u64Check |= ((u64)DIO_enuSetPinDirection(D0_PORT, D0_PIN, DIO_u8OUTPUT) << 21);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(D1_PORT, D1_PIN, DIO_u8OUTPUT) << 24);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(D2_PORT, D2_PIN, DIO_u8OUTPUT) << 27);
	Local_u64Check |= ((u64)DIO_enuSetPinDirection(D3_PORT, D3_PIN, DIO_u8OUTPUT) << 30);
	// Function Set
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0x38) << 33); //2 lines, 5*7 ,8 bit mode
	_delay_ms(1);

	//Display ON
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0x0f) << 36); //cursor appear, blink, LCD ON
	_delay_ms(1);

	//Display clear
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0x01) << 39);
	_delay_ms(2);

	//Set Entry mode
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0x06) << 42); // shift disabled , DDRAM address increase

	u8 Local_u8Iterator;
	for(Local_u8Iterator = 0; Local_u8Iterator <= 42; Local_u8Iterator+=3)
	{
		if(((Local_u64Check >> Local_u8Iterator) & (0b111)) != ES_OK)
		{
			return ES_NOK;
		}
	}

#elif  LCD_MODE == FOUR_BIT

	// Function Set
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0x20) << 21);
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0x20) << 24);
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0x80) << 27); //2 lines, 5*7 ,4 bit mode
	_delay_ms(1);

	//Display ON
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0x00) << 30);
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0xf0) << 33); //cursor appear, blink, LCD ON
	_delay_ms(1);

	//Display clear
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0x00) << 36);
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0x10) << 39);
	_delay_ms(2);

	//Set Entry mode
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0x00) << 42);
	Local_u64Check |= ((u64)LCD_inenuSendCommand(0x60) << 45); // shift disabled , DDRAM address increase

	u8 Local_u8Iterator;
	for(Local_u8Iterator = 0; Local_u8Iterator <= 45; Local_u8Iterator+=3)
	{
		if(((Local_u64Check >> Local_u8Iterator) & (0b111)) != ES_OK)
		{
			return ES_NOK;
		}
	}
	Global_u8Flag = 1;

#else
#error "Your LCD_Mode has wrong selection"
#endif

	Local_ErrorState = ES_OK;
	return Local_ErrorState;
}




static ES_t LCD_enuLatch(u8 Copy_u8Data)
{
	ES_t Local_ErrorState = ES_NOK;

	u32 Local_u32Check = 0;

	// Set RW as a write operation
	Local_u32Check |= (DIO_enuSetPinValue(RW_PORT, RW_PIN, DIO_u8LOW) << 0);

	//Enable is low
	Local_u32Check |= (DIO_enuSetPinValue(EN_PORT, EN_PIN, DIO_u8LOW) << 3);

	// write command
	Local_u32Check |= (     DIO_enuSetPinValue(D4_PORT, D4_PIN, (Copy_u8Data >> D4) & MASK_BIT) << 6 );
	Local_u32Check |= ((u32)DIO_enuSetPinValue(D5_PORT, D5_PIN, (Copy_u8Data >> D5) & MASK_BIT) << 9 );
	Local_u32Check |= ((u32)DIO_enuSetPinValue(D6_PORT, D6_PIN, (Copy_u8Data >> D6) & MASK_BIT) << 12);
	Local_u32Check |= ((u32)DIO_enuSetPinValue(D7_PORT, D7_PIN, (Copy_u8Data >> D7) & MASK_BIT) << 15);

#if LCD_MODE == EIGHT_BIT

	Local_u32Check |= ((u32)DIO_enuSetPinValue(D0_PORT, D0_PIN, (Copy_u8Data >> D0) & MASK_BIT) << 18);
	Local_u32Check |= ((u32)DIO_enuSetPinValue(D1_PORT, D1_PIN, (Copy_u8Data >> D1) & MASK_BIT) << 21);
	Local_u32Check |= ((u32)DIO_enuSetPinValue(D2_PORT, D2_PIN, (Copy_u8Data >> D2) & MASK_BIT) << 24);
	Local_u32Check |= ((u32)DIO_enuSetPinValue(D3_PORT, D3_PIN, (Copy_u8Data >> D3) & MASK_BIT) << 27);

	u8 Local_u8Iterator;
	for(Local_u8Iterator = 0; Local_u8Iterator <= 27; Local_u8Iterator+=3)
	{
		if(( (Local_u32Check >> Local_u8Iterator) & (0b111) ) != ES_OK)
		{
			return ES_NOK;
		}
	}
	Local_ErrorState = ES_OK;

#elif LCD_MODE == FOUR_BIT

	if(Global_u8Flag)
	{
		// Enable latch
		DIO_enuSetPinValue(EN_PORT, EN_PIN, DIO_u8HIGH);
		_delay_ms(1);
		DIO_enuSetPinValue(EN_PORT, EN_PIN, DIO_u8LOW);
		_delay_ms(1);

		Local_u32Check |= (     DIO_enuSetPinValue(D4_PORT, D4_PIN, (Copy_u8Data >> D0) & MASK_BIT) << 6 );
		Local_u32Check |= ((u32)DIO_enuSetPinValue(D5_PORT, D5_PIN, (Copy_u8Data >> D1) & MASK_BIT) << 9 );
		Local_u32Check |= ((u32)DIO_enuSetPinValue(D6_PORT, D6_PIN, (Copy_u8Data >> D2) & MASK_BIT) << 12);
		Local_u32Check |= ((u32)DIO_enuSetPinValue(D7_PORT, D7_PIN, (Copy_u8Data >> D3) & MASK_BIT) << 15);
	}

	u8 Local_u8Iterator;
	for(Local_u8Iterator = 0; Local_u8Iterator <= 15; Local_u8Iterator+=3)
	{
		if(( (Local_u32Check >> Local_u8Iterator) & (0b111) ) != ES_OK)
		{
			return ES_NOK;
		}
	}

#else
#error "Your LCD_Mode has wrong selection"
#endif

	// Enable latch
	DIO_enuSetPinValue(EN_PORT, EN_PIN, DIO_u8HIGH);
	_delay_ms(10);
	DIO_enuSetPinValue(EN_PORT, EN_PIN, DIO_u8LOW);

	Local_ErrorState = ES_OK;
	return Local_ErrorState;
}




ES_t LCD_enuDisplayString(u8 *Copy_Pu8Data)
{
	ES_t Local_ErrorState = ES_NOK;

	// Set RS as a Data Selection
	if(DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8HIGH) == ES_OK)
	{
		u8 Local_Iterator;
		for(Local_Iterator = 0; Local_Iterator < strlen(Copy_Pu8Data); ++Local_Iterator)
		{
			if(LCD_enuLatch((u8)Copy_Pu8Data[Local_Iterator]) != ES_OK)
			{
				return Local_ErrorState;
			}
		}
	}
	Local_ErrorState = ES_OK;
	return Local_ErrorState;
}




ES_t LCD_enuDisplayChar(u8  Copy_u8Char)
{
	ES_t Local_ErrorState = ES_NOK;

	// Set RS as a command
	if(DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8HIGH) == ES_OK)
	{
		Local_ErrorState = LCD_enuLatch(Copy_u8Char);
	}

	return Local_ErrorState;
}




ES_t LCD_enuClearDisplay(void)
{
	ES_t Local_ErrorState = ES_NOK;

	Local_ErrorState = LCD_inenuSendCommand(0x01);

	return Local_ErrorState;
}




ES_t LCD_enuSendCommand(u8 Copy_u8Command)
{
	ES_t Local_ErrorState = ES_NOK;

	// Set RS as a command
	if(DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8LOW) == ES_OK)
	{
		Local_ErrorState = LCD_enuLatch(Copy_u8Command);
	}

	return Local_ErrorState;
}




static inline ES_t LCD_inenuSendCommand(u8 Copy_u8Command)
{
	ES_t Local_ErrorState = ES_NOK;

	// Set RS as a command
	if(DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8LOW) == ES_OK)
	{
		Local_ErrorState = LCD_enuLatch(Copy_u8Command);
	}

	return Local_ErrorState;
}




ES_t LCD_enuSendExtraChar(u8 * Copy_Pu8ExtraChar, u8 Copy_u8NumOfChar, u8 Copy_u8WritrMode)
{
	ES_t Local_ErrorState = ES_NOK;

	Local_ErrorState = LCD_enuSendCommand(CHAR_POS);

	u8 Local_u8Iterator;
	for(Local_u8Iterator = 0; Local_u8Iterator < 8 * Copy_u8NumOfChar; ++Local_u8Iterator)
	{
		LCD_enuDisplayChar(Copy_Pu8ExtraChar[Local_u8Iterator]);
	}

	if(Copy_u8WritrMode == RIGHT_TO_LEFT)
	{

		Local_ErrorState = LCD_enuSendCommand(0x8f);// line 1   last block
		Local_ErrorState = LCD_enuSendCommand(0x04);// RIGHT_TO_LEFT

	}
	else if(Copy_u8WritrMode == LEFT_TO_RIGHT)
	{

	}
	else
	{
		Local_ErrorState = ES_OUT_OF_RANGE;
	}


	Local_ErrorState = ES_OK;
	return Local_ErrorState;
}



ES_t LCD_enuGoto(u8 Copy_u8Line,u8 Copy_u8Block)
{
	ES_t Local_ErrorState = ES_NOK;
	if (Copy_u8Line == 1)
	{
		Local_ErrorState = LCD_enuSendCommand(0x80 + Copy_u8Block);
	}
	else if(Copy_u8Line == 2)
	{
		Local_ErrorState = LCD_enuSendCommand(0x80+0x40+Copy_u8Block);
	}
	else
	{
		return ES_OUT_OF_RANGE;
	}
	return Local_ErrorState;
}


ES_t LCD_enuWriteNumber(f32 Copy_u8Num) // 0.004
{
	ES_t Local_enuErrorState = ES_NOK;
	Local_enuErrorState = DIO_enuSetPinValue(RS_PORT, RS_PIN, DIO_u8HIGH );

	if(Copy_u8Num == 0.0)
	{
		Local_enuErrorState = LCD_enuLatch('0');
		return Local_enuErrorState;
	}

	static u8 Local_Au8Digits[16] = {'0'};
	s8 Local_u8Iterator = -1;
	s32 Local_s32Number = Copy_u8Num;// 0
	if(Copy_u8Num < 0 )
	{
		LCD_enuDisplayChar('-');
		Local_s32Number *= -1;
	}
	if((Copy_u8Num < 1.0 && Copy_u8Num > 0.0) || (Copy_u8Num > -1.0 && Copy_u8Num < 0.0)) LCD_enuDisplayChar('0');

	while (Local_s32Number > 0 && Local_enuErrorState == ES_OK)
	{
		Local_u8Iterator++;
		Local_Au8Digits[Local_u8Iterator] = Local_s32Number % 10;
		Local_s32Number /= 10;
	}

	while( Local_u8Iterator >= 0 && Local_enuErrorState == ES_OK)
	{
		Local_enuErrorState = LCD_enuLatch( Local_Au8Digits[Local_u8Iterator] + '0');
		Local_u8Iterator--;
	}

	Copy_u8Num -= (s32)Copy_u8Num;
	if(Copy_u8Num != 0.0)
	{
		LCD_enuLatch('.');
		if(Copy_u8Num < 0) Copy_u8Num *= -1;
		u16 base = 10;
		while (base <= 10000)
		{
			Local_u8Iterator++;
			Local_Au8Digits[3 - Local_u8Iterator] = ((s32)(Copy_u8Num*base))%10;
			base *= 10;
		}

		while( Local_u8Iterator >= 0 && Local_enuErrorState == ES_OK)
		{
			Local_enuErrorState = LCD_enuLatch( Local_Au8Digits[Local_u8Iterator] + '0');
			Local_u8Iterator--;
		}

	}

	return Local_enuErrorState;
}
