// RedBlackTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Progream creates a Red Black Tree
// Author: Pranav Sharma
// Date: 12/21/2020

#include <iostream>
#include <string>
using namespace std;
// this struct defines the properties of the node
struct Node {
public:
    bool isRed;
    int data;
    Node* parent;
    Node* left;
    Node* right;
};

// functions for reading in a file and printing the tree
void readFile(char file[], int numbers, Node* &root);
void printTree(Node* head, int spacing);

// specific node functions, ideas from wikipedia
Node* getParent(Node* n);
Node* getGrandParent(Node* n);
Node* getSibling(Node* n);
Node* getUncle(Node* n);
void rotateLeft(Node* n);
void rotateRight(Node* n);

// inserting functions, ideas from wikipedia
Node* insertNode(Node* root, Node* node);
void insertRecurse(Node* root, Node* node);
void repairTree(Node* node);

// specific cases for inserting, from wikipedia
void insertCase1(Node* &n);
void insertCase2(Node* &n);
void insertCase3(Node* &n);
void insertCase4(Node* &n);

int main()
{
    // introduces user to progream and generates vairables
    char* input = new char();
    bool playing = true;
    cout << "Welcome to my Red-Black Tree Program!\n";
    Node* root = new Node();

    // loops through asking user what they would like to do until they quit
    while (playing) {
        cout << "What would you like to do? (ADD, PRINT, READ, or QUIT)\n";
        cin.getline(input, 80, '\n');
        // if user wants to add...
        if (strcmp(input, "ADD") == 0) {
            // program asks for number then inserts it
            int num;
            cout << "What number would you like to add?\n";
            cin >> num;
            cin.get();
            // assigns first number to root
            if (root->data == 0) {
                root->data = num;
            }
            else {
                Node* n = new Node();
                n->data = num;
                root = insertNode(root, n);
            }
        }
        // prints the Red Black Tree
        else if (strcmp(input, "PRINT") == 0) {
            printTree(root, 0);
        }
        // reads from a file and adds a certain number of numbers to the tree
        else if (strcmp(input, "READ") == 0) {
            char file[20];
            int num;
            cout << "How many numbers would you like to read from a file?\n";
            cin >> num;
            cin.get();
            cout << "Enter the file you would like to open, in this program there is a file called 'Number.txt'\n";
            cin.getline(file, 80, '\n');
            readFile(file, num, root);
        }
        // quits the program
        else if (strcmp(input, "QUIT") == 0) {
            cout << "Alrighty! Adios\n";
            playing = false;
        }
        // user didn't enter a proper command :(
        else {
            cout << "Looks like you didn't enter a proper command :(\n";
        }
    }
}

