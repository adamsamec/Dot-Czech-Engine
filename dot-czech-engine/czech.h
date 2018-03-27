/*
 * czech.h
 * Czech header file
 *
 * Author: Adam Samec
 *
 * Copyright (c) Dot Incorporation.
 */
#ifndef __CZECH_H__
#define __CZECH_H__
#include <stdint.h>

// Braille representation of a whitespace character
#define B_WHITESPACE_CHAR 0x00 // Empty cell

// Braille representation of an unknown character
#define B_UNKNOWN_CHAR 0x22 // "?" question mark

// Number prefix
#define B_NUM_PREFIX 0x3C // Dots 3-4-5-6

// Single Czech capital letter prefix
#define B_CZECH_CAP_PREFIX 0x20 // Dot 6

// Czech capital letter sequence prefix
#define B_CZECH_CAP_SeQ_PREFIX 0x30 // Dot 5-6

// Capital letter sequence or number postfix
#define B_POSTFIX 0x10 // Dot 5

// Greek small letter prefix
#define B_GREEK_SMALL_PREFIX 0x18 // Dots 4-5

// Greek capital letter prefix
#define B_GREEK_CAP_PREFIX 0x28 // Dots 4-6

// Max length of a braille code
#define MAX_BRAILLE_CODE_LENGTH 10

// Code indicating termination of braille codes
#define BRAILLE_TERMINATION 0xFF

// Code indicating multi-character braille  translation
#define BRAILLE_MULTICHAR 0xC0

// Structure to represent a braille character and its properties
typedef struct {
  // Braille code
  uint8_t code[MAX_BRAILLE_CODE_LENGTH];

  // Character properties flags
  unsigned int isUnknown : 1;
  unsigned int isWhitespace : 1;
  unsigned int isNumber : 1;
  unsigned int isCzechCapLetter : 1;
  unsigned int isCzechSmallLetter : 1;
  unsigned int isSpecial : 1;
  unsigned int isPunctuation : 1;
  unsigned int hasPrefix : 1;
  unsigned int hasNumPrefix : 1;
  unsigned int hasCzechCapPrefix : 1;
  unsigned int hasGreekSmallPrefix : 1;
  unsigned int hasGreekCapPrefix : 1;
  unsigned int isNonNumTerminating : 1;
  unsigned int isNumPostfixNeeding : 1;
} BrailleChar;

// Create a new initialized braille character
BrailleChar CreateBrailleChar();

// Copy the given (possibly multichar) braille code to the given braille code
// array
int CopyBrailleCode(uint8_t *from, uint8_t **to);

// Translate the given UTF-16 character to 8-bit braille code and determine the
// character properties
BrailleChar CzechCharIntoBraille(uint16_t *utf16char);

// Translate given UTF-16 string to 8-bit braille code array
int CzechTranslateIntoBraille(uint16_t *input, uint8_t *brailleCode);

#endif