#pragma once

#include <fstream>
#include <string>
#include <cassert>
#include <exception>
#include "HuffmanTree.h"

using BigLong = long long;

struct CharInfo{
  char _ch;
  BigLong _count;
  std::string _code;
  CharInfo() {}
  CharInfo(const BigLong count): _count(count) {}

  bool operator>(const CharInfo& ch){
    return _count > ch._count;
  }
  CharInfo operator+(const CharInfo& ch){
    return CharInfo(_count + ch._count); //maybe have bug
  }
  bool operator!=(const CharInfo& ch){
    return _count != ch._count;
  }
};

class FileCompress{
public:
  using Node = HuffmanTreeNode<CharInfo>;
  struct ConfigInfo{
    char _ch;
    BigLong _count;
  };
  FileCompress(){
    for(size_t i = 0; i < sizeof(_hashInfos) / sizeof(_hashInfos[0]); ++i){
      _hashInfos[i]._ch = static_cast<char>(i);
      _hashInfos[i]._count = 0;
    }
  }
 
  void GenerateHuffmanCode(Node* root, std::string code){
    if(nullptr == root) return;
    if(nullptr == root->_left && nullptr == root->_right){
      _hashInfos[static_cast<int>(root->_w._ch)]._code = code; //maybe have bug
    }
    if(nullptr != root->_left)
      GenerateHuffmanCode(root->_left, code + '0');
    if(nullptr != root->_right)
      GenerateHuffmanCode(root->_right, code + '1');
  }
  
  void Compress(const std::string& file){
    std::ifstream ifs(file.c_str(), std::ios::in | std::ios::binary); //maybe have bug
    char ch;
    while(ifs.get(ch)){
      _hashInfos[static_cast<int>(ch)]._count++; //maybe have bug
    }
    CharInfo invaild;
    invaild._count = 0;
    HuffmanTree<CharInfo> tree(_hashInfos, sizeof(_hashInfos) / sizeof(_hashInfos[0]), invaild);
    std::string code;
    GenerateHuffmanCode(tree.GetRoot(), code);
    
    std::string target_file = file;
    target_file += ".huffman";
    std::ofstream ofs(target_file.c_str(), std::ios::out | std::ios::binary); //maybe have bug

    //write UnCompress configeration
    for(size_t i = 0; i < sizeof(_hashInfos) / sizeof(_hashInfos[0]); ++i){
      if(_hashInfos[i]._count > 0){
        ConfigInfo info;
        info._ch = _hashInfos[i]._ch;
        info._count = _hashInfos[i]._count;
        ofs.write(reinterpret_cast<char*>(&info), sizeof(ConfigInfo)); //maybe have bug
      }
    }
    ConfigInfo end;
    end._count = 0;
    ofs.write(reinterpret_cast<char*>(&end), sizeof(ConfigInfo)); //maybe have bug

    ifs.clear();
    ifs.seekg(0);
    size_t pos = 0;
    char value = 0;
    while(ifs.get(ch)){
      std::string& code = _hashInfos[static_cast<int>(ch)]._code; //maybe have bug
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
    std::ifstream ifs(file.c_str(), std::ios::in | std::ios::binary);
    std::string result_file = file;
    size_t pos = result_file.rfind('.');
    assert(pos != std::string::npos);
    result_file.erase(pos);
#ifdef _DEBUG 
    result_file += ".unhuffman";
#endif
    std::ofstream ofs(result_file.c_str(), std::ios::out | std::ios::binary);
    while(true){
      ConfigInfo tmp;
      ifs.read(reinterpret_cast<char*>(&tmp), sizeof(ConfigInfo));
      if(tmp._count > 0){
        _hashInfos[static_cast<int>(tmp._ch)]._count = tmp._count;
      }else{
        break;
      }
    }//end while
    CharInfo invaild;
    invaild._count = 0;
    HuffmanTree<CharInfo> tree(_hashInfos, sizeof(_hashInfos) / sizeof(_hashInfos[0]), invaild);
    Node* root = tree.GetRoot();
    BigLong CharNum = root->_w._count;
    Node* cur = root;
    char ch;
    while(ifs.get(ch)){
      for(size_t pos = 0; pos < 8; ++pos){
        if (ch&(1 << pos)) {
          cur = cur->_right;
        }
        else {
          cur = cur->_left;
        }

        if (cur->_left == NULL && cur->_right == NULL) {
					ofs.put(cur->_w._ch);
					cur = root;
					if (--CharNum == 0) {
						break;
					}
				} //end if(cur->_left == NULL...)
      }//end for
    }//end while(ifs.get(ch))
  }

private:
  CharInfo _hashInfos[256];
};



