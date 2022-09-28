#ifndef OVERFLOWBLOCK_H
#define OVERFLOWBLOCK_H
#include "treenode.h"

struct OverflowBlock {
    public:
        Address *recordAddresses;
        OverflowBlock *next;
        OverflowBlock(int n);
};

#endif