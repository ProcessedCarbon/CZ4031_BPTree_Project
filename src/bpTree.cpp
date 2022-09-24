//#include <iostream>

#include "bpTree.h"
#include "treenode.h"
#include "record.h"

#include <cmath>
//using namespace std;


// private 
int BPTree::computeMinNumOfKeysInLeafNode(float n){
    return floor((n+1)/2);
}

int BPTree::computeMinNumOfKeysInLeafNodeCeil(float n){
    return ceil((n+1)/2);
}

int BPTree::computeMinNumOfChildrenInNonLeafNode(float n){
    return floor(n/2)+1;
}

int BPTree::computeMinNumOfChildrenInNonLeafNodeCeil(float n){
    return ceil(n/2)+1;
}

//public
void BPTree::insert(int numVotes, void * recordAddress){
    // Strategy
    // Insertion Step
    // 1. Traverse down to the lowest level (leaf nodes)
    // 2. Iterate thru the leaf nodes to insert
    //      1. If no space to insert in current leafnode, split it
    // Updating of non-leaf nodes (indexes)
    // TODO: test for repeated numVotes 

    cout << "Inserting " << numVotes <<  endl;
    if (__root == nullptr){
        // create first node
        __root = new Node(__n, true);
        __root->keys[0] = numVotes;
        __root->ptrs[0].blockAddress = recordAddress;

        __numNodes = 1;
        __height = 1;
    }
    else 
    {
        // traverse down the tree till you hit the leaf nodes
        Node * currNode;
        currNode = __root;
        while (!currNode->isLeaf)
        {
            // Find the node to traverse from
            for (int i = 0; i < __n; i++)
            {
                if (numVotes < currNode->keys[i] || currNode->keys[i] == NULL)
                {
                    currNode = (Node *) currNode->ptrs[i].blockAddress;
                    break;
                }

                // you will hit this if your target value is bigger than all the key values of a fully-filled node
                if (i == __n-1)
                {
                    currNode = (Node *) currNode->ptrs[i+1].blockAddress;
                    break;
                }
            }
        }

        // when you hit leaf nodes
        if (currNode->isLeaf)
        {
            bool leafNodeIsFull = isNodeFull(currNode);
            
            if (leafNodeIsFull)
            {
                // 1. find index to insert
                // 2. create a temp node to store all the keys and ptrs in sorted order
                // 3. Based on temp node, repopulate currNode and newNode (identify the pivot index)
                // 4. create newNode
                // 5. link currNode to newNode
                // 6. create new root node if need be, else update above indexes recursively

                // find index to insert
                int indexToInsert = findIndexOfFullNodeToInsert(currNode, numVotes);
                
                // create a temp node to store all the keys and ptrs in sorted order
                Node * tempNode = new Node(__n+1, true);
                tempNode->keys[indexToInsert] = numVotes;
                tempNode->ptrs[indexToInsert].blockAddress = recordAddress;

                int currNodeIndex = 0;
                // iterate thru tempNode to populate
                for (int i = 0; i < __n+1; i++)
                {
                    if (i == indexToInsert)
                        continue;
                    tempNode->keys[i] = currNode->keys[currNodeIndex];
                    tempNode->ptrs[i].blockAddress = currNode->ptrs[currNodeIndex].blockAddress;
                    currNodeIndex++;
                }

                // based on temp node,
                int pivotIndex = computeMinNumOfKeysInLeafNodeCeil(__n);
                // repopulate currNode
                for (int i = 0; i < pivotIndex; i++)
                {
                    currNode->keys[i] = tempNode->keys[i];
                    currNode->ptrs[i].blockAddress = tempNode->ptrs[i].blockAddress;
                }
                for (int i = pivotIndex; i < __n; i++)
                {
                    currNode->keys[i] = NULL;
                    currNode->ptrs[i].blockAddress = nullptr;
                }
                //currNode->ptrs[__n].blockAddress = nullptr;

                // create newNode
                Node * newNode = new Node(__n, true);
                __numNodes++;
                // populate newNode
                for (int i = 0; i < computeMinNumOfKeysInLeafNode(__n); i++)
                {
                    newNode->keys[i] = tempNode->keys[pivotIndex];
                    newNode->ptrs[i].blockAddress = tempNode->ptrs[pivotIndex].blockAddress;
                    pivotIndex++;
                }
                
                delete tempNode;                

                // linking the nodes together
                Node * temp = (Node *) currNode->ptrs[__n].blockAddress;
                currNode->ptrs[__n].blockAddress = newNode;
                newNode->ptrs[__n].blockAddress = temp;
                // notify parent node of new child
                if (currNode->parentAddress.blockAddress != nullptr)
                    newNode->parentAddress.blockAddress = currNode->parentAddress.blockAddress;

                // you need to recur if there is a split in node
                updateParentNode(currNode, newNode);
            } 
            else
                insertKeyAndAddrToNonFullLeafNode(currNode, numVotes, recordAddress);
        }
    }


    //printBpTreeInfo();
    
    

    // for (int i = 0; i < __n; i++)
    // {
    //     if (__root->keys[i] == NULL) continue;
    //     cout << "key:" << __root->keys[i] << endl;

    //     Record * recordAddress = (Record *) __root->ptrs[i].blockAddress;
    //     cout << "Movie Id: " << recordAddress->getMovieId() << endl;
    //     cout << "Average Rating: " << recordAddress->getAverageRating() << endl;
    //     cout << "Num votes: " << recordAddress->getNumVotes() << endl;
    // }

}

