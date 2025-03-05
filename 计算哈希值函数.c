#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
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

int main() {
  const char *filename = ".\\test1\\The_Wretched.txt";  // 请替换为实际的文件名
  uint64_t hash = calculateFileHash(filename);
  printf("文件 %s 的 FNV-1a 64 位哈希值为: 0x%016llx\n", filename, hash);
  return 0;
}
