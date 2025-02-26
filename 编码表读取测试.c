#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// 哈夫曼树节点结构体
typedef struct HuffmanNode {
  char ch;
  struct HuffmanNode *left, *right;
} HuffmanNode;

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
  // 将十六进制字符串转换为二进制字符串
void hexToBinary(const char *hex, char *binary, int bitCount) {
  int hexIndex = 0;
  int binIndex = 0;
  while (bitCount > 0) {
      int hexVal = hexCharToInt(hex[hexIndex]);
      for (int i = 3; i >= 0 && bitCount > 0; i--) {
          binary[binIndex++] = (hexVal & (1 << i))? '1' : '0';
          bitCount--;
      }
      hexIndex++;
  }
  binary[binIndex] = '\0';
}

// 创建新的哈夫曼树节点
HuffmanNode* newNode() {
  HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
  if (node == NULL) {
      perror("内存分配失败");
    }
  node->ch = '\0';
  node->left = node->right = NULL;
  return node;
}


// 构建哈夫曼树
void buildHuffmanTree(HuffmanNode *root, char *code, char ch) {
  HuffmanNode *current = root;
  for (int i = 0; code[i] != '\0'; i++) {
      if (code[i] == '0') {
          if (current->left == NULL) {
              current->left = newNode();
          }
          current = current->left;
      } else {
          if (current->right == NULL) {
              current->right = newNode();
          }
          current = current->right;
      }
  }
  current->ch = ch;
}



 int main(){
  HuffmanNode *root = newNode();
  char ch[3];
  int count,i;
  char line[100] = "0x2E0x120xC10x720x80";
  char hexCode[30];
  ch[0] = line[2];
  ch[1] = line[3];
  ch[2] = '\0';
  int bitCount = hexCharToInt(line[6]) * 16 + hexCharToInt(line[7]);
  for (count = 0,i=0; count < ((bitCount-1) / 8 + 1); count++)
  {
    hexCode[i] = line[i+10+2*count];
    i++;
    hexCode[i] = line[i+10+2*count];
    i++;
  }
  hexCode[i] = '\0';

  printf("%s\n", hexCode);
  char binaryCode[100];
  hexToBinary(hexCode, binaryCode, bitCount);
  printf("%s\n", binaryCode);
  buildHuffmanTree(root, binaryCode, (char)strtol(ch, NULL, 16));
 }
