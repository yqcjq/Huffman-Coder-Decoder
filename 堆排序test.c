#include <stdio.h>
#include <stdlib.h>

// 假设 CharFreq 和 HuffmanNode 结构体定义如下
typedef struct {
    char ch;
    int freq;
} CharFreq;

typedef struct HuffmanNode {
    char ch;
    int freq;
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} HuffmanNode;

// 交换两个 CharFreq 元素的函数
void swap(CharFreq *a, CharFreq *b) {
    CharFreq temp = *a;
    *a = *b;
    *b = temp;
}

// 调整最大堆的函数
void minHeapify(CharFreq arr[], int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // 先比较频率，如果频率相同再比较字符的字节值
    if (left < n) {
      if (arr[left].freq < arr[smallest].freq || 
          (arr[left].freq == arr[smallest].freq && arr[left].ch < arr[smallest].ch)) {
          smallest = left;
      }
    }

    if (right < n) {
      if (arr[right].freq < arr[smallest].freq || 
          (arr[right].freq == arr[smallest].freq && arr[right].ch < arr[smallest].ch)) {
          smallest = right;
      }
    }

    if (smallest != i) {
      swap(&arr[i], &arr[smallest]);
      minHeapify(arr, n, smallest);
    }
}

// 堆排序函数
void heapSort(CharFreq arr[], int n) {
    // 构建最大堆
    for (int i = n / 2 - 1; i >= 0; i--) {
      minHeapify(arr, n, i);
    }

    // 一个个从堆中取出元素
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        minHeapify(arr, i, 0);
    }

     // 反转数组，使结果按从小到大排列
     for (int i = 0; i < n / 2; i++) {
      swap(&arr[i], &arr[n - i - 1]);
  }
}

// 测试代码
int main() {
    CharFreq arr[] = {{'c', 2}, {'f', 2},{'a', 2}, {'b', 1}, {'e', 2}};
    int n = sizeof(arr) / sizeof(arr[0]);

    heapSort(arr, n);

    // 输出排序结果
    for (int i = 0; i < n; i++) {
        printf("字符 '%c' 出现的频次为: %d\n", arr[i].ch, arr[i].freq);
    }

    return 0;
}
