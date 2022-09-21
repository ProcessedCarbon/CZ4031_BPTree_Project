#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <memory>
#include <vector>
#include <utility>

#include "treenode.h"
#include "block.h"

using namespace std;
class BPTree{
    private:
        Node * __root;
        int __n;    // max number of keys in a tree node
        int __numNodes;    // number of nodes in this tree
        int __height;
        int __minNumOfKeysInLeafNode; // floor((n+1)/2)
        int __minNumOfChildrenInNonLeafNode; // floor(n/2)+1

        int computeMinNumOfKeysInLeafNode(float n);
        int computeMinNumOfKeysInLeafNodeCeil(float n);
        int computeMinNumOfChildrenInNonLeafNode(float n);
        int computeMinNumOfChildrenInNonLeafNodeCeil(float n);

    public:
        BPTree(int n){ // TODO: n to be determined by size of the block to simulate node being a block
            __root = nullptr;
            __n = n;
            __numNodes = 0;
            __height = 0;
            __minNumOfKeysInLeafNode = computeMinNumOfKeysInLeafNode(n);
            __minNumOfChildrenInNonLeafNode = computeMinNumOfChildrenInNonLeafNode(n);
        }

        ~BPTree() = default;
        void insert(int numVotes, void * recordAddress);
        void query(int numVotes);

        //void delete(int numVotes);

        // Getters
        int getN();
        int getNumNodes();
        int getHeight();
        int getMinNumOfKeysInLeafNode();
        int getMinNumOfChildrenInNonLeafNode();

        // Helpers
        bool isNodeFull(Node * currNode);
        void insertKeyAndAddrToNonFullLeafNode(Node * currNode, int key, void * addr);
        void insertKeyAndAddrToNonFullParentNode(Node * currNode, Node * newNode);
        int findIndexOfFullNodeToInsert(Node * currNode, int key);
        void updateParentNode(Node * currNode, Node * newNode);
};

#endif    // BPLUSTREE_H
