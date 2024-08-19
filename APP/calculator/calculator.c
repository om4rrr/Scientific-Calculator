/*
 * calculator.c
 *
 *  Created on: ??‏/??‏/????
 *      Author: DELL
 */

#include "../../SERVICE/stdTypes.h"
#include "../../SERVICE/ErrorStates.h"
#include "../../SERVICE/Macro_To_Function.h"
#include <util/delay.h>
#include "../../SERVICE/Math_Functions.h"

#include "../../MCAL/DIO/DIO_int.h"

#include "../../HAL/LCD/LCD_int.h"
#include "../../HAL/LCD/LCD_config.h"
#include "../../HAL/Keypad/Keypad_int.h"

#include "calculator.h"


//             0         1        2                3              4             5         6      7        8           9           10           11           12
//          negative    dot   math_error      last_is_num     last_is_op     high_pre    ans   number   alpha   last_is_dec  last_is_hex  last_is_bin  last_is_oct

static u16 Global_u16Flags = 0;

enum {
	negative,
	dot,
	math_error,
	last_is_num,
	last_is_op,
	High_Pre,
	ans,
	number_repres,
	alpha,
	last_is_hex,
	last_is_dec,
	last_is_bin,
	last_is_oct,
	syntax_error,
	division_error
};

static s8 Global_s8Op_Iterator = 0, Global_s8Num_Iterator = -1, Global_s8IterOfNumberRepres = NUM_OF_BITS - 1, Global_u8TypeOfNumber;

static f32 Local_u8DotCalc = 10.0, Local_f32ANS;

static f32 Global_Af32Operands[NUM_OF_OPERANDS], Local_f32Mode = 1;
static u8 Global_u8Operators[NUM_OF_OPERATORS], Global_Au8NumberRepresentation[NUM_OF_BITS], Global_u8PressedKey = NOT_PRESSED_KEY;
static s8 Global_u8ShiftIter = 0, Global_u8CountingLeftShift = 0;


