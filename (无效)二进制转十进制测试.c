#include <stdio.h>
#include <string.h>
#include <math.h>

// 二进制字符串转十进制数的函数，返回转换后的十进制数
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

int main() {
    const char *binaryStr = "01001";
    int result = binaryToDecimal(binaryStr);
    printf("二进制字符串 %s 转换为十进制数是 %d，十六进制是%x\n", binaryStr, result, result);
    return 0;
}