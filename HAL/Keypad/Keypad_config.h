/*
 * Keypad_config.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: DELL
 */

#ifndef KEYPAD_CONFIG_H_
#define KEYPAD_CONFIG_H_

#define NOT_PRESSED_KEY        0xff


#define KPD_ROW0_PORT           DIO_u8PORTA
#define KPD_ROW0_PIN            DIO_u8PIN7

#define KPD_ROW1_PORT           DIO_u8PORTB
#define KPD_ROW1_PIN            DIO_u8PIN0

#define KPD_ROW2_PORT           DIO_u8PORTB
#define KPD_ROW2_PIN            DIO_u8PIN1

#define KPD_ROW3_PORT           DIO_u8PORTB
#define KPD_ROW3_PIN            DIO_u8PIN2

#define KPD_COLUMN0_PORT        DIO_u8PORTB
#define KPD_COLUMN0_PIN         DIO_u8PIN3

#define KPD_COLUMN1_PORT        DIO_u8PORTB
#define KPD_COLUMN1_PIN         DIO_u8PIN4

#define KPD_COLUMN2_PORT        DIO_u8PORTB
#define KPD_COLUMN2_PIN         DIO_u8PIN5

#define KPD_COLUMN3_PORT        DIO_u8PORTB
#define KPD_COLUMN3_PIN         DIO_u8PIN6

#define KPD_COLUMN4_PORT        DIO_u8PORTB
#define KPD_COLUMN4_PIN         DIO_u8PIN7

#define KPD_COLUMN5_PORT        DIO_u8PORTC
#define KPD_COLUMN5_PIN         DIO_u8PIN0

#define KPD_COLUMN6_PORT        DIO_u8PORTC
#define KPD_COLUMN6_PIN         DIO_u8PIN1

#define KPD_COLUMN7_PORT        DIO_u8PORTC
#define KPD_COLUMN7_PIN         DIO_u8PIN2

#define KPD_COLUMN8_PORT        DIO_u8PORTC
#define KPD_COLUMN8_PIN         DIO_u8PIN3

#define KPD_COLUMN9_PORT        DIO_u8PORTC
#define KPD_COLUMN9_PIN         DIO_u8PIN4



#define KPD_BUTTONS             {{'c', '7', '8', '9', 'X', '/', 'k', 'R', 'S', 'n'},\
                                 {'G', '4', '5', '6', '-', 'I', 'L', 'M', 'N', 'O'},\
								 {'H', '1', '2', '3', '+', 'J', 'P', 'Q', 'A', 'B'},\
								 {'?', '0', '.', '=', '+', 'K', 'C', 'D', 'E', 'F'}}



#endif /* KEYPAD_CONFIG_H_ */
