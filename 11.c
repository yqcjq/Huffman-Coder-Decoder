#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#define MAX_SIZE 1024

// 手动定义 TIME_UTC
#ifndef TIME_UTC
#define TIME_UTC 1
#endif

// FNV-1a 64位哈希算法的初始值
#define FNV1A_64_INIT 0xcbf29ce484222325ULL
// FNV-1a 64位哈希算法的质数
#define FNV1A_64_PRIME 0x100000001b3
#define BUFFER_SIZE 500

 
uint64_t fnv1a_64(const void *data, size_t length) 
{
    uint64_t hash = FNV1A_64_INIT;
    const uint8_t *byte_data = (const uint8_t *)data; 
    for (size_t i = 0; i < length; i++) {
        hash ^= byte_data[i];
        hash *= FNV1A_64_PRIME;
    }
     return hash;
}


// 函数用于计算文本文件的 FNV-1a 64 位哈希值
uint64_t calculateFileHash(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
      perror("无法打开文件");
      return 0;
  }

  // 获取文件大小
  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  // 分配内存来读取文件内容
  char *buffer = (char *)malloc(fileSize);
  if (buffer == NULL) {
      perror("内存分配失败");
      fclose(file);
      return 0;
  }

  // 读取文件内容到缓冲区
  size_t bytesRead = fread(buffer, 1, fileSize, file);
  fclose(file);

  if (bytesRead!= fileSize) {
      perror("读取文件时出错");
      free(buffer);
      return 0;
  }

  // 计算哈希值
  uint64_t hash = fnv1a_64(buffer, bytesRead);

  // 释放内存
  free(buffer);

  return hash;
}


// 定义一个结构体来存储字符及其频次
typedef struct
{
  char ch;
  int freq;
  struct CharFreq *left, *right;
} CharFreq;

// 定义哈夫曼树节点结构体
typedef struct HuffmanNode
{
  char ch;
  int freq;
  struct HuffmanNode *left, *right;
} HuffmanNode;

// 交换两个 CharFreq 结构体的函数
void swap(CharFreq *a, CharFreq *b)
{
  CharFreq temp = *a;
  *a = *b;
  *b = temp;
}

// 交换两个 CharFreq 结构体的函数
void swap1(HuffmanNode **a, HuffmanNode **b)
{
  HuffmanNode *temp = *a;
  *a = *b;
  *b = temp;
}

// 清除输入缓冲区的函数
void clearInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

//读取文件信息
char *readFile(char *filePath)
{
  FILE *file;
  char *fileContent;
  int fileSize;
  // 打开文件，以只读模式打开
  file = fopen(filePath, "rb");
  if (file == NULL)
  {
    perror("无法打开文件");
    return NULL;
  }
  // 将文件指针移动到文件末尾
  fseek(file, 0, SEEK_END);
  // 获取文件大小
  fileSize = ftell(file);
  // 将文件指针移回文件开头
  fseek(file, 0, SEEK_SET);
  printf("文件大小为: %d 字节\n", fileSize);


  // 动态分配内存
  fileContent = (char *)malloc((fileSize + 1) * sizeof(char));
  if (fileContent == NULL)
  {
    perror("内存分配失败");
    fclose(file);
    return NULL;
  }

  // 读取文件内容
  fread(fileContent, sizeof(char), fileSize, file);
  // 确保字符串以 '\0' 结尾
  fileContent[fileSize] = '\0';

  // 关闭文件
  fclose(file);

  return fileContent;
}

//读取文件信息
char *readFileAndWriteSize(char *filePath,char *filePath3)
{
  FILE *file;
  char *fileContent;
  int fileSize;
  // 打开文件，以只读模式打开
  file = fopen(filePath, "rb");
  if (file == NULL)
  {
    perror("无法打开文件");
    return NULL;
  }
  // 将文件指针移动到文件末尾
  fseek(file, 0, SEEK_END);
  // 获取文件大小
  fileSize = ftell(file);
  // 将文件指针移回文件开头
  fseek(file, 0, SEEK_SET);
  printf("文件大小为: %d 字节\n", fileSize);


  // 动态分配内存
  fileContent = (char *)malloc((fileSize + 1) * sizeof(char));
  if (fileContent == NULL)
  {
    perror("内存分配失败");
    fclose(file);
    return NULL;
  }

  // 读取文件内容
  fread(fileContent, sizeof(char), fileSize, file);
  // 确保字符串以 '\0' 结尾
  fileContent[fileSize] = '\0';

  // 关闭文件
  fclose(file);

  // 打开用于写入的文件
  FILE *file3 = fopen(filePath3, "wb");
  if (file3 == NULL) {
      perror("无法打开输出文件");
      return NULL;
  }

  // 将文件大小信息写入文件
  fprintf(file3, "%ld\n", fileSize);

  // 关闭输出文件
  fclose(file3);

  return fileContent;
}

// 将文件每个字节加上0x55并覆盖原文件
int addOffsetToFile(const char *filename, int m) {
  FILE *file = fopen(filename, "rb+");  // 以读写二进制模式打开文件
  if (file == NULL) {
      perror("无法打开文件");
      return -1;
  }

  // 获取文件大小
  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  // 读取文件内容到缓冲区
  unsigned char *buffer = (unsigned char *)malloc(fileSize);
  if (buffer == NULL) {
      perror("内存分配失败");
      fclose(file);
      return -1;
  }

  if (fread(buffer, 1, fileSize, file) != fileSize) {
      perror("读取文件时出错");
      free(buffer);
      fclose(file);
      return -1;
  }

  if(m == 1) {
      // 对每个字节加上0x55
      for (long i = 0; i < fileSize; i++) {
          buffer[i] = (buffer[i] + 0x55);
      }
  }else if (m == 0){
      for (long i = 0; i < fileSize; i++) {
          buffer[i] = (buffer[i] - 0x55);
      }
  }

  // 将修改后的内容写回文件
  fseek(file, 0, SEEK_SET);
  if (fwrite(buffer, 1, fileSize, file) != fileSize) {
      perror("写入文件时出错");
      free(buffer);
      fclose(file);
      return -1;
  }

  // 关闭文件并释放内存
  fclose(file);
  free(buffer);
  return 0;
}


// 插入收件人和发件人信息到文件开头的函数
void insertInfoToFile(char *filePath, char *senderId, char *senderName, char *recipientId, char *recipientName) {
  // 打开原文件以读取内容
  FILE *originalFile = fopen(filePath, "r");
  if (originalFile == NULL) {
      perror("无法打开原文件");
      return;
  }

  // 创建一个临时文件以写入新内容
  FILE *tempFile = fopen("temp.txt", "w");
  if (tempFile == NULL) {
      perror("无法创建临时文件");
      fclose(originalFile);
      return;
  }

  // 生成收件人和发件人信息字符串
  char recipientInfo[100];
  char senderInfo[100];
  snprintf(recipientInfo, sizeof(recipientInfo), "U%s，%s\n", recipientId, recipientName);
  snprintf(senderInfo, sizeof(senderInfo), "U%s，%s\n", senderId, senderName);

  // 将收件人和发件人信息写入临时文件
  fputs(recipientInfo, tempFile);
  fputs(senderInfo, tempFile);

  // 将原文件内容复制到临时文件
  int ch;
  while ((ch = fgetc(originalFile)) != EOF) {
      fputc(ch, tempFile);
  }

  // 关闭原文件和临时文件
  fclose(originalFile);
  fclose(tempFile);

  // 删除原文件
  if (remove(filePath) != 0) {
      perror("无法删除原文件");
      return;
  }

  // 将临时文件重命名为原文件
  if (rename("temp.txt", filePath) != 0) {
      perror("无法重命名临时文件");
      return;
  }

  printf("信息插入成功！\n");
}