void Local_voidCalCulator(void)
{
	DIO_enuInit(); // Initialize DIO and Keypad
	KPD_enuInit();

	while(KPD_enuGetPressedKey(&Global_u8PressedKey) == ES_OK && Global_u8PressedKey != 'c'); // Wait to press ON button to turn on keypad
	LCD_enuInit();

	Local_voidClrArray(Global_Au8NumberRepresentation, NUM_OF_BITS); // Initialize Array by '0'


	while(1)
	{
		Global_u8PressedKey = NOT_PRESSED_KEY;
		while(KPD_enuGetPressedKey(&Global_u8PressedKey) == ES_OK && Global_u8PressedKey == NOT_PRESSED_KEY); // Wait to press any button

		Local_voidShiftText(); // Shift text to left when lcd screen is full..

		if(Global_u8PressedKey == 'k') Set_Bit(Global_u16Flags, alpha); // Set alpha mode to write number in hex. or get output in rad..

		else if(Global_u8PressedKey == 'S' )
		{
			LCD_enuDisplayString("Pi");
			Global_Af32Operands[++Global_s8Num_Iterator] = M_PI;
		}
		else if(Global_u8PressedKey == 'A')
		{
			Local_f32Mode = 1;
		}
		else if(Global_u8PressedKey == 'B' )
		{
			Local_f32Mode = M_PI/180;
		}
		else if(Get_Bit(Global_u16Flags, number_repres) && !Get_Bit(Global_u16Flags, last_is_dec) // Take Number in (hex. or oct. or bin.) that be convert to other number_systems
				&& ( (Global_u8PressedKey >= '0' && Global_u8PressedKey <= '9')
						|| ( Global_u8PressedKey >= 'A' && Global_u8PressedKey <= 'F' &&  Get_Bit(Global_u16Flags, alpha)) ) )
		{
			if(!Get_Bit(Global_u16Flags, last_is_hex))
			{
				if(Get_Bit(Global_u16Flags, last_is_oct)  && !(Global_u8PressedKey >= '0'  && Global_u8PressedKey <= '7')) Set_Bit(Global_u16Flags, syntax_error);
				if(Get_Bit(Global_u16Flags, last_is_bin)  &&  (Global_u8PressedKey != '0'  && Global_u8PressedKey != '1')) Set_Bit(Global_u16Flags, syntax_error);
			}

			LCD_enuDisplayChar(Global_u8PressedKey);
			for(u8 Local_u8Iter = Global_s8IterOfNumberRepres; Local_u8Iter < NUM_OF_BITS - 1; ++Local_u8Iter)
			{
				Global_Au8NumberRepresentation[Local_u8Iter] = Global_Au8NumberRepresentation[Local_u8Iter + 1];
			}
			Global_Au8NumberRepresentation[NUM_OF_BITS - 1] = Global_u8PressedKey;
			Global_s8IterOfNumberRepres--;
		}
		else if(Global_u8PressedKey >= '0' && Global_u8PressedKey <= '9') // Take Operands in deci.
		{
			Local_voidGetOperand();
		}
		else if(Global_u8PressedKey == '.') // Take dot and set flag dot..
		{
			LCD_enuDisplayChar(Global_u8PressedKey);
			Set_Bit(Global_u16Flags, dot);
			Local_u8DotCalc = 10.0;
		}
		else if(Global_u8PressedKey == '?') // Set Answer and Display "ANS"..
		{
			Local_SetAnswer();
		}
		else
		{

			//Clr_Bit(Global_u16Flags, negative); Clr_Bit(Global_u16Flags, dot);

			if(Get_Bit(Global_u16Flags, High_Pre))
			{
				if(Global_u8PressedKey == '-' && Get_Bit(Global_u16Flags, last_is_op))
				{
					LCD_enuDisplayChar(Global_u8PressedKey);
					Set_Bit(Global_u16Flags, negative);
					continue;
				}
				Local_voidCalculateFunctions();// Calculate Specific functions..
			}

			if(Global_u8PressedKey == '-')
			{
				LCD_enuDisplayChar(Global_u8PressedKey);
				if(Get_Bit(Global_u16Flags, last_is_op) || (!Get_Bit(Global_u16Flags, last_is_num) && !Global_s8Op_Iterator)) // Check the number is negative or not..
				{
					Set_Bit(Global_u16Flags, negative);
					continue;
				}
			}

			if(Get_Bit(Global_u16Flags, High_Pre)) Local_voidCalculateFunctions();// Calculate Specific functions..

			if(Global_u8PressedKey == '+')
			{
				LCD_enuDisplayChar(Global_u8PressedKey);
				if(Get_Bit(Global_u16Flags, last_is_op) || (!Get_Bit(Global_u16Flags, last_is_num) && !Global_s8Op_Iterator)) // Check the number is negative or not..
				{
					continue;
				}
			}
			Clr_Bit(Global_u16Flags, negative); Clr_Bit(Global_u16Flags, dot);

			Set_Bit(Global_u16Flags, last_is_op); Clr_Bit(Global_u16Flags, last_is_num);

			if(Global_u8PressedKey >= 'G' && Global_u8PressedKey <= 'S') Local_voidDisplayFunction(); // Display Specific functions..

			// set last is operator and clear that last is number
			if(Global_u8PressedKey != '=')
			{
				Global_u8Operators[Global_s8Op_Iterator] = Global_u8PressedKey; // Set operator in operators array..
				Global_s8Op_Iterator++;
			}


			if(Global_u8PressedKey >= 'C' && Global_u8PressedKey <= 'F') { Local_voidNumberRepresentation(); continue; } // Number Representation

			if(Global_u8PressedKey != '=') { Clr_Bit(Global_u16Flags, number_repres); Clr_Bit(Global_u16Flags, alpha);}

			if(Global_u8PressedKey == 'n') Local_ClearDiplayedText();  // Clear displayed text..

			else if(Global_u8PressedKey == '=') { Local_voidDisplayOutput();}// Calculate Output and display it..

			else if(Global_u8PressedKey == 'X' || Global_u8PressedKey == '/') // Display mul. operator or div. operator
			{
				LCD_enuDisplayChar(Global_u8PressedKey);
			}
		}
	}
}

