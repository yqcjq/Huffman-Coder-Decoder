#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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


//读取文件信息
char *readFile(char *filePath)
{
  FILE *file;
  char *fileContent;
  int fileSize;
  // 打开文件，以只读模式打开
  file = fopen(filePath, "r");
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

int main() {
  // char a[] = "“没有网络安全就没有国家安全，……”";
  char *a;
 
  char filePath[] = "test.txt";
  // char filePath[] = "yuanxi.txt";
  a = readFile(filePath);
  if (a != NULL)
  {
    printf("文件内容如下：\n%s\n", a);
    // 释放动态分配的内存
    
  }
  uint64_t hash_value = fnv1a_64(a, strlen(a) ); // 计算字符串的哈希值，不包括结尾的'\0'
  printf("文件内容如下：\n%s\n", a);
  printf("字符串 \"%s\" 的FNV-1a 64位哈希值为: 0x%016llx\n", a, hash_value);
}