// 输出十六进制编码文本文件的最后十六个字节
void printLastSixteenBytes(const char *filename) {
  // 以二进制只读模式打开文件
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
      // 若文件打开失败，输出错误信息
      perror("无法打开文件");
      return;
  }

  // 将文件指针移动到文件末尾
  if (fseek(file, 0, SEEK_END) != 0) {
      // 若移动文件指针失败，输出错误信息，关闭文件并返回
      perror("无法定位到文件末尾");
      fclose(file);
      return;
  }

  // 获取文件的当前位置，即文件大小
  long fileSize = ftell(file);
  if (fileSize == -1) {
      // 若获取文件大小失败，输出错误信息，关闭文件并返回
      perror("无法获取文件大小");
      fclose(file);
      return;
  }

  // 计算需要读取的起始位置
  long startPosition = (fileSize >= 16) ? fileSize - 16 : 0;
  // 将文件指针移动到起始位置
  if (fseek(file, startPosition, SEEK_SET) != 0) {
      // 若移动文件指针失败，输出错误信息，关闭文件并返回
      perror("无法定位到读取起始位置");
      fclose(file);
      return;
  }

  unsigned char buffer[16];
  // 从文件中读取最多 16 个字节到缓冲区
  size_t bytesRead = fread(buffer, 1, 16, file);
  if (bytesRead == 0) {
      // 若读取失败，输出错误信息
      perror("读取文件时出错");
  } else {
    printf("文件%s的最后十六字节为：", filename);
    for (size_t i = 0; i < bytesRead; i++)
    {
      if (i > 0)
      {
        // 除了第一个字节，每个字节前输出一个空格
        printf(" ");
      }
      // 以 0x 开头，两位十六进制数的格式输出字节
      printf("0x%02x", buffer[i]);
      }
      printf("\n");
  }

  // 关闭文件
  fclose(file);
}


// 函数用于获取文件的字节数大小
long getFileSize(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
      perror("无法打开文件");
      return -1;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
      perror("无法定位到文件末尾");
      fclose(file);
      return -1;
  }

  long fileSize = ftell(file);
  if (fileSize == -1) {
      perror("无法获取文件大小");
      fclose(file);
      return -1;
  }

  if (fseek(file, 0, SEEK_SET) != 0) {
      perror("无法定位到文件开头");
      fclose(file);
      return -1;
  }

  fclose(file);
  return fileSize;
}

// 函数用于计算并输出第一个文件字节数占第二个文件字节数的百分比
void printFileSizePercentage(const char *file1Path, const char *file2Path) {
  long size1 = getFileSize(file1Path);
  long size2 = getFileSize(file2Path);

  if (size1 == -1 || size2 == -1) {
      // 如果有任何一个文件获取大小失败，直接返回
      return;
  }

  if (size2 == 0) {
      printf("第二个文件大小为 0，无法计算百分比。\n");
      return;
  }

  double percentage = ((double)size1 / size2) * 100;
  printf("文件 %s 的字节数占文件 %s 字节数的百分比为: %.2f%%\n", file1Path, file2Path, percentage);
}

int writeFile(const char *filename, const void *array, size_t element_size, size_t len) {
  // 以写入模式打开文件
  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    perror("无法打开文件");
    return -1;
}

// 使用 fwrite 函数将数组内容写入文件
size_t elements_written = fwrite(array, element_size, len, file);
if (elements_written != len) {
    perror("写入文件时发生错误");
    fclose(file);
    return -1;
}

// 关闭文件
if (fclose(file) != 0) {
    perror("关闭文件时发生错误");
    return -1;
}

  // 写入成功，返回 0
  printf("关闭文件成功\n");
  return 0;
}

// 调整最小堆的函数
void minHeapify(CharFreq arr[], int n, int i) {
  int smallest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  // 先比较频率，如果频率相同再比较字符的字节值
  if (left < n) {
    if (arr[left].freq < arr[smallest].freq || 
        (arr[left].freq == arr[smallest].freq && (unsigned char)arr[left].ch < (unsigned char)arr[smallest].ch)) {
        smallest = left;
    }
  }

  if (right < n) {
    if (arr[right].freq < arr[smallest].freq || 
        (arr[right].freq == arr[smallest].freq && (unsigned char)arr[right].ch < (unsigned char)arr[smallest].ch)) {
        smallest = right;
    }
  }

  if (smallest != i) {
    swap(&arr[i], &arr[smallest]);
    minHeapify(arr, n, smallest);
  }
}

// 调整最小堆的函数
void minHeapify1(HuffmanNode *arr[], int n, int i) {
  int smallest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  // 先比较频率，如果频率相同再比较字符的字节值
  if (left < n) {
    if (arr[left]->freq < arr[smallest]->freq || 
        (arr[left]->freq == arr[smallest]->freq && (unsigned char)arr[left]->ch < (unsigned char)arr[smallest]->ch)) {
        smallest = left;
    }
  }

  if (right < n) {
    if (arr[right]->freq < arr[smallest]->freq || 
        (arr[right]->freq == arr[smallest]->freq && (unsigned char)arr[right]->ch < (unsigned char)arr[smallest]->ch)) {
        smallest = right;
    }
  }

  if (smallest != i) {
    swap1(&arr[i], &arr[smallest]);
    minHeapify1(arr, n, smallest);
  }
}

// 堆排序函数
void heapSort(CharFreq arr[], int n)
{
  // 构建最小堆，构建完后，此时arr[0]存着最小值
  for (int i = n / 2 - 1; i >= 0; i--)
    minHeapify(arr, n, i);

  // 一个个从堆中取出元素
  for (int i = n - 1; i > 0; i--)
  {
    //把最小值存在数组最后，对n-1进行最小堆排序，此时最小的存在a[0]中
      swap(&arr[0], &arr[i]);
      minHeapify(arr, i, 0);
  }

   // 反转数组，使结果按从小到大排列
   for (int i = 0; i < n / 2; i++) {
    swap(&arr[i], &arr[n - i - 1]);
  }
}

// 创建新的哈夫曼树节点
HuffmanNode *newNode(char ch, int freq)
{
  HuffmanNode *node = (HuffmanNode *)malloc(sizeof(HuffmanNode));
  if (node == NULL) {
    fprintf(stderr, "内存分配失败\n");
    return NULL;
  }
  node->ch = ch;
  node->freq = freq;
  node->left = node->right = NULL;
  return node;
}

