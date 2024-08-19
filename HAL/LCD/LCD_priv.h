/*
 * LCD_priv.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: DELL
 */

#ifndef LCD_PRIV_H_
#define LCD_PRIV_H_


#define  FOUR_BIT        55
#define  EIGHT_BIT       66

#define  RIGHT_TO_LEFT   88

#define  LEFT_TO_RIGHT   99

#define  D0              0
#define  D1              1
#define  D2              2
#define  D3              3
#define  D4              4
#define  D5              5
#define  D6              6
#define  D7              7

#define POS_0            0x40
#define POS_1            0x48
#define POS_2            0x50
#define POS_3            0x58
#define POS_4            0x60
#define POS_5            0x68
#define POS_6            0x70
#define POS_7            0x78


#define  MASK_BIT    1

static inline ES_t LCD_inenuSendCommand(u8 Copy_u8Command);

static ES_t LCD_enuLatch(u8 Copy_u8Data);

static ES_t LCD_enuTurnOn(void);

#endif /* LCD_PRIV_H_ */
