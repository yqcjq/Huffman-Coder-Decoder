#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 插入收件人和发件人信息到文件开头的函数
void insertInfoToFile(char *filePath, char *senderId, char *senderName, char *recipientId, char *recipientName) {
    // 打开原文件以读取内容
    FILE *originalFile = fopen(filePath, "r");
    if (originalFile == NULL) {
        perror("无法打开原文件");
        return;
    }

    // 创建一个临时文件以写入新内容
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("无法创建临时文件");
        fclose(originalFile);
        return;
    }

    // 生成收件人和发件人信息字符串
    char recipientInfo[100];
    char senderInfo[100];
    snprintf(senderInfo, sizeof(senderInfo), "U%s，%s\n", senderId, senderName);
    snprintf(recipientInfo, sizeof(recipientInfo), "U%s，%s\n", recipientId, recipientName);


    // 将收件人和发件人信息写入临时文件
    fputs(senderInfo, tempFile);
    fputs(recipientInfo, tempFile);

    // 将原文件内容复制到临时文件
    int ch;
    while ((ch = fgetc(originalFile)) != EOF) {
        fputc(ch, tempFile);
    }

    // 关闭原文件和临时文件
    fclose(originalFile);
    fclose(tempFile);

    // 删除原文件
    if (remove(filePath) != 0) {
        perror("无法删除原文件");
        return;
    }

    // 将临时文件重命名为原文件
    if (rename("temp.txt", filePath) != 0) {
        perror("无法重命名临时文件");
        return;
    }

    printf("信息插入成功！\n");
    printf("发送人信息：U%s，%s\n", senderId,senderName);
    printf("接收人信息：U%s，%s\n", recipientId,recipientName);
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    // const char *filePath = "yuanxi.txt";
    char *filePath = "The_Wretched.txt";
    char recipientId[10] = "123451234";
    char recipientName[100];
    char senderId[10] = "543215432";
    char senderName[100];


    printf("请设置发送人的姓名：");
    if (fgets(senderName, sizeof(senderName), stdin) != NULL) {
        // 去除换行符
        senderName[strcspn(senderName, "\n")] = '\0';
    }
    printf("发送人的姓名是%s\n", senderName);

    printf("请设置收件的姓名：");
    if (fgets(recipientName, sizeof(recipientName), stdin) != NULL) {
        // 去除换行符
        recipientName[strcspn(recipientName, "\n")] = '\0';
    }

    insertInfoToFile(filePath, senderId, senderName, recipientId, recipientName);
    printf("发送人姓名的长度是%d\n", (int)strlen(senderName));

    return 0;
}