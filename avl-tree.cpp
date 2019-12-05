#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct node //each node must have a key(value), height, and leftChild/rightChild pointers
{
    int key, height;
    node *leftChild, *rightChild, *parent;
};

class avl //avl class and functions: predecessor, findHeight, place, rotateLeft and rotateRight, insert, and balance
{

  public:
    node *root;

    avl()
    {
        //set empty tree root to null
        root = NULL;
    };

    void predecessor(int value, int &p, node * root) { 
        //compare node->key to value and determine which child to recurse into
        if (root == NULL)
            return;

        if (root->key == value) {
           p = value;
           return;
        }
        else if (root->key > value) {
            predecessor(value, p, root->leftChild);
        }
        else {
            p = root->key;
            return predecessor(value, p, root->rightChild);
        }
    };

    node *rotateRight(node * y)
    {
        node *x = y->leftChild;
        node *z = x->rightChild;
        
        x->rightChild = y;
        y->leftChild = z;
        
        //Recalculate heights
        y->height = max(findHeight(y->leftChild), findHeight(y->rightChild)) + 1;
        x->height = max(findHeight(x->leftChild), findHeight(x->rightChild)) + 1;

        return x;
    };

    node *rotateLeft(node * x)
    {
        node *y = x->rightChild;
        node *z = y->leftChild;

        y->leftChild = x;
        x->rightChild = z;

        //Recalculate heights
        x->height = max(findHeight(x->leftChild),findHeight(x->rightChild)) + 1;
        y->height = max(findHeight(y->leftChild), findHeight(y->rightChild)) + 1;

        return y;
    };

    int findHeight(node * n)
    {
        if (n == NULL)
            return -1; //Height of child to a leaf node is -1
        return n->height;
    };
    
    node * place(int value) {
        root = new node();
        root->key = value;
        root->height = 0;
        root->leftChild = NULL;
        root->rightChild = NULL;
        return root;
    };

    node * insert(node *root, int value)
    {   //(Simple BST Insert) + AVL Balancing
        //1. Find correct point of insertion and then place() value
        //2. Calculate new height
        //3. Balance AVL properties
        if (root == NULL)
            return place(value);

        if (value < root->key) 
            root->leftChild = insert(root->leftChild, value);
           
        else if (value > root->key) 
            root->rightChild = insert(root->rightChild, value);
            
        // Duplicate element
       else
            return root;

        // Calculating new height
        root->height = max(findHeight(root->leftChild), findHeight(root->rightChild)) + 1; 
        return balance(root, value);
        
    };

    node *balance(node *root, int value) {
        // Variable to store balancing factor
        int balance = 0;
        if (root != NULL)
            balance = findHeight(root->leftChild) - findHeight(root->rightChild);

        // Rotation: Left-Right
        if (balance > 1 && value > root->leftChild->key)
        {
            root->leftChild = rotateLeft(root->leftChild);
            return rotateRight(root);
        }

        // Rotation: Right-Left
        if (balance < -1 && value < root->rightChild->key)
        {
            root->rightChild = rotateRight(root->rightChild);
            return rotateLeft(root);
        }
        // Rotation: Left-Left
        if (balance > 1 && value < root->leftChild->key)
            return rotateRight(root);

        // Rotation: Right-Right
        if (balance < -1 && value > root->rightChild->key)
            return rotateLeft(root);

        // No rotation case
        return root;
    };
};

int main()
{
    //Init new tree, open test/result files and loop through ops
    avl *tree = new avl();
    ifstream testFile;
    ofstream resultFile;
    resultFile.open("results.txt");
    testFile.open("ops-half.txt"); //Assuming "ops.txt" is file name
    string input;

    while (getline(testFile, input))
    {
        //Parse operation and value line by line
        string operation = input.substr(0, 4);
        int val = stoi(input.substr(4));
        if (operation.compare("ins ") == 0)
        {
            tree->root = tree->insert(tree->root, val);
        }
        if (operation.compare("qry ") == 0)
        {
            int p = -999;
            tree->predecessor(val, p, tree->root);
            resultFile << p << endl;
        }
    }
    testFile.close();
    resultFile.close();

    return 0;
}