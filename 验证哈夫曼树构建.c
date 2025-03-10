// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// // 定义哈夫曼树节点结构体
// typedef struct HuffmanNode {
//     char ch;
//     int freq;
//     struct HuffmanNode *left, *right;
// } HuffmanNode;

// // 根据哈夫曼编码遍历哈夫曼树找到对应字符
// char findCharByHuffmanCode(HuffmanNode *root, const char *huffmanCode) {
//     HuffmanNode *current = root;
//     for (int i = 0; huffmanCode[i] != '\0'; i++) {
//         if (huffmanCode[i] == '0') {
//             if (current->left == NULL) {
//                 printf("哈夫曼编码与哈夫曼树不匹配，无法继续遍历\n");
//                 return '\0';
//             }
//             current = current->left;
//         } else if (huffmanCode[i] == '1') {
//             if (current->right == NULL) {
//                 printf("哈夫曼编码与哈夫曼树不匹配，无法继续遍历\n");
//                 return '\0';
//             }
//             current = current->right;
//         } else {
//             printf("无效的哈夫曼编码字符: %c\n", huffmanCode[i]);
//             return '\0';
//         }
//     }
//     return current->ch;
// }

// // 示例代码，用于测试
// int main() {
//     // 这里简单构建一个示例哈夫曼树，实际应用中需要根据字符频次构建
//     HuffmanNode *root = (HuffmanNode *)malloc(sizeof(HuffmanNode));
//     root->ch = '\0';
//     root->freq = 0;
//     root->left = (HuffmanNode *)malloc(sizeof(HuffmanNode));
//     root->left->ch = 'A';
//     root->left->freq = 10;
//     root->left->left = NULL;
//     root->left->right = NULL;
//     root->right = (HuffmanNode *)malloc(sizeof(HuffmanNode));
//     root->right->ch = '\0';
//     root->right->freq = 0;
//     root->right->left = (HuffmanNode *)malloc(sizeof(HuffmanNode));
//     root->right->left->ch = 'B';
//     root->right->left->freq = 15;
//     root->right->left->left = NULL;
//     root->right->left->right = NULL;
//     root->right->right = (HuffmanNode *)malloc(sizeof(HuffmanNode));
//     root->right->right->ch = (char)0xc4;
//     root->right->right->freq = 20;
//     root->right->right->left = NULL;
//     root->right->right->right = NULL;

//     const char *huffmanCode = "101110110001010111111";
//     char foundChar = findCharByHuffmanCode(root, huffmanCode);
//     if (foundChar != '\0') {
//         printf("找到字符: 0x%02x\n", (unsigned char)foundChar);
//     }

//     // 释放哈夫曼树的内存
//     free(root->left);
//     free(root->right->left);
//     free(root->right->right);
//     free(root->right);
//     free(root);

//     return 0;
// }



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#define MAX_SIZE 1024



// FNV-1a 64位哈希算法的初始值
#define FNV1A_64_INIT 0xcbf29ce484222325ULL
// FNV-1a 64位哈希算法的质数
#define FNV1A_64_PRIME 0x100000001b3

 
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

// 根据哈夫曼编码遍历哈夫曼树找到对应字符
char findCharByHuffmanCode(HuffmanNode *root, const char *huffmanCode) {
    HuffmanNode *current = root;
    for (int i = 0; huffmanCode[i] != '\0'; i++) {
        if (huffmanCode[i] == '0') {
            if (current->left == NULL) {
                printf("哈夫曼编码与哈夫曼树不匹配，无法继续遍历\n");
                return '\0';
            }
            current = current->left;
        } else if (huffmanCode[i] == '1') {
            if (current->right == NULL) {
                printf("哈夫曼编码与哈夫曼树不匹配，无法继续遍历\n");
                return '\0';
            }
            current = current->right;
        } else {
            printf("无效的哈夫曼编码字符: %c\n", huffmanCode[i]);
            return '\0';
        }
    }
    return current->ch;
}

//读取文件信息
char *readFile(char *filePath,char *filePath3)
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
  FILE *file3 = fopen(filePath3, "w");
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
   FILE *file3 = fopen(filename3, "a");
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


