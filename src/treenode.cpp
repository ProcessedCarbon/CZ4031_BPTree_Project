#include <iostream>
#include <tuple> 
#include "treenode.h"

using namespace std;

/**
 * @brief Attempt to find key in current node
 * returns the last key if key cannot be found in node
 * 
 * @param numVotes 
 * @return int 
 */
int Node::FindKey(int numVotes)
{
    int pos = 0;
    while (pos < __size && key[pos] < numVotes)
    {
        pos++;
    }
    return pos;
}

// int Node::getLeftChild(int pos)
// {
//     Node *curr = ptr[pos];
//     while (curr->__isLeaf == false)
//     {
//         curr = curr->ptr[curr->__size];
//     }
//     return curr->key[curr->__size - 1];
// }

/**
 * @brief Returns the key of the right side child of pos
 * 
 * @param pos 
 * @return int 
 */

int Node::getRightChildKey(int pos)
{
    Node *curr = (Node *)ptr[pos + 1];
    while (curr->__isLeaf == false)
    {
        curr = (Node *)curr->ptr[0];
    }
    return curr->key[1];
}

/**
 * @brief Start deletion process attempt to find key in nodes 
 * if key cannot be found in current node, traverse the tree to find
 * till either it cannot be found or it is found
 * 
 * @param numVotes 
 */
void Node::Deletion(int numVotes)
{
    // << "[DELETION....]" << endl;
    int pos = FindKey(numVotes);

    if (pos < __size && key[pos] == numVotes)
    {
        if (__isLeaf == true)
        {
            RemoveFromLeafNode(pos);
        }
        else
        {
            RemoveFromInternalNode(pos, numVotes);
        }
    }
    else
    {
        if (__isLeaf == true)
        {
            cout << "[MISSING KEY]: " << numVotes << "\n";
            return;
        }
        bool flag = ((pos == __size) ? true : false);

        if (((Node *)ptr[pos])->__size < __minkeys)
            Fill(pos);

        if (flag == true && pos > __size)
        {
            ((Node *)ptr[pos - 1])->Deletion(numVotes);
        }
        else
        {
            ((Node *)ptr[pos])->Deletion(numVotes);
        }
    }
}

/**
 * @brief Removes the targetted key using its pos in the node.
 * Executed when current node is a leaf.
 * After removal of leaf key, checks if the node needs to be Filled.
 * 
 * @param keypos 
 */
void Node::RemoveFromLeafNode(int keypos)
{
    //cout << "[REMOVING FROM LEAF KEY POS]: " << keypos << endl;
    //printNodeContent(parent);
    int k = key[keypos];
    for (int i = keypos + 1; i < __size; i++)
    {
        key[i - 1] = key[i];
    }
    __size--;   

    // cout << "Leaf Node after removal: ";
    // printNodeContent(this);

    // cout << "Parent node currently: ";
    // printNodeContent(parent);

    if(parent == NULL) return;

    int i = 0;
    int fillIndex = -1;
    //cout <<"Searching for leaf key in parent" <<endl;
    if(__size < __minkeys){
        while(i <= parent->__size){
            fillIndex = GetKeyLocationInParent(parent, i, k);
            if(fillIndex != -1)
                break;
            i++;
        }

       //cout <<"Checking if key is found" <<endl;
        if(fillIndex == - 1){
            //cout << "No common key between leaf and parent, compare keys" <<endl;
            fillIndex = (parent->key[keypos] > ((Node *)parent->ptr[keypos])->key[__size - 1]) ? keypos + 1: keypos;
        } 
        // Case should not happen at all
        if(fillIndex == - 1) return;

        parent->Fill(fillIndex);
    }
}

/**
 * @brief Gets ptr of parent of key provided
 * 
 * @param parent 
 * @param parentIndex 
 * @param key 
 * @return int 
 */
int Node::GetKeyLocationInParent(Node *parent, int parentIndex,int key){
    for(int i = 0; i < ((Node *)parent->ptr[parentIndex])->__size + 1; i++){
        if(((Node *)parent->ptr[parentIndex])->key[i] == key){
            return parentIndex;
        }
        else if(((Node *)parent->ptr[parentIndex + 1])->key[i] == key){
            return parentIndex + 1;
        }
    }
    return -1;
}

/**
 * @brief Removal of key from an internal node.
 * Will perform a call to Deletion to the corresponding child node
 * with its key as well.
 * 
 * Always replace key deleted with its right child key
 * 
 * @param keypos 
 * @param numVotes 
 */
void Node::RemoveFromInternalNode(int keypos, int numVotes)
{
    //cout << "[REMOVING FROM INTERNAL KEY POS]: " << keypos << endl;
    int succ = getRightChildKey(keypos);

   ((Node *)ptr[keypos + 1])->Deletion(numVotes);

    key[keypos] = succ;

    // cout << "Internal Node after removal: ";
    // printNodeContent(this);
}

