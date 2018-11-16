#pragma once

#include <fstream>
#include <string>
//#include <cassert>
#include <exception>
#include "HuffmanTree.h"

using LongType = long long;

struct CharInfo{
  char _ch;
  LongType _count;
  std::string _code;

  bool operator<(const CharInfo& ch){
    return _count < ch._count;
  }
  CharInfo operator+(const CharInfo& ch){
    CharInfo ret;
    ret._count = _count + ch._count;
    return ret;
  }
  bool operator!=(const CharInfo& ch){
    return _count != ch._count;
  }
};

class FileCompress{
public:
  using Node = HuffmanTreeNode<CharInfo>;
  FileCompress(){
    for(size_t i = 0; i < 256; ++i){
      _hashInfos[i]._ch = static_cast<char>(i);
      _hashInfos[i]._count = 0;
    }
  }
 
  void GenerateHuffmanCode(Node* root, std::string code){
    if(nullptr == root) return;
    if(nullptr == root->_left && nullptr == root->_right){
      _hashInfos[static_cast<int>(root->_w._ch)]._code = code;
    }
    GenerateHuffmanCode(root->_left, code + '0');
    GenerateHuffmanCode(root->_right, code + '1');
  }
  
  void Compress(const std::string& file){
    std::ifstream ifs(file.c_str(), std::ios::in | std::ios::binary);
    char ch;
    while(ifs.get(ch)){
      _hashInfos[static_cast<int>(ch)]._count++;
    }
    CharInfo invaild;
    invaild._count = 0;
    HuffmanTree<CharInfo> tree(_hashInfos, 256, invaild);
    std::string code;
    GenerateHuffmanCode(tree.GetRoot(), code);
    
    std::string target_file = file;
    target_file += ".huffman";
    std::ofstream ofs(target_file.c_str(), std::ios::out | std::ios::binary);

    ifs.clear();
    ifs.seekg(0);
    size_t pos = 0;
    char value = 0;
    while(ifs.get(ch)){
      std::string& code = _hashInfos[static_cast<int>(ch)]._code;
      for(size_t i = 0; i < code.size(); ++i){
        if('0' == code[i]){
          value &= (~(1 << pos));
        } else if('1' == code[i]){
          value |= (1 << pos);
        } else{
          throw std::runtime_error("invaild huffman code");
        }
        ++pos;
        if(8 == pos){
          ofs.put(value);
          pos = 0;
          value = 0;
        }
      }//end for(size_t)
    }//end while
    if(pos > 0){
      ofs.put(value);
    }
  }

  void UnCompress(const std::string& file){

  }

private:
  CharInfo _hashInfos[256];
};



