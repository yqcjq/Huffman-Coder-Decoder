#include <stdio.h>
#include <string.h>
#define MAX_SIZE 1024


// 将二进制字符串转换为位编码
void binaryToByteData(const char *binary, unsigned char *byteData) {
  int len = strlen(binary);
  int byteIndex = 0;
  for (int i = 0; i < len; i += 8) {
      int value = 0;
      for (int j = 0; j < 8; j++) {
          value = (value << 1) | (binary[i + j] - '0');
      }
      byteData[byteIndex++] = (unsigned char)value;
  }
  byteData[byteIndex] = '\0';
}


int main(){
  const char binary[] = "1010101011001100";
  unsigned char byteData[100];

  binaryToByteData(binary, byteData);
  printf("Binary: %s\n", binary);
  int len = strlen(binary) / 8;
  for (int i = 0; i < len; i++) {
      printf("%02x", byteData[i]);
  }
  printf("\n");
  printf("byteData: %02x\n",  byteData[0]);
  printf("byteData: %02x\n",  byteData[1]);
}
