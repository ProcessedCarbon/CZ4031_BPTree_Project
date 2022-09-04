#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <memory>
#include <vector>
#include <utility>

#include "treenode.h"
#include "block.h"

using namespace std;
class BPTree{
    shared_ptr<Node> root;
    int __treesize;    // max number of keys in a tree node
    int numNodes;    // number of nodes in this tree

    public:
        BPTree(int size){
            __treesize = size;
            root = nullptr;
            numNodes = 0;
        }

        ~BPTree() = default;
        void Insert(float numVotes, Block block);
        void Delete(float numVotes);
};

#endif    // BPLUSTREE_H