void BPTree::query(int numVotes){
    if (__root == nullptr)
    {
        cout << "Query error: B+ tree has no root" << endl;
        return;
    }

    int numOfIndexNodesAccessed = 0;
    // traverse down the tree till you hit the leaf nodes
    Node * currNode;
    currNode = __root;
    while (!currNode->isLeaf)
    {
        numOfIndexNodesAccessed++;
        
        // print content of indexNode
        cout << "Content of index node " << numOfIndexNodesAccessed << endl;
        for (int i = 0; i < __n; i++)
        {
            if (currNode->keys[i] == NULL)
                break;
            cout << currNode->keys[i] << " ";
        }
        cout << currNode->ptrs[0].blockAddress << " ";
        for (int i = 1; i <= __n; i++)
        {
            if (currNode->ptrs[i].blockAddress == NULL)
                break;
            cout << currNode->ptrs[i].blockAddress << " ";
        }
        cout << endl;

        // Find the node to traverse from
        for (int i = 0; i < __n; i++)
        {
            if (numVotes < currNode->keys[i] || currNode->keys[i] == NULL)
            {
                currNode = (Node *) currNode->ptrs[i].blockAddress;
                break;
            }

            // you will hit this if your target value is bigger than all the key values of a fully-filled node
            if (i == __n-1)
            {
                currNode = (Node *) currNode->ptrs[i+1].blockAddress;
                break;
            }
        }
    }
    
    cout << "Number of index nodes accessed: " << numOfIndexNodesAccessed << endl;
    cout << endl;

    int numOfDatablocksAccessed = 0;
    float sumOfAverageRatings = 0;
    // hit leaf nodes
    if (currNode->isLeaf)
    {
        for (int i = 0; i < __n; i++)
        {
            if (currNode->keys[i] == numVotes)
            {
                cout << "You found a movie of " << numVotes << " numVotes!" << endl;
                Record * recordAddress = (Record *) currNode->ptrs[i].blockAddress;
                cout << "Movie Id: " << recordAddress->getMovieId() << endl;
                cout << "Average Rating: " << recordAddress->getAverageRating() << endl;
                cout << "Num votes: " << recordAddress->getNumVotes() << endl;
                cout << endl;

                numOfDatablocksAccessed++;
                sumOfAverageRatings += recordAddress->getAverageRating();
            }
            else if (currNode->keys[i] > numVotes)
            {
                break;
            }
        }
    }
    cout << "Number of data blocks accessed: " << numOfDatablocksAccessed << endl;
    cout << "Average of averageRatings: " << sumOfAverageRatings/numOfDatablocksAccessed << endl;
}

void BPTree:: queryRangeInclusively(int startNumVotes, int endNumVotes){
    if (__root == nullptr)
    {
        cout << "Query error: B+ tree has no root" << endl;
        return;
    }

    int numOfIndexNodesAccessed = 0;
    // traverse down the tree till you hit the leaf nodes
    Node * currNode;
    currNode = __root;
    while (!currNode->isLeaf)
    {
        numOfIndexNodesAccessed++;
        
        // print content of indexNode
        cout << "Content of index node " << numOfIndexNodesAccessed << endl;
        for (int i = 0; i < __n; i++)
        {
            if (currNode->keys[i] == NULL)
                break;
            cout << currNode->keys[i] << " ";
        }
        cout << currNode->ptrs[0].blockAddress << " ";
        for (int i = 1; i <= __n; i++)
        {
            if (currNode->ptrs[i].blockAddress == NULL)
                break;
            cout << currNode->ptrs[i].blockAddress << " ";
        }
        cout << endl;

        // Find the node to traverse from
        for (int i = 0; i < __n; i++)
        {
            if (startNumVotes < currNode->keys[i] || currNode->keys[i] == NULL)
            {
                currNode = (Node *) currNode->ptrs[i].blockAddress;
                break;
            }

            // you will hit this if your target value is bigger than all the key values of a fully-filled node
            if (i == __n-1)
            {
                currNode = (Node *) currNode->ptrs[i+1].blockAddress;
                break;
            }
        }
    }
    
    cout << "Number of index nodes accessed: " << numOfIndexNodesAccessed << endl;
    cout << endl;

    int numOfDatablocksAccessed = 0;
    float sumOfAverageRatings = 0;
    // hit leaf nodes
    if (currNode->isLeaf)
    {
        int i = 0;
        while (true)
        {
            if (currNode->keys[i] >= startNumVotes && currNode->keys[i] <= endNumVotes)
            {
                cout << "You found a movie that lies within the range of " << startNumVotes << " and " << endNumVotes << " numVotes!" << endl;
                Record * recordAddress = (Record *) currNode->ptrs[i].blockAddress;
                cout << "Movie Id: " << recordAddress->getMovieId() << endl;
                cout << "Average Rating: " << recordAddress->getAverageRating() << endl;
                cout << "Num votes: " << recordAddress->getNumVotes() << endl;
                cout << endl;

                numOfDatablocksAccessed++;
                sumOfAverageRatings += recordAddress->getAverageRating();

                if (currNode->keys[i+1] != NULL)
                    i++;
                else
                {
                    // point to next leaf node
                    Node * nextLeafNode = (Node *) currNode->ptrs[__n].blockAddress;
                    if (nextLeafNode == nullptr)
                        break;
                    currNode = nextLeafNode;
                    i = 0;
                }
            }
            else
                break;
        }
    }
    cout << "Number of data blocks accessed: " << numOfDatablocksAccessed << endl;
    cout << "Average of averageRatings: " << sumOfAverageRatings/numOfDatablocksAccessed << endl;
}

