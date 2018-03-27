/*
 * czech.c
 * Main of Czech Braille Engine
 *
 * Author: Adam Samec
 *
 * Copyright (c) Dot Incorporation.
 */
#include <stdio.h>
#include <string.h>

#include "app_config.h"
#include "czech.h"
#include "language.h"

// UTF-16 known character codes
const uint16_t UTF16_KNOWN_CHARS[] = {
    0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049,
    0x004A, // Latin letters A through J #0-#9
    0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053,
    0x0054, // Latin letters K through T #10-#19
    0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, // Latin letters U through Z
                                                    // #20-#25
    0x00C1, 0x010C, 0x010E, 0x00C9, 0x011A, 0x00CD, 0x0147, 0x00D3, 0x0158,
    0x0160, 0x0164, 0x00DA, 0x016E, 0x00DD, 0x017D, // Czech lETTERS Á,
                                                    // Č, Ď, É, Ě, Í, Ň,
                                                    // Ó, Ř, Š, Ť, Ú, Ů,
                                                    // Ý, Ž #26-#40
    0x00D6, 0x00DC, // German letters Ö, Ü #41-#42
    0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069,
    0x006A, // Latin letters a through j #43-#52
    0x006B, 0x006C, 0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 0x0072, 0x0073,
    0x0074, // Latin letters k through t #53-#62
    0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, // Latin letters u through z
                                                    // #63-#68
    0x00E1, 0x010D, 0x010F, 0x00E9, 0x011B, 0x00ED, 0x0148, 0x00F3, 0x0159,
    0x0161, 0x0165, 0x00FA, 0x016F, 0x00FD, 0x017E, // Czech letters á,
                                                    // č, ď, é, ě, í, ň,
                                                    // ó, ř, š, ť, ú, ů,
                                                    // ý, ž #69-#83
    0X00F6, 0x00FC, // German letters ö, ü #84-#85
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038,
    0x0039,                 // Numbers 0 through 9 #86-#95
    0x00B9, 0x00B2, 0x00B3, // Superscript numbers 1, 2, 3 #96-#98
    0x002C,                 // "," comma #99
    0x002E,                 // "." period #100
    0x003A,                 // ":" colon #101
    0X003B,                 // ";" semicolon #102
    0x003F,                 // "?" question mark #103
    0x0021,                 // "!" exclamation mark #104
    0x0022,                 // '"' quotation mark #105
    0x0084,                 // "„" quotation mark  #106
    0x0093,                 // "“" left double quotation mark #107
    0x0094,                 // "”" right straight double quotation mark	 #108
    0x0027,                 // "'" apostrophe #109
    0x00B4,                 // "´" acute accent #110
    0x0028,                 // "(" left parenthesis #111
    0x0029,                 // ")" right parenthesis #112
    0x002F,                 // "/" slash #113
    0x007C,                 // "|" vertical line #114
    0x002D,                 // "-" hyphen #115
    0x0096,                 // "–" n-dash #116
    0x0097,                 // "—" m-dash #117
    0x005F,                 // "_" underscore #118
    0x002B,                 // "+" plus sign #119
    0x003C,                 // "<" less than sign #120
    0x003E,                 // ">" greater than sign #121
    0x003D,                 // "=" equal sign #122
    0x002A,                 // "*" asterisk #123
    0x0023,                 // "#" number sign #124
    0x0026,                 // "&" ampersand #125
    0x0040,                 // "@" at sign #126
    0x005E,                 // "^" circumflex accent #127
    0x0095,                 // "•" bullet #128
    0x005B,                 // "[" left square bracket #129
    0x005D,                 // "]" right square bracket #130
    0x007E,                 // "~" tilde #131
    0x0060,                 // "`" grave accent #132
    0x007B,                 // "{" left curly bracket #133
    0x007D,                 // "}" right curly bracket #134
    0x005C,                 // "\" backslash #135
    0x00B0,                 // "°" degree sign #136
    0x0025,                 // "%" percent sign #137
    0x0089,                 // "‰" per mille #138
    0x0024,                 // "$" dollar sign #139
    0x20AC,                 // "€" euro sign #140
    0x00A7,                 // "§" section sign #141
    0x2026,                 // "…" ellipsis #142
    0x00A3,                 // "£" British pound #143
    0x201E,                 // "„" left double quote #144
    0x201C,                 // "“" right double quote #145
    0x201D,                 // "”" right double quote #146
    0x2013,                 // "–" n-dash #147
    0};

