#include <iostream>
#include "FileCompress.h"

void TestCompress(){
  FileCompress f;
  f.Compress("/home/zanda/Desktop/Project/FileCompress/src/input");
}
void TestUnCompress(){
  FileCompress f;
  f.UnCompress("/home/zanda/Desktop/Project/FileCompress/src/input.huffman");
}



int main() {
  FileCompress f;
  f.Compress("/home/zanda/Desktop/Project/FileCompress/src/input");
  //TestUnCompress(); 
  return 0;
}