void BPTree::remove(int numVotes){
    // approach
    // 1. Find the leaf node to delete record from
    // deletion scenarios
    // delete a record and
    // 1. leaf node still satisfies min number of keys
    //      a. if first key of leaf node is deleted, 
    //         might need to update the direct parent node if it's not it's first child.
    //         If leaf node is direct parent node's first child, check the direct parent's parent.
    //         Do this recursively 
    //          
    // 2. check if leaf node can borrow from adjacent sibling node (sibling before & after)
    //      
    // 3. leaf node cannot borrow and needs to merge with adjacent sibling node 

    if (__root == nullptr)
    {
        cout << "Deletion error: B+ tree has no root" << endl;
        return;
    }

    // traverse down the tree till you hit the leaf nodes
    Node * currNode;
    currNode = __root;
    while (!currNode->isLeaf)
    {
        // Find the node to traverse from
        for (int i = 0; i < __n; i++)
        {
            if (numVotes < currNode->keys[i] || currNode->keys[i] == NULL)
            {
                currNode = (Node *) currNode->ptrs[i].blockAddress;
                break;
            }

            // you will hit this if your target value is bigger than all the key values of a fully-filled node
            if (i == __n-1)
            {
                currNode = (Node *) currNode->ptrs[i+1].blockAddress;
                break;
            }
        }
    }

    int indexOfFirstRecordDeleted = -1;
    bool deletedFirstRecordOfLeafNode = false;
    int indexOfFirstRecordToShiftBackwards = -1;
    // hit leaf nodes and delete all records with numVotes in that leaf node
    if (currNode->isLeaf)
    {
        for (int i = 0; i < __n; i++)
        {
            if (currNode->keys[i] == numVotes)
            {
                if (indexOfFirstRecordDeleted == -1)
                {
                    indexOfFirstRecordDeleted = i;
                    if (i == 0)
                        deletedFirstRecordOfLeafNode = true;
                }
                    
                cout << "You found a movie of " << numVotes << " numVotes to remove!" << endl;
                Record * recordAddress = (Record *) currNode->ptrs[i].blockAddress;
                cout << "Movie Id: " << recordAddress->getMovieId() << endl;
                cout << "Average Rating: " << recordAddress->getAverageRating() << endl;
                cout << "Num votes: " << recordAddress->getNumVotes() << endl;
                cout << endl;

                currNode->keys[i] = NULL;
                currNode->ptrs[i].blockAddress = NULL;
            }
            else if (currNode->keys[i] > numVotes)
            {
                indexOfFirstRecordToShiftBackwards = i;
                break;
            }
        }
    }
    if (indexOfFirstRecordToShiftBackwards != -1)
    {
        // shift records backwards in affected node to maintain sorted order
        for (int i = indexOfFirstRecordToShiftBackwards; i < __n; i++)
        {
            currNode->keys[indexOfFirstRecordDeleted] = currNode->keys[i];
            currNode->ptrs[indexOfFirstRecordDeleted].blockAddress = currNode->ptrs[i].blockAddress;
            indexOfFirstRecordDeleted++;
            currNode->keys[i] = NULL;
            currNode->ptrs[i].blockAddress = NULL;
        }
    }


    if (getNumOfKeysInNode(currNode) >= computeMinNumOfKeysInLeafNode(__n))
    {
        // leaf node satisfies min num of keys
        if (deletedFirstRecordOfLeafNode)
        {
            // recursion 
            Node * affectedLeafNode = currNode;
            updateParentNodeAfterRecordDeletion(affectedLeafNode, currNode);
            return;
        }
    }
    else
    {
        // leaf node did not satisfy min num of keys
        // leaf node needs to either borrow from adjacent sibling node or merge with it
        
        // check if leaf node can borrow from adjacent sibling nodes
        bool borrowed = borrowedFromSiblingLeafNodeIfPossible(currNode);
        if (borrowed)
            return;

        // leaf node cannot borrow from adjacent sibling nodes
        // leaf node needs to merge with an adjacent sibling node
        bool merged = mergedWithSiblingLeafNodeIfPossible(currNode);
        if (merged)
            return;
    }
}

int BPTree::getN(){
    return __n;
}

int BPTree::getNumNodes(){
    return __numNodes;
}

int BPTree::getHeight(){
    return __height;
}

int BPTree::getMinNumOfKeysInLeafNode(){
    return __minNumOfKeysInLeafNode;
}

int BPTree::getMinNumOfChildrenInNonLeafNode(){
    return __minNumOfChildrenInNonLeafNode;
}

bool BPTree::isNodeFull(Node * currNode){
    for (int i = 0; i < __n; i++)
    {
        if (currNode->keys[i] == NULL)
            return false;
    }
    return true;
}

int BPTree::getNumOfKeysInNode(Node * currNode){
    int numOfKeys = 0;
    for (int i = 0; i < __n; i++)
    {
        if (currNode->keys[i] == NULL)
            break;
        numOfKeys++;
    }
    return numOfKeys;
}

int BPTree::getNumOfChildrenInNonLeafNode(Node * currNode){
    int numOfChildren = 0;
    for (int i = 0; i <= __n; i++)
    {
        if (currNode->ptrs[i].blockAddress == NULL)
            break;
        numOfChildren++;
    }
    return numOfChildren;
}

// find somewhere to insert to ensure keys are sorted
void BPTree::insertKeyAndAddrToNonFullLeafNode(Node * currNode, int key, void * addr){
    for (int i = 0; i < __n; i++)
    {
        if (currNode->keys[i] == NULL){
            currNode->keys[i] = key;
            currNode->ptrs[i].blockAddress = addr;
            break;
        }
        else if (key < currNode->keys[i])
        {
            int index_to_decrement_from;
            for (int j = 0; j < __n; j++)
            {
                if (currNode->keys[j] == NULL){
                    index_to_decrement_from = j;
                    break;
                }
            }

            // shift the remaining keys and ptrs forward to maintain sorted arrays
            for (int k = index_to_decrement_from; k > i; k--)
            {
                currNode->keys[k] = currNode->keys[k-1];
                currNode->ptrs[k].blockAddress = currNode->ptrs[k-1].blockAddress;
            }
            currNode->keys[i] = key;
            currNode->ptrs[i].blockAddress = addr;
            break;
        }
    }
}

// TODO: find out if this is a fluke
// find somewhere to insert to ensure keys are sorted
void BPTree::insertKeyAndAddrToNonFullParentNode(Node * parentNode, Node * newNode){
    int keyValueToInsert = newNode->keys[0];
    
    if (keyValueToInsert == 20)
        cout << "this is it!" << endl;

    for (int i = 0; i < __n; i++)
    {
        if (parentNode->keys[i] == NULL)
        {
            parentNode->keys[i] = keyValueToInsert;
            parentNode->ptrs[i+1].blockAddress = newNode;
            break;
        }
        else if (keyValueToInsert < parentNode->keys[i])
        {
            int index_to_decrement_from;
            for (int j = 0; j < __n; j++)
            {
                if (parentNode->keys[j] == NULL)
                {
                    index_to_decrement_from = j;
                    break;
                }
            }
            // the problem lies with this, not sure if it's a fluke??
            // shift the remaining keys and ptrs forward to maintain sorted arrays
            for (int k = index_to_decrement_from; k > i; k--)
            {
                parentNode->keys[k] = parentNode->keys[k-1];
                parentNode->ptrs[k+1].blockAddress = parentNode->ptrs[k].blockAddress;
                
                //parentNode->ptrs[k].blockAddress = parentNode->ptrs[k-1].blockAddress;
            }
            parentNode->keys[i] = keyValueToInsert;
            parentNode->ptrs[i+1].blockAddress = newNode;
            break;
        }
    }
}

