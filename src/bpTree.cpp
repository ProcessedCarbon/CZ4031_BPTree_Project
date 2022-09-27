#include <iostream>

#include "bpTree.h"

using namespace std;

/**
 * @brief Inserts a node in the tree
 * Case 1: If tree is empty, insert node as root
 * Case 2: If tree is not empty, check if there are any available leaf nodes with enough __size to insert to.
 *         Case 2.1: If there are insert to that node
 *         Case 2.2: If there is not, create a new leaf node
 * Case 3: No leaf nodes, create internal node
 * @param numVotes
 */
void BPTree::Insert(int numVotes, void *recordptr)
{
    cout << "[INSERTING]: " << numVotes << endl;
    // If empty tree insert as root
    if (root == NULL)
    {
        root = new Node(MAX, true);
        root->key[0] = numVotes;
        root->ptr[0] = recordptr;
        
        root->parent = NULL;
        root->__size = 1;
    }
    else
    {
        // Traverse tree and try to find an empty leaf node to insert into.
        Node *curr = (Node *)root;
        Node *parent;
        while (curr->__isLeaf == false)
        {
            parent = curr;
            for (int i = 0; i < curr->__size; i++)
            {
                if (numVotes < curr->key[i])
                {
                    curr = (Node *)curr->ptr[i];
                    break;
                }
                if (i == curr->__size - 1)
                {
                    curr = (Node *)curr->ptr[i + 1];
                    break;
                }
            }
        }
        // Stop at potential node and check if its __size exceeds the maximum
        // If it does, insert into the correct position
        if (curr->__size < MAX)
        {
            cout << "1" << endl;
            int i = 0;
            while (numVotes > curr->key[i] && i < curr->__size)
            {
                i++;
            }
            cout << "2" << endl;
            // Duplicate found in leaf
            if(curr->key[i] == numVotes){
                cout << "2.1.0" << endl;
                if(typeid(curr->ptr[i]) != typeid(vector<void *>)){
                    vector<void *> v = *reinterpret_cast<vector<void *>*>(curr->ptr[i]);
                    v.push_back(curr->ptr[i]);
                    v.push_back(recordptr);
                }
                cout << "2.1.1" << endl;
            }
            else{
                cout << "2.2.0" << endl;
                for (int j = curr->__size; j > i; j--)
                {
                    curr->key[j] = curr->key[j - 1];
                }
                cout << "Not duplicate" << endl;
                curr->key[i] = numVotes;
                curr->__size++;
                curr->ptr[curr->__size] = curr->ptr[curr->__size - 1];
                curr->ptr[curr->__size - 1] = NULL;
                cout << "2.2.1" << endl;
            }
        }
        // If node has no space left
        // Create new leaf node and split the current node
        else
        {
            Node *newLeafNode = new Node(MAX, true);
            int tempKey[MAX + 1];

            for (int i = 0; i < MAX; i++)
            {
                tempKey[i] = curr->key[i];
            }

            int i = 0;
            while (numVotes > tempKey[i] && i < MAX)
            {
                i++;
            }

            for (int j = MAX + 1; j > i; j--)
            {
                tempKey[j] = tempKey[j - 1];
            }

            tempKey[i] = numVotes;

            curr->__size = (MAX + 1) / 2;
            newLeafNode->__size = MAX + 1 - (MAX + 1) / 2;
            curr->ptr[curr->__size] = newLeafNode;
            newLeafNode->ptr[newLeafNode->__size] = curr->ptr[MAX];

            curr->ptr[MAX] = NULL;

            for (i = 0; i < curr->__size; i++)
            {
                curr->key[i] = tempKey[i];
            }
            for (int i = 0, j = curr->__size; i < newLeafNode->__size; i++, j++)
            {
                newLeafNode->key[i] = tempKey[j];
            }
            // Handle if the node you need to split is root
            if (curr == root)
            {
                Node *newRoot = new Node(MAX, false);

                newRoot->key[0] = newLeafNode->key[0];

                newRoot->ptr[0] = curr;
                newRoot->ptr[1] = newLeafNode;

                newRoot->__size = 1;
                root = newRoot;
            }
            // If root has ran out of space to hold more keys, need to create an internal node.
            else
            {
                InsertInternal(newLeafNode->key[0], parent, newLeafNode);
            }
        }
    }
    cout << "[INSERTING SUCCESS]"<< endl;
}
/**
 * @brief Insert internal node in tree
 * Case 1: There is space in current node -> Just insert in current node
 * Case 2: No space -> Create internal node
 * @param numVotes
 * @param curr
 * @param child
 */
