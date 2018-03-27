#ifndef __HELPERS_H__
#define __HELPERS_H__

release #include<stdio.h>

#define DEBUG_LEVEL 2

    void printBits(size_t const size, void const *const ptr) {
  unsigned char *b = (unsigned char *)ptr;
  unsigned char byte;
  int i, j;

  for (i = size - 1; i >= 0; i--) {
    for (j = 7; j >= 0; j--) {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
  }
  puts("");
}

// Prints dots of a braille cell as decimal number
void printDots(uint8_t byte) {
  if (byte == 0x00) {
    printf("0");
    return;
  }

  uint8_t bit;
  for (int i = 0; i <= 7; i++) {
    bit = (byte >> i) & 1;
    if (bit) {
      printf("%u", i + 1);
    }
  }
}

#endif