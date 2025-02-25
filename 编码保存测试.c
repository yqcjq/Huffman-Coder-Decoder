#include <stdio.h>
#include <string.h>
#include <math.h>

// 封装的函数，完成二进制字符串处理
void processBinary(const char *input, int maxLength) {
    char binaryStr[100];
    strncpy(binaryStr, input, maxLength);
    binaryStr[maxLength] = '\0';

    char processedBinary[100];
    strcpy(processedBinary, binaryStr);

    int len = strlen(processedBinary);
    int padding = 8 - (len % 8);
    if (padding < 8) {
        for (int i = 0; i < padding; i++) {
            processedBinary[len + i] = '0';
        }
        processedBinary[len + padding] = '\0';
    }

    printf("补齐后的二进制字符串: %s\n", processedBinary);
    printf("按八位一组转换为十进制数的结果:\n");

    len = strlen(processedBinary);
    for (int i = 0; i < len; i += 8) {
        char group[9];
        strncpy(group, processedBinary + i, 8);
        group[8] = '\0';
        int decimal = 0;
        for (int j = 0; j < 8; j++) {
            if (group[j] == '1') {
                decimal += (int)pow(2, 7 - j);
            }
        }
        printf("第 %d 组: 二进制 %s 转换为十进制数是 %d，十六进制是 0x%02x\n", (i / 8) + 1, group, decimal, decimal);
    }
}

int main() {
    const char *testBinary = "010001110";
    processBinary(testBinary, strlen(testBinary));
    return 0;
}