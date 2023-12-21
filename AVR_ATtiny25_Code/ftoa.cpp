#include "ftoa.h"

int ftoa(float f, char *p, uint8_t precision)
{

	typedef union
	{
		int32_t Long;
		float Float;
	} int32_Float_t;

	int32_t mantissa, int_part, frac_part;
	int16_t exp2;
	int32_Float_t x;

	if (f == 0.0)
	{
		*p++ = '0';
		if (precision > (uint8_t) 0)
		{
			*p++ = '.';
			*p++ = '0';
		}
		*p = 0;
		return 0;
	}

	x.Float = f;
	exp2 = (unsigned char) (x.Long >> 23) - 127;
	mantissa = (x.Long & 0xFFFFFF) | 0x800000;
	frac_part = 0;
	int_part = 0;

	if (exp2 >= 31)
	{
		return _FTOA_TOO_LARGE;

	}
	else if (exp2 < -23)
	{
		return _FTOA_TOO_SMALL;

	}
	else if (exp2 >= 23)
	int_part = mantissa << (exp2 - 23);
	else if (exp2 >= 0)
	{
		int_part = mantissa >> (23 - exp2);
		frac_part = (mantissa << (exp2 + 1)) & 0xFFFFFF;
	}
	else
	{
		//if (exp2 < 0)
		frac_part = (mantissa & 0xFFFFFF) >> -(exp2 + 1);
	}

	if (x.Long < 0)
	{
		*p++ = '-';
	}

	if (int_part == 0)
	*p++ = '0';
	else
	{
		ltoa(int_part, p, 10);
		while (*p)
		p++;
	}

	if (precision > (uint8_t) 0)
	{
		*p++ = '.';
		if (frac_part == 0)
		{
			*p++ = '0';
		}
		else
		{
			char m;

			for (m = 0; m < precision; m++)
			{
				//frac_part *= 10;
				frac_part = (frac_part << 3) + (frac_part << 1);
				*p++ = (frac_part >> 24) + '0';
				frac_part &= 0xFFFFFF;
			}

			//delete ending zeroes
			for (--p; p[0] == '0' && p[-1] != '.'; --p)
			{
			}
			++p;
		}
	}
	*p = 0;

	return 0;
}