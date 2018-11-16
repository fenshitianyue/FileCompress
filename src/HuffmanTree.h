#pragma once
#include <queue>
#include <vector>
#include <algorithm>

template<class W>
struct HuffmanTreeNode{
  W _w;
  HuffmanTreeNode* _left;
  HuffmanTreeNode* _right;
  HuffmanTreeNode(const W& w):
    _w(w),_left(nullptr),_right(nullptr){}
};

template<class W>
class HuffmanTree{
public:
  typedef HuffmanTreeNode<W> Node;
  struct NodeCompare{
    bool operator()(Node* wr, Node* wl){
      return wr->_w  < wl->_w;
    }
  };
  HuffmanTree():
    _root(nullptr){}
  HuffmanTree(const HuffmanTree<W>& h) = delete;
  HuffmanTree<W>& operator=(const HuffmanTree<W>& h) = delete;
  HuffmanTree(W* w, unsigned int n, const W& invaild){
    std::priority_queue<Node*, std::vector<Node*>, NodeCompare> minHeap;
    for(unsigned int i = 0; i < n; ++i){
      if(w[i] != invaild){
        minHeap.push(new Node(w[i]));
      }
    } 
    while(minHeap.size() > 1){
      Node* left = minHeap.top();
      minHeap.pop();
      Node* right = minHeap.top();
      minHeap.pop();
      Node* parent = new Node(left->_w + right->_w);
      parent->_left = left;
      parent->_right = right;
      minHeap.push(parent);
    }
    _root = minHeap.top();
  }
  Node* GetRoot(){
    return _root;
  }
  ~HuffmanTree(){
    Destroy(_root);
  }
  void Destroy(Node* root){
    if(nullptr != root){
      Destroy(root->_left);
      Destroy(root->_right);
      delete root;
    }
  }
private:
  Node* _root;
};
