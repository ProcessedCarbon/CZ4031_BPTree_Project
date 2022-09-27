#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <memory>
#include <vector>
#include <utility>
#include <stack>
#include <typeinfo>

#include "treenode.h"
#include "block.h"

using namespace std;

class BPTree
{
public:
    int MAX = 3;
    Node *root;
    BPTree(int MAXKEYS)
    {
        root = NULL;
        MAX = MAXKEYS;
    }

    ~BPTree() = default;
    void Insert(int numVotes, void *recordptr);
    void InsertInternal(int numVotes, Node *curr, Node *child);
    Node *FindParent(Node *curr, Node *child);
    void DisplayTree(Node *curr);
    void *Search(int numVotes);
    int SearchKey(int numVotes);
    int *SearchRangeKey(int min, int max);
    void Delete(int numVotes);
    int getHeight();
    Node *getParent(Node *child, int numVotes);
    void AssignAllParents(Node *curr);
    void setNewRoot();
    void DeleteAndCheckNewRoot(int numVotes);
    void InsertWithAssignParents(int numVotes, void *recordptr);
    void CheckRootKeys();
    int getNumNodes();
    int computeNumNodesInTree();
};



#endif // BPLUSTREE_H