void Local_voidShiftText(void)
{
	if( (Global_u8PressedKey >= 'L' && Global_u8PressedKey <= 'Q') || Global_u8PressedKey == 'H') Global_u8ShiftIter += 5;

	if( (Global_u8PressedKey >= '0' && Global_u8PressedKey <= '9')
			|| Global_u8PressedKey == '+' || Global_u8PressedKey == '-'
					|| Global_u8PressedKey == 'X' || Global_u8PressedKey == '/'
							|| Global_u8PressedKey == 'R' || Global_u8PressedKey == '.'	) Global_u8ShiftIter++;

	else if(Global_u8PressedKey == '?') Global_u8ShiftIter += 3;
	else if(Global_u8PressedKey == 'S' || Global_u8PressedKey == 'G') Global_u8ShiftIter += 2;
	else if(Global_u8PressedKey >= 'I' && Global_u8PressedKey <= 'K') Global_u8ShiftIter += 4;

	if(Global_u8ShiftIter > 16)
	{
		for(; Global_u8ShiftIter > 16; --Global_u8ShiftIter)
		{
			LCD_enuSendCommand(LEFT_SHIFT);
			Global_u8CountingLeftShift++;
		}
	}
}


void Local_ClearDiplayedText(void)
{
	LCD_enuClearDisplay(); // Clear Displayed Text on screen
	Local_voidClrArray(Global_Af32Operands, NUM_OF_OPERANDS); // Clear Operands array to use it again..
	Local_voidClrArray(Global_u8Operators, NUM_OF_OPERATORS); // Clear Operators array to use it again..
	Local_voidClrArray(Global_Au8NumberRepresentation, NUM_OF_BITS); // Clear NumberRepresentation array to use it again..
	Global_s8Op_Iterator = 0, Global_s8Num_Iterator = -1;
	if(Global_u8CountingLeftShift)
	{
		for(; Global_u8CountingLeftShift >= 0; --Global_u8CountingLeftShift) LCD_enuSendCommand(RIGHT_SHIFT); // Shift text to right
	}
	LCD_enuGoto(1, 0); // return cursor in 1st block in 1st line..
	Global_u8ShiftIter = 0, Global_u8CountingLeftShift = 0;
	Clr_Bit(Global_u16Flags, last_is_op);
	Clr_Bit(Global_u16Flags, last_is_num);
}



