
/*
 * verification.h
 * Verification of the engine BUF (Braille Universal format) braille output
 * against reference texts in BUF braille format
 *
 * Author: Adam Samec
 *
 * Copyright (c) Dot Incorporation.
 */

#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "./../dot-czech-engine/debug.h"
#include "./../dot-czech-engine/language.h"

#define MAX_BRAILLE_LENGTH 512

// Max length of the last processed word
#define MAX_WORD_LENGTH 50

// UTF-16 word separating character codes
const uint16_t WORD_SEPARATORS[] = {0x0020, // " " space
                                    0x00A0, // " " no-break space
                                    0x0009, // "	" character tabulation
                                    0x000B, // line tabulation
                                    0x000A, // line feed
                                    0x000D, // carriage return
                                    0x002D, // hyphen
                                    0x0096, // "–" n-dash #116
                                    0x2013, // "–" n-dash
                                    // 0x002F, // slash
                                    0};

// Determines if the given UTF-16 character is a word separator
int isWordSeparator(uint16_t chr) {
  for (int i = 0; WORD_SEPARATORS[i]; i++) {
    if (chr == WORD_SEPARATORS[i]) {
      return 1;
    }
  }
  return 0;
}

// Verify the given engine BUF braille output against the given reference text
// in BUF braille format
void Verify(uint16_t *referenceBufPiece, uint16_t *engineBufPiece,
            int *wordsNum, int *failuresNum) {
  *wordsNum = 0;
  *failuresNum = 0;

  // Set the locale according to system environment
  char *locale = setlocale(LC_ALL, "");

  // Create 32-bit strings for debugging
  int x;
  uint32_t referenceBufPieceUtf32[MAX_BRAILLE_LENGTH] = {0};
  for (x = 0; referenceBufPiece[x]; x++) {
    referenceBufPieceUtf32[x] = referenceBufPiece[x];
  }

  uint32_t engineBufPieceUtf32[MAX_BRAILLE_LENGTH] = {0};
  for (x = 0; engineBufPiece[x]; x++) {
    engineBufPieceUtf32[x] = engineBufPiece[x];
  }

  int i = 0;
  int j = 0;
  int r = 0;
  int e = 0;
  int isReferenceWordSeparator = 0;
  int isEngineWordSeparator = 0;
  int skipreferenceWord = 0;
  int isPrevReferenceCharWordSeparator = 1;
  uint32_t referenceWord[MAX_WORD_LENGTH];
  uint32_t engineWord[MAX_WORD_LENGTH];
  while (referenceBufPiece[i]) // begin while 1
  {
    isReferenceWordSeparator = isWordSeparator(referenceBufPiece[i]);
    if (isReferenceWordSeparator) // begin if 1
    {
      // Do nothing for word separators, just reset flags
      skipreferenceWord = 0;
      isPrevReferenceCharWordSeparator = 1;
    } else {                                // else if 1
      if (isPrevReferenceCharWordSeparator) // begin if 2
      {
        // Beginning of a reference text word, i.e., the word is not a word
        // separator but the previous char was a word separator
        (*wordsNum)++;
        isPrevReferenceCharWordSeparator = 0;
        r = 0;
      } // end if 2
      // Copy the reference text word
      referenceWord[r] = referenceBufPiece[i];
      r++;
      if (skipreferenceWord &&
          (!referenceBufPiece[i + 1] ||
           (referenceBufPiece[i + 1] &&
            isWordSeparator(referenceBufPiece[i + 1])))) // begin if 2
      {
        // End of an erroneous reference text word, i.e.,The current char is
        // erroneous, is not a word separator and it is the last char or it is a
        // char before a word separator
        referenceWord[r] = 0;
        if (DEBUG_LEVEL >= 2)
          wprintf(L"reference word: %ls\n", referenceWord);
        if (DEBUG_LEVEL >= 3)
          wprintf(L"r: %ls\ne: %ls\n", referenceBufPieceUtf32,
                  engineBufPieceUtf32);
      }                                                // end if 2
    }                                                  // end if 1
    if (isReferenceWordSeparator || skipreferenceWord) // begin if 1
    {
      // skip the reference word separator or erroneous word
      i++;
      continue;
    } else { // else if 1
      // The reference text char is not a word separator nor it is an erroneous
      // word
      while (isWordSeparator(engineBufPiece[j])) // begin while 2
      {
        // Skip the engine word separator
        j++;
      } // end while 2
      isEngineWordSeparator = 0;

      // compare the reference text character with the engine character
      if (referenceBufPiece[i] != engineBufPiece[j]) // begin if 2
      {
        // The characters differ
        (*failuresNum)++;
        skipreferenceWord = 1;
        i++;
        while (!isWordSeparator(engineBufPiece[j])) // begin while 2
        {
          // Skip the engine word because it differs and stop at a word
          // seperator Copy the engine word
          engineWord[e] = engineBufPiece[j];
          e++;
          j++;
        } // end while 2
        isEngineWordSeparator = 1;
      } else { // else if 2
        // The characters are the same, proceed to the next char
        // Copy the engine word
        engineWord[e] = engineBufPiece[j];
        e++;
        i++;
        j++;
        isEngineWordSeparator = isWordSeparator(engineBufPiece[j]);
      }                        // end if 2
    }                          // end if 1
    if (isEngineWordSeparator) // begin if 1
    {
      // Engine word separator, reset the engine word
      engineWord[e] = 0;
      e = 0;
      if (skipreferenceWord) // begin if 2
      {
        // the engine word has had a failure
        if (DEBUG_LEVEL >= 2)
          wprintf(L"> Failure:\nengine word: %ls\n", engineWord);
        if (!referenceBufPiece[i] ||
            (referenceBufPiece[i] &&
             isWordSeparator(referenceBufPiece[i]))) // begin if 3
        {
          // End of an erroneous reference text word, i.e.,The current char is
          // erroneous, is not a word separator and it is the last char or it is
          // a char before a word separator
          referenceWord[r] = 0;
          if (DEBUG_LEVEL >= 2)
            wprintf(L"reference word: %ls\n", referenceWord);
          if (DEBUG_LEVEL >= 3)
            wprintf(L"r: %ls\ne: %ls\n", referenceBufPieceUtf32,
                    engineBufPieceUtf32);
        } // end if 3
      }   // end if 2
    }     // end if 1
  }       // end while 1
}