// 补零，并将二进制字符串转换为位编码
void binaryToByteData(char *binary, unsigned char *byteData) {
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

// 将二进制字符串转换为十六进制字符串
void binaryToHex(const char *binary, char *hex) {
  int len = strlen(binary);
  int i, j = 0;
  for (i = 0; i < len; i += 4) {
      int value = 0;
      for (int k = 0; k < 4 && i + k < len; k++) {
          value = (value << 1) | (binary[i + k] - '0');
      }
      if (value < 10) {
          hex[j++] = value + '0';
      } else {
          hex[j++] = value - 10 + 'A';
      }
  }
  hex[j] = '\0';
}

// 二进制字符串转十进制数的函数
int binaryToDecimal(const char *binaryStr) {
  int len = strlen(binaryStr);
  int decimal = 0;
  for (int i = 0; i < len; i++) {
      if (binaryStr[i] == '1') {
          decimal += pow(2, len - i - 1);
      }
  }
  return decimal;
}

// 构建哈夫曼树
HuffmanNode *buildHuffmanTree(CharFreq arr[], int n)
{
  HuffmanNode **nodes = (HuffmanNode **)malloc(n * sizeof(HuffmanNode *));
  for (int i = 0; i < n; i++)
  {
    nodes[i] = newNode(arr[i].ch, arr[i].freq);
  }
  HuffmanNode *min1;
  HuffmanNode *min2;
  int i;
 
  while (n > 1)
  {
    // printf("进入循环时n的值为：%d\n", n);
    min1 = nodes[0];
    // printf("此时min1的值为：%c,%d\n", nodes[0]->ch, nodes[0]->freq);
    nodes[0] = nodes[n - 1];
    n--;
    for (int i = n / 2 - 1; i >= 0; i--) {
        minHeapify1(nodes, n, i);
    }
    min2 = nodes[0];
    // printf("此时min2的值为：%c,%d\n", nodes[0]->ch, nodes[0]->freq);
    nodes[0] = nodes[n - 1];
    n--;
    
    // 取两个孩子节点中的最大字节值
    unsigned char maxCh = ((unsigned char)min1->ch > (unsigned char)min2->ch) ? (unsigned char)min1->ch : (unsigned char)min2->ch;

    // 创建新节点，合并两个最小频率节点
    HuffmanNode *newInternalNode = newNode((char)maxCh, min1->freq + min2->freq);
    // printf("%c,%d\n", newInternalNode->ch, newInternalNode->freq);
    newInternalNode->left = min1;
    newInternalNode->right = min2;

    // 将新节点放入数组中
    nodes[n] = newInternalNode;
    n++;

    int i = n - 1;
    // while (i > 0)
    // {
    //     int parent = (i - 1) / 2;
    //     if (nodes[i]->freq < nodes[parent]->freq ||
    //       (nodes[i]->freq == nodes[parent]->freq && (unsigned char)nodes[i]->ch < (unsigned char)nodes[parent]->ch))
    //     {
    //         HuffmanNode *temp = nodes[i];
    //         nodes[i] = nodes[parent];
    //         nodes[parent] = temp;
    //         i = parent;
    //     }
      
    //     else
    //     {
    //         break;
    //     }
    // }
    for (int i = n / 2 - 1; i >= 0; i--) {
      minHeapify1(nodes, n, i);
  //     for (int i = 0; i < n; i++)
  // {
  //   printf("字符 '0x%02x' 出现的频次为: %d\n", (unsigned char)nodes[i]->ch, nodes[i]->freq);
  // }
  }
    // printf("\n");
  }
  // printf("成功结束哈夫曼树的构建了！\n");
  // printf("\n");
  // printf("0%c,%d\n", nodes[0]->ch, nodes[0]->freq);
  // printf("1%c,%d\n", nodes[0]->left->ch, nodes[0]->left->freq);
  // printf("2%c,%d\n", nodes[0]->right->ch, nodes[0]->right->freq);
  // printf("3%c,%d\n", nodes[0]->right->left->ch, nodes[0]->right->left->freq);
  // printf("4%c,%d\n", nodes[0]->right->right->ch, nodes[0]->right->right->freq);
  // // printf("5%c,%d\n", nodes[0]->right->right->left->ch, nodes[0]->right->right->left->freq);
  // // printf("6%c,%d\n", nodes[0]->right->right->right->ch, nodes[0]->right->right->right->freq);
  return nodes[0];
}

//保存哈夫曼编码表到文件
void saveHuffmanCodes(HuffmanNode *root, char huffmanCodes[256][100] , const char *filename3){

  char processedBinary[100];
  strcpy(processedBinary, huffmanCodes[(unsigned char)root->ch]);

   // 计算需要补零的位数
   int binaryLength = strlen(processedBinary);
   int padding = 8 - (binaryLength % 8);
   if (padding < 8) {
       for (int i = 0; i < padding; i++) {
           strcat(processedBinary, "0");
       }
   }

   char *byteData=processedBinary;
   FILE *file3 = fopen(filename3, "ab");
   if (file3 == NULL) {
    perror("无法打开输出文件");
   }

   fprintf(file3, "0x%02x 0x%02x", (unsigned char)root->ch, binaryLength);

  int binaryLength2 = strlen(processedBinary);
  for (int i = 0; i < binaryLength2; i += 8) {
      char group[9];
      strncpy(group, processedBinary + i, 8);
      group[8] = '\0';
      int decimal = 0;
      for (int j = 0; j < 8; j++) {
          if (group[j] == '1') {
              decimal += (int)pow(2, 7 - j);
          }
      }

     
      // 将文件大小信息写入文件
      fprintf(file3, " 0x%02x", decimal);
      // if(decimal !=)
      // 关闭输出文件
      
  }
  fprintf(file3, "\n");
  fclose(file3);

}

// 生成哈夫曼编码并存储到数组中
void generateHuffmanCodes(HuffmanNode *root, int arr[], int top, char huffmanCodes[256][100], const char *filename3) {
  if (root->left) {
      arr[top] = 0;
      generateHuffmanCodes(root->left, arr, top + 1, huffmanCodes, filename3);
  }
  if (root->right) {
      arr[top] = 1;
      generateHuffmanCodes(root->right, arr, top + 1, huffmanCodes, filename3);
  }
  if (!root->left && !root->right) {
      int i;
      for (i = 0; i < top; i++) {
          huffmanCodes[(unsigned char)root->ch][i] = arr[i] + '0';
      }
      huffmanCodes[(unsigned char)root->ch][i] = '\0';
      printf("字符 '0x%02x' 的哈夫曼编码为: %s\n", (unsigned char)root->ch, huffmanCodes[(unsigned char)root->ch]);
      saveHuffmanCodes(root, huffmanCodes, filename3);
  }
}

// 计算哈夫曼树的 WPL
int calculateWPL(HuffmanNode *root, int depth)
{
  if (root == NULL)
  {
    return 0;
  }
  if (root->left == NULL && root->right == NULL)
  {
    return root->freq * depth;
  }
  return calculateWPL(root->left, depth + 1) + calculateWPL(root->right, depth + 1);
}


// // 对文本进行哈夫曼编码，补零后，输出压缩后的二进制、十六进制、哈希值
// void encodeText(const char *filename, char huffmanCodes[256][100], char *content, const char *filename2)
// {
//   printf("正在进行哈夫曼编码\n ");
//   char binary[1000000] = "";  // 假设编码后的二进制字符串长度不超过 1000000
//   printf("压缩后的二进制文本为:\n ");
//   for (int i = 0; content[i] != '\0'; i++)
//   {
//     strcat(binary, huffmanCodes[(unsigned char)content[i]]);
//     printf("%s", huffmanCodes[(unsigned char)content[i]]);
//   }
//   printf("\n");

//   printf("已经压缩成二进制文本了\n ");
//  // 计算需要补零的位数
//  int binaryLength = strlen(binary);
//  int padding = 8 - (binaryLength % 8);
//  if (padding < 8) {
//      for (int i = 0; i < padding; i++) {
//          strcat(binary, "0");
//      }
//  }

 
 
//   // 将二进制字符串转换为十六进制字符串
//   unsigned char byteData[125000];  // 二进制转十六进制长度除以四
//   binaryToByteData(binary, byteData);
//   printf("补零后的二进制文本为:  %s\n",binary);
//   int len = strlen(binary) / 8;
//   printf("压缩后的bit位编码为: \n");
//   for (int i = 0,  j = 0; i < len; i++,j++) {
//     if(j == 16) {
//       printf("\n");
//       j = 0;
//     }
//       printf("%02x", byteData[i]);
//   }
//   printf("\n");
//   printf("压缩后的bit位编码第一位为: %x\n", byteData[0]);

//   //把编码写入文本文件

//   writeFile(filename2, byteData, sizeof(char), len);

//   // 计算 FNV-1a 64位哈希值
//   uint64_t hash1 = fnv1a_64(binary, strlen(binary));
//   uint64_t hash2 = fnv1a_64(byteData, strlen(byteData));
//   printf("压缩后文本二进制的HASH1值为: 0x%016llx\n", hash1);
//   printf("压缩后文本bit位编码的HASH2值为: 0x%016llx\n", hash2);
// }


// 对文本进行哈夫曼编码，补零后，输出压缩后的二进制、十六进制、哈希值
void encodeText(const char *filename, char huffmanCodes[256][100], char *content, const char *filename2)
{

    FILE *outputFile1 = fopen(filename2, "wb");
    fclose(outputFile1);
    FILE *outputFile = fopen(filename2, "ab");
    if (outputFile == NULL) {
        perror("无法打开输出文件");
        return;
    }

  char binary[1000] = "";  // 假设编码后的二进制字符串长度不超过 100
  int totalBytesWritten = 0;
  // printf("压缩后的二进制文本为:\n ");
    // 遍历 content 统计字符频次
  long size1 = getFileSize(filename);

  for (int i = 0; i < size1; i++)
  {
    strcat(binary, huffmanCodes[(unsigned char)content[i]]);
    // printf("%s", huffmanCodes[(unsigned char)content[i]]);
    if(strlen(binary)>=800) {
      int temp = strlen(binary) % 8;
      if (temp == 0) {
        unsigned char byteData[250];
        binaryToByteData(binary, byteData);
        int len = strlen(binary) / 8;
        // printf("十六进制代码\n%s\n ",byteData);
        // 写入十六进制数据到文件
        size_t bytesWritten = fwrite(byteData, sizeof(char), len, outputFile);
        totalBytesWritten += bytesWritten;
        binary[0] = '\0';
      } else {
        // 如果二进制字符串长度不是 8 的倍数，截取前面是 8 的倍数的部分进行处理
        int validLength = strlen(binary) - temp;
        char validBinary[1000];
        strncpy(validBinary, binary, validLength);
        validBinary[validLength] = '\0';
        unsigned char byteData[250];
        binaryToByteData(validBinary, byteData);
        // printf("十六进制代码\n%s\n ",byteData);
        // 写入十六进制数据到文件
        int len = validLength / 8;
        size_t bytesWritten = fwrite(byteData, sizeof(char), len, outputFile);
        totalBytesWritten += bytesWritten;
        strcpy(binary, binary + validLength);
        validBinary[0] = '\0';
      }
    }
  }
  // printf("\n");

  // printf("已经压缩成二进制文本了\n ");
  // printf("补零前%s\n ",binary);
 // 计算需要补零的位数
 int binaryLength = strlen(binary);
 int padding = 8 - (binaryLength % 8);
 if (padding < 8) {
     for (int i = 0; i < padding; i++) {
         strcat(binary, "0");
     }
 }

//  printf("补零后%s\n ",binary);
 
  // 将二进制字符串转换为十六进制字符串
  unsigned char byteData[250];  // 二进制转十六进制长度除以四
  binaryToByteData(binary, byteData);
  // printf("十六进制代码\n%02x\n ",byteData);
  // printf("补零后的二进制文本为:  %s\n",binary);
  int len = strlen(binary) / 8;
  // printf("压缩后的bit位编码为: \n");

  // printf("\n");
  // printf("压缩后的bit位编码第一位为: %x\n", byteData[0]);

  //把编码写入文本文件

  size_t bytesWritten = fwrite(byteData, sizeof(char), len, outputFile);
  totalBytesWritten += bytesWritten;

  // 计算 FNV-1a 64位哈希值
  // uint64_t hash1 = fnv1a_64(binary, strlen(binary));
  // uint64_t hash2 = fnv1a_64(byteData, strlen(byteData));
  // printf("压缩后文本二进制的HASH1值为: 0x%016llx\n", hash1);
  // printf("压缩后文本bit位编码的HASH2值为: 0x%016llx\n", hash2);
  fclose(outputFile);
}

// 压缩前，对文本进行位编码，输出编码后的十六进制值，并计算文本的哈希值
void bitEncodeAndHash(const char *content)
{
    int len = strlen(content);
    char binary[len * 8 + 1];  // 每个字符 8 位
    binary[0] = '\0';

    // 将每个字符转换为 8 位二进制表示
    for (int i = 0; i < len; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            char bit = ((content[i] >> j) & 1) + '0';
            strncat(binary, &bit, 1);
        }
    }

     // 将二进制字符串转换为十六进制字符串
     unsigned char byteData[len + 1];
     binaryToByteData(binary, byteData);
 
     // 计算 FNV-1a 64位哈希值
     uint64_t hash3 = fnv1a_64(binary, strlen(binary));
     uint64_t hash4 = fnv1a_64(binary, strlen(byteData));
     printf("压缩前文本转换成二进制后的哈希值为: 0x%016llx\n", hash3);
     printf("压缩前文本转换成bit位编码后的哈希值为: 0x%016llx\n", hash4);
}