// UTF-16 Whitespace character codes
const uint16_t UTF16_WHITESPACE_CHARS[] = {0x0020, // " " space
                                           0x00A0, // " " no-break space
                                           0x0009, // "	" character tabulation
                                           0x000B, // line tabulation
                                           0x000A, // line feed
                                           0x000D, // carriage return
                                           0};

// Braille codes of UTF translated characters without prefix
const uint8_t TRANSLATED_BRAILLE_CODES[] = {
    0x01,
    0x03,
    0x09,
    0x19,
    0x11,
    0x0B,
    0x1B,
    0x13,
    0x0A,
    0x1A, // Latin letters A through J #0-#9
    0x05,
    0x07,
    0x0D,
    0x1D,
    0x15,
    0x0F,
    0x1F,
    0x17,
    0x0E,
    0x1E, // Latin letters K through T #10-#19
    0x25,
    0x27,
    0x37,
    0x2D,
    0x3D,
    0x35, // Latin letters U through Z #20-#25
    0x21,
    0x29,
    0x39,
    0x1C,
    0x23,
    0x0C,
    0x2B,
    0x2A,
    0x3A,
    0x31,
    0x33,
    0x2C,
    0x3E,
    0x2F,
    0x2E, // Czech lETTERS Á, Č, Ď, É, Ě, Í, Ň, Ó, Ř, Š, Ť, Ú, Ů, Ý, Ž #26-#40
    BRAILLE_MULTICHAR,
    BRAILLE_MULTICHAR, // German letters Ö, Ü #41-#42
    0x01,
    0x03,
    0x09,
    0x19,
    0x11,
    0x0B,
    0x1B,
    0x13,
    0x0A,
    0x1A, // Latin letters a through j #43-#52
    0x05,
    0x07,
    0x0D,
    0x1D,
    0x15,
    0x0F,
    0x1F,
    0x17,
    0x0E,
    0x1E, // Latin letters k through t #53-#62
    0x25,
    0x27,
    0x37,
    0x2D,
    0x3D,
    0x35, // Latin letters u through z #63-#68
    0x21,
    0x29,
    0x39,
    0x1C,
    0x23,
    0x0C,
    0x2B,
    0x2A,
    0x3A,
    0x31,
    0x33,
    0x2C,
    0x3E,
    0x2F,
    0x2E, // Czech letters á, č, ď, é, ě, í, ň, ó, ř, š, ť, ú, ů, ý, ž #69-#83
    BRAILLE_MULTICHAR,
    BRAILLE_MULTICHAR, // German letters ö, ü #84-#85
    0x1A,
    0x01,
    0x03,
    0x09,
    0x19,
    0x11,
    0x0B,
    0x1B,
    0x13,
    0x0A, // Numbers 0 through 9 #86-#95
    0x01,
    0x03,
    0x09,              // Superscript numbers 1, 2, 3 #96-#98
    0x02,              // "," comma #99
    0x04,              // "." period #100
    0x12,              // ":" colon #101
    0X06,              // ";" semicolon #102
    0x22,              // "?" question mark #103
    0x16,              // "!" exclamation mark #104
    0x36,              // '"' quotation mark #105
    0x36,              // "„" quotation mark  #106
    0x36,              // "“" left double quotation mark #107
    0x36,              // "”" right straight double quotation mark	 #108
    0x08,              // "'" apostrophe #109
    0x08,              // "´" acute accent #110
    0x26,              // "(" left parenthesis #111
    0x34,              // ")" right parenthesis #112
    0x3B,              // "/" slash #113
    0x38,              // "|" vertical line #114
    0x24,              // "-" hyphen #115
    0x24,              // "–" n-dash #116
    0x24,              // "—" m-dash #117
    0x24,              // "_" underscore #118
    0x32,              // "+" plus sign #119
    0x23,              // "<" less than sign #120
    0x1C,              // ">" greater than sign #121
    0x36,              // "=" equal sign #123
    0x14,              // "*" asterisk #123
    0x3C,              // "#" number sign #124
    0x2F,              // "&" ampersand #125
    0x3B,              // "@" at sign #126
    0x0C,              // "^" circumflex accent #127
    0x14,              // "•" bullet #128
    0x26,              // "[" left square bracket #129
    0x34,              // "]" right square bracket #130
    0x10,              // "~" tilde #131
    0x08,              // "`" grave accent #132
    0x26,              // "{" left curly bracket #133
    0x34,              // "}" right curly bracket #134
    0x33,              // "\" backslash #135
    0x0E,              // "°" degree sign #136
    0x0F,              // "%" percent sign #137
    0x17,              // "‰" per mille #138
    0x2B,              // "$" dollar sign #139
    0x22,              // "€" euro sign #140
    0x2C,              // "§" section sign #141
    BRAILLE_MULTICHAR, // "…" ellipsis #142
    BRAILLE_MULTICHAR, // "£" British pound sign #143
    0x36,              // "„" left double quote #144
    0x36,              // "“" right double quote #145
    0x36,              // "”" right double quote #146
    0x24,              // "–" n-dash #147
    0};