void Local_voidDisplayOutput(void)
{
	if(Get_Bit(Global_u16Flags, math_error)) { Local_voidMathError(); return; } // Check Math Error

	else if(Get_Bit(Global_u16Flags, syntax_error)) { Local_voidSyntaxError(); return; }// Check Syntax Error

	else if(Get_Bit(Global_u16Flags, number_repres)) // Display number representation mode
	{
		LCD_enuGoto(2,0);
		if(Global_u8TypeOfNumber == 'C')
		{
			u32 tmp = Global_Af32Operands[0];
			u8 cnt = 0;
			while(tmp) // calculate number of digits
			{
				tmp/=10;
				cnt++;
				LCD_enuDisplayChar(' ');
			}
			LCD_enuGoto(2, 16 - cnt); // leave number of digits from blocks
			LCD_enuWriteNumber(Global_Af32Operands[0]);
		}
		else { LCD_enuDisplayString(Global_Au8NumberRepresentation); Clr_Bit(Global_u16Flags, alpha); Global_s8IterOfNumberRepres = NUM_OF_BITS - 1;}//
	}
	else // 5^4 - 100 + sqrt(4) + 2 * cos(60)
	{
		s8 Local_u8Iter1, Local_u8Iter2;
		for(Local_u8Iter1 = 0; Local_u8Iter1 < NUM_OF_OPERATORS - 1; ++Local_u8Iter1) // Iterate all the elements and calculate all the multiplication and division operations
		{
			if(Global_u8Operators[Local_u8Iter1] == 'X' || Global_u8Operators[Local_u8Iter1] == '/')
			{
				if(Global_u8Operators[Local_u8Iter1] == 'X') Global_Af32Operands[Local_u8Iter1] *= Global_Af32Operands[Local_u8Iter1 + 1]; // calculate  mul. operation and assign res.
				else
				{
					if(Global_Af32Operands[Local_u8Iter1 + 1] == 0) // Check that divisor is not zero..
					{
						Local_voidMathError();// Calling function that display Math Error..
						Set_Bit(Global_u16Flags, division_error);// Set flag division_error to skip remaining operations
						break;
					}
					Global_Af32Operands[Local_u8Iter1] /= Global_Af32Operands[Local_u8Iter1 + 1]; // calculate  div. operation and assign res.
				}

				Global_Af32Operands[Local_u8Iter1 + 1] = 0;
				Global_u8Operators[Local_u8Iter1] = 0;

				for(Local_u8Iter2 = Local_u8Iter1 + 1; Local_u8Iter2 < NUM_OF_OPERANDS - 1; ++Local_u8Iter2) // Shifting all operands to left
				{
					Global_Af32Operands[Local_u8Iter2] = Global_Af32Operands[Local_u8Iter2 + 1];
				}
				for(Local_u8Iter2 = Local_u8Iter1 ; Local_u8Iter2 < NUM_OF_OPERATORS - 1; ++Local_u8Iter2) // Shifting all operators to left
				{
					Global_u8Operators[Local_u8Iter2] = Global_u8Operators[Local_u8Iter2 + 1];
				}
				Local_u8Iter1--;// back one step after Shifting to calculate new operation
			}
		}

		if(Get_Bit(Global_u16Flags, division_error)) return;// check flag division_error to skip remaining operations

		while(Global_u8Operators[0] == '+' || Global_u8Operators[0] == '-') // calculate all the Addition and Subtraction operations
		{
			if(Global_u8Operators[0] == '+') Global_Af32Operands[0] += Global_Af32Operands[1]; // calculate  Add. operation and assign res.
			else Global_Af32Operands[0] -= Global_Af32Operands[1]; // calculate  Sub. operation and assign res.

			Global_Af32Operands[1] = 0;
			Global_u8Operators[0] = 0;

			for(Local_u8Iter2 = 1; Local_u8Iter2 < NUM_OF_OPERANDS - 1; ++Local_u8Iter2) // Shifting all operands to left
			{
				Global_Af32Operands[Local_u8Iter2] = Global_Af32Operands[Local_u8Iter2 + 1];
			}
			for(Local_u8Iter2 = 0 ; Local_u8Iter2 < NUM_OF_OPERATORS - 1; ++Local_u8Iter2) // Shifting all operators to left
			{
				Global_u8Operators[Local_u8Iter2] = Global_u8Operators[Local_u8Iter2 + 1];
			}
		}
		f32 tmp = Global_Af32Operands[0];// 25.000000
		if(tmp < 0.0) tmp*=-1;
		u8 cnt = 1;
		while(tmp > 10.0) // calculate number of digits
		{
			tmp /= 10;
			cnt++;
		}

		if(Global_u8CountingLeftShift == 1) LCD_enuSendCommand(RIGHT_SHIFT);
		else if(Global_u8CountingLeftShift > 1)
		{
			for(; Global_u8CountingLeftShift >= 0; --Global_u8CountingLeftShift) LCD_enuSendCommand(RIGHT_SHIFT); // Shift text to right
		}


		if(Global_Af32Operands[0] - (s32)Global_Af32Operands[0] != 0)
		{
			if(Global_Af32Operands[0] < 0) LCD_enuGoto(2, NUM_OF_BITS - cnt - 5); // If the number is a decimal fraction, leave 5 blocks plus number of digits
			else LCD_enuGoto(2, NUM_OF_BITS - cnt - 4);
		}
		else
		{
			if(Global_Af32Operands[0] < 0) LCD_enuGoto(2, NUM_OF_BITS - cnt - 1); // leave number of digits from blocks
			else LCD_enuGoto(2, NUM_OF_BITS - cnt) ;
		}
		LCD_enuWriteNumber(Global_Af32Operands[0]); // Display The Result
		Local_f32ANS = Global_Af32Operands[0]; // Set res. in ANS
		Global_Af32Operands[0] = 0;// Set remaining elements to zero to ready to new operations
		Global_s8Op_Iterator = 0, Global_s8Num_Iterator = -1; Global_u8ShiftIter = 0, Global_u8CountingLeftShift = 0;
	}
}