void BPTree::InsertInternal(int numVotes, Node *curr, Node *child)
{
    //cout << "[INSERTING INTERNAL]: " << numVotes << endl;
    if (curr->__size < MAX)
    {
        int i = 0;
        while (numVotes > curr->key[i] && i < curr->__size)
        {
            i++;
        }
        for (int j = curr->__size; j > i; j--)
        {
            curr->key[j] = curr->key[j - 1];
        }
        for (int j = curr->__size + 1; j > i + 1; j--)
        {
            curr->ptr[j] = curr->ptr[j - 1];
        }

        curr->key[i] = numVotes;

        curr->__size++;
        curr->ptr[i + 1] = child;
    }
    else
    {
        Node *newInternalNode = new Node(MAX, false);
        Node *tempPtr[MAX + 2];

        int tempKey[MAX + 1];

        for (int i = 0; i < MAX; i++)
        {
            tempKey[i] = curr->key[i];
        }
        for (int i = 0; i < MAX + 1; i++)
        {
            tempPtr[i] = (Node *)curr->ptr[i];
        }
        int i = 0;
        while (numVotes > tempKey[i] && i < MAX)
        {
            i++;
        }
        for (int j = MAX + 1; j > i; j--)
        {
            tempKey[j] = tempKey[j - 1];
        }
        tempKey[i] = numVotes;
        for (int j = MAX + 2; j > i + 1; j--)
        {
            tempPtr[j] = tempPtr[j - 1];
        }
        tempPtr[i + 1] = child;
        curr->__size = (MAX + 1) / 2;
        /**
         * @brief New internal node should have at least this amount since
         * the node is split from has at least the minimum amount
         */
        newInternalNode->__size = MAX - (MAX + 1) / 2;
        for (int i = 0, j = curr->__size + 1; i < newInternalNode->__size; i++, j++)
        {
            newInternalNode->key[i] = tempKey[j];
        }
        for (int i = 0, j = curr->__size + 1; i < newInternalNode->__size + 1; i++, j++)
        {
            newInternalNode->ptr[i] = tempPtr[j];
        }
        if (curr == root)
        {
            Node *newRoot = new Node(MAX, false);
            //newRoot->key[0] = curr->key[curr->__size];
            newRoot->key[0] = curr->key[curr->__size];

            newRoot->ptr[0] = curr;
            newRoot->ptr[1] = newInternalNode;
            //newRoot->dataptr[0] = NULL;
            //newRoot->dataptr[1] = NULL;

            newRoot->__size = 1;
            root = newRoot;
        }
        else
        {
            InsertInternal(curr->key[curr->__size], FindParent(root, curr), newInternalNode);
        }
    }
    //cout << "[INSERTING INTERNAL SUCCESS]" << endl;
}
int BPTree::computeNumNodesInTree()
{
    int counter = 0;
    stack<Node*> traversal_stack;
    traversal_stack.push(root);

    while (!traversal_stack.empty()) {
        Node *top = traversal_stack.top();
        traversal_stack.pop();

        counter++;
        for(int i = 0; i < top->__size + 1; i++){
            if(top->ptr[i] != NULL && top->__isLeaf != true){
                traversal_stack.push((Node *)top->ptr[i]);
            }
        }
    }
    return counter;
}
int BPTree::getNumNodes(){
    return computeNumNodesInTree();
}
/**
 * @brief Assign the parents of all nodes in the tree
 * 
 * @param curr 
 */
void BPTree::AssignAllParents(Node *curr)
{
    //cout << "[ASSIGNING PARENT]" <<endl;
    if (curr != NULL)
    {
        //cout << "[1]" <<endl;
        for (int i = 0; i < curr->__size; i++)
        {
            //cout << "[1.1]" <<endl;
            curr->parent = getParent(curr, curr->key[i]);
            //cout << "[1.2]" <<endl;
        }
        //cout << "[2]" <<endl;
        if (curr->__isLeaf != true)
        {
            for (int i = 0; i < curr->__size + 1; i++)
            {
                AssignAllParents((Node *)curr->ptr[i]);
            }
        }
        //cout << "[3]" <<endl;
    }
    //cout << "[SUCCESSFUL ASSIGNMENT OF PARENT NODES]" <<endl;
}

/**
 * @brief Get parent of current node
 * 
 * @param child 
 * @param numVotes 
 * @return Node* 
 */
Node *BPTree::getParent(Node *child, int numVotes)
{
    //cout << "[RETRIEVING PARENT]: " << numVotes << endl;

    if (root == NULL || child == root)
    {
        //cout << "NO PARENT / EMPTY TREE" << endl;
        return NULL;
    }

    Node *curr = root;
    Node *parent;
    while (curr != child)
    {
        if(curr->__isLeaf){
            return NULL;
        }
        parent = curr;
        for (int i = 0; i < curr->__size; i++)
        {
            if (numVotes < curr->key[i])
            {
                curr = (Node *)curr->ptr[i];
                break;
            }
            if (i == curr->__size - 1)
            {
                curr = (Node *)curr->ptr[i + 1];
                break;
            }
        }
    }
    //cout << "[PARENT GOTTEN]" << endl;
    return parent;
}

