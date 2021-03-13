#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

#define RED 1 //define RED as true
#define BLACK 0 //define RED as false

//Node class to keep element of Red-Black Tree
class Node {
public:
    //Parameters of class
    string name; //player's name as key of node
    int point; //player's point number
    int rebound; //player's rebound number
    int assist; //player's assists number
    bool color; // color of Node (0-black, 1-red)
    Node *parent; //parent of Node
    Node *left; //left child of Node
    Node *right; //right child of Node
    //Constructors
    Node() {};

    Node(string &name, int point, int rebound, int assist);

    // == Overload to simplify comparison operations
    bool operator==(Node *node) const;
};

//Node constructor
Node::Node(string &name, int rebound, int assist, int point) {
    this->name = name;
    this->point = point;
    this->rebound = rebound;
    this->assist = assist;
}

//Operator overload of ==
bool Node::operator==(Node *node) const {
    //if their keys are equal, returns true
    if (this->name == node->name)
        return true;
    return false;
}

//Red-Black Tree Class
class RedBlackTree {
public:
    //Parameters of class
    Node *root; //keeps root Node of tree
    Node *NIL; //keeps NIL (empty node)
    Node *maxAssist; //Node that contains maximum assist number
    Node *maxPoint; //Node that contains maximum point number
    Node *maxRebs; //Node that contains maximum rebound number
    //Constructor
    RedBlackTree();

    //Deconstructor
    ~RedBlackTree();

    //Functions
    void deleteTree(Node *node); //Deletes all tree

    void printTree(Node *node, int depth); //Prints Nodes of tree

    void insert(Node *node); //Adds new Node to tree

    void insertFixUp(Node *node); //Fix function for insertion

    void leftRotate(Node *node); //Left rotate according to given Node

    void rightRotate(Node *node); //Right rotate according to given Node

    void updateMaxs(Node *node); //Updates maximum values
};

//Constructor for Red Black Tree
RedBlackTree::RedBlackTree() {
    this->NIL = new Node(); //initializes Node as NIL
    this->NIL->color = BLACK; //NIL must be black
    this->NIL->name = '\0'; //Sets NIL name as end character
    this->root = this->NIL; //Root initialized as NIL
    //initialized max Nodes as NULL
    this->maxAssist = NULL;
    this->maxPoint = NULL;
    this->maxRebs = NULL;
}

//Left Rotate Function
void RedBlackTree::leftRotate(Node *node) {
    Node *y = node->right; //takes right child as y
    node->right = y->left; //make y's left child node's right child
    if (y->left != this->NIL) {
        //sets parent node for node's right child
        y->left->parent = node;
    }
    //connect node and y to same parent node
    y->parent = node->parent;
    if (node->parent == this->NIL) {
        //if parent is NIL, it must be root
        this->root = y;
    } else if (node == node->parent->left) {
        //if node is left child, make y left child
        node->parent->left = y;
    } else {
        //if node is right child, make y right child
        node->parent->right = y;
    }
    y->left = node; //makes node y's left child
    node->parent = y; //make y parent of node
}

//Right Rotate Function
void RedBlackTree::rightRotate(Node *node) {
    Node *y = node->left; //takes left child as y
    node->left = y->right; //make y's right child node's left child
    if (y->right != this->NIL) {
        //sets parent node for node's left child
        y->right->parent = node;
    }
    //connect node and y to same parent node
    y->parent = node->parent;
    if (node->parent == this->NIL) {
        //if parent is NIL, it must be root
        this->root = y;
    } else if (node == node->parent->right) {
        //if node is right child, make y right child
        node->parent->right = y;
    } else {
        //if node is left child, make y left child
        node->parent->left = y;
    }
    y->right = node; //makes node y's right child
    node->parent = y; //make y parent of node
}