// find index to insert
int BPTree::findIndexOfFullNodeToInsert(Node * currNode, int key){
    for (int i = 0; i < __n; i++)
    {
        if (key < currNode->keys[i])
            return i;
    }
    return __n;
}

void BPTree::updateParentNode(Node * currNode, Node * newNode){
    if (currNode->parentAddress.blockAddress == nullptr)
    {
        // create a new root node
        Node * newRootNode = new Node(__n, false);
        newRootNode->ptrs[0].blockAddress = currNode;
        newRootNode->ptrs[1].blockAddress = newNode;
        
        // get min value of right subtree
        Node * temp = newNode;
        while (!temp->isLeaf)
        {
            Node * placeholder = (Node *) temp->ptrs[0].blockAddress;
            temp = placeholder;
        }
        
        newRootNode->keys[0] = temp->keys[0];
        __root = newRootNode;
        
        currNode->parentAddress.blockAddress = newRootNode;
        newNode->parentAddress.blockAddress = newRootNode;

        __numNodes++;
        __height++;
        return;
    }
    else
    {   
        // check for null key in index node
        //      a. if have: got empty ptr to point to new child node
        //          a1. update the parent node's index
        //          a2. by checking if newNode->keys[0] < parentNode->keys[i], i = indexToInsert
        //          a3. shift the remaining keys and ptrs forward to maintain sorted order                            
        //      b. else: need to split up the index node
        //          b1. do the above approach of splitting node
        //          b2. reference it's parent to check if need split/update


        // check for null key in index node
        Node * parentNode = (Node *) currNode->parentAddress.blockAddress; 
        bool parentNodeIsFull = isNodeFull(parentNode);
        
        if (!parentNodeIsFull)
        {
            insertKeyAndAddrToNonFullParentNode(parentNode, newNode);

            // cout << "parentNode content after insertKeyAndAddrToNonFullParentNode" << endl; 
            // for (int i = 0; i < __n; i++)
            // {
            //     if (parentNode->keys[i] == NULL)
            //         break;
            //     cout << parentNode->keys[i] << " ";
            // }
            // for (int i = 0; i <= __n; i++)
            // {
            //     if (parentNode->ptrs[i].blockAddress == NULL)
            //     {
            //         cout << "break";
            //         break;
            //     }
            //     cout << parentNode->ptrs[i].blockAddress << " ";
            // }
            // cout << endl;
            return;
        }
        else
        {
            // parent node no space, need to split up

            // 1. find index to insert
            // 2. create a temp node to store all the keys and ptrs in sorted order
            // 3. create newNode
            // 4. Based on temp node, repopulate parentNode and newNode (identify the pivot index)
            // 5. create new root node if need be, else update above indexes recursively

            int keyValueToInsert = newNode->keys[0];

            // find index to insert
            int indexToInsert = findIndexOfFullNodeToInsert(parentNode, keyValueToInsert);

            // create a temp node to store all the keys and ptrs in sorted order
            Node * tempNode = new Node(__n+1, true);
            tempNode->keys[indexToInsert] = keyValueToInsert;
            tempNode->ptrs[indexToInsert+1].blockAddress = newNode;

            int parentNodeIndex = 0;
            // this is done to make life easier
            tempNode->ptrs[0].blockAddress = parentNode->ptrs[parentNodeIndex].blockAddress;



            // cout << "parentNode content before" << endl; 
            // for (int i = 0; i < __n; i++)
            // {
            //     if (parentNode->keys[i] == NULL)
            //         break;
            //     cout << parentNode->keys[i] << " ";
            // }
            // for (int i = 0; i <= __n; i++)
            // {
            //     if (parentNode->ptrs[i].blockAddress == NULL)
            //     {
            //         cout << "break";
            //         break;
            //     }
            //     cout << parentNode->ptrs[i].blockAddress << " ";
            // }
            // cout << endl;



            // iterate thru tempNode to populate
            for (int i = 0; i < __n+1; i++)
            {
                if (i == indexToInsert)
                    continue;
                tempNode->keys[i] = parentNode->keys[parentNodeIndex];
                tempNode->ptrs[i+1].blockAddress = parentNode->ptrs[parentNodeIndex+1].blockAddress;
                parentNodeIndex++;
            }


            // cout << "tempNode content after populated" << endl; 
            // for (int i = 0; i < __n+1; i++)
            // {
            //     if (tempNode->keys[i] == NULL)
            //         break;
            //     cout << tempNode->keys[i] << " ";
            // }
            // for (int i = 0; i <= __n; i++)
            // {
            //     if (tempNode->ptrs[i].blockAddress == NULL)
            //     {
            //         cout << "break";
            //         break;
            //     }
            //     cout << tempNode->ptrs[i].blockAddress << " ";
            // }
            // cout << tempNode->ptrs[__n+1].blockAddress << endl;

            // based on temp node,

            int pivotIndex = computeMinNumOfChildrenInNonLeafNodeCeil(__n) - 1;
            // repopulate parentNode
            // this is done to make life easier
            parentNode->ptrs[0].blockAddress = tempNode->ptrs[0].blockAddress;
            for (int i = 0; i < pivotIndex; i++)
            {
                parentNode->keys[i] = tempNode->keys[i];
                parentNode->ptrs[i+1].blockAddress = tempNode->ptrs[i+1].blockAddress;
            }
            for (int i = pivotIndex; i < __n; i++)
            {
                parentNode->keys[i] = NULL;
                parentNode->ptrs[i+1].blockAddress = nullptr;
            }

            // cout << "parentNode content after" << endl; 
            // for (int i = 0; i < __n; i++)
            // {
            //     if (parentNode->keys[i] == NULL)
            //         break;
            //     cout << parentNode->keys[i] << " ";
            // }
            // for (int i = 0; i <= __n; i++)
            // {
            //     if (parentNode->ptrs[i].blockAddress == NULL)
            //     {
            //         cout << "break";
            //         break;
            //     }
            //     cout << parentNode->ptrs[i].blockAddress << " ";
            // }
            // cout << endl;

            // create newNode
            Node * newNode = new Node(__n, false);
            __numNodes++;
            // populate newNode
            // this is done to make life easier
            newNode->ptrs[0].blockAddress = tempNode->ptrs[pivotIndex+1].blockAddress;

            

            // this is to update it's child of it's new parent due to creation of new parent
            Node * currChildNode = (Node *) newNode->ptrs[0].blockAddress;
            currChildNode->parentAddress.blockAddress = newNode;

            pivotIndex++;
            for (int i = 0; i < computeMinNumOfChildrenInNonLeafNode(__n) - 1; i++)
            {
                newNode->keys[i] = tempNode->keys[pivotIndex];
                newNode->ptrs[i+1].blockAddress = tempNode->ptrs[pivotIndex+1].blockAddress;
                
                currChildNode = (Node *) newNode->ptrs[i+1].blockAddress;
                currChildNode->parentAddress.blockAddress = newNode;

                pivotIndex++;
            }
            
            // cout << "newNode content" << endl; 
            // for (int i = 0; i < __n; i++)
            // {
            //     if (newNode->keys[i] == NULL)
            //         break;
            //     cout << newNode->keys[i] << " ";
            // }
            // for (int i = 0; i <= __n; i++)
            // {
            //     if (newNode->ptrs[i].blockAddress == NULL)
            //     {
            //         cout << "break";
            //         break;
            //     }
            //     cout << newNode->ptrs[i].blockAddress << " ";
            // }
            // cout << endl;


            delete tempNode;

            // do recursion!
            updateParentNode(parentNode, newNode);
        }
    }
}

