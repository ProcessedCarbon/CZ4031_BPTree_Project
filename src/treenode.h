#ifndef TREENODE_H
#define TREENODE_H

#include <memory>
//#include <vector>
#include <utility>

#include <iostream>
using namespace std;

// Defines an address of a record stored as a block address with an offset.
struct Address
{
  void *blockAddress;
  short int offset; // not sure what's the pt of this?
};

struct Node {
    public:
        bool isLeaf;
        int *keys;
        Address *ptrs; // can point to either nodes or blocks
        Address parentAddress; // you need this to traverse back up
        Node(int n, bool isLeafNode);

        //vector<int> keys;
        //vector<void *> ptrs; // non-leaf node points to node, leaf node points to vector with shared_ptr of the blocks
};

#endif