void Local_voidNumberRepresentation(void)
{
	Global_u8TypeOfNumber = Global_u8PressedKey;
	LCD_enuClearDisplay();
	Global_u8ShiftIter = 0, Global_u8CountingLeftShift = 0;

	if(!Get_Bit(Global_u16Flags, number_repres))  // Check Number_System Mode On or Off
	{
		Set_Bit(Global_u16Flags, number_repres); // activate Number_System mode
		LCD_enuGoto(1, 13); // Display Number_System(HEX, BIN, OCT, DEC) in last three blocks in 1st line
		switch(Global_u8PressedKey)
		{
		case 'C':
			LCD_enuDisplayString("DEC");
			Set_Bit(Global_u16Flags, last_is_dec);
			Clr_Bit(Global_u16Flags, last_is_hex);
			Clr_Bit(Global_u16Flags, last_is_bin);
			Clr_Bit(Global_u16Flags, last_is_oct);
			break;
		case 'D':
			LCD_enuDisplayString("HEX");
			Set_Bit(Global_u16Flags, last_is_hex);
			Clr_Bit(Global_u16Flags, last_is_dec);
			Clr_Bit(Global_u16Flags, last_is_bin);
			Clr_Bit(Global_u16Flags, last_is_oct);
			break;
		case 'E':
			LCD_enuDisplayString("BIN");
			Set_Bit(Global_u16Flags, last_is_bin);
			Clr_Bit(Global_u16Flags, last_is_hex);
			Clr_Bit(Global_u16Flags, last_is_dec);
			Clr_Bit(Global_u16Flags, last_is_oct);
			break;
		case 'F':
			LCD_enuDisplayString("OCT");
			Set_Bit(Global_u16Flags, last_is_oct);
			Clr_Bit(Global_u16Flags, last_is_hex);
			Clr_Bit(Global_u16Flags, last_is_bin);
			Clr_Bit(Global_u16Flags, last_is_dec);
			break;
		}
		LCD_enuGoto(2, 0); // Display Number in first block in 2nd line
	}
	else
	{
		LCD_enuGoto(1, 13); // Display Number_System(HEX, BIN, OCT, DEC) in last three blocks in 1st line

		if(Get_Bit(Global_u16Flags, last_is_dec)) // Check Current number_system representation is decimal or not
		{
			if(Global_u8PressedKey != 'C')
			{
				Clr_Bit(Global_u16Flags, last_is_dec);

				Local_voidClrArray(Global_Au8NumberRepresentation, NUM_OF_BITS); // Clear Array to use it again in converting process

				switch(Global_u8PressedKey)
				{
				case 'D':
					LCD_enuDisplayString("HEX");
					Dec2Hex((u16)Global_Af32Operands[0], Global_Au8NumberRepresentation); // Convert Decimal to Hexa and assign it in global array
					Set_Bit(Global_u16Flags, last_is_hex);
					break;
				case 'E':
					LCD_enuDisplayString("BIN");
					Dec2Bin((u16)Global_Af32Operands[0], Global_Au8NumberRepresentation);// Convert Decimal to Binary and assign it in global array
					Set_Bit(Global_u16Flags, last_is_bin);
					break;
				case 'F':
					LCD_enuDisplayString("OCT");
					Dec2Oct((u16)Global_Af32Operands[0], Global_Au8NumberRepresentation);// Convert Decimal to Octal and assign it in global array
					Set_Bit(Global_u16Flags, last_is_oct);
					break;
				}
			}
			else return;//LCD_enuDisplayString("DEC");
		}
		else if(Get_Bit(Global_u16Flags, last_is_hex))// Check Current number_system representation is hexa_decimal or not
		{
			if(Global_u8PressedKey != 'D')
			{
				Clr_Bit(Global_u16Flags, last_is_hex);

				Global_Af32Operands[0] = Hex2Dec(Global_Au8NumberRepresentation);// Convert Hexa to Decimal and assign it in first element global operands array
				Local_voidClrArray(Global_Au8NumberRepresentation, NUM_OF_BITS);// Clear Array to use it again in converting process

				switch(Global_u8PressedKey)
				{
				case 'C':
					LCD_enuDisplayString("DEC");
					Set_Bit(Global_u16Flags, last_is_dec);
					break;
				case 'E':
					LCD_enuDisplayString("BIN");
					Dec2Bin((u16)Global_Af32Operands[0], Global_Au8NumberRepresentation);// Convert Decimal to Binary and assign it in global array
					Set_Bit(Global_u16Flags, last_is_bin);
					break;
				case 'F':
					LCD_enuDisplayString("OCT");
					Dec2Oct((u16)Global_Af32Operands[0], Global_Au8NumberRepresentation);// Convert Decimal to octal and assign it in global array
					Set_Bit(Global_u16Flags, last_is_oct);
					break;
				}
			}
			else LCD_enuDisplayString("HEX");
		}
		else if(Get_Bit(Global_u16Flags, last_is_bin)) // Check Current number_system representation is binary or not
		{
			if(Global_u8PressedKey != 'E')
			{
				Clr_Bit(Global_u16Flags, last_is_bin);

				Global_Af32Operands[0] = Bin2Dec(Global_Au8NumberRepresentation);// Convert binary to Decimal and assign it in first element global operands array
				Local_voidClrArray(Global_Au8NumberRepresentation, NUM_OF_BITS); // Clear Array to use it again in converting process

				switch(Global_u8PressedKey)
				{
				case 'C':
					LCD_enuDisplayString("DEC");
					Set_Bit(Global_u16Flags, last_is_dec);
					break;
				case 'D':
					LCD_enuDisplayString("HEX");
					Dec2Hex((u16)Global_Af32Operands[0], Global_Au8NumberRepresentation);// Convert Decimal to hexa and assign it in global array
					Set_Bit(Global_u16Flags, last_is_hex);
					break;
				case 'F':
					LCD_enuDisplayString("OCT");
					Dec2Oct((u16)Global_Af32Operands[0], Global_Au8NumberRepresentation);// Convert Decimal to octal and assign it in global array
					Set_Bit(Global_u16Flags, last_is_oct);
					break;
				}
			}
			else LCD_enuDisplayString("BIN");
		}
		else //  Current number_system representation is octal
		{
			if(Global_u8PressedKey != 'F')
			{
				Clr_Bit(Global_u16Flags, last_is_oct);

				Global_Af32Operands[0] = Oct2Dec(Global_Au8NumberRepresentation);// Convert octal to Decimal and assign it in first element global operands array
				Local_voidClrArray(Global_Au8NumberRepresentation, NUM_OF_BITS); // Clear Array to use it again in converting process

				switch(Global_u8PressedKey)
				{
				case 'C':
					LCD_enuDisplayString("DEC");
					Set_Bit(Global_u16Flags, last_is_dec);
					break;
				case 'D':
					LCD_enuDisplayString("HEX");
					Dec2Hex((u16)Global_Af32Operands[0], Global_Au8NumberRepresentation);// Convert Decimal to hexa and assign it in global array
					Set_Bit(Global_u16Flags, last_is_hex);
					break;
				case 'E':
					LCD_enuDisplayString("BIN");
					Dec2Bin((u16)Global_Af32Operands[0], Global_Au8NumberRepresentation);// Convert Decimal to binary and assign it in global array
					Set_Bit(Global_u16Flags, last_is_bin);
					break;
				}
			}
			else LCD_enuDisplayString("OCT");
		}

		LCD_enuGoto(2, 0);// Display Number in first block in 2nd line
		if(Global_u8PressedKey == 'C')
		{
			u32 tmp = Global_Af32Operands[0];
			u8 cnt = 0;
			while(tmp) // calculate number of digits
			{
				tmp/=10;
				cnt++;
				LCD_enuDisplayChar(' ');
			}
			LCD_enuGoto(2, 16 - cnt); // leave number of digits from blocks
			LCD_enuWriteNumber(Global_Af32Operands[0]);// Display Decimal Number
		}
		else
		{
			LCD_enuDisplayString(Global_Au8NumberRepresentation);// Display (Hexa or Binary or Octal) Number
		}
	}
}