// Multi character braille codes of translated UTF characters
const uint8_t TRANSLATED_MULTICHAR_CODES[][MAX_BRAILLE_CODE_LENGTH] = {
    {0x15, 0x11, BRAILLE_TERMINATION},       // German letter "Oe"
    {0x25, 0x11, BRAILLE_TERMINATION},       // German letter "Ue"
    {0x15, 0x11, BRAILLE_TERMINATION},       // German letter "oe"
    {0x25, 0x11, BRAILLE_TERMINATION},       // German letter "ue"
    {0x04, 0x04, 0x04, BRAILLE_TERMINATION}, // "…" ellipsis
    {0x07, 0x0A, 0x03, 0x17, 0x01,
     BRAILLE_TERMINATION}, // "£" British pound sign
    0};

// Indices of Czech capital letters
const int CZECH_CAP_LETTERS[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                                 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
                                 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, -1};

// Indices of Czech small letters
const int CZECH_SMALL_LETTERS[] = {43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
                                   54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
                                   65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
                                   76, 77, 78, 79, 80, 81, 82, 83, 84, 85, -1};

// Indices of numbers
const int NUMS[] = {86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, -1};

// Indices of special characters
const int SPECIAL_CHARS[] = {101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                             99,  100, 111, 112, 113, 114, 115, 116, 117, 118,
                             119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
                             129, 130, 131, 132, 133, 134, 135, 136, 137, 138,
                             139, 140, 141, 142, 143, 144, 145, 146, 147, -1};

// Indices of punctuation marks
const int PUNCTUATION_MARKS[] = {99,  100, 101, 102, 103, 104, 105, 106,
                                 107, 108, 109, 110, 111, 112, 113, 114,
                                 115, 116, 117, 129, 130, 132, 133, 134,
                                 135, 142, 144, 145, 146, 147, -1};

// Indices of special characters prefixed by the Czech capital letter prefix
const int CZECH_CAP_PREFIX_CHARS[] = {118, 129, 130, 131, -1};

// Indices of special characters prefixed by the Greek small letter prefix
const int GREEK_SMALL_PREFIX_CHARS[] = {132, -1};
;

// Indices of special characters prefixed by the Greek capital letter prefix
const int GREEK_CAP_PREFIX_CHARS[] = {133, 134, -1};

// Indices of special characters prefixed by  the number prefix
const int NUM_PREFIX_CHARS[] = {125, 126, 135, 136, 137,
                                138, 139, 140, 141, -1};

// Indices of characters that do not terminate a number
const int NON_NUM_TERMINATING_CHARS[] = {99, 100, -1};

// Indices of characters that need the postfix to terminate a number if they
// stay right after the number
const int NUM_POSTFIX_NEEDING_CHARS[] = {43, 44, 45, 46, 47, 48,
                                         49, 50, 51, 52, -1};