// 对文本进行哈夫曼编码，补零后，输出压缩后的二进制、十六进制、哈希值
void encodeText(const char *filename, char huffmanCodes[256][100], char *content, const char *filename2)
{

  char binary[1000000] = "";  // 假设编码后的二进制字符串长度不超过 100000
  printf("压缩后的二进制文本为: ");
  for (int i = 0; content[i] != '\0'; i++)
  {
    strcat(binary, huffmanCodes[(unsigned char)content[i]]);
    // printf("%s", huffmanCodes[(unsigned char)content[i]]);
  }
  printf("\n");

 // 计算需要补零的位数
 int binaryLength = strlen(binary);
 int padding = 8 - (binaryLength % 8);
 if (padding < 8) {
     for (int i = 0; i < padding; i++) {
         strcat(binary, "0");
     }
 }

 
 
  // 将二进制字符串转换为十六进制字符串
  unsigned char byteData[125000];  // 二进制转十六进制长度除以四
  binaryToByteData(binary, byteData);
  printf("补零后的二进制文本为:  %s\n",binary);
  int len = strlen(binary) / 8;
  // printf("压缩后的bit位编码为: \n");
  // for (int i = 0,  j = 0; i < len; i++,j++) {
  //   if(j == 16) {
  //     printf("\n");
  //     j = 0;
  //   }
  //     printf("%02x", byteData[i]);
  // }
  printf("\n");
  printf("压缩后的bit位编码第一位为: %x\n", byteData[0]);

  //把编码写入文本文件

  writeFile(filename2, byteData, sizeof(char), len);

  // 计算 FNV-1a 64位哈希值
  uint64_t hash1 = fnv1a_64(binary, strlen(binary));
  uint64_t hash2 = fnv1a_64(byteData, strlen(byteData));
  printf("压缩后文本二进制的HASH1值为: 0x%016llx\n", hash1);
  printf("压缩后文本bit位编码的HASH2值为: 0x%016llx\n", hash2);
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
  for (int i = 0; content[i] != '\0'; i++)
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

  // // 计算 WPL
  // printf("接下来开始计算WPL了\n");
  // int wpl = calculateWPL(root, 0);
  // printf("哈夫曼树的带权路径长度 (WPL) 为: %d\n", wpl);

  // // 对文本进行哈夫曼编码
  // encodeText(filename, huffmanCodes, content, filename2);
  // return root;
}

//释放哈夫曼树内存的函数
void freeHuffmanTree(HuffmanNode *root) {
  if (root == NULL) return;
  freeHuffmanTree(root->left);
  freeHuffmanTree(root->right);
  free(root);
}




int main()
{
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

  content = readFile(filePath1,filePath3);
  if (content != NULL)
  {
    // printf("文件内容如下：\n%s\n", content);
  }

  root = sortSingleByteCharsByFrequency(filePath1, content, filePath2, filePath3);
    // const char *huffmanCode = "101110110001010111111";
    // const char *huffmanCode = "110000010010010010111";
    const char *huffmanCode = "1";
    //四五位
//0xe6是011，我输出的是0011，正确应该0011✅
//0xe4是110，我输出的是0110，正确应该0110✅
//0xe9是0001，我输出的是00001，正确应该00001✅
//0xba是0000，我输出的是00000，正确应该00000✅

//六七位
//0xaf我输出的是011110，正确应该是011110
//0x9c我输出的是100100，正确应该是100100

//八位
//0x93我输出的是10000111，正确应该是10000111
//0x9E我输出的是00101011，正确应该是00101011
//0x93我输出的是10000111，正确应该是10000111
//0x93我输出的是10000111，正确应该是10000111
//0x93我输出的是10000111，正确应该是10000111
//0x93我输出的是10000111，正确应该是10000111
//0x93我输出的是10000111，正确应该是10000111

//总结我建出来的树和实际建出来的树，开头差了一个0
    char foundChar = findCharByHuffmanCode(root, huffmanCode);
    if (foundChar != '\0') {
        printf("找到字符: 0x%02x\n", (unsigned char)foundChar);
    }

  // 释放动态分配的内存
  freeHuffmanTree(root);
  free(content);   

  return 0;
}