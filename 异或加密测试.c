#include <stdio.h>
#include <stdlib.h>

// 异或加密/解密函数
void xorEncryptDecrypt(char *data, int length, char key) {
    for (int i = 0; i < length; i++) {
        data[i] = data[i] ^ key;
    }
}

// 通用的加密/解密函数
int processFile(const char *inputFilePath, const char *outputFilePath, char key) {
    FILE *inputFile, *outputFile;
    char buffer[1024];
    int bytesRead;

    // 打开输入文件进行读取
    inputFile = fopen(inputFilePath, "rb");
    if (inputFile == NULL) {
        perror("无法打开输入文件");
        return 1;
    }

    // 打开输出文件进行写入
    outputFile = fopen(outputFilePath, "wb");
    if (outputFile == NULL) {
        perror("无法打开输出文件");
        fclose(inputFile);
        return 1;
    }

    // 逐块读取输入文件，进行异或操作并写入输出文件
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0) {
        xorEncryptDecrypt(buffer, bytesRead, key);
        fwrite(buffer, 1, bytesRead, outputFile);
    }

    // 关闭文件
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

int main() {
    char key = 'A';  // 加密密钥
    const char *inputFilePath = "The_Wretched.txt";
    const char *encryptedFilePath = "txt_123.txt";
    const char *decryptedFilePath = "txt_456.txt";

    // 加密文件
    if (processFile(inputFilePath, encryptedFilePath, key) == 0) {
        printf("文件加密成功。\n");
    } else {
        printf("文件加密失败。\n");
        return 1;
    }

    // 解密文件
    if (processFile(encryptedFilePath, decryptedFilePath, key) == 0) {
        printf("文件解密成功。\n");
    } else {
        printf("文件解密失败。\n");
        return 1;
    }

    return 0;
}