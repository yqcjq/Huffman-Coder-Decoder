#include<stdio.h>
// 从十六进制字符转换为对应的整数值
int hexCharToInt(char hexChar) {
  if (hexChar >= '0' && hexChar <= '9') {
      return hexChar - '0';
  } else if (hexChar >= 'A' && hexChar <= 'F') {
      return hexChar - 'A' + 10;
  } else if (hexChar >= 'a' && hexChar <= 'f') {
      return hexChar - 'a' + 10;
  }
  return -1;
}

// // 将十六进制字符串转换为二进制字符串
// void hexToBinary(const char *hex, char *binary, int bitCount, int hexIndex) {
//   int binIndex = 0;
//   while (bitCount > 0) {
//       int hexVal = hexCharToInt(hex[hexIndex]);
//       for (int i = 7; i >= 0 && bitCount > 0; i--) {
//           binary[binIndex++] = (hexVal & (1 << i))? '1' : '0';
//           bitCount--;
//       }
//       hexIndex++;
//   }
//   binary[binIndex] = '\0';
// }

void hexToBit(const unsigned char *hex, char *binary, int byteCount) {
  int binIndex = 0;
  for (int i = 0; i < byteCount; i++) {
      unsigned char byte = hex[i];
      for (int j = 7; j >= 0; j--) {
          binary[binIndex++] = (byte & (1 << j)) ? '1' : '0';
      }
  }
  binary[binIndex] = '\0';
}

int main() {


  // 测试 hexToBinary 函数
  char *testHexStr ;
  testHexStr[0] = 0x61;
  testHexStr[1] = 0x0;
  char a = 0x61;
  char binaryResult[100]="";
  int bitCount = 1;
  int hexIndex = 0;
  hexToBit(testHexStr, binaryResult, bitCount);
  printf("十六进制字符串 \"%s\" 转换后的二进制字符串是: %s\n", testHexStr, binaryResult);
  printf("a是%c", a);

  return 0;
}