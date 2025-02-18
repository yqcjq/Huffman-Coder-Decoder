 for (int i = 0; content[i] != '\0'; i++)
  {
    strcat(binary, huffmanCodes[(unsigned char)content[i]]);
  }
  