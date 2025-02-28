#include <stdio.h>
#include <stdlib.h>

// 函数用于读取文本文件的字节数大小
long getFileSize(const char *filename) {
    FILE *file = fopen(filename, "rb");  // 以二进制只读模式打开文件
    if (file == NULL) {
        perror("无法打开文件");
        return -1;  // 返回 -1 表示打开文件失败
    }

    // 将文件指针移动到文件末尾
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("无法定位到文件末尾");
        fclose(file);
        return -1;  // 返回 -1 表示定位文件末尾失败
    }

    // 获取文件当前位置，即文件大小
    long fileSize = ftell(file);
    if (fileSize == -1) {
        perror("无法获取文件大小");
        fclose(file);
        return -1;  // 返回 -1 表示获取文件大小失败
    }

    // 将文件指针移回文件开头
    if (fseek(file, 0, SEEK_SET) != 0) {
        perror("无法定位到文件开头");
        fclose(file);
        return -1;  // 返回 -1 表示定位文件开头失败
    }

    fclose(file);  // 关闭文件
    return fileSize;  // 返回文件大小
}

int main() {
  const char *filename = ".\\result1\\The_Wretched.hfm";  // 请将此处替换为实际的文件名
  // const char *filename1 = ".\\result1\\The_Wretched_j.txt";
  const char *filename1 = ".\\test1\\The_Wretched.txt";
  long size1 = getFileSize(filename);
  long size2 = getFileSize(filename1);
  if (size1 != -1) {
      printf("文件 %s 的字节数大小为: %ld\n", filename, size1);
  }
  if (size2 != -1) {
    printf("文件 %s 的字节数大小为: %ld\n", filename1, size2);
}
  return 0;
}