void BPTree::updateParentNodeAfterRecordDeletion(Node * affectedLeafNode, Node * currNode){
    Node * parentNode = (Node *) currNode->parentAddress.blockAddress;
    // parent node's first child is the affected leaf node
    if (parentNode->ptrs[0].blockAddress == currNode)
    {
        // terminating condition
        if (parentNode == __root)
            return;

        // no need to update parent node
        // but need to check parent node's parent node to see if it is it's first child
        // if it isn't first child, update it

        // do recursion
        updateParentNodeAfterRecordDeletion(affectedLeafNode, parentNode);
    }
    else
    {
        // terminating condition
        // if the parent node is updated, there's no need to traverse upwards already
        for (int i = 1; i <= __n; i++)
        {
            if (parentNode->ptrs[i].blockAddress == currNode)
            {
                parentNode->keys[i-1] = affectedLeafNode->keys[0];
                break;
            }
        }
        return;
    }
}

// TODO: do recursion to check if currNode has sufficient children
// if not sufficient, 
// 1. see if can borrow from sibling
// 2. if not merge with sibling

// Need to look into greater detail for the borrowing and merging
void BPTree::updateParentNodeAfterLosingChild(Node * currNode){
    if (getNumOfChildrenInNonLeafNode(currNode) >= computeMinNumOfChildrenInNonLeafNode(__n))
    {
        // terminating condition
        return;
    }
    else
    {
        if (currNode == __root)
        {
            // delete root if it only has one child
            if (getNumOfChildrenInNonLeafNode(currNode) == 1)
            {
                Node * newRoot = (Node *) currNode->ptrs[0].blockAddress;
                __root = newRoot;
                __height--;
                __numNodes--;
                return;
            }
        }

        // currNode has insufficient children
        // check if can borrow from sibling
        // terminate if can borrow
        bool borrowed = borrowedFromSiblingNonLeafNodeIfPossible(currNode);
        if (borrowed)
            return;

        // TODO: mergedFromSiblingNonLeafNodeIfPossible
        // draw out a scenario in paper, easier to visualise
        // recur if merged
        Node * affectedParentNode = mergedWithSiblingNonLeafNodeIfPossible(currNode);
        if (affectedParentNode != NULL)
        {
            updateParentNodeAfterLosingChild(affectedParentNode);
            return;
        }
            
    }
}


bool BPTree::borrowedFromSiblingLeafNodeIfPossible(Node * currNode){
    Node * parentNode = (Node *) currNode->parentAddress.blockAddress;

    // get the index of parentNode's ptr pointing at curr leaf node
    int j = -1;
    for (int i = 0; i <= __n; i++)
    {
        if (parentNode->ptrs[i].blockAddress == currNode)
        {
            j = i;
            break;
        }
    }

    if (j == -1)
    {
        cout << "Error: parentNode does not point to currNode" << endl;
        return false;
    }

    int indexOfSiblingBefore = j - 1;
    int indexOfSiblingAfter = j + 1;
    
    // check if can borrow from sibling before
    if (indexOfSiblingBefore >= 0)
    {
        // currNode has a sibling before it
        // check if sibling can afford to lend it's key
        Node * siblingBefore = (Node *) parentNode->ptrs[indexOfSiblingBefore].blockAddress;
        if (getNumOfKeysInNode(siblingBefore) - 1 >= computeMinNumOfKeysInLeafNode(__n))
        {
            // borrow from the last key of siblingBefore
            int iOfBorrowedKeyAndAddr = getNumOfKeysInNode(siblingBefore) - 1;
            int borrowedKey = siblingBefore->keys[iOfBorrowedKeyAndAddr];
            void * borrowedAddr = siblingBefore->ptrs[iOfBorrowedKeyAndAddr].blockAddress;

            siblingBefore->keys[iOfBorrowedKeyAndAddr] = NULL;
            siblingBefore->ptrs[iOfBorrowedKeyAndAddr].blockAddress = NULL;

            // currNode borrows key/addr from sibling before
            insertKeyAndAddrToNonFullLeafNode(currNode, borrowedKey, borrowedAddr);
            
            // recursion is needed here since the first key is affected
            Node * affectedLeafNode = currNode;
            updateParentNodeAfterRecordDeletion(affectedLeafNode, currNode);
            return true;
        }
    }
    
    // check if can borrow from sibling after
    if (parentNode->ptrs[indexOfSiblingAfter].blockAddress != NULL)
    {
        // currNode has a sibling after it
        // check if sibling can afford to lend it's key
        Node * siblingAfter = (Node *) parentNode->ptrs[indexOfSiblingAfter].blockAddress;
        if (getNumOfKeysInNode(siblingAfter) - 1 >= computeMinNumOfKeysInLeafNode(__n))
        {
            int borrowedKey = siblingAfter->keys[0];
            void * borrowedAddr = siblingAfter->ptrs[0].blockAddress;

            siblingAfter->keys[0] = NULL;
            siblingAfter->ptrs[0].blockAddress = NULL;

            // shift the keys and addresses back for sibling after
            // shift records backwards to maintain sorted order
            int indexOfFirstRecordDeleted = 0;
            for (int i = 1; i < __n; i++)
            {
                siblingAfter->keys[indexOfFirstRecordDeleted] = siblingAfter->keys[i];
                siblingAfter->ptrs[indexOfFirstRecordDeleted].blockAddress = siblingAfter->ptrs[i].blockAddress;
                indexOfFirstRecordDeleted++;
                siblingAfter->keys[i] = NULL;
                siblingAfter->ptrs[i].blockAddress = NULL;
            }

            // currNode borrows key/addr from sibling after
            insertKeyAndAddrToNonFullLeafNode(currNode, borrowedKey, borrowedAddr);
            
            // recursion is needed here since the first key is affected 
            Node * affectedLeafNode = currNode;
            updateParentNodeAfterRecordDeletion(affectedLeafNode, currNode);

            // recursion is needed here since the first key is affected 
            affectedLeafNode = siblingAfter;
            currNode = siblingAfter;
            updateParentNodeAfterRecordDeletion(affectedLeafNode, currNode);
            return true;
        }
    }
    return false;
}

