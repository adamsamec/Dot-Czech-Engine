/*
 * main.c
 * Main of Dot Watch Braille Engine
 *
 * Author: Adam Samec
 *
 * Copyright (c) Dot Incorporation.
 */
#include <stdint.h>
#include <stdio.h>

#include "./../dot-czech-engine/app_config.h"
#include "./../dot-czech-engine/czech.h"
#include "./../dot-czech-engine/debug.h"
#include "./../dot-czech-engine/language.h"
#include "buf.h"
#include "verification.h"

// maximum size(byte) of input character in dot watch
#define MAX_INPUT_LENGTH 256

// maximum size(byte) of dot watch braille code
#define MAX_BRAILLE_LENGTH 512

// Max length of a piece of sample text for translation
#define MAX_PIECE_LENGTH 200

// Size of the file buffer which stores the whole input sample text
#define SAMPLE_FILE_BUFFER_SIZE 200000

// Size of the file buffer which stores the whole reference text
#define REFERENCE_FILE_BUFFER_SIZE 200000

// Input file paths
const char INPUT_FILEPATHS[][3][40] = {
    {"input/input-sample-economics.txt", "input/reference-text-economics.txt",
     "Economics high school textbook"},
    {"input/input-sample-fb.txt", "input/reference-text-fb.txt",
     "Facebook posts"},
    {"input/input-sample-physics.txt", "input/reference-text-physics.txt",
     "Physics high school textbook"},
    {"input/input-sample-twitter.txt", "input/reference-text-twitter.txt",
     "Twitter posts"},
    0};

/*
 * main function in braille translator
 *
 * parameter
 *     uint8_t *samplePiece - charactet(UTF8) array to translate (from mobile)
 *     uint8_t *enginePiece - translated braille code array (to dot watch)
 * return
 *     int - length of braille code array
 */
int EngineMain(uint16_t *samplePieceUtf16, uint8_t *enginePiece) {

  //    uint16_t samplePieceUtf16[MAX_INPUT_LENGTH] = { 0 };
  // braille code array
  uint8_t brailleCode[MAX_BRAILLE_LENGTH] = {0};

  // unicode braille code array
  uint16_t unicodeBrailleCode[MAX_BRAILLE_LENGTH] = {0};

  // braille code length
  int brailleCodeLength = 0;

  // convert UTF-8 data to UTF-16 data
  //    Utf8IntoUtf16(samplePiece, samplePieceUtf16);

  // select language
  switch (LanguageType(m_app_config->lang.major)) {
  // additional language e.g. Korean
  case CZECH:
    brailleCodeLength =
        CzechTranslateIntoBraille(samplePieceUtf16, brailleCode);
    break;

  // case KOREAN:
  // brailleCodeLength = KoreanTranslateIntoBraille(samplePieceUtf16,
  // brailleCode); break;

  // default language: English
  default:
    // brailleCodeLength = EnglishTranslateIntoBraille(samplePieceUtf16,
    // brailleCode);
    break;
  }

  // write braille code data into enginePiece from brailleCode
  for (int i = 0; i < brailleCodeLength; i++) {
    enginePiece[i] = brailleCode[i];
  }

  //    EngineBrailleCodeIntoBrailleUnicode(enginePiece, unicodeBrailleCode,
  //    brailleCodeLength);

  // printf("Braille:\n");
  for (int i = 0; i < brailleCodeLength; i++) {
    // gprintBits(1, brailleCode[i]);
    // printf("\n");
    // printDots(brailleCode[i]);
    // printf("-");
  }
  // printf("\n");

  // return braille code length
  return brailleCodeLength;
}

// Load the file in the given path into the given buffers
int LoadFile(const char *path, char *buffer, size_t bufferSize,
             uint16_t *textUtf16) {
  size_t readPos = 0;
  int remainingBufferSpace = bufferSize;
  FILE *file = fopen(path, "r");

  if (file == 0) {
    perror("Error opening file");
    return -1;
  }

  while (fgets(buffer + readPos, remainingBufferSpace, file)) {
    readPos = strlen(buffer);
    remainingBufferSpace = bufferSize - readPos;
  }
  fclose(file);

  // Fix some false failures by skipping the strange file beginnings created by
  // Windows Notepad
  if (buffer[0] == 0xFFFFFFEF && buffer[1] == 0xFFFFFFBB &&
      buffer[2] == 0xFFFFFFBF) {
    buffer += 3;
  }

  // Convert the UTF-8 buffer to the UTF:16 text
  Utf8IntoUtf16((uint8_t *)buffer, textUtf16);
  int textLength = LetterLengthUTF16(textUtf16);

  return textLength;
}

int main()

