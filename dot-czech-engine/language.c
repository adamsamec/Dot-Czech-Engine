/* 
* language.c
* set of serveral functions
*
* Author: Tony Hong
*
* Copyright (c) Dot Incorporation.
*/
#include <stdint.h>

#include "language.h"

/*
* UTF-8 input data length
*
* parameter
*     uint8_t *input - unsigned int array of input data
* return
*     int - length of input data
*/
int LetterLengthUTF8(uint8_t* input)
{
	int length = 0;

	for (; *input++;)
	{
		length++;
	}

	return length;
}

/*
* UTF-16 input data length
*
* parameter
*     uint16_t *input - unsigned int array of input data
* return
*     int - length of input data
*/
int LetterLengthUTF16(uint16_t* input)
{
	int length = 0;

	for (; *input++;)
	{
		length++;
	}

	return length;
}

/*
* convert UTF-8 data to UTF-16 data
*
* parameter
*     uint8_t *utf8 - unsigned int array of UTF-8 data
*     uint16_t *utf16 - unsigned int array of UTF-16 data
*/
// UTF8 -> UTF16
void Utf8IntoUtf16(uint8_t* inputUtf8, uint16_t* utf16)
{
//	uint8_t inputUtf8[MAX_INPUT_SIZE] = { 0 };
	int i = 0;
    int utf8Length = LetterLengthUTF8(inputUtf8);
/*
	do
	{
		inputUtf8[i] = *utf8;
		i++;
	} while (*utf8++ != 0);
*/
	for (i = 0; i < utf8Length; i++)
	{
		if (inputUtf8[i] <= 0x7F)
		{
			*utf16++ = inputUtf8[i];
		}
		else if ((inputUtf8[i] >= 0xC0 && inputUtf8[i] <= 0xDF) && (inputUtf8[i + 1] >= 0 && inputUtf8[i + 1] <= 0xBF))
		{
			uint16_t temp = 0x00;
			temp = temp | (0x1f & inputUtf8[i]);
			temp = temp << 6;
			temp = temp | (0x3f & inputUtf8[i + 1]);
			*utf16++ = temp;
			i = i + 1;
		}
		else if ((inputUtf8[i] >= 0xE0 && inputUtf8[i] <= 0xEF) && (inputUtf8[i + 1] >= 0x80 && inputUtf8[i + 1] <= 0xBF) && (inputUtf8[i + 2] >= 0x80 && inputUtf8[i + 2] <= 0xBF))
		{
            uint16_t temp = 0x00;
			temp = temp | (0x0f & inputUtf8[i]);
			temp = temp << 6;
			temp = temp | (0x3f & inputUtf8[i + 1]);
			temp = temp << 6;
			temp = temp | (0x3f & inputUtf8[i + 2]);
			*utf16++ = temp;
			i = i + 2;
		}
		else if ((inputUtf8[i] > 0xF0 && inputUtf8[i] <= 0xF7) && (inputUtf8[i + 1] >= 0x80 && inputUtf8[i + 1] <= 0xBF) && (inputUtf8[i + 2] >= 0x80 && inputUtf8[i + 2] <= 0xBF) && (inputUtf8[i + 3] >= 0x80 && inputUtf8[i + 3] <= 0xBF))
		{
            uint16_t temp1 = 0x00;
            uint16_t temp2 = 0x00;

			temp1 = (temp1 | (0x07 & inputUtf8[i])) << 2;
			temp2 = (temp2 | (0x30 & inputUtf8[i + 1])) >> 4;
			temp1 = (((temp1 + temp2) - 0x01) << 6) | 0xD800;
			temp1 = temp1 | ((0x00 | (0x0F & inputUtf8[i + 1])) << 2);
			temp1 = temp1 | (0x00 | ((0x30 & inputUtf8[i + 2]) >> 4));

			*utf16++ = temp1;

			temp1 = 0x00; temp2 = 0x00;
			temp1 = ((temp1 | (0x0F & inputUtf8[i + 2])) << 6) | 0xDC00;

			temp2 = temp1 | (0x3F & inputUtf8[i + 3]);
			temp1 = temp1 | temp2;

			*utf16++ = temp1;

			i = i + 3;
		}
	}
}
/*
* Return Language Type of given language code
*
* parameter
*     uint8_t languageCode - language code  
* return
*     Language - language type
*/
Language LanguageType(uint8_t languageCode)
{
	if (languageCode == 2)
	{
		return KOREAN;
} else if (languageCode == 3) {
return CZECH;
	}
	else 
	{
		return ENGLISH;
	}
}