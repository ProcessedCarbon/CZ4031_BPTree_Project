#include "overflowBlock.h"
using namespace std;

OverflowBlock::OverflowBlock(int n)
{
    recordAddresses = new Address[n];
    
    Address nullAddress{nullptr};
    for (int i = 0; i< n + 1; i++){
        recordAddresses[i] = nullAddress;
    }

    next = nullptr;

    cout << "created a new overflow block" << endl;
} 