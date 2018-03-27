/*
* main.c
* Main of Dot Watch Braille Engine
*
* Author: Tony Hong
*
* Copyright (c) Dot Incorporation.
*/
#include <stdint.h>
#include <stdio.h>

#include "app_config.h"
#include "language.h"
#include "english.h"
#include "czech.h"

/*
* main function in braille translator
*
* parameter
*     uint8_t *inputBuffer - charactet(UTF8) array to translate (from mobile)
*     uint8_t *outputBuffer - translated braille code array (to dot watch)
* return
*     int - length of braille code array
*/
int EngineMain(uint8_t *inputBuffer, uint8_t *outputBuffer)
{
    // converted character array(UTF-16)
    uint16_t inputUtf16[MAX_INPUT_SIZE] = { 0 };
    // braille code array
    uint8_t brailleCode[MAX_BRAILLE_SIZE] = { 0 };
    // braille code length
    int brailleCodeLength = 0;

    // convert UTF-8 data to UTF-16 data
    Utf8IntoUtf16(inputBuffer, inputUtf16);

    // select language
    switch (LanguageType(m_app_config->lang.major))
    {
    // additional language e.g. Korean
    // case KOREAN:
    // brailleCodeLength = KoreanTranslateIntoBraille(input, brailleCode);
        // break;

    // default language: English
    default:
        brailleCodeLength = EnglishTranslateIntoBraille(inputUtf16, brailleCode);
        break;
    }

    // write braillce code data into outputBuffer from brailleCode 
    for (int i = 0; i < brailleCodeLength; i++)
    {
        outputBuffer[i] = brailleCode[i];
    }

    // return braille code length
    return brailleCodeLength;
}

int main()
{
    // UTF-8 characters in dot watch firmware from mobile, (e.g. abcdefghijklmnopqrstuvwxyz)
    uint8_t inputBuffer[MAX_INPUT_SIZE] = { 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a };
    // braille code array in dot watch firmware
    uint8_t outputBuffer[MAX_BRAILLE_SIZE] = { 0 };

    // main function in braille translator
    EngineMain(inputBuffer, outputBuffer);
    return 0;
}
