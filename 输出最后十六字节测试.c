#include <stdio.h>
#include <stdlib.h>



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
      for (size_t i = 0; i < bytesRead; i++) {
          if (i > 0) {
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

int main() {
  const char *filename = ".\\result1\\The_Wretched.hfm";
  printLastSixteenBytes(filename);
  return 0;
}