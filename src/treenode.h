#include <memory>
#include <vector>
#include <utility>
using namespace std;

struct Node {
    private:
        bool __isLeaf;
        int __size;    // max number of keys in the Node
    public:
        Node(bool isLeaf, int size){
            __isLeaf = isLeaf;
            __size = size;
        }
        //Getters
        bool getIsLeaf();
        int getSize();
        
        vector<shared_ptr<void>> ptrs; // non-leaf node points to node, leaf node points to vector with shared_ptr of the blocks
        vector<float> keys;
};