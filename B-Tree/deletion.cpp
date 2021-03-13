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

    void BTreeDeleteNode(int k); //Function that deletes node with key

    void BTreeMerge(Node *left, Node *right, int index); //Function that merges left and right nodes

    void BTreeMove(int index); //it moves key from  one child to another
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

//Function that returns possible index of given key
int Node::indexOfKey(int key) const {
    int index = 0;
    //it iterates until it reaches higher key value
    while (index < this->n && key > this->keys[index]->getKey(this->key_definition)) {
        index++;
    }
    return index;
}

//Function that deletes node with key
void Node::BTreeDeleteNode(int key) {
    //it searches for possible key position
    int index = this->indexOfKey(key);
    //if key is placed current node
    if (this->n > index && this->keys[index]->getKey(this->key_definition) == key) {
        //if current node is leaf
        if (this->leaf) {
            //it shifts next nodes left by one
            for (int i = index; i < this->n - 1; i++){
                this->keys[i] = this->keys[i + 1];
            }
            //decreases key number
            this->n -= 1;
            return;
        } else { //if current node is not leaf
            //if left child has more keys than t
            if (this->children[index]->n >= this->t) {
                //it iterates to find predecessor
                Node *traverse = this->children[index];
                //it find right-most element
                while (!traverse->leaf) {
                    traverse = traverse->children[traverse->n];
                }
                keyNode *predecessorNode = traverse->keys[traverse->n - 1];
                //it replaces key with predecessor
                this->keys[index] = predecessorNode;
                //it calls delete for predecessor
                this->children[index]->BTreeDeleteNode(predecessorNode->getKey(this->key_definition));
                return;
            }
            //if right child has more keys than t
            if (this->children[index + 1]->n >= this->t) {
                Node *traverse = this->children[index + 1];
                //it find left-most element
                while (!traverse->leaf) {
                    traverse = traverse->children[0];
                }
                keyNode *successorNode = traverse->keys[0];
                //it replaces key with successor
                this->keys[index] = successorNode;
                //it calls delete for successor
                this->children[index + 1]->BTreeDeleteNode(successorNode->getKey(this->key_definition));
                return;
            }
            //if they have not key more than t, it merges children
            this->BTreeMerge(this->children[index], this->children[index + 1], index);
            //it calls delete node again
            this->children[index]->BTreeDeleteNode(key);
        }
    } else { //if it not in the current node
        //if current node is leaf, it means that key does not exist
        if (leaf) {
            return;
        }
        //if index is last child
        bool indexflag = false;
        if (index == this->n) {
            indexflag = true;
        }
        //if child has less than t keys, we borrow key from its child
        if (this->children[index]->n < this->t) {
            this->BTreeMove(index);
        }
        if (indexflag && index > n) {
            this->children[index - 1]->BTreeDeleteNode(key);
        } else {
            this->children[index]->BTreeDeleteNode(key);
        }
    }
}

//Function that merges left and right nodes
void Node::BTreeMerge(Node *left, Node *right, int index) {
    //it takes key to left child
    left->keys[this->t - 1] = this->keys[index];

    //It copies keys from right to left
    for (int i = 0; i < right->n; i++) {
        left->keys[t + i] = right->keys[i];
    }
    //if left child if not leaf
    if (!left->leaf) {
        //it copies children from right to left
        for (int i = 0; i <= right->n; i++) {
            left->children[t + i] = right->children[i];
        }
    }
    //it shifts keys left
    for (int i = index + 1; i < n; i++) {
        this->keys[i - 1] = this->keys[i];
        this->children[i] = this->children[i + 1];
    }
    this->n -= 1; //decreases key number
    left->n += right->n + 1; //adds key right key number to left
    delete right; //deletes unnecessary node
}

//it moves key from  one child to another
void Node::BTreeMove(int index) {
    //if left child has more than t keys
    if (index != 0 && this->children[index - 1]->n >= this->t) {
        //it takes left and right children
        Node *right = this->children[index];
        Node *left = this->children[index - 1];
        //it shifts keys to right by one
        for (int i = right->n - 1; i >= 0; i--) {
            right->keys[i + 1] = right->keys[i];
        }
        //if right node is not leaf
        if (!right->leaf) {
            //it shifts children to right by one
            for (int i = right->n; i >= 0; i--) {
                right->children[i + 1] = right->children[i];
            }
        }
        //it places previous key as right's first element
        right->keys[0] = keys[index - 1];
        //if it is not leaf, also child is placed
        if (!right->leaf) {
            right->children[0] = left->children[left->n];
        }
        //it moves a key from left right-most to current node
        this->keys[index - 1] = left->keys[left->n - 1];

        right->n++; //increases right node's key number
        left->n--; //decreases left node's key number
        return;
    }//if right child has more than t keys
    if (index != n && this->children[index + 1]->n >= this->t) {
        //it takes left and right children
        Node *left = children[index];
        Node *right = children[index + 1];
        //it takes current key as left node's right-most element
        left->keys[left->n] = keys[index];
        //if left node is not leaf
        if (!left->leaf) {
            //it takes first child of right node as right-most child of left
            left->children[left->n + 1] = right->children[0];
        }
        //it takes first key of right child to current key index
        this->keys[index] = right->keys[0];

        //it shifts keys to left by one
        for (int i = 0; i < right->n - 1; i++) {
            right->keys[i] = right->keys[i + 1];
        }
        //if right node is not leaf
        if (!right->leaf) {
            //it shifts children to left by one
            for (int i = 1; i <= right->n; i++) {
                right->children[i - 1] = right->children[i];
            }
        }

        left->n++; //increments left node's key number
        right->n--; //decrements right node's key number
        return;
    }
    //if they have not t keys and it is not a last element
    if (index != this->n)
        //merges children
        BTreeMerge(children[index], children[index + 1], index);
    else //if index is last element then merges from previous key
        BTreeMerge(children[index - 1], children[index], index - 1);
}

//B-Tree Class
class BTree {
public:
    Node *root; //keeps root node of the tree
    int t; //keeps tree's degree
    char key_definition; //it keeps which attribute is key

    //Constructor for B-Tree class
    BTree(int t, char key_definition);

    //Deletes node from the tree according to given key
    void BTreeDelete(int key);

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

//Deletes node from the tree according to given key
void BTree::BTreeDelete(int key) {
    //if root is null, directly returns
    if (this->root == NULL) {
        return;
    }
    //calls delete function
    this->root->BTreeDeleteNode(key);

    //if delete function deleted from root
    //checks root key number
    if (this->root->n == 0) {
        //keeps root's pointer
        Node *temp_root = this->root;
        //if root has child
        if (!this->root->leaf) {
            //take first child as root
            this->root = this->root->children[0];
        } else {
            this->root = NULL;
        }
        delete temp_root;
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
    //if key is character, read character from input and deletes it
    if (key == 'z') {
        char deleted_key;
        cin >> deleted_key;
        tree->BTreeDelete(deleted_key);
    } else {//if key is integer, read integer from input and deletes it
        int deleted_key;
        cin >> deleted_key;
        tree->BTreeDelete(deleted_key);
    }
    //prints tree in preorder format
    tree->printBTree();
    //deletes tree
    delete tree;

    return 0;
}