// 统计文件中单字节字符频次并排序的函数
HuffmanNode* sortSingleByteCharsByFrequency(const char *filename, char *content, const char *filename2, const char *filename3)
{
 
  // 假设单字节字符集，共 256 个字符
  CharFreq freq[256];
  for (int i = 0; i < 256; i++)
  {
    freq[i].ch = (char)i;
    freq[i].freq = 0;
  }

  // 遍历 content 统计字符频次
  long size1 = getFileSize(filename);
  for (int i = 0; i < size1; i++)
  {
      freq[(unsigned char)content[i]].freq++;
  }

  // 过滤掉频次为 0 的字符
  int validCount = 0;
  for (int i = 0; i < 256; i++)
  {
    if (freq[i].freq > 0)
    {
      freq[validCount] = freq[i];
      validCount++;
    }
  }

  // 对字符按频次进行堆排序
  heapSort(freq, validCount);

  // 输出排序结果
  for (int i = 0; i < validCount; i++)
  {
    printf("字符 '0x%02x' 出现的频次为: %d\n", (unsigned char)freq[i].ch, freq[i].freq);
  }
  // 构建哈夫曼树
  HuffmanNode *root = buildHuffmanTree(freq, validCount);

  // 生成哈夫曼编码
  int arr[100], top = 0;
  char huffmanCodes[256][100];
  for (int i = 0; i < 256; i++) {
    huffmanCodes[i][0] = '\0';
  }
  
  generateHuffmanCodes(root, arr, top, huffmanCodes,filename3);

  // 计算 WPL
  // printf("接下来开始计算WPL了\n");
  int wpl = calculateWPL(root, 0);
  printf("哈夫曼树的带权路径长度 (WPL) 为: %d\n", wpl);

  // 对文本进行哈夫曼编码
  encodeText(filename, huffmanCodes, content, filename2);
  return root;
}

//释放哈夫曼树内存的函数
void freeHuffmanTree(HuffmanNode *root) {
  if (root == NULL) return;
  freeHuffmanTree(root->left);
  freeHuffmanTree(root->right);
  free(root);
}





//----------------------------以下开始是解码的函数---------------------------------




void deleteSpace(char *str)
{
	char *str_c=str;
	int i,j=0;
	for(i=0;str[i]!='\0';i++)
	{
		if(str[i]!=' ')
			str_c[j++]=str[i];
	}
	str_c[j]='\0';
	str=str_c;	
}

// 创建新的哈夫曼树节点
HuffmanNode* decodenewNode() {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    if (node == NULL) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }
    node->ch = '\0';
    node->left = node->right = NULL;
    return node;
}

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
void hexToBinary(const  char  *hex,unsigned char *binary, int bitCount) {
    int binIndex = 0;
    int hexIndex = 0;
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

// 将十六进制文件内容转换为二进制字符串
void hexFileToBinary(const char *filename, char *binary) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("无法打开文件");
        return;
    }

    int binIndex = 0;
    unsigned char byte;
    while (fread(&byte, 1, 1, file) == 1) {
        for (int i = 7; i >= 0; i--) {
            binary[binIndex++] = (byte & (1 << i))? '1' : '0';
        }
    }
    binary[binIndex] = '\0';
    fclose(file);
}

