// 生成哈夫曼编码并存储到数组中
void generateHuffmanCodes(HuffmanNode *root, int arr[], int top)
{
  if (root->left)
  {
    arr[top] = 0;
    generateHuffmanCodes(root->left, arr, top + 1);
  }
  if (root->right)
  {
    arr[top] = 1;
    generateHuffmanCodes(root->right, arr, top + 1);
  }
  if (!root->left && !root->right)
  {
    printf("字符 '%c' 的哈夫曼编码为: ", root->ch);
    for (int i = 0; i < top; i++)
    {
      printf("%d", arr[i]);
    }
    printf("\n");
  }
}