/*
 * Create a new initialized braille character
 * return
 *     BrailleChar - the new braille character
 */
BrailleChar CreateBrailleChar() {
  BrailleChar brailleChar = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < MAX_BRAILLE_CODE_LENGTH; i++) {
    brailleChar.code[i] = BRAILLE_TERMINATION;
  }
  return brailleChar;
}

/*
 * Copy the given (possibly multichar) braille code to the given braille code
 * array parameter uint8_t *from - braille code to be copied uint8_t **to -
 * pointer to the copy destination array return int - number of braille
 * characters written
 */
int CopyBrailleCode(uint8_t *from, uint8_t **to) {
  int i = 0;
  for (; from[i] != BRAILLE_TERMINATION; i++) {
    **to = from[i];
    (*to)++;
  }
  return i;
}

/*
 * Translate the given UTF-16 character to 8-bit braille code and determine the
 * character properties parameter uint16_t *utf16char - input UTF-16 character
 * to translate return BrailleChar - translated braille character and its
 * properties
 */
BrailleChar CzechCharIntoBraille(uint16_t *utf16char) {
  BrailleChar brailleChar = CreateBrailleChar();
  int isWhitespace = 0;
  int i;

  // Determine if the input UTF-16 char is whitespace
  for (i = 0; UTF16_WHITESPACE_CHARS[i]; i++) // begin for 1
  {
    if (UTF16_WHITESPACE_CHARS[i] == *utf16char) // begin if 1
    {
      isWhitespace = 1;
      break;
    }               // end if 1
  }                 // end for 1
  if (isWhitespace) // begin if 1
  {
    brailleChar.code[0] = B_WHITESPACE_CHAR;
    brailleChar.isWhitespace = 1;
  } else { // else if 1
    // Find the input UTF-16 character in the known characters array
    int isKnownChar = 0;
    for (i = 0; UTF16_KNOWN_CHARS[i]; i++) // begin for 1
    {
      if (UTF16_KNOWN_CHARS[i] == *utf16char) // begin if 2
      {
        isKnownChar = 1;
        break;
      } // end if 2
    }   // end for 1

    // Translate the input character
    if (isKnownChar) // begin if 2
    {
      if (TRANSLATED_BRAILLE_CODES[i] == BRAILLE_MULTICHAR) // begin if 3
      {
        // Translated character consist of more braille characters
        int multicharIndex = -1;
        for (int j = 0; j <= i; j++) // begin for 1
        {
          if (TRANSLATED_BRAILLE_CODES[j] == BRAILLE_MULTICHAR) // begin if 4
          {
            multicharIndex++;
          } // end if 4
        }   // end for 1
        strcpy((char *)brailleChar.code,
               (char *)TRANSLATED_MULTICHAR_CODES[multicharIndex]);
      } else { // else if 3
        // Translated character consist of a single braille character
        brailleChar.code[0] = TRANSLATED_BRAILLE_CODES[i];
      } // end if 3

      // Determine braille character properties
      int j;
      for (j = 0; NUMS[j] >= 0; j++) // begin for 1
      {
        if (NUMS[j] == i) // begin if 3
        {
          brailleChar.isNumber = 1;
          break;
        }                                         // end if 3
      }                                           // end for 1
      for (j = 0; CZECH_CAP_LETTERS[j] >= 0; j++) // begin for 1
      {
        if (CZECH_CAP_LETTERS[j] == i) // begin if 3
        {
          brailleChar.isCzechCapLetter = 1;
          break;
        }                                           // end if 3
      }                                             // end for 1
      for (j = 0; CZECH_SMALL_LETTERS[j] >= 0; j++) // begin for 1
      {
        if (CZECH_SMALL_LETTERS[j] == i) // begin if 3
        {
          brailleChar.isCzechSmallLetter = 1;
          break;
        }                                     // end if 3
      }                                       // end for 1
      for (j = 0; SPECIAL_CHARS[j] >= 0; j++) // begin for 1
      {
        if (SPECIAL_CHARS[j] == i) // begin if 3
        {
          brailleChar.isSpecial = 1;
          break;
        }                                         // end if 3
      }                                           // end for 1
      for (j = 0; PUNCTUATION_MARKS[j] >= 0; j++) // begin for 1
      {
        if (PUNCTUATION_MARKS[j] == i) // begin if 3
        {
          brailleChar.isPunctuation = 1;
          break;
        }                                              // end if 3
      }                                                // end for 1
      for (j = 0; CZECH_CAP_PREFIX_CHARS[j] >= 0; j++) // begin for 1
      {
        if (CZECH_CAP_PREFIX_CHARS[j] == i) // begin if 3
        {
          brailleChar.hasCzechCapPrefix = 1;
          brailleChar.hasPrefix = 1;
          break;
        }                                                // end if 3
      }                                                  // end for 1
      for (j = 0; GREEK_SMALL_PREFIX_CHARS[j] >= 0; j++) // begin for 1
      {
        if (GREEK_SMALL_PREFIX_CHARS[j] == i) // begin if 3
        {
          brailleChar.hasGreekSmallPrefix = 1;
          brailleChar.hasPrefix = 1;
          break;
        }                                              // end if 3
      }                                                // end for 1
      for (j = 0; GREEK_CAP_PREFIX_CHARS[j] >= 0; j++) // begin for 1
      {
        if (GREEK_CAP_PREFIX_CHARS[j] == i) // begin if 3
        {
          brailleChar.hasGreekCapPrefix = 1;
          brailleChar.hasPrefix = 1;
          break;
        }                                        // end if 3
      }                                          // end for 1
      for (j = 0; NUM_PREFIX_CHARS[j] >= 0; j++) // begin for 1
      {
        if (NUM_PREFIX_CHARS[j] == i) // begin if 3
        {
          brailleChar.hasNumPrefix = 1;
          brailleChar.hasPrefix = 1;
          break;
        }                                                 // end if 3
      }                                                   // end for 1
      for (j = 0; NON_NUM_TERMINATING_CHARS[j] >= 0; j++) // begin for 1
      {
        if (NON_NUM_TERMINATING_CHARS[j] == i) // begin if 3
        {
          brailleChar.isNonNumTerminating = 1;
          break;
        }                                                 // end if 3
      }                                                   // end for 1
      for (j = 0; NUM_POSTFIX_NEEDING_CHARS[j] >= 0; j++) // begin for 1
      {
        if (NUM_POSTFIX_NEEDING_CHARS[j] == i) // begin if 3
        {
          brailleChar.isNumPostfixNeeding = 1;
          break;
        }    // end if 3
      }      // end for 1
             // end for 1
    } else { // else if 2
      brailleChar.code[0] = B_UNKNOWN_CHAR;
      brailleChar.isUnknown = 1;
    } // end if 2
  }   // end if 1
  return brailleChar;
} // end func

