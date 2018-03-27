/*
 * buf.h
 * Convertor to braille output of BUF (Braile Universal Format)
 *
 * Author: Adam Samec
 *
 * Copyright (c) Dot Incorporation.
 */

#include "./../dot-czech-engine/czech.h"

// Braille codes that can be translated
const uint8_t BRAILLE_CODES[] = {
    0x01, 0x03, 0x09, 0x19, 0x11, 0x0B, 0x1B, 0x13, 0x0A,
    0x1A, // Latin letters a through j #0-#9
    0x05, 0x07, 0x0D, 0x1D, 0x15, 0x0F, 0x1F, 0x17, 0x0E,
    0x1E,                               // Latin letters k through t #10-#19
    0x25, 0x27, 0x37, 0x2D, 0x3D, 0x35, // Latin letters u through z #20-#25
    0x21, 0x29, 0x39, 0x1C, 0x23, 0x0C, 0x2B, 0x2A, 0x3A, 0x31, 0x33, 0x2C,
    0x3E, 0x2F,
    0x2E, // Czech letters á, č, ď, é, ě, í, ň, ó, ř, š, ť, ú, ů, ý, ž #26-#40
    0x02, // "," comma #41
    0x04, // "." period #42
    0x12, // ":" colon #43
    0X06, // ";" semicolon #44
    0x22, // "?" question mark #45
    0x16, // "!" exclamation mark #46
    0x36, // '"' quotation mark #47
    0x08, // "'" apostrophe #48
    0x26, // "(" left parenthesis #49
    0x34, // ")" right parenthesis #50
    0x3B, // "/" slash #51
    0x38, // "|" vertical line #52
    0x24, // "-" hyphen #53
    0x32, // "+" plus sign #54
    0x14, // "*" asterisk #55
    0x3C, // "#" number sign #56
    0x00, // " " space #57

    // Prefixes
    0x20, // "}" right curly bracket - Latin capital letter prefix #58
    0x30, // "@" at sign - Sequence of Latin capital letters prefix #59
    0x10, // "%" percent sign - Postfix #60
    0x28, // "$" dollar sign - Greek capital letter prefix #61
    0x18, // "&" ampersand - Greek small letter prefix #62
    BRAILLE_TERMINATION};

// Translated UTF-16 character codes
const uint16_t TRANSLATED_UTF16_CHARS[] = {
    0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069,
    0x006A, // Latin letters a through j #0-#9
    0x006B, 0x006C, 0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 0x0072, 0x0073,
    0x0074, // Latin letters k through t #10-#19
    0x0075, 0x0076, 0x0077, 0x0078, 0x0079,
    0x007A, // Latin letters u through z #20-#25
    0x00E1, 0x010D, 0x010F, 0x00E9, 0x011B, 0x00ED, 0x0148, 0x00F3, 0x0159,
    0x0161, 0x0165, 0x00FA, 0x016F, 0x00FD,
    0x017E, // Czech letters á, č, ď, é, ě, í, ň, ó, ř, š, ť, ú, ů, ý, ž #26-#40
    0x002C, // "," comma #41
    0x002E, // "." period #42
    0x003A, // ":" colon #43
    0X003B, // ";" semicolon #44
    0x003F, // "?" question mark #45
    0x0021, // "!" exclamation mark #46
    0x0022, // '"' quotation mark #47
    0x0027, // "'" apostrophe #48
    0x0028, // "(" left parenthesis #49
    0x0029, // ")" right parenthesis #50
    0x002F, // "/" slash #51
    0x007C, // "|" vertical line #52
    0x002D, // "-" hyphen #53
    0x002B, // "+" plus sign #54
    0x002A, // "*" asterisk #55
    0x0023, // "#" number sign #56
    0x0020, // " " space #57

    // Prefixes
    0x007D, // "}" right curly bracket - Latin capital letter prefix #58
    0x0040, // "@" at sign - Sequence of Latin capital letters prefix #59
    0x0025, // "%" percent sign - Postfix #60
    0x0024, // "$" dollar sign - Greek capital letter prefix #61
    0x0026, // "&" ampersand - Greek small letter prefix #62
    0};

void BrailleIntoBuf(uint8_t *brailleCode, uint16_t *bufText,
                    int brailleCodeLength) {
  int i;
  for (i = 0; i < brailleCodeLength; i++) // begin for 1
  {
    for (int j = 0; BRAILLE_CODES[j] != BRAILLE_TERMINATION; j++) // begin for 2
    {
      if (BRAILLE_CODES[j] == brailleCode[i]) // begin if 1
      {
        bufText[i] = TRANSLATED_UTF16_CHARS[j];
        break;
        ;
      } // end if 1
    }   // end for 2
  }     // end for 1
  // Terminate the array with null
  bufText[i] = 0;
} // end func