bool BPTree::mergedWithSiblingLeafNodeIfPossible(Node * currNode){
    // when nodes are merged, a node is deleted.

    Node * parentNode = (Node *) currNode->parentAddress.blockAddress;

    // get the index of parentNode's ptr pointing at curr leaf node
    int j = -1;
    for (int i = 0; i <= __n; i++)
    {
        if (parentNode->ptrs[i].blockAddress == currNode)
        {
            j = i;
            break;
        }
    }

    if (j == -1)
    {
        cout << "Error: parentNode does not point to currNode" << endl;
        return false;
    }

    int indexOfSiblingBefore = j - 1;
    int indexOfSiblingAfter = j + 1;

    if (indexOfSiblingBefore >= 0)
    {
        // currNode has a sibling before it
        // populate siblingBefore with currNode, deleting currNode in the process
        Node * siblingBefore = (Node *) parentNode->ptrs[indexOfSiblingBefore].blockAddress;
        for (int i = 0; i < __n; i++)
        {
            if (currNode->keys[i] == NULL)
            {
                break;
            } 
            insertKeyAndAddrToNonFullLeafNode(siblingBefore, currNode->keys[i], currNode->ptrs[i].blockAddress);
        }
        siblingBefore->ptrs[__n].blockAddress = currNode->ptrs[__n].blockAddress;
        delete currNode;
        __numNodes--;

        // update the parent of deleted node
        // done in the perspective of ptrs instead of keys
        // ptrs and keys are being shifted forward
        for (int i = j; i <= __n; i++)
        {
            if (parentNode->ptrs[i+1].blockAddress == NULL)
            {
                parentNode->ptrs[i].blockAddress = NULL;
                parentNode->keys[i-1] = NULL;
                break;
            }

            parentNode->ptrs[i].blockAddress = parentNode->ptrs[i+1].blockAddress;
            Node * child = (Node *) parentNode->ptrs[i].blockAddress;
            parentNode->keys[i-1] = child->keys[0];
        }

        // TODO: do recursion
        // parentNode loses a child, need to check if it has sufficient children
        // if not sufficient, 
        // 1. see if can borrow from sibling
        // 2. if not merge with sibling
        updateParentNodeAfterLosingChild(parentNode);

        // recursion is needed here in case the first key is affected
        // Node * affectedLeafNode = currNode;
        // updateParentNodeAfterRecordDeletion(affectedLeafNode, currNode);

        return true;
    }
    else
    {
        // populate currNode with siblingAfter, deleting siblingAfter in the process
        Node * siblingAfter = (Node *) parentNode->ptrs[indexOfSiblingAfter].blockAddress;
        for (int i = 0; i < __n; i++)
        {
            if (siblingAfter->keys[i] == NULL)
            {
                break;
            } 
            insertKeyAndAddrToNonFullLeafNode(currNode, siblingAfter->keys[i], siblingAfter->ptrs[i].blockAddress);
        }
        currNode->ptrs[__n].blockAddress = siblingAfter->ptrs[__n].blockAddress;
        delete siblingAfter;
        __numNodes--;

        // update the parent of deleted node
        // done in the perspective of ptrs instead of keys
        for (int i = indexOfSiblingAfter; i <= __n; i++)
        {
            if (parentNode->ptrs[i+1].blockAddress == NULL)
            {
                parentNode->ptrs[i].blockAddress = NULL;
                parentNode->keys[i-1] = NULL;
                break;
            }

            parentNode->ptrs[i].blockAddress = parentNode->ptrs[i+1].blockAddress;
            Node * child = (Node *) parentNode->ptrs[i].blockAddress;
            parentNode->keys[i-1] = child->keys[0];
        }

        // TODO: do recursion to update parent nodes
        updateParentNodeAfterLosingChild(parentNode);

        // might need to update parent node's key as well if it is min value of right subtree
        // recursion is needed here since the first key might be affected 
        // Node * affectedLeafNode = currNode;
        // updateParentNodeAfterRecordDeletion(affectedLeafNode, currNode);
        return true;
    }
    return false;
}