void readFile(char file[], int numbers, Node* &root) {
    FILE* pfile = NULL;
    char line[1000];
    // opens the file with the given name from user
    pfile = fopen(file, "r");
    // alerts if file is unreadable
    if (pfile == NULL)
    {
        cout << "Error in opening the file Numbers.txt";
        return;
    }

    // goes through each line, and stores whats beween the commas in the integer array
    int index = 0;
    bool reading = true;
    while (fgets(line, 1000, pfile) && reading == true) {
        char* buffer = new char[20];
        memset(buffer, '\0', 20);
        int pos = 0;
        while (buffer != NULL && reading == true) {
            buffer = strchr(line + pos, ',');
            if (buffer != NULL) {
                char lineBuffer[20];
                memset(lineBuffer, '\0', 20);
                strncpy(lineBuffer, line + pos, sizeof(line - buffer));
                if (root->data == 0) {
                    root->data = stoi(lineBuffer);
                }
                else {
                    Node* n = new Node();
                    n->data = stoi(lineBuffer);
                    root = insertNode(root, n);
                }
                index++;
                if (index == numbers) {// stops when intended numbers are read
                    reading = false;
                    return;
                }
                pos = (buffer - line) + 1;
            }
        }
    }
}
// prints the binary tree (sideways) using spaces
void printTree(Node* head, int spacing) {
    if (head != NULL) {
        spacing = spacing + 10;
        printTree(head->right, spacing);
        cout << endl;
        for (int i = 10; i < spacing; i++) {
            cout << " ";
        }
        if(head->isRed == true){
            cout << "\033[1m\033[31m" << head->data << endl << "\033[0m";
        }
        else{
            cout << "\033[1m\033[37m" << head->data << endl << "\033[0m";
        }
        printTree(head->left, spacing);
    }
}
// gets the parent of any given node
Node* getParent(Node* n) {
    if (n == NULL) {
        return NULL;
    }
    else {
        return n->parent;
    }
}
// gets the grand parent of any given node
Node* getGrandParent(Node* n) {
    return getParent(getParent(n));
}
// returns the sibling of a node
Node* getSibling(Node* n) {
    Node* parent = getParent(n);

    if (parent == NULL) {
        return NULL;
    }

    if (n == parent->right) {
        return parent->left;
    }
    else {
        return parent->right;
    }
}
// returns the uncle of a node
Node* getUncle(Node* n) {
    Node* parent = getParent(n);
    return getSibling(parent);
}
// rotates the tree left when the condition appears
void rotateLeft(Node* n) {
    Node* newNode = n->right;
    Node* parent = getParent(n);
    // makes sure newNode is not null before shifting
    if (newNode != NULL) { 
        n->right = newNode->left;
        newNode->left = n;
        n->parent = newNode;
        if (n->right != NULL) {
            n->right->parent = n;
        }

        if (parent != NULL) {
            if (n == parent->left) {
                parent->left = newNode;
            }
            else if (n == parent->right) {
                parent->right = newNode;
            }
        }
        newNode->parent = parent;
    }
}
// rotates the tree right
void rotateRight(Node* n) {
    Node* newNode = n->left;
    Node* parent = getParent(n);
    if (newNode != NULL) {
        n->left = newNode->right;
        newNode->right = n;
        n->parent = newNode;
        if (n->left != NULL) {
            n->left->parent = n;
        }

        if (parent != NULL) {
            if (n == parent->left) {
                parent->left = newNode;
            }
            else if (n == parent->right) {
                parent->right = newNode;
            }
        }
        newNode->parent = parent;
    }
}
// inserts a node into the tree
Node* insertNode(Node* root, Node* node) {
    insertRecurse(root, node);
    repairTree(node);

    root = node;
    while (getParent(root) != NULL) {
        root = getParent(root);
    }
    return root;
}
// recursively decends the tree until a leaf is found
void insertRecurse(Node* root, Node* node) {
    if (root != NULL) {
        if (node->data < root->data) {
            if (root->left != NULL) {
                insertRecurse(root->left, node);
                return;
            }
            else {
                root->left = node;
            }
        }
        else {
            if (root->right != NULL) {
                insertRecurse(root->right, node);
                return;
            }
            else {
                root->right = node;
            }
        }
    }
    // intializes conditions for a new node
    node->parent = root;
    node->left = NULL;
    node->right = NULL;
    node->isRed = true;
}

// makes sure tree fits all conditions after each move
void repairTree(Node* n) {
    if (getParent(n) == NULL) {
        insertCase1(n);
    }
    else if (getParent(n)->isRed == false) {
        insertCase2(n);
    }
    else if (getUncle(n) != NULL && getUncle(n)->isRed == true) {
        insertCase3(n);
    }
    else {
        insertCase4(n);
    }
}
// The current node is at the root of tree and must be repainted to black
void insertCase1(Node* &n) {
    n->isRed = false;
}
// tree remains valid and no action is needed
void insertCase2(Node* &n) {
    return;
}
// Parent and uncle are both red and must be repainted red. Granparents becomes red too.
void insertCase3(Node*& n) {
    getParent(n)->isRed = false;
    getUncle(n)->isRed = false;
    getGrandParent(n)->isRed = true;
    repairTree(getGrandParent(n));
}
// Rotates the new node N to the grandparent position. Left rotation switches node with parent and the right rotation switches parents and gparent
void insertCase4(Node*& n) {
    Node* p = getParent(n);
    Node* g = getGrandParent(n);

    if (n == p->right && p == g->left) {
        rotateLeft(p);
        n = n->left;
    }
    else if (n == p->left && p == g->right) {
        rotateRight(p);
        n = n->right;
    }

    p = getParent(n);
    g = getGrandParent(n);

    if (n == p->left) {
        rotateRight(g);
    }
    else {
        rotateLeft(g);
    }

    p->isRed = false;
    g->isRed = true;
}