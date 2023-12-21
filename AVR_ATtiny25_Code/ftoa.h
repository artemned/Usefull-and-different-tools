/*
 * ftoa.h
 *
 * Created: 18.03.2023 17:50:20
 *  Author: sWe
 */ 

#pragma once
#include <stdint.h>
#include <stdlib.h>


#define _FTOA_TOO_LARGE -2  // |input| > 2147483520
#define _FTOA_TOO_SMALL -1  // |input| < 0.0000001

int ftoa(float f, char *p, uint8_t precision);