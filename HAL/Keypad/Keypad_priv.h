/*
 * Keypad_priv.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: DELL
 */

#ifndef KEYPAD_PRIV_H_
#define KEYPAD_PRIV_H_

#define KPD_ROW_NUM             4
#define KPD_COLUMN_NUM          10

#define KPD_ROWS                {{KPD_ROW0_PORT, KPD_ROW0_PIN},\
			                    {KPD_ROW1_PORT, KPD_ROW1_PIN},\
			                    {KPD_ROW2_PORT, KPD_ROW2_PIN},\
			                    {KPD_ROW3_PORT, KPD_ROW3_PIN}}



#define KPD_COLUMNS             {{KPD_COLUMN0_PORT, KPD_COLUMN0_PIN},\
			                    {KPD_COLUMN1_PORT, KPD_COLUMN1_PIN},\
			                    {KPD_COLUMN2_PORT, KPD_COLUMN2_PIN},\
			                    {KPD_COLUMN3_PORT, KPD_COLUMN3_PIN},\
                                {KPD_COLUMN4_PORT, KPD_COLUMN4_PIN},\
								{KPD_COLUMN5_PORT, KPD_COLUMN5_PIN},\
			                    {KPD_COLUMN6_PORT, KPD_COLUMN6_PIN},\
			                    {KPD_COLUMN7_PORT, KPD_COLUMN7_PIN},\
                                {KPD_COLUMN8_PORT, KPD_COLUMN8_PIN},\
                                {KPD_COLUMN9_PORT, KPD_COLUMN9_PIN}}

#endif /* KEYPAD_PRIV_H_ */
