#include <memory>
#include <vector>
#include <utility>
#include "block.h"

using namespace std;

struct Node
{
public:
    bool __isLeaf;
    int __size; // curr number of keys in the Node
    void **ptr;
    int *key;
    int __minkeys; // min number of keys in the Node
    int __minChildren;
    int __maxsize;
    Node *parent;
    
    Node(int MAXSIZE, bool leaf)
    {
        key = new int[MAXSIZE];
        ptr = new void *[MAXSIZE + 1];
        __isLeaf = leaf;
        __minkeys = __isLeaf ? ((MAXSIZE + 1) / 2) : (MAXSIZE / 2);
        __minChildren = __isLeaf ? 0 : __minkeys * 2;
        __maxsize = MAXSIZE;
        __size = 0;
    }

    //int getLeftChild(int keypos);
    int getRightChildKey(int keypos);
    int FindKey(int numVotes);
    void Deletion(int numVotes);
    void RemoveFromLeafNode(int keypos);
    void RemoveFromInternalNode(int keypos, int numVotes);
    void Fill(int keypos);
    void Merge(int keypos);
    void BorrowPrev(int keypos);
    void BorrowNext(int keypos);
    void printNodeContent(Node *node);
    int GetKeyLocationInParent(Node *parent, int parentIndex,int key);
    int getSizeOfAllButKeys();
};