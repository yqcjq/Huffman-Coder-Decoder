#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BUFFER_SIZE 2000

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
    // printf("错误：无法识别的十六进制字符: %c\n", hexChar); 
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

     // 添加调试输出，完整显示生成的二进制编码
     printf("生成的二进制编码: %s\n", binary); 
    
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


void hexToBit(const unsigned char *hex, char *binary, int byteCount) {
    int binIndex = 0;
    for (int i = 0; i < byteCount; i++) {
        unsigned char byte = hex[i];
        for (int j = 7; j >= 0; j--) {
            binary[binIndex++] = (byte & (1 << j)) ? '1' : '0';
        }
    }
    binary[binIndex] = '\0';
  }

// 解码函数
void decode(char *filename, char *encodedFilename,  char *filename2) {
    FILE *file = fopen(filename, "rb");
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
    
    // uint64_t hash1 = fnv1a_64(decodedText, strlen(decodedText));
    // printf("解码后的文本为: %s\n", decodedText);
    // printf("解码后的哈希值为: 0x%016llx\n", hash1);

    // // 将解码后的文本写入文件
    // char decodedFilename[100];

    // FILE *decodedFile = fopen(filename2, "w");
    // if (decodedFile == NULL) {
    //     perror("无法打开输出文件");
    //     return;
    // }
    // fputs(decodedText, decodedFile);
    // fclose(decodedFile);








    // FILE *outputFile1 = fopen(filename2, "wb");
    // fclose(outputFile1);

    // FILE *decodedFile = fopen(filename2, "ab");
    // if (decodedFile == NULL) {
    //     perror("无法打开输出文件");
    //     return;
    // }

    // unsigned char *content;
    // content = readFile(encodedFilename);
    // unsigned char byteData[1000] = "";
    // int len = 0;
    // unsigned char binary[8000] = "";

    // int decodedBytes = 0;
    
    // HuffmanNode *current = root;
    // unsigned char decodedText[2000];  // 用于存储每次解码的结果
    // printf("马上进入循环啦！\n");
    
    // int j = 0;
    // printf("totalBytes是%d\n", totalBytes);
    // while (decodedBytes < totalBytes) {
    //   // printf("进入while循环啦！\n");
    //   for (int i = 0; content[j] != '\0'; j ++) {
    //     // printf("进入for循环啦！\n");
    //     byteData[len++] = content[j];
    //     // printf("content[%d]是：%02x\n", j, content[j]);
    //     // printf("byteData[%d]是：%02x\n", len, byteData[len]);
    //     byteData[len + 1] = '\0'; 
    //     // int startIndex = 0;
    //     i = 0;
    //     if(len >= 200) {
    //         printf("字符达到200啦！\n");
            
    //         // 将十六进制文件内容转换为二进制字符串
    //         hexToBit(byteData, binary, len);
    //         // printf("十六进制是%s\n", byteData);
    //         // printf("二进制是%s\n", binary);
    //         //接下来开始解码
        
    //         // int blockDecodedBytes = 0;
    //         int decodedIndex = 0;
           
    //         for ( i = 0; binary[i] != '\0'; i++) {
             
    //             if (binary[i] == '0') {
    //                 current = current->left;
    //             } else {
    //                 current = current->right;
    //             }
    //             if (current->ch != '\0') {
                  
    //                 decodedText[decodedIndex++] = current->ch;
    //                 if(i == 1) {
    //                   printf("现在进入字符达到200内部的二叉树，找到叶子啦！\n");
    //                   printf("第一个输出的字符是%c\n", current->ch);
    //                 }
    //                 // printf("字符为%c\n",current->ch);
    //                 // blockDecodedBytes++;
    //                 decodedBytes++;
    //                 current = root;
    //                 if (decodedBytes >= totalBytes) {
    //                     break;
    //                 }
    //             }
    //         }  
    
    //         // 处理跨块未完成的编码
    //         if (current != root) {
    //           // 这里可以添加更复杂的处理逻辑，暂时简单提示
    //           printf("警告：存在跨块未完成的编码，可能需要更复杂处理\n");
    //           // strcpy(binary, binary + i);
    //           // memset(byteData, 0, sizeof(byteData));
    //           // HuffmanNode *current = root;
    //           // 可以考虑记录状态，结合下一块继续解码

    //         //   decodedText[decodedIndex] = '\0';
    //         // // printf("此时写入文件，最后二个单词的%c,最后一个单词的%c\n",decodedText[decodedIndex-2],decodedText[decodedIndex-1]);
    //         // // 将解码结果写入文件
    //         // // fputs(decodedText, decodedFile);
    //         // size_t bytesWritten = fwrite(decodedText, sizeof(char), strlen(decodedText), decodedFile);
    //         // printf("写入文件的内容是：%s\n", decodedText);

    //         // memset(decodedText, 0, sizeof(decodedText));
    //         // memset(byteData, 0, sizeof(byteData));
    //         // memset(binary, 0, sizeof(binary));
    //           continue;
    //         }
            
    //         decodedText[decodedIndex] = '\0';
    //         // printf("此时写入文件，最后二个单词的%c,最后一个单词的%c\n",decodedText[decodedIndex-2],decodedText[decodedIndex-1]);
    //         // 将解码结果写入文件
    //         // fputs(decodedText, decodedFile);
    //         size_t bytesWritten = fwrite(decodedText, sizeof(char), strlen(decodedText), decodedFile);
    //         printf("写入文件的内容是：%s\n", decodedText);

            
    //         // memset(decodedText, 0, sizeof(decodedText));
    //         memset(byteData, 0, sizeof(byteData));
    //         memset(binary, 0, sizeof(binary));
    //         len = 0;
    //         // startIndex = i + 1; 
           
        
    //     }
    //   }
    // //   for (int i = 0; i < 20; i++) {
    // //     printf("字符 %c 的十六进制ASCII码值是: 0x%02x\n", byteData[i], (unsigned char)byteData[i]);
    // // }
    // //   printf("十六进制是%llx\n", byteData);

    // // for (int i = 0; i < 20; i++) {
    // //     printf("字符 %c 的十六进制ASCII码值是: 0x%02x\n", byteData[i], (unsigned char)byteData[i]);
    // // }
    //   //检测最后的文本
    //   // printf("strlen(byteData)是%d\n", strlen(byteData));
    //   if(content[j] == '\0') {
    //     hexToBit(byteData, binary, strlen(byteData));
    //     // printf("二进制是%s\n", binary);
    //     int decodedIndex = 0;
    //     // HuffmanNode *current = root;
    //     for (int i = 0; binary[i] != '\0'; i++) {
                  
    //       if (binary[i] == '0') {
    //           current = current->left;
    //       } else {
    //           current = current->right;
    //       }
    //       if (current->ch != '\0') {
    //           decodedText[decodedIndex++] = current->ch;
    //           if(i == 1) {
    //             printf("第一个输出的字符是%c\n", current->ch);
    //           }
    //           decodedBytes++;
    //           printf("decodedBytes是%d\n", decodedBytes);
    //           current = root;
    //           if ( decodedBytes >= totalBytes) {
    //               break;
    //           }
    //       }
    //     }  
    //     decodedText[decodedIndex] = '\0';
    //     // printf("此时写入文件，最后二个单词的%c,最后一个单词的%c\n",decodedText[decodedIndex-2],decodedText[decodedIndex-1]);
    //     // 将解码结果写入文件
    //     // fputs(decodedText, decodedFile);
    //     size_t bytesWritten = fwrite(decodedText, sizeof(char), strlen(decodedText), decodedFile);
    //     // printf("写入文件的内容是：%s\n", decodedText);

    //     // memset(decodedText, 0, sizeof(decodedText));
    //     memset(byteData, 0, sizeof(byteData));
    //     memset(binary, 0, sizeof(binary));
    //     // printf("decodedBytes是：%d\n",decodedBytes);
    //   }

    // }

   
    

    // // 打开输出文件
    

    
   
    

    // fclose(decodedFile);
    // free(content);


    
    FILE *file2 = fopen(encodedFilename, "rb");
    if (encodedFilename == NULL) {
      perror("无法打开输出文件");
      return;
    }

    FILE *file3 = fopen(filename2, "wb");
    if (filename2 == NULL) {
      perror("无法打开输出文件");
      return;
    }
    fclose(file3);
    FILE *decodedFile = fopen(filename2, "ab");
    if (decodedFile == NULL) {
        perror("无法打开输出文件");
        return;
    }

    // unsigned char content[501];
    // content = readFile(encodedFilename);
    unsigned char byteData[BUFFER_SIZE] = "";
    unsigned char binary[BUFFER_SIZE * 8 + 1] = "";
    char remainingBinary[BUFFER_SIZE * 9 + 1] = "";  // 存储跨块未完成的二进制数据
    int remainingLen = 0;
    int decodedBytes = 0;
    size_t bytesRead;
    // int decodedBytes = 0;
    
    // printf("马上进入循环啦！\n");

    printf("totalBytes是%d\n", totalBytes);

      while ((bytesRead = fread(byteData, 1, BUFFER_SIZE, file2)) > 0)
      {
        // printf("进入循环了");
        hexToBit(byteData, binary, bytesRead);
        strcat( remainingBinary,binary);
        HuffmanNode *current = root;
        // printf("byteData的内容是%s", byteData);
        // printf("进入while循环啦！\n");

        int decodedIndex = 0;
        int lastValidPos = 0;

        for (int i = 0; remainingBinary[i] != '\0' && decodedBytes < totalBytes; i++) {
          // printf("进入for循环啦！\n");
            if (remainingBinary[i] == '0') {
                current = current->left;
            } else {
                current = current->right;
            }
            if (current->left == NULL && current->right == NULL) {
              // printf("向文件写入字符%c啦！\n",current->ch);
              fputc(current->ch, decodedFile);
                current = root;
                decodedBytes++;
                lastValidPos = i + 1;
                if ( decodedBytes >= totalBytes) {
                  break;
                }
            }
        }  

         // 处理跨块未完成的编码
         remainingLen = strlen(remainingBinary) - lastValidPos;
         strcpy(remainingBinary, remainingBinary + lastValidPos);

        
      }
    // }
       
    // 处理最后可能的未完成编码
      // printf("现在开始处理最后一段二进制\n");
      HuffmanNode *current = root;
      for (int i = 0; remainingBinary[i] != '\0' && decodedBytes < totalBytes; i++)
      {
        if (remainingBinary[i] == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current->left == NULL && current->right == NULL) {
          // printf("向文件写入字符%c啦！\n",current->ch);
            fputc(current->ch, decodedFile);
            current = root;
            decodedBytes++;
            if ( decodedBytes >= totalBytes) {
              break;
            }
        }
    }
    fclose(file);
    fclose(decodedFile);
  
}









