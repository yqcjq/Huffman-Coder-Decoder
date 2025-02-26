#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义一个结构体来存储字符和词频
typedef struct {
    unsigned char ch;
    int freq;
} CharFreq;

// 比较函数，用于 qsort 排序
int compare(const void *a, const void *b) {
    CharFreq *charFreqA = (CharFreq *)a;
    CharFreq *charFreqB = (CharFreq *)b;

    // 先按词频排序
    if (charFreqA->freq != charFreqB->freq) {
        return charFreqA->freq - charFreqB->freq;
    }
    // 词频相同，按字符排序
    return (int)charFreqA->ch - (int)charFreqB->ch;
}

int main() {
    FILE *file;
    CharFreq *charFreqs = NULL;
    int count = 0;
    int capacity = 10; // 初始容量

    // 打开文件
    file = fopen("1111111111111111.txt", "r");
    if (file == NULL) {
        perror("无法打开文件");
        return 1;
    }

    // 动态分配初始内存
    charFreqs = (CharFreq *)malloc(capacity * sizeof(CharFreq));
    if (charFreqs == NULL) {
        perror("初始内存分配失败");
        fclose(file);
        return 1;
    }

    unsigned char ch;
    int freq;
    // 逐行读取文件
    while (1) {
        int result = fscanf(file, "0x%02hhx: %d", &ch, &freq);
        if (result == EOF) {
            break;
        }
        if (result != 2) {
            printf("解析文件时出错，格式不匹配，跳过该行。\n");
            // 跳过当前行
            while (fgetc(file) != '\n' && !feof(file));
            continue;
        }

        if (count >= capacity) {
            // 容量不足，扩展内存
            capacity *= 2;
            charFreqs = (CharFreq *)realloc(charFreqs, capacity * sizeof(CharFreq));
            if (charFreqs == NULL) {
                perror("内存分配失败");
                fclose(file);
                return 1;
            }
            printf("成功扩展内存，新容量为 %d\n", capacity);
        }
        // 存储字符和词频
        charFreqs[count].ch = ch;
        charFreqs[count].freq = freq;
        count++;
    }

    // 关闭文件
    fclose(file);

    // 对字符频率数组进行排序
    qsort(charFreqs, count, sizeof(CharFreq), compare);

    // 输出排序结果
    for (int i = 0; i < count; i++) {
        printf("0x%02x: %d\n", charFreqs[i].ch, charFreqs[i].freq);
    }

    // 释放内存
    free(charFreqs);

    return 0;
}