bool BPTree::borrowedFromSiblingNonLeafNodeIfPossible(Node * currNode){
    Node * parentNode = (Node *) currNode->parentAddress.blockAddress;

    // get the index of parentNode's ptr pointing at curr leaf node
    int j = -1;
    for (int i = 0; i <= __n; i++)
    {
        if (parentNode->ptrs[i].blockAddress == currNode)
        {
            j = i;
            break;
        }
    }

    if (j == -1)
    {
        cout << "Error: parentNode does not point to currNode" << endl;
        return false;
    }

    int indexOfSiblingBefore = j - 1;
    int indexOfSiblingAfter = j + 1;

    // check if can borrow from sibling before
    if (indexOfSiblingBefore >= 0)
    {
        // currNode has a sibling before it
        // check if sibling can afford to lend it's ptr
        Node * siblingBefore = (Node *) parentNode->ptrs[indexOfSiblingBefore].blockAddress;
        if (getNumOfChildrenInNonLeafNode(siblingBefore) - 1 >= computeMinNumOfChildrenInNonLeafNode(__n))
        {
            // borrow last ptr of siblingBefore
            int iOfBorrowedAddr = getNumOfChildrenInNonLeafNode(siblingBefore) - 1;
            void * borrowedAddr = siblingBefore->ptrs[iOfBorrowedAddr].blockAddress;

            siblingBefore->ptrs[iOfBorrowedAddr].blockAddress = NULL;
            siblingBefore->keys[iOfBorrowedAddr-1] = NULL;

            // currNode borrows addr from sibling before
            // shift ptrs forward
            int initialNumOfChildren = getNumOfChildrenInNonLeafNode(currNode); 
            for (int i = initialNumOfChildren; i > 0; i--)
            {
                currNode->ptrs[i].blockAddress = currNode->ptrs[i-1].blockAddress;
            }
            currNode->ptrs[0].blockAddress = borrowedAddr;
            
            // reparent the borrowed child
            Node * childNode = (Node *) currNode->ptrs[0].blockAddress;
            childNode->parentAddress.blockAddress = currNode;

            // populate their respective keys (TODO: this only works for 3 lvls and below)
            for (int i = 0; i < getNumOfChildrenInNonLeafNode(currNode) - 1; i++)
            {
                Node * childNode = (Node *) currNode->ptrs[i+1].blockAddress;
                if (childNode == NULL)
                    break;

                while (!childNode->isLeaf)
                {
                    Node * nextChildNode = (Node *) childNode->ptrs[0].blockAddress;
                    childNode = nextChildNode;
                }
                currNode->keys[i] = childNode->keys[0];
            }
            
            // update parent if need be
            childNode = (Node *) currNode->ptrs[0].blockAddress;
            while (!childNode->isLeaf)
            {
                Node * nextChildNode = (Node *) childNode->ptrs[0].blockAddress;
                childNode = nextChildNode;
            }
            updateParentNodeAfterRecordDeletion(childNode, childNode);

            return true;
        }
    }

    // check if can borrow from sibling after
    if (parentNode->ptrs[indexOfSiblingAfter].blockAddress != NULL)
    {
        // currNode has a sibling after it
        // check if sibling can afford to lend it's ptr
        Node * siblingAfter = (Node *) parentNode->ptrs[indexOfSiblingAfter].blockAddress;
        if (getNumOfChildrenInNonLeafNode(siblingAfter) - 1 >= computeMinNumOfChildrenInNonLeafNode(__n))
        {
            // borrow first ptr of siblingAfter;
            void * borrowedAddr = siblingAfter->ptrs[0].blockAddress;

            int i = getNumOfChildrenInNonLeafNode(currNode);
            currNode->ptrs[i].blockAddress = borrowedAddr;
            Node * childNode = (Node *) currNode->ptrs[i].blockAddress;
            
            // reparent the borrowed child
            childNode->parentAddress.blockAddress = currNode;
            
            // adding currNode's new key
            while (!childNode->isLeaf)
            {
                Node * nextChildNode = (Node *) childNode->ptrs[0].blockAddress;
                childNode = nextChildNode;
            }
            currNode->keys[i-1] = childNode->keys[0];

            // shift ptrs backward in siblingAfter
            for (int i = 0; i < getNumOfChildrenInNonLeafNode(siblingAfter); i++)
            {
                siblingAfter->ptrs[i].blockAddress = siblingAfter->ptrs[i+1].blockAddress;
            }
            siblingAfter->ptrs[getNumOfChildrenInNonLeafNode(siblingAfter)].blockAddress = NULL;

            // populate their respective keys
            for (int i = 0; i < getNumOfChildrenInNonLeafNode(siblingAfter) - 1; i++)
            {
                Node * childNode = (Node *) siblingAfter->ptrs[i+1].blockAddress;
                if (childNode == NULL)
                    break;

                while (!childNode->isLeaf)
                {
                    Node * nextChildNode = (Node *) childNode->ptrs[0].blockAddress;
                    childNode = nextChildNode;
                }
                siblingAfter->keys[i] = childNode->keys[0];
            }

            // update parent if need be
            childNode = (Node *) siblingAfter->ptrs[0].blockAddress;
            while (!childNode->isLeaf)
            {
                Node * nextChildNode = (Node *) childNode->ptrs[0].blockAddress;
                childNode = nextChildNode;
            }
            updateParentNodeAfterRecordDeletion(childNode, childNode);

            return true;
        }
    }
    return false;
}