// 验证接收人信息的函数
int verifyRecipientInfo(char *codeTablePath, char *encodedFilePath, char *decodedPath, const char *recipientId, const char *recipientName, const char *senderId, const char *senderName) {
    FILE *file = fopen(codeTablePath, "rb");
    if (file == NULL) {
        perror("无法打开编码表文件");
        return 0;
    }

    // 读取解码后文本的总字节数
    int totalBytes;
    fscanf(file, "%d\n", &totalBytes);

    // 构建哈夫曼树的根节点
    HuffmanNode *root = decodenewNode();

    // 读取编码表
    char line[100];
    int k = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        deleteSpace(line);
        printf("读取到的行: %s\n", line);
        char ch[3];
        int bitCount;
        char hexCode[30];
        ch[0] = line[2];
        ch[1] = line[3];
        ch[2] = '\0';
        bitCount = hexCharToInt(line[6]) * 16 + hexCharToInt(line[7]);
        int count, i;
        for (count = 0, i = 0; count < ((bitCount - 1) / 8 + 1); count++) {
            hexCode[i] = line[i + 10 + 2 * count];
            i++;
            hexCode[i] = line[i + 10 + 2 * count];
            i++;
        }
        hexCode[i] = '\0';
        char binaryCode[100];
        hexToBinary(hexCode, binaryCode, bitCount);
        // printf("字符: 0x%s, 编码: %s\n", ch, binaryCode);
        decodebuildHuffmanTree(root, binaryCode, (char)strtol(ch, NULL, 16));
    }
    fclose(file);

    printf("成功建立第二个编码表\n");

    FILE *encodedFile = fopen(encodedFilePath, "rb");
    if (encodedFile == NULL) {
        perror("无法打开编码文件");
        return 0;
    }

    unsigned char byteData[BUFFER_SIZE];
    char binary[BUFFER_SIZE * 8 + 1];
    char remainingBinary[BUFFER_SIZE * 9 + 1] = "";
    int remainingLen = 0;
    int decodedBytes = 0;
    size_t bytesRead;
    char decodedInfo[256] = "";
    printf("准备进入while循环\n");
    while ((bytesRead = fread(byteData, 1, BUFFER_SIZE, encodedFile)) > 0 && decodedBytes < 50) {
        hexToBit(byteData, binary, bytesRead);
        printf("十六进制数据: ");
        for (int i = 0; i < bytesRead; i++) {
            printf("%02x ", byteData[i]);
        }
        printf("\n二进制数据: %s\n", binary);
        strcat(remainingBinary, binary);
        // printf("111111\n");
        HuffmanNode *current = root;
        for (int i = 0; remainingBinary[i] != '\0' && decodedBytes < 2560; i++) {
            // printf("进入for循环\n");
            if (remainingBinary[i] == '0') {
                current = current->left;
                // printf("2222\n");
            } else {
                current = current->right;
                // printf("3333\n");
            }
            // printf("444\n");
            if (current->left == NULL && current->right == NULL) {
                // printf("555\n");
                // printf("decodedInfo的第%d位是%c\n", decodedBytes, current->ch);
                decodedInfo[decodedBytes++] = current->ch;
                current = root;
                if (decodedBytes >= 50 ) {
                break;
                }
            }
            // printf("66666\n");
        }
        // printf("出for循环\n");
        // remainingLen = strlen(remainingBinary);
        // for (int i = 0; i < remainingLen; i++) {
        //     if (remainingBinary[i] != '\0') {
        //        remainingBinary[i] = remainingBinary[i + decodedBytes];
        //     }
        // }
        // remainingBinary[remainingLen - decodedBytes] = '\0';

        // // 处理跨块未完成的编码
        // remainingLen = strlen(remainingBinary) - lastValidPos;
        // strcpy(remainingBinary, remainingBinary + lastValidPos);
    }
    fclose(encodedFile);
    decodedInfo[decodedBytes] = '\0';
    // printf("出while循环\n");
    printf("decodedInfo是%s\n", decodedInfo);
    // 解析前两行信息
    char *firstLine = strtok(decodedInfo, "\n");
    char *secondLine = strtok(NULL, "\n");

    printf("解析出来第一行是%s", firstLine);
    printf("解析出来第二行是%s", secondLine);

    if (firstLine == NULL || secondLine == NULL) {
        printf("解码的前两行信息不完整\n");
        return 0;
    }

    char id1[20], name1[10], id2[20], name2[10];
    sscanf(firstLine, "U%9s，%6s", id1, name1);
    sscanf(secondLine, "U%9s，%6s", id2, name2);

    printf("解析出来第一行id是%s,姓名是%s\n", id1,name1);
    printf("解析出来第一行id是%s,姓名是%s\n", id2,name2);
    if ((strcmp(id1, senderId) != 0 || strcmp(name1, senderName) != 0) ||
    (strcmp(id2, recipientId) != 0 || strcmp(name2, recipientName) != 0)) {
        printf("收件人或发件人信息不匹配\n");
        return 0;
    }

    // 调用 decode 函数解码全部
    decode(codeTablePath, encodedFilePath, decodedPath);

    // 删去前两行
    FILE *decodedFile = fopen(decodedPath, "r");
    if (decodedFile == NULL) {
        perror("无法打开解码后的文件");
        return 0;
    }
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("无法创建临时文件");
        fclose(decodedFile);
        return 0;
    }
    int lineCount = 0;
    char c;
    while ((c = fgetc(decodedFile)) != EOF) {
        if (lineCount >= 2) {
         fputc(c, tempFile);
        }
        if (c == '\n') {
          lineCount++;
        }
    }
    fclose(decodedFile);
    fclose(tempFile);

    remove(decodedPath);
    rename("temp.txt", decodedPath);

    return 1;
}


// 测试用的 main 函数
int main() {
    char codeTablePath[] = "code.txt";
    char encodedFilePath[] = "The_Wretched.hfm";
    const char *recipientId = "123451234";
    const char *recipientName = "张三";
    const char *senderId = "543215432";
    const char *senderName = "李四";
    char decodedPath[] = "The_Wretched_j.txt";

    // printf("1\n");
    if (verifyRecipientInfo(codeTablePath, encodedFilePath, decodedPath, recipientId, recipientName, senderId, senderName)) {
        printf("验证通过，解码完成并删除前两行信息\n");
    } else {
        printf("验证失败，终止解码\n");
    }
    printf("0\n");
    return 0;
}