/**
 * @brief Fills a node if it does not meet its minimum key requirement.
 * Check is adjacent nodes to see of they have enough keys to borrow first,
 * if not then merge itself to one of them.
 * 
 * @param keypos 
 */
void Node::Fill(int keypos)
{
    // cout << "FILLING FOR POS: " << keypos << endl;
    if (keypos != 0 && ((Node *)ptr[keypos - 1])->__size > ((Node *)ptr[keypos - 1])->__minkeys)
    {
        //cout << "[FILLING - BORROW FROM PREV]" << endl;
        BorrowPrev(keypos);
    }
    else if (keypos != __size && ((Node *)ptr[keypos + 1])->__size > ((Node *)ptr[keypos + 1])->__minkeys)
    {
        //cout << "[FILLING - BORROW FROM NEXT]" << endl;
        BorrowNext(keypos);
    }
    else
    {
        if (keypos != __size)
        {
            //cout << "[FILLING - MERGE NON LAST KEY]" << endl;
            Merge(keypos);
        }
        else
        {
            //cout << "[FILLING - MERGE LAST KEY]" << endl;
            Merge(keypos - 1);
        }
    }
}

/**
 * @brief Merge 2 nodes together given a keypos in parent
 * Method is executed when node == parent.
 * Merges the 2 children of that key
 * 
 * @param keypos 
 */
void Node::Merge(int keypos)
{
    //cout << "[MERGE - KEYPOS]: "<< keypos << endl;
    //if(parent != NULL){ printNodeContent(parent);}
    Node *child = (Node *)ptr[keypos];
    Node *sibling = (Node *)ptr[keypos + 1];

    if (key[keypos] > child->key[0])
        key[keypos] = child->key[0];

    for (int i = 0; i < sibling->__size; i++)
    {
        child->key[child->__size + i] = sibling->key[i];
    }

    if (child->__isLeaf == false)
    {
        for (int i = 0; i <= sibling->__size; i++)
        {
            if(child->ptr[child->__size + i] != NULL)
                continue;

            child->ptr[child->__size + i] = sibling->ptr[i];
        }
    }
    // cout << "Child: "; 
    // printNodeContent(ptr[keypos]);

    // cout << "Sibling: "; 
    // printNodeContent(ptr[keypos+1]);

    child->__size += sibling->__size;
    sibling->__size = 0;

    if (__size > __minkeys)
    {
        //cout << "[MERGE - SIZE > MINKEYS]" << endl;
        for (int i = keypos + 1; i < __size; i++)
        {
            key[i - 1] = key[i];
        }
        for (int i = keypos + 2; i <= __size; i++)
        {            
            ptr[i - 1] = ptr[i];
        }
        __size--;
    }
    else{
        delete(ptr[keypos + 1]);
        ptr[keypos + 1]= NULL;
    }
    int i = 0;
    while(ptr[i] != NULL) i++;

    if(parent == NULL || i >= __minChildren) return;

    //cout << "[PERFORM SELF FILL]" << endl;
    parent->Fill(keypos);
}

/**
 * @brief Borrows key from node next to it.
 * Only executes in a leaf node;
 * @param keypos 
 */
void Node::BorrowNext(int keypos)
{
    Node *child = (Node *)ptr[keypos];
    Node *sibling = (Node *)ptr[keypos + 1];

    child->key[child->__size] = key[keypos];

    if (child->__isLeaf == true)
    {
        child->ptr[child->__size + 1] = sibling->ptr[0];
    }
    key[keypos] = sibling->key[0];

    for (int i = 1; i < sibling->__size; i++)
    {
        sibling->key[i - 1] = sibling->key[i];
    }

    if (sibling->__isLeaf == false)
    {
        for (int i = 1; i <= sibling->__size; i++)
        {
            sibling->ptr[i - 1] = sibling->ptr[i];
        }
    }
    child->__size++;
    sibling->__size--;
}

/**
 * @brief Borrows key from node prior to it.
 * Only executes in a leaf node.
 * @param keypos 
 */
void Node::BorrowPrev(int keypos)
{
    Node *child = (Node *)ptr[keypos];
    Node *sibling = (Node *)ptr[keypos - 1];

    for (int i = child->__size - 1; i >= 0; i--)
    {
        child->key[i + 1] = child->key[i];
    }

    if (child->__isLeaf == false)
    {
        for (int i = child->__size; i >= 0; i--)
        {
            child->ptr[i + 1] = child->ptr[i];
        }
    }

    child->key[0] = key[keypos - 1];

    if (child->__isLeaf == false)
    {
        child->ptr[0] = sibling->ptr[sibling->__size];
    }

    key[keypos - 1] = sibling->key[sibling->__size - 1];

    child->__size++;
    sibling->__size--;
}


/**
 * @brief Print the keys of a given node
 * 
 * @param node 
 */
void Node::printNodeContent(Node *node){
    if(node == NULL) return;
    cout << " [ ";
    for(int i = 0; i < node->__size; i++){
        cout << node->key[i]  << " ";
    }
    cout << "]";
    cout << "\n";
}