/*
 * Translate the given UTF-16 Czech string into braille code array using
 * appropriate braille rules
 *
 * parameter
 *     uint16_t *input - UTF-16 string to translate
 *     uint8_t *brailleCode - translated braille code array
 * return
 *     int - translated braille code length
 */
int CzechTranslateIntoBraille(uint16_t *input, uint8_t *brailleCode) {
  int brailleCodeLength = 0;
  int inputLength = LetterLengthUTF16(input);

  // Grade 1
  if (m_app_config->lang.item[0].grade == 1) // begin if 1
  {
    BrailleChar currentBrailleChar;
    BrailleChar nextBrailleChar;
    int isCapLetterSeqStarted = 0;
    int isNumberStarted = 0;

    if (inputLength) // begin if 2
    {
      currentBrailleChar = CzechCharIntoBraille(input);
      int isNextBrailleChar = 0;

      for (int i = 0; i < inputLength; i++) // begin for 1
      {
        if (i + 1 < inputLength) // begin if 3
        {
          isNextBrailleChar = 1;
          nextBrailleChar = CzechCharIntoBraille(++input);
        } else { // else if 3
          isNextBrailleChar = 0;
        } // end if 3

        // printf("%x-", *input);
        // printDots(currentBrailleChar.code[0]);
        // printf(":%d:%d-", currentBrailleChar.hasPrefix,
        // currentBrailleChar.isSpecial);

        // Translation rules follow
        if (currentBrailleChar.isNumber) // begin if 3
        {
          // Numbers
          if (!isNumberStarted) // begin if 4
          {
            // beginning of one or more numbers. Write the number prefix.
            *brailleCode++ = B_NUM_PREFIX;
            brailleCodeLength += 1;
            isNumberStarted = 1;
          } // end if 4
            // Write the number
          brailleCodeLength +=
              CopyBrailleCode(currentBrailleChar.code, &brailleCode);
        } else if (!currentBrailleChar.isCzechCapLetter) // else if 3
        {
          // Small letter or special character
          if (currentBrailleChar.isCzechSmallLetter &&
              (isCapLetterSeqStarted ||
               (isNumberStarted &&
                currentBrailleChar.isNumPostfixNeeding))) // begin if 4
          {
            // Small letter ending a sequence of capital letters or small letter
            // "a" through "j" ending a number. Write the postfix.
            *brailleCode++ = B_POSTFIX;
            brailleCodeLength += 1;
          }                                 // end if 4
          if (currentBrailleChar.isSpecial) // begin if 4
          {
            // Special character
            if (currentBrailleChar.hasNumPrefix) // begin if 5
            {
              // Write a number prefix for the special character
              *brailleCode++ = B_NUM_PREFIX;
              brailleCodeLength += 1;
            } else if (currentBrailleChar.hasCzechCapPrefix) // else if 5
            {
              // Write a Czech capital letter prefix for the special character
              *brailleCode++ = B_CZECH_CAP_PREFIX;
              brailleCodeLength += 1;
            } else if (currentBrailleChar.hasGreekSmallPrefix) // else if 5
            {
              // Write a Greek small letter prefix for the special character
              *brailleCode++ = B_GREEK_SMALL_PREFIX;
              brailleCodeLength += 1;
            } else if (currentBrailleChar.hasGreekCapPrefix) // else if 5
            {
              // Write a Greek capital letter prefix for the special character
              *brailleCode++ = B_GREEK_CAP_PREFIX;
              brailleCodeLength += 1;
            } // end if 5
          }   // end if 4
              // Write the small letter or special character
          brailleCodeLength +=
              CopyBrailleCode(currentBrailleChar.code, &brailleCode);
        } else { // else if 3
          // Capital letter
          if (isNextBrailleChar &&
              nextBrailleChar.isCzechCapLetter) // begin if 4
          {
            // The next letter is also capital
            if (!isCapLetterSeqStarted) // begin if 5
            {
              // Beginning of a sequence of capital letters

              *brailleCode++ = B_CZECH_CAP_SeQ_PREFIX;
              brailleCodeLength += 1;
              isCapLetterSeqStarted = 1;
            }                                // end if 5
          } else if (!isCapLetterSeqStarted) // else if 4
          {
            // The current letter is capital but the next character is not
            // capital. And it's not an end of a capital letter sequence
            *brailleCode++ = B_CZECH_CAP_PREFIX;
            brailleCodeLength += 1;
          } // end if 4
          brailleCodeLength +=
              CopyBrailleCode(currentBrailleChar.code, &brailleCode);
        } // end if 3

        // Actualize the variables for the next loop
        if (!currentBrailleChar.isNumber &&
            !currentBrailleChar.isNonNumTerminating) // begin if 3
        {
          // Anything other than number except for non number terminating
          // characters terminates the number
          isNumberStarted = 0;
        } // end if 3
        if (currentBrailleChar.isPunctuation ||
            currentBrailleChar.isWhitespace || currentBrailleChar.hasPrefix ||
            currentBrailleChar.isNumber) // begin if 3
        {
          // Punctuation, whitespace, characters with a prefix and numbers
          // terminates the sequence of capital letters
          isCapLetterSeqStarted = 0;
        } // end if 3

        if (isNextBrailleChar) // begin if 3
        {
          currentBrailleChar = nextBrailleChar;
        } // end if 3
      }   // end for 1
    }     // end if 2
  }       // end if 1
  return brailleCodeLength;
} // end func