void Local_voidCalculateFunctions(void)
{

	Clr_Bit(Global_u16Flags, High_Pre);
	Global_s8Op_Iterator--;

	if(Get_Bit(Global_u16Flags, negative) && Global_u8Operators[Global_s8Op_Iterator] == 'H')
	{
		LCD_enuClearDisplay();
		Set_Bit(Global_u16Flags, math_error);
		if(Global_u8ShiftIter >= 16) LCD_enuSendCommand(0x18);
		LCD_enuDisplayChar(')');
		return;
	}
	else
	{
		if(Global_u8Operators[Global_s8Op_Iterator] == 'L' || Global_u8Operators[Global_s8Op_Iterator] == 'M')
		{
			if(Global_Af32Operands[Global_s8Num_Iterator] > 1 || Global_Af32Operands[Global_s8Num_Iterator] < -1)
			{
				Set_Bit(Global_u16Flags, math_error);
				Global_Af32Operands[Global_s8Num_Iterator] = 0;
			}
		}
		if(Global_u8Operators[Global_s8Op_Iterator] == 'K')
		{
			if(Local_f32Mode != 1)
			{
				Global_Af32Operands[Global_s8Num_Iterator] *= 180/M_PI;
			}
			if((s32)Global_Af32Operands[Global_s8Num_Iterator]%360 == 90  || (s32)Global_Af32Operands[Global_s8Num_Iterator]%360 == -90
					|| (s32)Global_Af32Operands[Global_s8Num_Iterator]%360 == 270 || (s32)Global_Af32Operands[Global_s8Num_Iterator]%360 == -270)
			{
				Set_Bit(Global_u16Flags, math_error);
				Global_Af32Operands[Global_s8Num_Iterator] = 0;
			}
			if(Local_f32Mode != 1)
			{
				Global_Af32Operands[Global_s8Num_Iterator] *= Local_f32Mode;
			}
		}
		if(Global_u8Operators[Global_s8Op_Iterator] != 'G' &&  Global_u8Operators[Global_s8Op_Iterator] != 'R')
		{
			if(Global_u8ShiftIter >= 16) LCD_enuSendCommand(0x18);
			Global_u8ShiftIter++;
			LCD_enuDisplayChar(')');
		}
		switch(Global_u8Operators[Global_s8Op_Iterator])
		{
		case 'I':
			Global_Af32Operands[Global_s8Num_Iterator] = _sin(Global_Af32Operands[Global_s8Num_Iterator] * ( (M_PI/180) /Local_f32Mode) );
			break;
		case 'J':
			Global_Af32Operands[Global_s8Num_Iterator] = _cos(Global_Af32Operands[Global_s8Num_Iterator] * ( (M_PI/180) /Local_f32Mode) );
			break;
		case 'K':
			Global_Af32Operands[Global_s8Num_Iterator] = _tan(Global_Af32Operands[Global_s8Num_Iterator] * ( (M_PI/180) /Local_f32Mode));
			break;
		case 'G':
			Global_Af32Operands[Global_s8Num_Iterator] = expo(Global_Af32Operands[Global_s8Num_Iterator]);
			break;
		case 'H':
			Global_Af32Operands[Global_s8Num_Iterator] = _sqrt(Global_Af32Operands[Global_s8Num_Iterator]);
			break;
		case 'L':
			Global_Af32Operands[Global_s8Num_Iterator] = _asin(Global_Af32Operands[Global_s8Num_Iterator] ) * Local_f32Mode;
			break;
		case 'M':
			Global_Af32Operands[Global_s8Num_Iterator] = _acos(Global_Af32Operands[Global_s8Num_Iterator] ) * Local_f32Mode;
			break;
		case 'N':
			Global_Af32Operands[Global_s8Num_Iterator] = _atan(Global_Af32Operands[Global_s8Num_Iterator] ) * Local_f32Mode;
			break;
		case 'O':
			Global_Af32Operands[Global_s8Num_Iterator] = _sinh(Global_Af32Operands[Global_s8Num_Iterator]);
			break;
		case 'P':
			Global_Af32Operands[Global_s8Num_Iterator] = _cosh(Global_Af32Operands[Global_s8Num_Iterator]);
			break;
		case 'Q':
			Global_Af32Operands[Global_s8Num_Iterator] = _tanh(Global_Af32Operands[Global_s8Num_Iterator]);
			break;
		case 'R':
			Global_Af32Operands[Global_s8Num_Iterator - 1] = _pow(Global_Af32Operands[Global_s8Num_Iterator - 1], (s8)Global_Af32Operands[Global_s8Num_Iterator]);
			Global_Af32Operands[Global_s8Num_Iterator] = 0;
			Global_s8Num_Iterator--;
			break;
		}
	}

}

