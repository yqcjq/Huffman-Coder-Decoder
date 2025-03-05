#include <stdio.h>
#include <stdlib.h>

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
int main() {
    const char *file1Path = ".\\result1\\The_Wretched.hfm";  // 请替换为实际的文件路径
    const char *file2Path = ".\\test1\\The_Wretched.txt";  // 请替换为实际的文件路径
    printFileSizePercentage(file1Path, file2Path);
    return 0;
}