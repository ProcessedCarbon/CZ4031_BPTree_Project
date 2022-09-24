#include <memory>
#include <vector>
#include <utility>

#include "treenode.h"
using namespace std;

Node::Node(int n, bool isLeafNode)
{
    keys = new int[n];
    ptrs = new Address[n + 1];
    isLeaf = isLeafNode;

    Address nullAddress{nullptr};
    for (int i = 0; i< n + 1; i++){
        ptrs[i] = nullAddress;
    }
    parentAddress = nullAddress;

    cout << "created a new node" << endl;
}