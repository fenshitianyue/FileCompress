#include <iostream>
#include "FileCompress.h"

void TestCompress(){
  FileCompress f;
  f.Compress("input");
}
void TestUnCompress(){
  FileCompress f;
  f.UnCompress("/home/zanda/Desktop/Project/FileCompress/src/input.huffman");
}



int main() {
  TestCompress();
  //TestUnCompress(); 
  return 0;
}