{
  int totalWordsNum = 0;
  int totalFailuresNum = 0;
  int totalSampleCharsNum = 0;
  int fileWordsNum = 0;
  int fileFailuresNum = 0;

  // Loop all the input files
  for (int f = 0; f < 4; f++) {
    fileWordsNum = 0;
    fileFailuresNum = 0;

    printf("%s\n", INPUT_FILEPATHS[f][2]);

    // Read the input sample text file
    char sampleFileBuffer[SAMPLE_FILE_BUFFER_SIZE] = {0};
    uint16_t sampleTextUtf16[SAMPLE_FILE_BUFFER_SIZE] = {0};
    int sampleTextLength = LoadFile(INPUT_FILEPATHS[f][0], sampleFileBuffer,
                                    SAMPLE_FILE_BUFFER_SIZE, sampleTextUtf16);

    totalSampleCharsNum += sampleTextLength;

    if (sampleTextLength < 0) {
      return -1;
    }

    // Read the reference text file
    char referenceFileBuffer[REFERENCE_FILE_BUFFER_SIZE] = {0};
    uint16_t referenceTextUtf16[REFERENCE_FILE_BUFFER_SIZE] = {0};
    int referenceTextLength =
        LoadFile(INPUT_FILEPATHS[f][1], referenceFileBuffer,
                 REFERENCE_FILE_BUFFER_SIZE, referenceTextUtf16);

    if (referenceTextLength < 0) {
      return -1;
    }

    // Replace the less and greater than signs in the reference text with the
    // proper characters fixing some false failures
    for (int i = 0; i < referenceTextLength; i++) {
      if (referenceTextUtf16[i] == 0x003C) // less than sign
      {
        referenceTextUtf16[i] = 0x011B; // letter "ě"
      }
      if (referenceTextUtf16[i] == 0x003E) // greater than sign
      {
        referenceTextUtf16[i] = 0x00E9; // letter "é"
      }
    }

    size_t samplePieceStart = 0;
    size_t samplePieceEnd = 0;
    uint16_t samplePiece[MAX_INPUT_LENGTH];

    size_t referenceBufPieceStart = 0;
    size_t referenceBufPieceEnd = 0;
    uint16_t referenceBufPiece[MAX_BRAILLE_LENGTH];

    int referenceWordsToRead = 0;
    int isSamplePrevWordSeparator = 1;
    ;
    int isReferencePrevWordSeparator = 1;
    ;
    int i = 0;

    // Cut the input sample text into pieces, translate them and compare them
    for (; i < sampleTextLength; i++) // begin for 1
    {
      // Check if a word separator or the end of the text is reached
      if (isWordSeparator(sampleTextUtf16[i]) ||
          (i + 1) == sampleTextLength) // begin if 1
      {
        if (!isSamplePrevWordSeparator) // begin if 2
        {
          referenceWordsToRead++;
        } // end if 2
        isSamplePrevWordSeparator = 1;
        // Check if the piece length exceeds the maximum length or if it is the
        // end of the sample text
        if (((i - samplePieceStart) >= MAX_PIECE_LENGTH) ||
            (i + 1) == sampleTextLength) // begin if 2
        {

          // Extract the input sample text into a smaller piece
          int j = 0;
          for (j = 0; j < (i - samplePieceStart + 1); j++) // begin for 2
          {
            samplePiece[j] = sampleTextUtf16[samplePieceStart + j];
          } // end for 2
          // Terminate the piece string
          samplePiece[j] = 0;

          // printf("sample: %s ", samplePiece);

          uint8_t enginePiece[MAX_BRAILLE_LENGTH] = {0};

          // Main function in braille translator
          int brailleCodeLength = EngineMain(samplePiece, enginePiece);

          // printf("BUF\n");
          // printf("words: %d,, chars: %d\n", referenceWordsToRead, j);

          // Convert 8-bit braille into 16-bit BUF braille for verification
          uint16_t engineBufPiece[MAX_BRAILLE_LENGTH];
          BrailleIntoBuf(enginePiece, engineBufPiece, brailleCodeLength);

          // Extract the reference text piece for verification

          while (referenceWordsToRead > 0) // begin while 1
          {
            if (isWordSeparator(referenceTextUtf16[referenceBufPieceEnd]) ||
                (referenceBufPieceEnd) == referenceTextLength) // begin if 3
            {
              if (!isReferencePrevWordSeparator) // begin if 4
              {
                referenceWordsToRead--;
                isReferencePrevWordSeparator = 1;
              }      // end if 4
            } else { // else if 3
              isReferencePrevWordSeparator = 0;
            }                              // end if 3
            if (referenceWordsToRead == 0) // begin if 3
            {
              int k;
              // Extract the reference text into a smaller piece
              for (k = 0; k < (referenceBufPieceEnd - referenceBufPieceStart);
                   k++) // begin for 2
              {
                referenceBufPiece[k] =
                    referenceTextUtf16[referenceBufPieceStart + k];

              } // end for 2
              // Terminate the piece string
              referenceBufPiece[k] = 0;

              // printf("ref: %s", referenceBufPiece);

              // Verify the engine BUF braille output against the translated
              // reference text in BUF braille format
              int wordsNum, failuresNum;
              Verify(referenceBufPiece, engineBufPiece, &wordsNum,
                     &failuresNum);

              fileWordsNum += wordsNum;
              fileFailuresNum += failuresNum;

              totalWordsNum += wordsNum;
              totalFailuresNum += failuresNum;

              referenceBufPieceStart = referenceBufPieceEnd;
            } // end if 3
            referenceBufPieceEnd++;

          } // end while 1
          samplePieceStart = i;
          referenceWordsToRead = 0;
        } // end if 2
        samplePieceEnd = i;
      } else { // else if 1
        // Thecharacter is not a word separator
        isSamplePrevWordSeparator = 0;
      } // end if 1
    }   // end for 1
    // Print the results for the current file
    double fileAccuracyRate =
        100 - (double)100 * fileFailuresNum / fileWordsNum;
    printf("File failures: %d, file words: %d, accuracy rate: %.3f %%, file "
           "characters: %d\n",
           fileFailuresNum, fileWordsNum, fileAccuracyRate, sampleTextLength);
  }
  // Print the total results
  double totalAccuracyRate =
      100 - (double)100 * totalFailuresNum / totalWordsNum;
  printf("Total failures: %d, total words: %d, accuracy rate: %.3f %%, total "
         "characterss: %d\n",
         totalFailuresNum, totalWordsNum, totalAccuracyRate,
         totalSampleCharsNum);

  return 0;
}
