
// #include <stdio.h>
// #include <string.h>

// // 将二进制字符串转换为字符编码，每 8 位二进制存为一个字符
// void binaryToHex(const char *binary, char *result) {
//     int len = strlen(binary);
//     int resultIndex = 0;

//     // 按每 8 位一组处理二进制字符串
//     for (int i = 0; i < len; i += 8) {
//         int value = 0;
//         // 处理当前 8 位二进制数
//         for (int j = 0; j < 8; j++) {
//             // 将二进制字符转换为整数并累加到 value 中
//             value = (value << 1) | (binary[i + j] - '0');
//         }
//         // 将 8 位二进制对应的整数值存储为一个字符
//         result[resultIndex++] = (char)value;
//     }
//     // 在结果字符串末尾添加字符串结束符
//     result[resultIndex] = '\0';
// }

// int main() {
//     const char binary[] = "1010101011001100";
//     char result[100];

//     binaryToHex(binary, result);
//     printf("Binary: %s\n", binary);

//     // 输出存储结果（可能包含不可见字符，这里以十六进制形式输出每个字符）
//     for (int i = 0; i < strlen(result); i++) {
//         printf("Hex: %02x\n", (unsigned char)result[i]);
//     }

//     return 0;
// }