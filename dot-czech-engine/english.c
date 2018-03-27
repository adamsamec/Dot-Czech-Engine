/*
* english.c
* Main of English Braille Engine
*
* Author: Tony Hong
*
* Copyright (c) Dot Incorporation.
*/
#include "app_config.h"
#include "language.h"
#include "english.h"

#define SINGLE_CHARACTER_LENGTH 1

/*
* Determine letter type of given character
*
* parameter
*     uint16_t *input - input code
* return
*     LETTER - letter type
*/
LETTER LetterType(uint16_t *input)
{
    // English
    if ((*input >= 0x0041 && *input <= 0x005A) || (*input >= 0x0061 && *input <= 0x007A))
    {
        return ENGLISHCHARACTER;
    }
    // punuctutaion mark
    // else if
    // {
            // return PUNCTUATIONMARKS;
    // }
    // numeral letter
    // else if
    // {
            // return NUMERAL;
    // }

    return NOCHAR;
}

/*
* translate upper/lower case alphabet, abbreviations using appropriate braille rule
*
* parameter
*     uint16_t *input - english letter to translate
*     uint8_t *brailleCode - translated braille code
* return
*     int - translated braille code length
*/
int English(uint16_t *input, uint8_t *brailleCode)
{
    // a
    if (*input == 0x0061)
    {
        *brailleCode++ = 0x01;

        return SINGLE_CHARACTER_LENGTH;
    }
    // b
    else if (*input == 0x0062)
    {
        *brailleCode++ = 0x03;

        return SINGLE_CHARACTER_LENGTH;
    }
    // c
    else if (*input == 0x0063)
    {
        *brailleCode++ = 0x09;

        return SINGLE_CHARACTER_LENGTH;
    }
    // d
    else if (*input == 0x0064)
    {
        *brailleCode++ = 0x19;

        return SINGLE_CHARACTER_LENGTH;
    }
    // e
    else if (*input == 0x0065)
    {
        *brailleCode++ = 0x11;

        return SINGLE_CHARACTER_LENGTH;
    }
    // f
    else if (*input == 0x0066)
    {
         *brailleCode++ = 0x0B;

        return SINGLE_CHARACTER_LENGTH;
    }
    // g
    else if (*input == 0x0067)
    {
        *brailleCode++ = 0x1B;

        return SINGLE_CHARACTER_LENGTH;
    }
    // h
    else if (*input == 0x0068)
    {
        *brailleCode++ = 0x13;

        return SINGLE_CHARACTER_LENGTH;
    }
    // i
    else if (*input == 0x0069)
    {
        *brailleCode++ = 0x0A;

        return SINGLE_CHARACTER_LENGTH;
    }
    // j
    else if (*input == 0x006a)
    {
        *brailleCode++ = 0x1A;

        return SINGLE_CHARACTER_LENGTH;
    }
    // k
    else if (*input == 0x006b)
    {
        *brailleCode++ = 0x05;

        return SINGLE_CHARACTER_LENGTH;
    }
    // l
    else if (*input == 0x006c)
    {
        *brailleCode++ = 0x07;

        return SINGLE_CHARACTER_LENGTH;
    }
    // m
    else if (*input == 0x006d)
    {
        *brailleCode++ = 0x0D;

        return SINGLE_CHARACTER_LENGTH;
    }
    // n
    else if (*input == 0x006e)
    {
        *brailleCode++ = 0x1D;

        return SINGLE_CHARACTER_LENGTH;
    }
    // o
    else if (*input == 0x006f)
    {
        *brailleCode++ = 0x15;

        return SINGLE_CHARACTER_LENGTH;
    }
    // p
    else if (*input == 0x0070)
    {
        *brailleCode++ = 0x0F;

        return SINGLE_CHARACTER_LENGTH;
    }
    // q
    else if (*input == 0x0071)
    {
        *brailleCode++ = 0x1F;

        return SINGLE_CHARACTER_LENGTH;
    }
    // r
    else if (*input == 0x0072)
    {
        *brailleCode++ = 0x17;

        return SINGLE_CHARACTER_LENGTH;
    }
    // s
    else if (*input == 0x0073)
    {
        *brailleCode++ = 0x0E;

        return SINGLE_CHARACTER_LENGTH;
    }
    // t
    else if (*input == 0x0074)
    {
        *brailleCode++ = 0x1E;

        return SINGLE_CHARACTER_LENGTH;
    }
    // u
    else if (*input == 0x0075)
    {
        *brailleCode++ = 0x25;

        return SINGLE_CHARACTER_LENGTH;
    }
    // v
    else if (*input == 0x0076)
    {
        *brailleCode++ = 0x27;

        return SINGLE_CHARACTER_LENGTH;
    }
    // w
    else if (*input == 0x0077)
    {
        *brailleCode++ = 0x3A;

        return SINGLE_CHARACTER_LENGTH;
    }
    // x
    else if (*input == 0x0078)
    {
        *brailleCode++ = 0x2D;

        return SINGLE_CHARACTER_LENGTH;
    }
    // y
    else if (*input == 0x0079)
    {
        *brailleCode++ = 0x3D;

        return SINGLE_CHARACTER_LENGTH;
    }
    // z
    else if (*input == 0x007a)
    {
        *brailleCode++ = 0x35;

        return SINGLE_CHARACTER_LENGTH;
    }
    else
    {
        return 0;
    }
    // ...
}

/*
* translate punctuation mark into braille code using appropriate braille rule
*
* parameter
*     uint16_t *input - punctuation mark letter
*     uint8_t *brailleCode - translated braille code
* return
*     int - translated braille code length
*/
int PunctuationMarks(uint16_t *input, uint8_t *brailleCode)
{
    return 0;
}

/*
* translate numeric letters into braille code using appropriate braille rule
*
* parameter
*     uint16_t *input - numeral letter
*     uint8_t *brailleCode - translated braille code
* return
*     int - translated braille code length
*/
int Numeral(uint16_t *input, uint8_t *brailleCode)
{
    return 0;
}

/*
* translate Roman English, punctuation mark, numeral letter, etc into braille code using appropriate braille rule
*
* parameter
*     uint16_t *input - string to translate
*     uint8_t *brailleCode - translated braille code
* return
*     int - translated braille code length
*/
int EnglishTranslateIntoBraille(uint16_t *input, uint8_t *brailleCode)
{
    // braille code length
    int brailleCodeLength = 0; 

    // grade 1
    if (m_app_config->lang.item[0].grade == 1)
    {
        int inputLength = LetterLengthUTF16(input);

        for (int i = 0; i < inputLength; i++)
        {
            switch (LetterType(input))
            {
            // Roman(English)
            case ENGLISHCHARACTER:
                brailleCodeLength = brailleCodeLength + English(input, (brailleCode + brailleCodeLength));
                break;

            // punctuation mark
            case PUNCTUATIONMARKS:
                brailleCodeLength = brailleCodeLength + PunctuationMarks(input, (brailleCode + brailleCodeLength));
                break;

            // numeral
            case NUMERAL:
                brailleCodeLength = brailleCodeLength + Numeral(input, (brailleCode + brailleCodeLength));
                break;

            // untranslatable or invalid letter
            case NOCHAR:
            default:
                break;
            }

            // shift
            *input++;
        }
    }
    //grade 2
    else if (m_app_config->lang.item[0].grade == 2)
    {
        // ...
    }

    // return braille code length
    return brailleCodeLength;
}