#include <memory>
#include <vector>
#include <utility>

#include "treenode.h"
using namespace std;
bool Node::getIsLeaf(){
    return __isLeaf;
}

int Node::getSize(){
    return __size;
}