// 构建哈夫曼树
void decodebuildHuffmanTree(HuffmanNode *root, char *code, char ch) {
    HuffmanNode *current = root;
    for (int i = 0; code[i] != '\0'; i++) {
        if (code[i] == '0') {
            if (current->left == NULL) {
                current->left = decodenewNode();
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = decodenewNode();
            }
            current = current->right;
        }
    }
    current->ch = ch;
}

// 解码函数
void decode(char *filename, char *encodedFilename,  char *filename2) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("无法打开文件1");
        return;
    }

    // 读取解码后文本的总字节数
    int totalBytes;
    fscanf(file, "%d\n", &totalBytes);
    printf("读到的字节数为: %d\n", totalBytes);
    // 构建哈夫曼树的根节点
    HuffmanNode *root = decodenewNode();
    printf("成功建表\n");
    // 读取编码表
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        char ch[3];
        int bitCount;
        char hexCode[30];
        // sscanf(line, "0x%2s 0x%x %s", ch, &bitCount, hexCode);
        deleteSpace(line);
        ch[0] = line[2];
        ch[1] = line[3];
        ch[2] = '\0';
        bitCount = hexCharToInt(line[6]) * 16 + hexCharToInt(line[7]);
        int count,i;

        for (count = 0,i=0; count < ((bitCount-1) / 8 + 1); count++)
        {
          hexCode[i] = line[i+10+2*count];
          i++;
          hexCode[i] = line[i+10+2*count];
          i++;
        }
        hexCode[i] = '\0';
      
        char binaryCode[100];
        hexToBinary(hexCode, binaryCode, bitCount);
        printf("字符%s为%s\n", ch, binaryCode);
        decodebuildHuffmanTree(root, binaryCode, (char)strtol(ch, NULL, 16));
    }
    fclose(file);
    printf("成功建树\n");
    // int wpl1 = calculateWPL(root, 0);
    // printf("哈夫曼树的带权路径长度 (WPL) 为: %d\n", wpl1);

    // // 将十六进制文件内容转换为二进制字符串
    // char encodedBinary[100000];
    // hexFileToBinary(encodedFilename, encodedBinary);

    // // 解码过程
    // char decodedText[100000];
    // int decodedIndex = 0;
    // HuffmanNode *current = root;
    // for (int i = 0; encodedBinary[i] != '\0'; i++) {
    //     if (encodedBinary[i] == '0') {
    //         current = current->left;
    //     } else {
    //         current = current->right;
    //     }
    //     if (current->ch != '\0') {
    //         decodedText[decodedIndex++] = current->ch;
    //         current = root;
    //     }
    // }
    // decodedText[decodedIndex] = '\0';

    // // 输出解码后的文本
    
    // uint64_t hash1 = fnv1a_64(decodedText, strlen(decodedText));
    // printf("解码后的文本为: %s\n", decodedText);
    // printf("解码后的哈希值为: 0x%016llx\n", hash1);

    // // 将解码后的文本写入文件
    // char decodedFilename[100];

    // FILE *decodedFile = fopen(filename2, "w");
    // if (decodedFile == NULL) {
    //     perror("无法打开输出文件");
    //     return;
    // }
    // fputs(decodedText, decodedFile);
    // fclose(decodedFile);








    // FILE *outputFile1 = fopen(filename2, "wb");
    // fclose(outputFile1);

    // FILE *decodedFile = fopen(filename2, "ab");
    // if (decodedFile == NULL) {
    //     perror("无法打开输出文件");
    //     return;
    // }

    // unsigned char *content;
    // content = readFile(encodedFilename);
    // unsigned char byteData[1000] = "";
    // int len = 0;
    // unsigned char binary[8000] = "";

    // int decodedBytes = 0;
    
    // HuffmanNode *current = root;
    // unsigned char decodedText[2000];  // 用于存储每次解码的结果
    // printf("马上进入循环啦！\n");
    
    // int j = 0;
    // printf("totalBytes是%d\n", totalBytes);
    // while (decodedBytes < totalBytes) {
    //   // printf("进入while循环啦！\n");
    //   for (int i = 0; content[j] != '\0'; j ++) {
    //     // printf("进入for循环啦！\n");
    //     byteData[len++] = content[j];
    //     // printf("content[%d]是：%02x\n", j, content[j]);
    //     // printf("byteData[%d]是：%02x\n", len, byteData[len]);
    //     byteData[len + 1] = '\0'; 
    //     // int startIndex = 0;
    //     i = 0;
    //     if(len >= 200) {
    //         printf("字符达到200啦！\n");
            
    //         // 将十六进制文件内容转换为二进制字符串
    //         hexToBit(byteData, binary, len);
    //         // printf("十六进制是%s\n", byteData);
    //         // printf("二进制是%s\n", binary);
    //         //接下来开始解码
        
    //         // int blockDecodedBytes = 0;
    //         int decodedIndex = 0;
           
    //         for ( i = 0; binary[i] != '\0'; i++) {
             
    //             if (binary[i] == '0') {
    //                 current = current->left;
    //             } else {
    //                 current = current->right;
    //             }
    //             if (current->ch != '\0') {
                  
    //                 decodedText[decodedIndex++] = current->ch;
    //                 if(i == 1) {
    //                   printf("现在进入字符达到200内部的二叉树，找到叶子啦！\n");
    //                   printf("第一个输出的字符是%c\n", current->ch);
    //                 }
    //                 // printf("字符为%c\n",current->ch);
    //                 // blockDecodedBytes++;
    //                 decodedBytes++;
    //                 current = root;
    //                 if (decodedBytes >= totalBytes) {
    //                     break;
    //                 }
    //             }
    //         }  
    
    //         // 处理跨块未完成的编码
    //         if (current != root) {
    //           // 这里可以添加更复杂的处理逻辑，暂时简单提示
    //           printf("警告：存在跨块未完成的编码，可能需要更复杂处理\n");
    //           // strcpy(binary, binary + i);
    //           // memset(byteData, 0, sizeof(byteData));
    //           // HuffmanNode *current = root;
    //           // 可以考虑记录状态，结合下一块继续解码

    //         //   decodedText[decodedIndex] = '\0';
    //         // // printf("此时写入文件，最后二个单词的%c,最后一个单词的%c\n",decodedText[decodedIndex-2],decodedText[decodedIndex-1]);
    //         // // 将解码结果写入文件
    //         // // fputs(decodedText, decodedFile);
    //         // size_t bytesWritten = fwrite(decodedText, sizeof(char), strlen(decodedText), decodedFile);
    //         // printf("写入文件的内容是：%s\n", decodedText);

    //         // memset(decodedText, 0, sizeof(decodedText));
    //         // memset(byteData, 0, sizeof(byteData));
    //         // memset(binary, 0, sizeof(binary));
    //           continue;
    //         }
            
    //         decodedText[decodedIndex] = '\0';
    //         // printf("此时写入文件，最后二个单词的%c,最后一个单词的%c\n",decodedText[decodedIndex-2],decodedText[decodedIndex-1]);
    //         // 将解码结果写入文件
    //         // fputs(decodedText, decodedFile);
    //         size_t bytesWritten = fwrite(decodedText, sizeof(char), strlen(decodedText), decodedFile);
    //         printf("写入文件的内容是：%s\n", decodedText);

            
    //         // memset(decodedText, 0, sizeof(decodedText));
    //         memset(byteData, 0, sizeof(byteData));
    //         memset(binary, 0, sizeof(binary));
    //         len = 0;
    //         // startIndex = i + 1; 
           
        
    //     }
    //   }
    // //   for (int i = 0; i < 20; i++) {
    // //     printf("字符 %c 的十六进制ASCII码值是: 0x%02x\n", byteData[i], (unsigned char)byteData[i]);
    // // }
    // //   printf("十六进制是%llx\n", byteData);

    // // for (int i = 0; i < 20; i++) {
    // //     printf("字符 %c 的十六进制ASCII码值是: 0x%02x\n", byteData[i], (unsigned char)byteData[i]);
    // // }
    //   //检测最后的文本
    //   // printf("strlen(byteData)是%d\n", strlen(byteData));
    //   if(content[j] == '\0') {
    //     hexToBit(byteData, binary, strlen(byteData));
    //     // printf("二进制是%s\n", binary);
    //     int decodedIndex = 0;
    //     // HuffmanNode *current = root;
    //     for (int i = 0; binary[i] != '\0'; i++) {
                  
    //       if (binary[i] == '0') {
    //           current = current->left;
    //       } else {
    //           current = current->right;
    //       }
    //       if (current->ch != '\0') {
    //           decodedText[decodedIndex++] = current->ch;
    //           if(i == 1) {
    //             printf("第一个输出的字符是%c\n", current->ch);
    //           }
    //           decodedBytes++;
    //           printf("decodedBytes是%d\n", decodedBytes);
    //           current = root;
    //           if ( decodedBytes >= totalBytes) {
    //               break;
    //           }
    //       }
    //     }  
    //     decodedText[decodedIndex] = '\0';
    //     // printf("此时写入文件，最后二个单词的%c,最后一个单词的%c\n",decodedText[decodedIndex-2],decodedText[decodedIndex-1]);
    //     // 将解码结果写入文件
    //     // fputs(decodedText, decodedFile);
    //     size_t bytesWritten = fwrite(decodedText, sizeof(char), strlen(decodedText), decodedFile);
    //     // printf("写入文件的内容是：%s\n", decodedText);

    //     // memset(decodedText, 0, sizeof(decodedText));
    //     memset(byteData, 0, sizeof(byteData));
    //     memset(binary, 0, sizeof(binary));
    //     // printf("decodedBytes是：%d\n",decodedBytes);
    //   }

    // }

   
    

    // // 打开输出文件
    

    
   
    

    // fclose(decodedFile);
    // free(content);









    
    
    FILE *file2 = fopen(encodedFilename, "rb");
    if (encodedFilename == NULL) {
      perror("无法打开输出文件");
      return;
    }

    FILE *file3 = fopen(filename2, "wb");
    if (filename2 == NULL) {
      perror("无法打开输出文件");
      return;
    }
    fclose(file3);
    FILE *decodedFile = fopen(filename2, "ab");
    if (decodedFile == NULL) {
        perror("无法打开输出文件");
        return;
    }

    // unsigned char content[501];
    // content = readFile(encodedFilename);
    unsigned char byteData[BUFFER_SIZE] = "";
    unsigned char binary[BUFFER_SIZE * 8 + 1] = "";
    char remainingBinary[BUFFER_SIZE * 9 + 1] = "";  // 存储跨块未完成的二进制数据
    int remainingLen = 0;
    int decodedBytes = 0;
    size_t bytesRead;
    // int decodedBytes = 0;
    
    // printf("马上进入循环啦！\n");

    printf("totalBytes是%d\n", totalBytes);

      while ((bytesRead = fread(byteData, 1, BUFFER_SIZE, file2)) > 0)
      {
        // printf("进入循环了");
        hexToBit(byteData, binary, bytesRead);
        strcat( remainingBinary,binary);
        HuffmanNode *current = root;
        // printf("byteData的内容是%s", byteData);
        // printf("进入while循环啦！\n");

        int decodedIndex = 0;
        int lastValidPos = 0;

        for (int i = 0; remainingBinary[i] != '\0' && decodedBytes < totalBytes; i++) {
          // printf("进入for循环啦！\n");
            if (remainingBinary[i] == '0') {
                current = current->left;
            } else {
                current = current->right;
            }
            if (current->left == NULL && current->right == NULL) {
              // printf("向文件写入字符%c啦！\n",current->ch);
              fputc(current->ch, decodedFile);
                current = root;
                decodedBytes++;
                lastValidPos = i + 1;
                if ( decodedBytes >= totalBytes) {
                  break;
                }
            }
        }  

         // 处理跨块未完成的编码
         remainingLen = strlen(remainingBinary) - lastValidPos;
         strcpy(remainingBinary, remainingBinary + lastValidPos);

        
      }
    // }
       
    // 处理最后可能的未完成编码
      // printf("现在开始处理最后一段二进制\n");
      HuffmanNode *current = root;
      for (int i = 0; remainingBinary[i] != '\0' && decodedBytes < totalBytes; i++)
      {
        if (remainingBinary[i] == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current->left == NULL && current->right == NULL) {
          // printf("向文件写入字符%c啦！\n",current->ch);
            fputc(current->ch, decodedFile);
            current = root;
            decodedBytes++;
            if ( decodedBytes >= totalBytes) {
              break;
            }
        }
    }
    fclose(file);
    fclose(decodedFile);
  
}