void Local_voidDisplayFunction(void)
{
	Set_Bit(Global_u16Flags, High_Pre);
	switch(Global_u8PressedKey)
	{
	case 'I':
		LCD_enuDisplayString("sin(");
		break;
	case 'J':
		LCD_enuDisplayString("cos(");
		break;
	case 'K':
		LCD_enuDisplayString("tan(");
		break;
	case 'G':
		LCD_enuDisplayString("e^");
		break;
	case 'H':
		LCD_enuDisplayString("sqrt(");
		break;
	case 'L':
		LCD_enuDisplayString("asin(");
		break;
	case 'M':
		LCD_enuDisplayString("acos(");
		break;
	case 'N':
		LCD_enuDisplayString("atan(");
		break;
	case 'O':
		LCD_enuDisplayString("sinh(");
		break;
	case 'P':
		LCD_enuDisplayString("cosh(");
		break;
	case 'Q':
		LCD_enuDisplayString("tanh(");
		break;
	case 'R':
		LCD_enuDisplayChar('^');
		break;
	}
}

void Local_voidGetOperand(void)
{
	LCD_enuDisplayChar(Global_u8PressedKey);

	if(!Get_Bit(Global_u16Flags, last_is_num)) Global_s8Num_Iterator++;

	Set_Bit(Global_u16Flags, last_is_num);
	Clr_Bit(Global_u16Flags, last_is_op);

	if(!Get_Bit(Global_u16Flags, dot)) Global_Af32Operands[Global_s8Num_Iterator] *= 10;

	if(Get_Bit(Global_u16Flags, negative) && Get_Bit(Global_u16Flags, dot))
	{
		Global_Af32Operands[Global_s8Num_Iterator] -= (Global_u8PressedKey - '0')/Local_u8DotCalc;
		Local_u8DotCalc *= 10;
	}
	else if(Get_Bit(Global_u16Flags, negative))
	{
		Global_Af32Operands[Global_s8Num_Iterator] -= (Global_u8PressedKey - '0');
	}
	else if(Get_Bit(Global_u16Flags, dot))
	{
		Global_Af32Operands[Global_s8Num_Iterator] += (Global_u8PressedKey - '0')/Local_u8DotCalc;
		Local_u8DotCalc *= 10;
	}
	else
	{
		Global_Af32Operands[Global_s8Num_Iterator] += (Global_u8PressedKey - '0');
	}
}

