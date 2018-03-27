/*
* language.h
* language header file
*
* Author: Tony Hong
*
* Copyright (c) Dot Incorporation.
*/
#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__
#include <stdint.h>

// maximum size(byte) of input character in dot watch
#define MAX_INPUT_SIZE 256 
// maximum size(byte) of dot watch braille code
#define MAX_BRAILLE_SIZE 512 

// language type
typedef enum LanguageClass
{
	ENGLISH,
	KOREAN,
CZECH
} Language;

// UTF-8 input data length
int LetterLengthUtf8(uint8_t* input);

// UTF-16 input data length
int LetterLengthUTF16(uint16_t* input);

// convert UTF-8 data to UTF-16 data
void Utf8IntoUtf16(uint8_t* utf8, uint16_t* utf16);

// Return Language Type of given language code
Language LanguageType(uint8_t languageCode);

#endif