// 验证接收人信息的函数
int verifyRecipientInfo(char *codeTablePath, char *encodedFilePath, char *decodedPath, const char *recipientId, const char *recipientName, const char *senderId, const char *senderName) {
  FILE *file = fopen(codeTablePath, "rb");
  if (file == NULL) {
      perror("无法打开编码表文件");
      return 0;
  }

  // 读取解码后文本的总字节数
  int totalBytes;
  fscanf(file, "%d\n", &totalBytes);

  // 构建哈夫曼树的根节点
  HuffmanNode *root = decodenewNode();

  // 读取编码表
  char line[100];
  int k = 0;
  while (fgets(line, sizeof(line), file) != NULL) {
      deleteSpace(line);
      // printf("读取到的行: %s\n", line);
      char ch[3];
      int bitCount;
      char hexCode[30];
      ch[0] = line[2];
      ch[1] = line[3];
      ch[2] = '\0';
      bitCount = hexCharToInt(line[6]) * 16 + hexCharToInt(line[7]);
      int count, i;
      for (count = 0, i = 0; count < ((bitCount - 1) / 8 + 1); count++) {
          hexCode[i] = line[i + 10 + 2 * count];
          i++;
          hexCode[i] = line[i + 10 + 2 * count];
          i++;
      }
      hexCode[i] = '\0';
      char binaryCode[100];
      hexToBinary(hexCode, binaryCode, bitCount);
      // printf("字符: 0x%s, 编码: %s\n", ch, binaryCode);
      decodebuildHuffmanTree(root, binaryCode, (char)strtol(ch, NULL, 16));
  }
  fclose(file);

  printf("成功建立第二个编码表\n");

  FILE *encodedFile = fopen(encodedFilePath, "rb");
  if (encodedFile == NULL) {
      perror("无法打开编码文件");
      return 0;
  }

  unsigned char byteData[BUFFER_SIZE];
  char binary[BUFFER_SIZE * 8 + 1];
  char remainingBinary[BUFFER_SIZE * 9 + 1] = "";
  int remainingLen = 0;
  int decodedBytes = 0;
  size_t bytesRead;
  char decodedInfo[256] = "";
  printf("准备进入while循环\n");
  while ((bytesRead = fread(byteData, 1, BUFFER_SIZE, encodedFile)) > 0 && decodedBytes < 50) {
      hexToBit(byteData, binary, bytesRead);
      // printf("十六进制数据: ");
      // for (int i = 0; i < bytesRead; i++) {
      //     printf("%02x ", byteData[i]);
      // }
      // printf("\n二进制数据: %s\n", binary);
      strcat(remainingBinary, binary);
      // printf("111111\n");
      HuffmanNode *current = root;
      for (int i = 0; remainingBinary[i] != '\0' && decodedBytes < 2560; i++) {
          // printf("进入for循环\n");
          if (remainingBinary[i] == '0') {
              current = current->left;
              // printf("2222\n");
          } else {
              current = current->right;
              // printf("3333\n");
          }
          // printf("444\n");
          if (current->left == NULL && current->right == NULL) {
              // printf("555\n");
              // printf("decodedInfo的第%d位是%c\n", decodedBytes, current->ch);
              decodedInfo[decodedBytes++] = current->ch;
              current = root;
              if (decodedBytes >= 50 ) {
              break;
              }
          }
          // printf("66666\n");
      }
      // printf("出for循环\n");
      // remainingLen = strlen(remainingBinary);
      // for (int i = 0; i < remainingLen; i++) {
      //     if (remainingBinary[i] != '\0') {
      //        remainingBinary[i] = remainingBinary[i + decodedBytes];
      //     }
      // }
      // remainingBinary[remainingLen - decodedBytes] = '\0';

      // // 处理跨块未完成的编码
      // remainingLen = strlen(remainingBinary) - lastValidPos;
      // strcpy(remainingBinary, remainingBinary + lastValidPos);
  }
  fclose(encodedFile);
  decodedInfo[decodedBytes] = '\0';
  // printf("出while循环\n");
  // printf("decodedInfo是%s\n", decodedInfo);
  // 解析前两行信息
  char *firstLine = strtok(decodedInfo, "\n");
  char *secondLine = strtok(NULL, "\n");

  printf("解析出来第一行是%s", firstLine);
  printf("解析出来第二行是%s", secondLine);

  if (firstLine == NULL || secondLine == NULL) {
      printf("解码的前两行信息不完整\n");
      return 0;
  }

  char id1[20], name1[10], id2[20], name2[10];
  sscanf(firstLine, "U%9s，%6s", id1, name1);
  sscanf(secondLine, "U%9s，%6s", id2, name2);

  printf("解析出来第一行id是%s,姓名是%s\n", id1,name1);
  printf("解析出来第一行id是%s,姓名是%s\n", id2,name2);
  if ((strcmp(id1, senderId) != 0 || strcmp(name1, senderName) != 0) ||
  (strcmp(id2, recipientId) != 0 || strcmp(name2, recipientName) != 0)) {
      printf("收件人或发件人信息不匹配\n");
      return 0;
  }

  // 调用 decode 函数解码全部
  decode(codeTablePath, encodedFilePath, decodedPath);

  // 删去前两行
  FILE *decodedFile = fopen(decodedPath, "r");
  if (decodedFile == NULL) {
      perror("无法打开解码后的文件");
      return 0;
  }
  FILE *tempFile = fopen("temp.txt", "w");
  if (tempFile == NULL) {
      perror("无法创建临时文件");
      fclose(decodedFile);
      return 0;
  }
  int lineCount = 0;
  char c;
  while ((c = fgetc(decodedFile)) != EOF) {
      if (lineCount >= 2) {
       fputc(c, tempFile);
      }
      if (c == '\n') {
        lineCount++;
      }
  }
  fclose(decodedFile);
  fclose(tempFile);

  remove(decodedPath);
  rename("temp.txt", decodedPath);

  return 1;
}



