/* @Author
Student Name: Basar Demir
Student ID: 150180080
Date: 10/02/2021  */

#include <iostream>
#include <vector>
using namespace std;

//Node for keeping key and attributes
class keyNode {
public:
    int x;
    int y;
    char z;

    //Default constructor for keyNode
    keyNode() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    };

    keyNode(int x, int y, char z); //Constructor with initialization

    keyNode(const keyNode &node); //Copy constructor

    int getKey(char key_definition) const; //Function that returns key attribute
};

//Constructor with initialization
keyNode::keyNode(int x, int y, char z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

//Copy constructor
keyNode::keyNode(const keyNode &node) {
    this->x = node.x;
    this->y = node.y;
    this->z = node.z;
}

//Function that returns key attribute
int keyNode::getKey(char key_definition) const {
    //Function compares given input with options,
    //Returns corresponding attributes
    if (key_definition == 'x')
        return this->x;
    if (key_definition == 'y')
        return this->y;
    if (key_definition == 'z')
        return this->z;
    return 0;
}

//Class for B-Tree Node
class Node {
public:
    int n; //Number of current keys
    int t; //Degree of tree
    vector<Node *> children; //Array for child nodes
    vector<keyNode *> keys; //Array for key nodes
    bool leaf; //Determines node is leaf or not
    char key_definition; //Determines which attribute is key

    Node() {}; //Default constructor

    Node(int t, bool isLeaf, char key_definition); //Constructor

    void printNode() const; //Function that prints node

    int indexOfKey(int key) const; //Function that returns possible index of given key

    void BTreeSplitChild(int i, Node *node); //Function that splits full node

    void BTreeInsertNonFull(keyNode *k); //Function that inserts key to non-full node
};


//Node constructor
Node::Node(int t, bool isLeaf, char key_definition) {
    this->t = t; //assigns t value
    this->n = 0; //there is not any key
    this->leaf = isLeaf; //assigns it is leaf or not
    this->key_definition = key_definition;
    //number of keys for degree t is 2t-1
    this->keys = vector<keyNode *>(2 * t - 1);
    //number of children for degree t is 2t
    this->children = vector<Node *>(2 * t);
}

//Function that splits full node
void Node::BTreeSplitChild(int i, Node *node) {
    //constructs new node according to node properties
    Node *newnode = new Node(node->t, node->leaf, node->key_definition);
    newnode->n = t - 1; //it contains t-1 keys

    //Copies last t-1 keys to new node
    for (int j = 0; j < t - 1; j++) {
        newnode->keys[j] = node->keys[j + t];
    }
    //If node is not leaf
    if (!node->leaf) {
        //Copies last t children to new node
        for (int j = 0; j < t; j++) {
            newnode->children[j] = node->children[j + t];
        }
    }

    node->n = t - 1; //Updates existing node's key number

    for (int j = this->n; j >= i + 1; j--) {
        //it shifts children to add new node
        this->children[j + 1] = this->children[j];
        //shifts keys to add new key
        this->keys[j] = this->keys[j - 1];
    }

    //adds new node as child
    this->children[i + 1] = newnode;

    //takes t-1 th key to proper place
    this->keys[i] = node->keys[t - 1];
    //increment key count
    this->n++;
}

//Function that inserts key to non-full node
void Node::BTreeInsertNonFull(keyNode *k) {
    int index = this->n - 1; //takes last index
    //if it is leaf node
    if (this->leaf) {
        //it creates space for new key by shifting
        while (index >= 0 && keys[index]->getKey(this->key_definition) > k->getKey(this->key_definition)) {
            keys[index + 1] = keys[index];
            index--;
        }
        //places new key
        keys[index + 1] = k;
        //increments node count
        this->n++;
    } else { //if it is not leaf
        //iterates for right place
        while (index >= 0 && keys[index]->getKey(this->key_definition) > k->getKey(this->key_definition)) {
            index--;
        }
        //if child is full
        if (this->children[index + 1]->n == 2 * t - 1) {
            //it splits child node
            this->BTreeSplitChild(index + 1, this->children[index + 1]);
            // it searches right index
            if (keys[index + 1]->getKey(this->key_definition) < k->getKey(this->key_definition)) {
                index++;
            }
        }
        //it calls insert for node
        this->children[index + 1]->BTreeInsertNonFull(k);
    }
}

//Function that prints node
void Node::printNode() const {
    //it prints node attributes respectively
    for (int i = 0; i < this->n; i++) {
        cout << "(" << keys[i]->x << "," << keys[i]->y << "," << keys[i]->z << ")";
    }
    cout << endl;
    //it calls print function for children
    for (int i = 0; i <= this->n; i++) {
        if (!leaf){
            this->children[i]->printNode();
        }
    }
}

//B-Tree Class
class BTree {
public:
    Node *root; //keeps root node of the tree
    int t; //keeps tree's degree
    char key_definition; //it keeps which attribute is key

    //Constructor for B-Tree class
    BTree(int t, char key_definition);

    //Adds node to the tree according to given key node
    void BTreeInsert(keyNode *key);

    //Prints B-Tree in preorder traversal
    void printBTree() const;
};

//Constructor for B-Tree class
BTree::BTree(int t, char key_definition) {
    this->root = NULL; //initialize root as null
    this->t = t; //defines tree degree
    this->key_definition = key_definition; //defines key attribute
}

//Adds node to the tree according to given key node
void BTree::BTreeInsert(keyNode *key) {
    //if root is null
    if (this->root == NULL) {
        //initializes root node
        root = new Node(this->t, true, this->key_definition);
        root->keys[0] = key;  // adds key
        root->n = 1;  // initializes number of keys in root as 1
    } else {
        //if root has maximum number of keys
        if (root->n == 2 * t - 1) {
            //it initializes new node
            Node *node = new Node(t, false, this->key_definition);
            //it defines existing root as child
            node->children[0] = this->root;
            node->n = 0; //defines node key number as 0
            //it divides root from first element
            node->BTreeSplitChild(0, root);
            int index = 0; //defines index as 0
            //it determines which child will be key
            if (node->keys[0]->getKey(key_definition) < key->getKey(key_definition)){
                index++;
            }
            //it calls insert for given child
            node->children[index]->BTreeInsertNonFull(key);
            root = node; //defines new node as root
        } else { //If root has enough space, calls BTreeInsertNonFull
            root->BTreeInsertNonFull(key);
        }
    }
}

//Prints B-Tree in preorder traversal
void BTree::printBTree() const {
    if (root != NULL) {
        root->printNode();
    }
}

int main() {
    //Reads input format information
    int node_number, degree;
    char key;
    cin >> node_number >> degree;
    cin >> key;

    //Constructs B-Tree according to input variables
    BTree *tree = new BTree(degree, key);
    //Reads attributes and inserts given node
    for (int i = 0; i < node_number; i++) {
        int x, y;
        char z;
        cin >> x >> y >> z;
        keyNode *key = new keyNode(x, y, z);
        tree->BTreeInsert(key);
    }
    //prints tree in preorder format
    tree->printBTree();
    //deletes tree
    delete tree;

    return 0;
}
