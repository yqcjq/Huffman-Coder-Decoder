#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// 哈夫曼树节点结构体
typedef struct HuffmanNode {
    char ch;
    int freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;


void deleteSpace(char *str)
{
	char *str_c=str;
	int i,j=0;
	for(i=0;str[i]!='\0';i++)
	{
		if(str[i]!=' ')
			str_c[j++]=str[i];
	}
	str_c[j]='\0';
	str=str_c;	
}

// 创建新的哈夫曼树节点
HuffmanNode* decodenewNode() {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    if (node == NULL) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }
    node->ch = '\0';
    node->left = node->right = NULL;
    return node;
}

// 从十六进制字符转换为对应的整数值
int hexCharToInt(char hexChar) {
    if (hexChar >= '0' && hexChar <= '9') {
        return hexChar - '0';
    } else if (hexChar >= 'A' && hexChar <= 'F') {
        return hexChar - 'A' + 10;
    } else if (hexChar >= 'a' && hexChar <= 'f') {
        return hexChar - 'a' + 10;
    }
    return -1;
}

// 将十六进制字符串转换为二进制字符串
void hexToBinary(const char *hex, char *binary, int bitCount) {
    int hexIndex = 0;
    int binIndex = 0;
    while (bitCount > 0) {
        int hexVal = hexCharToInt(hex[hexIndex]);
        for (int i = 3; i >= 0 && bitCount > 0; i--) {
            binary[binIndex++] = (hexVal & (1 << i))? '1' : '0';
            bitCount--;
        }
        hexIndex++;
    }
    binary[binIndex] = '\0';
}

// 将十六进制文件内容转换为二进制字符串
void hexFileToBinary(const char *filename, char *binary) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("无法打开文件");
        return;
    }

    int binIndex = 0;
    unsigned char byte;
    while (fread(&byte, 1, 1, file) == 1) {
        for (int i = 7; i >= 0; i--) {
            binary[binIndex++] = (byte & (1 << i))? '1' : '0';
        }
    }
    binary[binIndex] = '\0';
    fclose(file);
}

// 构建哈夫曼树
void decodebuildHuffmanTree(HuffmanNode *root, char *code, char ch) {
    HuffmanNode *current = root;
    for (int i = 0; code[i] != '\0'; i++) {
        if (code[i] == '0') {
            if (current->left == NULL) {
                current->left = decodenewNode();
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = decodenewNode();
            }
            current = current->right;
        }
    }
    current->ch = ch;
}

// 解码函数
void decode(const char *filename, const char *encodedFilename,  const char *filename2) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("无法打开文件1");
        return;
    }

    // 读取解码后文本的总字节数
    int totalBytes;
    fscanf(file, "%d\n", &totalBytes);
    printf("读到的字节数为: %d\n", totalBytes);
    // 构建哈夫曼树的根节点
    HuffmanNode *root = decodenewNode();
    printf("成功建表\n");
    // 读取编码表
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        char ch[3];
        int bitCount;
        char hexCode[30];
        // sscanf(line, "0x%2s 0x%x %s", ch, &bitCount, hexCode);
        deleteSpace(line);
        ch[0] = line[2];
        ch[1] = line[3];
        ch[2] = '\0';
        bitCount = hexCharToInt(line[6]) * 16 + hexCharToInt(line[7]);
        int count,i;

        for (count = 0,i=0; count < ((bitCount-1) / 8 + 1); count++)
        {
          hexCode[i] = line[i+10+2*count];
          i++;
          hexCode[i] = line[i+10+2*count];
          i++;
        }
        hexCode[i] = '\0';
      
        char binaryCode[100];
        hexToBinary(hexCode, binaryCode, bitCount);
        printf("字符%s为%s\n", ch, binaryCode);
        decodebuildHuffmanTree(root, binaryCode, (char)strtol(ch, NULL, 16));
    }
    fclose(file);
    printf("成功建树\n");


    // // 将十六进制文件内容转换为二进制字符串
    // char encodedBinary[100000];
    // hexFileToBinary(encodedFilename, encodedBinary);

    // // 解码过程
    // char decodedText[100000];
    // int decodedIndex = 0;
    // HuffmanNode *current = root;
    // for (int i = 0; encodedBinary[i] != '\0'; i++) {
    //     if (encodedBinary[i] == '0') {
    //         current = current->left;
    //     } else {
    //         current = current->right;
    //     }
    //     if (current->ch != '\0') {
    //         decodedText[decodedIndex++] = current->ch;
    //         current = root;
    //     }
    // }
    // decodedText[decodedIndex] = '\0';

    // // 输出解码后的文本
    // printf("解码后的文本: %s\n", decodedText);

    // // 将解码后的文本写入文件
    // char decodedFilename[100];

    // FILE *decodedFile = fopen(filename2, "w");
    // if (decodedFile == NULL) {
    //     perror("无法打开输出文件");
    //     return;
    // }
    // fputs(decodedText, decodedFile);
    // fclose(decodedFile);


    // 将十六进制文件内容转换为二进制字符串
    char encodedBinary[100000];
    hexFileToBinary(encodedFilename, encodedBinary);

    // 打开输出文件
    FILE *decodedFile = fopen(filename2, "w");
    if (decodedFile == NULL) {
        perror("无法打开输出文件");
        return;
    }

    int decodedBytes = 0;
    int startIndex = 0;
    HuffmanNode *current = root;
    char decodedText[505];  // 用于存储每次解码的结果

    int i;
    while (decodedBytes < totalBytes) {
        int blockDecodedBytes = 0;
        int decodedIndex = 0;
        for ( i = startIndex; encodedBinary[i] != '\0'; i++) {
            
            if (encodedBinary[i] == '0') {
                current = current->left;
            } else {
                current = current->right;
            }
            if (current->ch != '\0') {
                decodedText[decodedIndex++] = current->ch;
                blockDecodedBytes++;
                decodedBytes++;
                current = root;
                if (blockDecodedBytes >= 500 || decodedBytes >= totalBytes) {
                    break;
                }
            }
        }

        // 处理跨块未完成的编码
        if (current != root) {
            // 这里可以添加更复杂的处理逻辑，暂时简单提示
            printf("警告：存在跨块未完成的编码，可能需要更复杂处理\n");
            // 可以考虑记录状态，结合下一块继续解码
        }

        decodedText[decodedIndex] = '\0';
        printf("此时写入文件，最后二个单词的%c,最后一个单词的%c\n",decodedText[decodedIndex-2],decodedText[decodedIndex-1]);
        // 将解码结果写入文件
        fputs(decodedText, decodedFile);
        // startIndex += i - startIndex;
        startIndex = i+1;
    }

    fclose(decodedFile);

}

int main() {
    const char *filename1 = ".\\result2\\code.txt";
    const char *filename2 = ".\\result2\\yuanxi_j.txt";
    const char *filename4 = ".\\result2\\yuanxi.hfm";
    decode(filename1, filename4, filename2);
    return 0;
}