int main()
{
  clock_t start, end;
    double cpu_time_used;

    // 记录开始时间
    start = clock();

  char *content;
  HuffmanNode *root;

  //测试一
  char filePath1[] = ".\\test1\\The_Wretched.txt";
  char filePath2[] = ".\\result1\\The_Wretched.hfm";
  char filePath3[] = ".\\result1\\code.txt";

  char filename4[] = ".\\result1\\code.txt";
  char filename5[] = ".\\result1\\The_Wretched_j.txt";
  char filename6[] = ".\\result1\\The_Wretched.hfm";

  // // 测试二
  // char filePath1[] = ".\\test2\\yuanxi.txt";
  // char filePath2[] = ".\\result2\\yuanxi.hfm";
  // char filePath3[] = ".\\result2\\code.txt";

  // char filename4[] = ".\\result2\\code.txt";
  // char filename5[] = ".\\result2\\yuanxi_j.txt";
  // char filename6[] = ".\\result2\\yuanxi.hfm";

  // //测试三
  // char filePath1[] = ".\\test3\\middle.txt";
  // char filePath2[] = ".\\result3\\middle.hfm";
  // char filePath3[] = ".\\result3\\code.txt";

  // char filename4[] = ".\\result3\\code.txt";
  // char filename5[] = ".\\result3\\middle_j.txt";
  // char filename6[] = ".\\result3\\middle.hfm";

  // //测试四
  // char filePath1[] = ".\\test4\\test.txt";
  // char filePath2[] = ".\\result4\\test.hfm";
  // char filePath3[] = ".\\result4\\code.txt";

  // char filename4[] = ".\\result4\\code.txt";
  // char filename5[] = ".\\result4\\test_j.txt";
  // char filename6[] = ".\\result4\\test.hfm";


  // //样本文本
  // char filePath1[] = ".\\test0\\test.txt";
  // char filePath2[] = ".\\result0\\test.hfm";
  // char filePath3[] = ".\\result0\\code.txt";

  // char filename4[] = ".\\result0\\code.txt";
  // char filename5[] = ".\\result0\\test_j.txt";
  // char filename6[] = ".\\result0\\test.hfm";

  // // //测试接收人
  // char filePath1[] = "yuanxi.txt";
  // char filePath2[] = "yuanxi.hfm";
  // char filePath3[] = "code.txt";

  // char filename4[] = "code.txt";
  // char filename5[] = "yuanxi_j.txt";
  // char filename6[] = "yuanxi.hfm";

  content = readFileAndWriteSize(filePath1,filePath3);
  if (content != NULL)
  {
    // printf("文件内容如下：\n%s\n", content);
  }


  content = readFileAndWriteSize(filePath1, filePath3);
  // printf("文件内容如下：\n%s\n", content);
  // printf("文件长度为：\n%d\n", strlen(content));

  // printf("文件内容如下：\n%s\n", content);
  printf("文件长度为：\n%d\n", strlen(content));
  root = sortSingleByteCharsByFrequency(filePath1, content, filePath2, filePath3);
  // bitEncodeAndHash(content);
  // uint64_t hash_value = fnv1a_64(content, strlen(content) ); 
  // printf("原字符串 \"%s\" 的哈希值为: 0x%016llx\n", content, hash_value);
  // printf("原字符串的哈希值为: 0x%016llx\n",  hash_value);

  // 释放动态分配的内存

   // 凯撒解密
   for (int i = 0; content[i] != '\0'; i++) {
    content[i] = (unsigned char)(content[i] - 0x55) % 256;
}

  freeHuffmanTree(root);
  free(content);   

  printf("\n\n\n-------------------接下来是解码相关的数据--------------------\n\n\n");


  decode(filename4, filename6, filename5);
  // 记录结束时间
  end = clock();

  // 计算使用的 CPU 时间（秒）
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

  printf("程序运行时间: %f 秒\n", cpu_time_used);

  uint64_t hash1 = calculateFileHash(filePath1);
  printf("文件 %s 的 FNV-1a 64 位哈希值为: 0x%016llx\n", filePath1, hash1);
  uint64_t hash2 = calculateFileHash(filePath2);
  printf("文件 %s 的 FNV-1a 64 位哈希值为: 0x%016llx\n", filePath2, hash2);
  uint64_t hash3 = calculateFileHash(filename5);
  printf("文件 %s 的 FNV-1a 64 位哈希值为: 0x%016llx\n", filename5, hash3);
  printLastSixteenBytes(filePath2);
  printFileSizePercentage(filePath2, filePath1);
  long size1 = getFileSize(filePath1);
  printf("文件 %s 的 文件大小为: %d\n", filePath1, size1);
  long size2 = getFileSize(filename6);
  printf("文件 %s 的 文件大小为: %d\n", filename6, size2);
  long size3 = getFileSize(filename5);
  printf("文件 %s 的 文件大小为: %d\n", filename5, size3);
  return 0;
}






// int main() {
//   // clearInputBuffer();
   