void Local_voidClrArray(u8 * Copy_Pu8Array, u8 Copy_u8Length)
{
	while(Copy_u8Length)
	{
		*Copy_Pu8Array++ = '0';
		Copy_u8Length--;
	}
}

void Local_voidMathError(void)
{
	LCD_enuClearDisplay();
	LCD_enuDisplayString("Math ERROR");
	_delay_ms(1000);
	Local_ClearDiplayedText();
	Clr_Bit(Global_u16Flags, math_error);
}

void Local_voidSyntaxError(void)
{
	LCD_enuClearDisplay();
	LCD_enuDisplayString("Syntax ERROR");
	_delay_ms(1000);
	Local_ClearDiplayedText();
	Local_voidClrArray(Global_Au8NumberRepresentation, NUM_OF_BITS);
	Clr_Bit(Global_u16Flags, number_repres);
	Clr_Bit(Global_u16Flags, syntax_error);
}

void Local_SetAnswer(void)
{
	Set_Bit(Global_u16Flags, ans);
	LCD_enuDisplayString("ANS");
	Global_s8Num_Iterator++;
	Global_Af32Operands[Global_s8Num_Iterator] = Local_f32ANS;
	Clr_Bit(Global_u16Flags, ans);
	Set_Bit(Global_u16Flags, last_is_num);
	Clr_Bit(Global_u16Flags, last_is_op);
}