Node *BPTree::FindParent(Node *curr, Node *child)
{
    Node *parent;
    if (curr->__isLeaf == true|| ((Node *)curr->ptr[0])->__isLeaf == true)
    {
        //cout << "FIND PARENT CURR IS LEAF" << endl;
        return NULL;
    }
    for (int i = 0; i < curr->__size + 1; i++)
    {
        if (curr->ptr[i] == child)
        {
            parent = curr;
            return parent;
        }
        else
        {
            parent = FindParent((Node *)curr->ptr[i], child);
            if (parent != NULL){
                return parent;
            }
        }
    }
    return parent;
}

/**
 * @brief Display entire tree 
 * 
 * @param curr 
 */
void BPTree::DisplayTree(Node *curr)
{
    if (curr != NULL)
    {
        curr->printNodeContent(curr);
        cout << "\n";
        if (curr->__isLeaf != true)
        {
            for (int i = 0; i < curr->__size + 1; i++)
            {
                DisplayTree((Node *)curr->ptr[i]);
            }
        }
    }
}

/**
 * @brief Search for a specific key in node
 * 
 * @param numVotes 
 * @return Node* 
 */
void *BPTree::Search(int numVotes)
{
    if (root == NULL)
    {
        cout << "Tree is empty\n";
    }
    else
    {
        Node *curr = root;
        while (curr->__isLeaf == false)
        {
            for (int i = 0; i < curr->__size; i++)
            {
                if (numVotes < curr->key[i])
                {
                    curr = (Node *)curr->ptr[i];
                    break;
                }
                if (i == curr->__size - 1)
                {
                    curr = (Node *)curr->ptr[i + 1];
                    break;
                }
            }
        }
        for (int i = 0; i < curr->__size; i++)
        {
            if (curr->key[i] == numVotes)
            {
                cout << "Found\n";
                return (void *)curr;
            }
        }
    }
    cout << "Not found\n";
    return NULL;
}
int BPTree::SearchKey(int numVotes){
    if (root == NULL)
    {
        cout << "Tree is empty\n";
    }
    else
    {
        Node *curr = root;
        while (curr->__isLeaf == false)
        {
            for (int i = 0; i < curr->__size; i++)
            {
                if (numVotes < curr->key[i])
                {
                    curr = (Node *)curr->ptr[i];
                    break;
                }
                if (i == curr->__size - 1)
                {
                    curr = (Node *)curr->ptr[i + 1];
                    break;
                }
            }
        }
        for (int i = 0; i < curr->__size; i++)
        {
            if (curr->key[i] == numVotes)
            {
                cout << "Found\n";
                //return curr;
            }
        }
    }
}
/**
 * @brief Return the height of the tree
 * 
 * @return int 
 */
int BPTree::getHeight()
{
    if (root == NULL)
    {
        return 0;
    }
    Node *curr = root;
    int h = 0;
    while (curr->__isLeaf == false)
    {
        for (int i = 0; i < curr->__size; i++)
        {
            if (curr->ptr[i] != NULL)
            {
                curr = (Node *)curr->ptr[i];
                h++;
            }
        }
    }
    return h;
}
/**
 * @brief Delete a node in the tree
 * 
 * @param numVotes 
 */
void BPTree::Delete(int numVotes)
{
    if (root == NULL)
    {
        return;
    }

    root->Deletion(numVotes);

    if (root->__size == 0)
    {
        Node *tmp = root;
        if (root->__isLeaf == true)
        {
            root = NULL;
        }
        else
        {
            root = (Node *)root->ptr[0];
        }
        delete tmp;
    }
}

/**
 * @brief Check if there is a need to set new root
 * 
 */
void BPTree::setNewRoot()
{        
    int i = 0;
    while(root->ptr[i] != NULL){
        i++;
    }
    
    if (i < root->__minChildren || root->__size + 1 < root->__minChildren)
    {
        //cout << "[SETTING NEW ROOT...]" << endl;
        root = (root->ptr[0] != NULL) ? (Node *)root->ptr[0] : (Node *)root->ptr[1];
    }
}

void BPTree::CheckRootKeys(){
    Node *curr = root;
    if(curr->__isLeaf == true) return;

    for(int i = 0; i < root->__size; i++){
        if(curr->ptr[i + 1] == NULL) return;
        curr = (Node *)curr->ptr[i + 1];
        while(curr != NULL){
            if(curr->__isLeaf == true){
                root->key[i] = curr->key[0];
                break;
            }
            curr = (Node *)curr->ptr[0];
        }
    }
}

/**
 * @brief Performs deletion of key followed by if there is a need to replace
 * root after deletion
 * 
 * @param numVotes 
 */
void BPTree::DeleteAndCheckNewRoot(int numVotes){
    Delete(numVotes);
    setNewRoot();
}

/**
 * @brief Performs insertion of key and assigns parents for all keys in
 * the node.
 * 
 * @param numVotes 
 */
void BPTree::InsertWithAssignParents(int numVotes, void *recordptr){
    Insert(numVotes, recordptr);
    //CheckRootKeys();
    AssignAllParents(root);
}