// while(1){
//   int filePathChoice;
//   printf("请输入需要测试的数据文本集序号：");
//   scanf("%d", &filePathChoice);
//   while (getchar() != '\n');
// //   if (scanf("%d", &filePathChoice) != 1) {
// //     // 清除输入缓冲区中的无效字符
// //     clearInputBuffer();
// //     printf("输入无效，请输入一个整数。\n");
// //     continue;
// // }

//   if (filePathChoice == 1) {
//     //测试一
//     char filePath1[] = ".\\test1\\The_Wretched.txt";
//     char filePath2[] = ".\\result1\\The_Wretched.hfm";
//     char filePath3[] = ".\\result1\\code.txt";

//     char filename4[] = ".\\result1\\code.txt";
//     char filename5[] = ".\\result1\\The_Wretched_j.txt";
//     char filename6[] = ".\\result1\\The_Wretched.hfm";

//     char recipientId[10];
//     char recipientName[7];
//     char senderId[10];
//     char senderName[7];

//     printf("请设置发送人的学号：");
//     scanf("%9s", senderId);
//     while (getchar() != '\n');
//     printf("请设置发送人的姓名：");
//     scanf("%s", senderName);
//     while (getchar() != '\n');
//     printf("请设置接收人人的学号：");
//     scanf("%9s", recipientId);
//     while (getchar() != '\n');
//     printf("请设置接收人人的姓名：");
//     scanf("%s", recipientName);
//     while (getchar() != '\n');
//     if(senderId != NULL && senderName != NULL && recipientId != NULL && recipientName != NULL) {
//       insertInfoToFile(filePath1, recipientId, recipientName, senderId, senderName);
//     }

//     printf("是否需要对原文本进行偏移量为0x55的凯撒加密（y/n）？：");
//     char answer;
//     scanf("%c", &answer);
//     // scanf("%c", answer);

//     if ( answer == 'y') {
//       addOffsetToFile(filePath1, 1);
//     }


//     char *content;
//     HuffmanNode *root;
//     content = readFileAndWriteSize(filePath1,filePath3);
//     clock_t start, end;
//     double cpu_time_used;
//     // 记录开始时间1
//     start = clock();
//     root = sortSingleByteCharsByFrequency(filePath1, content, filePath2, filePath3);
//     //记录结束时间
//     end = clock();
//     // 计算使用的 CPU 时间（秒）
//     cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//     if ( answer == 'y') {
//       addOffsetToFile(filePath1, 0);
//     }
//     freeHuffmanTree(root);
//     free(content);
//     printLastSixteenBytes(filePath2);
//     printf("加密花费的时间为: %f 秒\n", cpu_time_used);
//     uint64_t hash1 = calculateFileHash(filePath1);
//     printf("文件 %s 的 FNV-1a 64 位哈希值为: 0x%016llx\n", filePath1, hash1);
//     uint64_t hash2 = calculateFileHash(filePath2);
//     printf("文件 %s 的 FNV-1a 64 位哈希值为: 0x%016llx\n", filePath2, hash2);
//     printFileSizePercentage(filePath2, filePath1);

//     printf("\n\n\n-------------------接下来是解码相关的数据--------------------\n\n\n");


//     printf("请设置发送人的学号：");
//     scanf("%9s", senderId);
//     while (getchar() != '\n');
//     printf("请设置发送人的姓名：");
//     scanf("%s", senderName);
//     while (getchar() != '\n');
//     printf("请设置接收人人的学号：");
//     scanf("%9s", recipientId);
//     while (getchar() != '\n');
//     printf("请设置接收人人的姓名：");
//     scanf("%s", recipientName);
//     while (getchar() != '\n');

//     clock_t start2, end2;
//     double cpu_time_used2;
//     // 记录开始时间1
//     start2 = clock();
//     verifyRecipientInfo(filePath3, filePath2, filename5, recipientId, recipientName, senderId, senderName);
//     end2 = clock();
//     // 计算使用的 CPU 时间（秒）
//     cpu_time_used2 = ((double) (end - start)) / CLOCKS_PER_SEC;
//     if ( answer == 'y') {
//       addOffsetToFile(filename5, 1);
//     }

//     printf("解密花费的时间为: %f 秒\n", cpu_time_used2);
//     break;
//   }
//   else if (filePathChoice == 2) {

//      // 测试二
//     char filePath1[] = ".\\test2\\yuanxi.txt";
//     char filePath2[] = ".\\result2\\yuanxi.hfm";
//     char filePath3[] = ".\\result2\\code.txt";

//     char filename4[] = ".\\result2\\code.txt";
//     char filename5[] = ".\\result2\\yuanxi_j.txt";
//     char filename6[] = ".\\result2\\yuanxi.hfm";

//     char recipientId[10];
//     char recipientName[7];
//     char senderId[10];
//     char senderName[7];

//     printf("请设置发送人的学号：");
//     scanf("%9s", senderId);
//     while (getchar() != '\n');
//     printf("请设置发送人的姓名：");
//     scanf("%s", senderName);
//     while (getchar() != '\n');
//     printf("请设置接收人人的学号：");
//     scanf("%9s", recipientId);
//     while (getchar() != '\n');
//     printf("请设置接收人人的姓名：");
//     scanf("%s", recipientName);
//     while (getchar() != '\n');
//     if(senderId != NULL && senderName != NULL && recipientId != NULL && recipientName != NULL) {
//       insertInfoToFile(filePath1, recipientId, recipientName, senderId, senderName);
//     }

//     printf("是否需要对原文本进行偏移量为0x55的凯撒加密（y/n）？：");
//     char answer;
//     scanf("%c", &answer);
//     // scanf("%c", answer);

//     if ( answer == 'y') {
//       addOffsetToFile(filePath1, 1);
//     }


//     char *content;
//     HuffmanNode *root;
//     content = readFileAndWriteSize(filePath1,filePath3);
//     clock_t start, end;
//     double cpu_time_used;
//     // 记录开始时间1
//     start = clock();
//     root = sortSingleByteCharsByFrequency(filePath1, content, filePath2, filePath3);
//     //记录结束时间
//     end = clock();
//     // 计算使用的 CPU 时间（秒）
//     cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//     if ( answer == 'y') {
//       addOffsetToFile(filePath1, 0);
//     }
//     freeHuffmanTree(root);
//     free(content);
//     printLastSixteenBytes(filePath2);
//     printf("加密花费的时间为: %f 秒\n", cpu_time_used);
//     uint64_t hash1 = calculateFileHash(filePath1);
//     printf("文件 %s 的 FNV-1a 64 位哈希值为: 0x%016llx\n", filePath1, hash1);
//     uint64_t hash2 = calculateFileHash(filePath2);
//     printf("文件 %s 的 FNV-1a 64 位哈希值为: 0x%016llx\n", filePath2, hash2);
//     printFileSizePercentage(filePath2, filePath1);

//     printf("\n\n\n-------------------接下来是解码相关的数据--------------------\n\n\n");


//     printf("请设置发送人的学号：");
//     scanf("%9s", senderId);
//     while (getchar() != '\n');
//     printf("请设置发送人的姓名：");
//     scanf("%s", senderName);
//     while (getchar() != '\n');
//     printf("请设置接收人人的学号：");
//     scanf("%9s", recipientId);
//     while (getchar() != '\n');
//     printf("请设置接收人人的姓名：");
//     scanf("%s", recipientName);
//     while (getchar() != '\n');

//     clock_t start2, end2;
//     double cpu_time_used2;
//     // 记录开始时间1
//     start2 = clock();
//     verifyRecipientInfo(filePath3, filePath2, filename5, recipientId, recipientName, senderId, senderName);
//     end2 = clock();
//     // 计算使用的 CPU 时间（秒）
//     cpu_time_used2 = ((double) (end - start)) / CLOCKS_PER_SEC;
//     if ( answer == 'y') {
//       addOffsetToFile(filename5, 1);
//     }

//     printf("解密花费的时间为: %f 秒\n", cpu_time_used2);
//     break;
//   }


// }


// }