//Function that inserts new node to tree
void RedBlackTree::insert(Node *node) {
    Node *y = this->NIL; //initializes y as NIL
    Node *x = this->root; //initializes x as root
    //Iterates until it reaches to NIL Node
    while (x != this->NIL) {
        y = x; //keeps x node in y
        if (node->name < x->name) {
            //if key value of out node smaller than current node
            x = x->left;
        } else if (node->name == x->name) {
            //if it finds same key value
            //adds new values to existing node
            x->assist += node->assist;
            x->point += node->point;
            x->rebound += node->rebound;
            //checks max players
            updateMaxs(x);
            //deletes node
            delete node;
            return;
        } else {
            //if key value of out node bigger than current node
            x = x->right;
        }
    }
    //y keeps parent of given node
    node->parent = y; //assigns y as parent
    if (y == this->NIL) {
        //if parent is NIL, it must be root
        this->root = node;
    } else if (node->name < y->name) {
        //if key value is smaller than parent node
        y->left = node;
    } else {
        //if key value is bigger than parent node
        y->right = node;
    }
    //checks max players
    updateMaxs(node);
    //connects left and right child to NIL node
    node->left = this->NIL;
    node->right = this->NIL;
    node->color = RED; //new node must colored as RED
    //calls fixup function for insertion
    this->insertFixUp(node);
}

//Function that fixes tree according to properties of Red Black Tree
void RedBlackTree::insertFixUp(Node *node) {
    //Until it reaches to RED colored parent
    while (node->parent->color == RED) {
        //if parent of node is left child
        if (node->parent == node->parent->parent->left) {
            //keeps temp as node's uncle
            Node *temp = node->parent->parent->right;
            //if uncle node is RED ->case 1
            if (temp->color == RED) {
                node->parent->color = BLACK; //color parent node with black
                temp->color = BLACK; //color uncle node with black
                node->parent->parent->color = RED; //make parent of parent red
                node = node->parent->parent; //fix parent of parent
            } else { //if uncle node is BLACK
                //if node is right child -> case 2
                if (node == node->parent->right) {
                    node = node->parent; // fix parent
                    this->leftRotate(node); //perform left rotation
                }// case 3
                node->parent->color = BLACK; //color parent node with black
                node->parent->parent->color = RED; //color parent of parent node with red
                this->rightRotate(node->parent->parent); //perform right rotation
            }
        } else { //if parent of node is right child
            //keeps temp as node's uncle
            Node *temp = node->parent->parent->left;
            //if uncle node is RED ->case 1
            if (temp->color == RED) {
                node->parent->color = BLACK; //color parent node with black
                temp->color = BLACK; //color uncle node with black
                node->parent->parent->color = RED; //make parent of parent red
                node = node->parent->parent; //fix parent of parent
            } else { //if uncle node is BLACK -> case 2
                //if node is left child
                if (node == node->parent->left) {
                    node = node->parent;// fix parent
                    this->rightRotate(node);//perform right rotation
                }// case 3
                node->parent->color = BLACK; //color parent node with black
                node->parent->parent->color = RED; //color parent of parent node with red
                this->leftRotate(node->parent->parent); //perform left rotation
            }
        }
    }
    this->root->color = BLACK; //colors root node with black
}

//The function that prints all nodes of tree (pre-order)
void RedBlackTree::printTree(Node *node, int depth = 0) {
    //if node is not empty
    if (node != this->NIL) {
        if(depth)
            cout << endl;//Prints current node properties
        //Dashes are used to indicate depth
        for (int i = 0; i < depth; i++)
            cout << '-';
        if (node->color == RED) {
            cout << "(RED) " << node->name;
        } else {
            cout << "(BLACK) " << node->name;
        }
        //Calls function for left and right child with incrementing depth by 1
        printTree(node->left, depth + 1);
        printTree(node->right, depth + 1);
    }
}

//Deconstructor for Red Black Tree
RedBlackTree::~RedBlackTree() {
    deleteTree(this->root); //Calls delete tree function
    delete this->NIL; //deletes NIL Node
}

