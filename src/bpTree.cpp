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
                if (currNode->parentAddress.blockAddress != nullptr)
                    newNode->parentAddress.blockAddress = currNode->parentAddress.blockAddress;
                // TODO: notify parent node of new child


                // TODO: this is better implemented via recursion
                // you need to recur if there is a split in node
                
                if (numVotes == 7)
                {
                    cout << "hooray";
                }
                
                updateParentNode(currNode, newNode);
            } 
            else
                insertKeyAndAddrToNonFullLeafNode(currNode, numVotes, recordAddress);
        }
    }


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

// void BPTree::delete(int numVotes){
    
// }

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

    for (int i = 0; i< __n; i++)
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

            cout << "parentNode content after insertKeyAndAddrToNonFullParentNode" << endl; 
            for (int i = 0; i < __n; i++)
            {
                if (parentNode->keys[i] == NULL)
                    break;
                cout << parentNode->keys[i] << " ";
            }
            for (int i = 0; i <= __n; i++)
            {
                if (parentNode->ptrs[i].blockAddress == NULL)
                {
                    cout << "break";
                    break;
                }
                cout << parentNode->ptrs[i].blockAddress << " ";
            }
            cout << endl;
            return;
        }
        else
        {
            // parent node no space, need to split up

            // 1. find index to insert
            // 2. create a temp node to store all the keys and ptrs in sorted order
            // 3. create newNode
            // 4. Based on temp node, repopulate currNode and newNode (identify the pivot index)
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



            cout << "parentNode content before" << endl; 
            for (int i = 0; i < __n; i++)
            {
                if (parentNode->keys[i] == NULL)
                    break;
                cout << parentNode->keys[i] << " ";
            }
            for (int i = 0; i <= __n; i++)
            {
                if (parentNode->ptrs[i].blockAddress == NULL)
                {
                    cout << "break";
                    break;
                }
                cout << parentNode->ptrs[i].blockAddress << " ";
            }
            cout << endl;



            // iterate thru tempNode to populate
            for (int i = 0; i < __n+1; i++)
            {
                if (i == indexToInsert)
                    continue;
                tempNode->keys[i] = parentNode->keys[parentNodeIndex];
                tempNode->ptrs[i+1].blockAddress = parentNode->ptrs[parentNodeIndex+1].blockAddress;
                parentNodeIndex++;
            }


            cout << "tempNode content after populated" << endl; 
            for (int i = 0; i < __n+1; i++)
            {
                if (tempNode->keys[i] == NULL)
                    break;
                cout << tempNode->keys[i] << " ";
            }
            for (int i = 0; i <= __n; i++)
            {
                if (tempNode->ptrs[i].blockAddress == NULL)
                {
                    cout << "break";
                    break;
                }
                cout << tempNode->ptrs[i].blockAddress << " ";
            }
            cout << tempNode->ptrs[__n+1].blockAddress << endl;

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

            cout << "parentNode content after" << endl; 
            for (int i = 0; i < __n; i++)
            {
                if (parentNode->keys[i] == NULL)
                    break;
                cout << parentNode->keys[i] << " ";
            }
            for (int i = 0; i <= __n; i++)
            {
                if (parentNode->ptrs[i].blockAddress == NULL)
                {
                    cout << "break";
                    break;
                }
                cout << parentNode->ptrs[i].blockAddress << " ";
            }
            cout << endl;

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
            
            cout << "newNode content" << endl; 
            for (int i = 0; i < __n; i++)
            {
                if (newNode->keys[i] == NULL)
                    break;
                cout << newNode->keys[i] << " ";
            }
            for (int i = 0; i <= __n; i++)
            {
                if (newNode->ptrs[i].blockAddress == NULL)
                {
                    cout << "break";
                    break;
                }
                cout << newNode->ptrs[i].blockAddress << " ";
            }
            cout << endl;


            delete tempNode;

            // do recursion!
            updateParentNode(parentNode, newNode);
        }
    }
}