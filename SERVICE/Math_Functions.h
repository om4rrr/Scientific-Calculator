/*
 * Math_Functions.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: DELL
 */

#ifndef SERVICE_MATH_FUNCTIONS_H_
#define SERVICE_MATH_FUNCTIONS_H_

#define M_PI		3.14159265358979323846

float _pow(float a, signed char b);

float _sin(float x);

float _cos(float x);

float _tan(float x);

float expo(float n);

float _sqrt(float n);

f32 _sinh(f32 x);

f32 _cosh(f32 x);

f32 _tanh(f32 x);

double _atan (double x);

double _asin (double x);

double _acos (double x);

void Dec2Hex(u16 DecNumber, u8 * HexNumber);

void Dec2Oct(u16 DecNumber, u8 * OctNumber);

void Dec2Bin(u16 DecNumber, u8 * BinNumber);

u16 Hex2Dec(char * HexNumber);

u16 Oct2Dec(u8 * OctNumber);

u16 Bin2Dec(u8 * BinNumber);






#endif /* SERVICE_MATH_FUNCTIONS_H_ */
