#include <stdio.h>
#include <stdlib.h>

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
            buffer[i] = (buffer[i] + 0x55) % 256;
        }
    }else {
        for (long i = 0; i < fileSize; i++) {
            buffer[i] = (buffer[i] - 0x55) % 256;
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



int main() {
  char filePath[] = ".\\test4\\test.txt";
  // char filePath[] = ".\\result4\\test_j.txt";
//   char filePath[] = ".\\test2\\yuanxi.txt";
  // char filePath[] = ".\\result2\\yuanxi_j.txt";
//   char filePath[] = ".\\test1\\The_Wretched.txt";
//   char filePath[] = ".\\result1\\The_Wretched_j.txt";
  if (addOffsetToFile(filePath, 1) == 0) {
      printf("文件修改成功\n");
  } else {
      printf("文件修改失败\n");
  }
  return 0;
}