//Function that deletes whole tree (post-order)
void RedBlackTree::deleteTree(Node *node) {
    //Until it reaches to NIL Node
    if (node != this->NIL) {
        deleteTree(node->left); //Calls delete function for left child
        deleteTree(node->right); //Calls delete function for right child
        delete node; //Deletes itself
    }
}

//Function that checks maximum property condition
void RedBlackTree::updateMaxs(Node *node) {
    //Checks maximum assist number
    if (this->maxAssist == NULL || this->maxAssist->assist < node->assist) {
        this->maxAssist = node;
    }
    //Checks maximum point number
    if (this->maxPoint == NULL || this->maxPoint->point < node->point) {
        this->maxPoint = node;
    }
    //Checks maximum rebound number
    if (this->maxRebs == NULL || this->maxRebs->rebound < node->rebound) {
        this->maxRebs = node;
    }
}

//Function that prints informative message after
void seasonEnd(string &season, RedBlackTree *tree) {
    //Prints season year
    cout << "End of the " << season << " Season" << endl;
    //Prints record holder players
    cout << "Max Points: " << tree->maxPoint->point << " - Player Name: " << tree->maxPoint->name << endl;
    cout << "Max Assists: " << tree->maxAssist->assist << " - Player Name: " << tree->maxAssist->name << endl;
    cout << "Max Rebs: " << tree->maxRebs->rebound << " - Player Name: " << tree->maxRebs->name;
}

int main(int argc, char *argv[]) {
    ifstream file; //initializes file operator
    if(argc != 2){
        cerr << "Check parameters!";
        return 1;
    }
    string fileName = argv[1]; //takes file name from command line
    file.open(fileName.c_str()); //opens corresponding file
    //if file cannot be opened
    if (!file) {
        //Returns with error
        cerr << "File cannot be opened!";
        return 1;
    }
    //Initializes Red-Black Tree
    RedBlackTree *rbTree = new RedBlackTree();

    //Reads header of the file
    string temp;
    getline(file, temp);

    Node* tempNode = new Node(); //Node for simplify reading process
    string last_season; //Keeps previous season name
    string season; //Keeps season name
    bool isFirstSeason = true; //flag that checks season is first or not

    //it iterates over the file
    while (!file.eof()) {
        getline(file, season, ','); //Reads current season
        getline(file, tempNode->name, ','); //Reads player's name
        getline(file, temp, ','); //Reads team of the player

        //Reads player's rebound number
        getline(file, temp, ',');
        stringstream reboundStream;
        reboundStream<<temp;
        reboundStream>>tempNode->rebound;

        //Reads player's assist number
        getline(file, temp, ',');
        stringstream assistStream;
        assistStream<<temp;
        assistStream>>tempNode->assist;

        //Reads player's point number
        getline(file, temp, '\n');
        stringstream pointStream;
        pointStream<<temp;
        pointStream>>tempNode->point;

        //Copies tempNode to persistent node
        Node *node = new Node(tempNode->name, tempNode->rebound, tempNode->assist, tempNode->point);
        //If season changed and last season is available
        if (season != last_season && !last_season.empty()) {
            if (!isFirstSeason)
                cout << endl;
            seasonEnd(last_season, rbTree); //Prints season statistics
            if (isFirstSeason) {
                //Prints current node properties
                cout << endl;
                //Prints first season's tree
                rbTree->printTree(rbTree->root);
                isFirstSeason = false; //updates flag
            }
        }
        rbTree->insert(node); //Inserts new player to tree
        last_season = season; //Updates last season
    }
    //if only one season is given
    if(isFirstSeason){
        seasonEnd(last_season, rbTree); //Prints last season statistics
        cout << endl;
        rbTree->printTree(rbTree->root); //Prints first season's tree
    }
    else{
        cout << endl;
        seasonEnd(last_season, rbTree); //Prints last season analytics
    }
    delete tempNode; //deletes temp node
    delete rbTree; //deletes Red-Black Tree
    return 0;
}