// merges and returns affected parent node
Node * BPTree::mergedWithSiblingNonLeafNodeIfPossible(Node * currNode){
    Node * parentNode = (Node *) currNode->parentAddress.blockAddress;

    // get the index of parentNode's ptr pointing at curr leaf node
    int j = -1;
    for (int i = 0; i <= __n; i++)
    {
        if (parentNode->ptrs[i].blockAddress == currNode)
        {
            j = i;
            break;
        }
    }

    if (j == -1)
    {
        cout << "Error: parentNode does not point to currNode" << endl;
        return parentNode;
    }

    int indexOfSiblingBefore = j - 1;
    int indexOfSiblingAfter = j + 1;

    if (indexOfSiblingBefore >= 0)
    {
        // currNode has a sibling before it
        // populate siblingBefore with currNode's children, deleting currNode in the process
        Node * siblingBefore = (Node *) parentNode->ptrs[indexOfSiblingBefore].blockAddress;

        int iOfSiblingBeforeToPopulate = getNumOfChildrenInNonLeafNode(siblingBefore);
        for (int i = 0; i < getNumOfChildrenInNonLeafNode(currNode); i++)
        {
            if (currNode->ptrs[i].blockAddress == NULL)
                break;
            siblingBefore->ptrs[iOfSiblingBeforeToPopulate].blockAddress = currNode->ptrs[i].blockAddress;
            
            // reparent the new child
            Node * childNode = (Node *) siblingBefore->ptrs[iOfSiblingBeforeToPopulate].blockAddress;
            childNode->parentAddress.blockAddress = siblingBefore;
            
            iOfSiblingBeforeToPopulate++;
        }

        // populate their respective keys
        // this implementation is needed for it work with n levels
        for (int i = 0; i < getNumOfChildrenInNonLeafNode(siblingBefore) - 1; i++)
        {
            Node * childNode = (Node *) siblingBefore->ptrs[i+1].blockAddress;
            if (childNode == NULL)
                break;
            
            while (!childNode->isLeaf)
            {
                Node * nextChildNode = (Node *) childNode->ptrs[0].blockAddress;
                childNode = nextChildNode;
            }
            siblingBefore->keys[i] = childNode->keys[0];
        }
        delete currNode;
        __numNodes--;

        // update the parent of deleted node
        // done in the perspective of ptrs instead of keys
        // ptrs and keys are being shifted forward
        for (int i = j; i <= __n; i++)
        {
            if (parentNode->ptrs[i+1].blockAddress == NULL)
            {
                parentNode->ptrs[i].blockAddress = NULL;
                parentNode->keys[i-1] = NULL;
                break;
            }

            parentNode->ptrs[i].blockAddress = parentNode->ptrs[i+1].blockAddress;
            Node * childNode = (Node *) parentNode->ptrs[i].blockAddress;

            while (!childNode->isLeaf)
            {
                Node * nextChildNode = (Node *) childNode->ptrs[0].blockAddress;
                childNode = nextChildNode;
            }
            parentNode->keys[i-1] = childNode->keys[0];
        }
    }
    else
    {
        // populate currNode with siblingAfter's children, deleting siblingAfter in the process
        Node * siblingAfter = (Node *) parentNode->ptrs[indexOfSiblingAfter].blockAddress;
        
        int iOfCurrNodeToPopulate = getNumOfChildrenInNonLeafNode(currNode);
        for (int i = 0; getNumOfChildrenInNonLeafNode(siblingAfter); i++)
        {
            if (siblingAfter->ptrs[i].blockAddress == NULL)
                break;
            currNode->ptrs[iOfCurrNodeToPopulate].blockAddress = siblingAfter->ptrs[i].blockAddress;

            // reparent the new child
            Node * childNode = (Node *) currNode->ptrs[iOfCurrNodeToPopulate].blockAddress;
            childNode->parentAddress.blockAddress = currNode;

            iOfCurrNodeToPopulate++;
        }

        // populate their respective keys
        // this implementation is needed for it work with n levels
        for (int i = 0; i < getNumOfChildrenInNonLeafNode(currNode) - 1; i++)
        {
            Node * childNode = (Node *) currNode->ptrs[i+1].blockAddress;
            if (childNode == NULL)
                break;
            
            while (!childNode->isLeaf)
            {
                Node * nextChildNode = (Node *) childNode->ptrs[0].blockAddress;
                childNode = nextChildNode;
            }
            currNode->keys[i] = childNode->keys[0];
        }
        delete siblingAfter;
        __numNodes--;
        
        // update the parent of deleted node
        // done in the perspective of ptrs instead of keys
        // ptrs and keys are being shifted forward
        for (int i = indexOfSiblingAfter; i <= __n; i++)
        {
            if (parentNode->ptrs[i+1].blockAddress == NULL)
            {
                parentNode->ptrs[i].blockAddress = NULL;
                parentNode->keys[i-1] = NULL;
                break;
            }

            parentNode->ptrs[i].blockAddress = parentNode->ptrs[i+1].blockAddress;
            Node * childNode = (Node *) parentNode->ptrs[i].blockAddress;

            while (!childNode->isLeaf)
            {
                Node * nextChildNode = (Node *) childNode->ptrs[0].blockAddress;
                childNode = nextChildNode;
            }
            parentNode->keys[i-1] = childNode->keys[0];
        }
    }
    return parentNode;
}

void BPTree::printBpTreeInfoSummary(){
    cout << "Parameter n: " << __n << endl;
    cout << "Num of nodes in B+ tree: " << __numNodes << endl;
    cout << "Height of B+ tree: " << __height << endl;
    
    cout << "Content of root node" << endl;
    for (int i = 0; i < __n; i++)
    {
        if (__root->keys[i] == NULL) break;
        cout << __root->keys[i] << " ";
    }
    cout << endl;

    Node * firstChild = (Node *) __root->ptrs[0].blockAddress;
    cout << "Content of first child node" << endl;
    for (int i = 0; i < __n; i++)
    {
        if (firstChild->keys[i] == NULL) break;
        cout << firstChild->keys[i] << " ";
    }
    cout << endl;
}

void BPTree::printBpTreeInfo(){
        // This is for testing purposes
    cout << "Num of nodes in B+ tree: " << __numNodes << endl;
    cout << "Height of B+ tree: " << __height << endl;
    cout << "Content of root node" << endl;
    for (int i = 0; i < __n; i++)
    {
        if (__root->keys[i] == NULL) break;
        cout << __root->keys[i] << " ";
    }
    cout << endl;
    
    // intial case of root node being leaf node
    if (__root->isLeaf)
    {
        cout << endl;
        return;
    }


    Node * currNode = (Node *) __root->ptrs[0].blockAddress;
    while (!currNode->isLeaf)
    {
        cout << "Content of child index node" << endl;
        for (int i = 0; i < __n; i++)
        {
            if (currNode->keys[i] == NULL)
                break;
            cout << currNode->keys[i] << " ";
        }
        Node * temp = (Node *) currNode->ptrs[0].blockAddress;
        currNode = temp;
        cout << endl;
    }

    // hit leaf nodes
    int childNum = 0;
    while (true)
    {
        childNum++;
        cout << "Content of child node " << childNum << endl;
        for (int i = 0; i < __n; i++)
        {
            if (currNode->keys[i] == NULL)
                break;
            cout << currNode->keys[i] << " ";
        }
        if (currNode->ptrs[__n].blockAddress == nullptr)
        {
            cout << endl;
            break;
        }
        Node * temp = (Node *) currNode->ptrs[__n].blockAddress;
        currNode = temp;
        cout << endl;
    }
    cout << endl;
}
