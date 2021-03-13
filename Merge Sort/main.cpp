/* @Author
Student Name: Başar Demir
Student ID: 150180080
Date: 10/12/2020  */

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <iomanip>

using namespace std;

//Node class for keeping values of each line of document
class Node{
public:
    string country; //country name
    string item_type; //item type
    string order_id; //order id
    int units_sold; //number of sold items 
    double total_profit; //total profit
    bool operator== (Node &node) const; //overload function for ==
    bool operator> (Node &node) const; //overload function for >
    bool operator< (Node &node) const; //overload function for <
};


// overload function for ==
bool Node::operator==(Node &node) const {
    //checks country names and total profits is equal or not
    if(this->country == node.country && this->total_profit == node.total_profit)
        return true; //if equal, returns true;
    return false;//if not equal, returns false;
}

// overload function for >
bool Node::operator>(Node &node) const {
    //if country name bigger than other country name lexicographically
    if(this->country>node.country){
        return true; //returns directly true
    }
    else if(this->country==node.country){ //if they are same country
        //it compares their total profit
        if(this->total_profit<node.total_profit)
            return true; //if total profit smaller, it means that element is bigger
        return false; //returns false
    }
    else{
        return false; // if country name smaller, returns false
    }
}

// overload function for <
bool Node::operator<(Node &node) const {
    //if second country name bigger than country name lexicographically
    if(this->country<node.country){
        return true; //returns directly true
    }
    else if(this->country==node.country){ //if they are same country
        //it compares their total profit
        if(this->total_profit>node.total_profit)
            return true; //if total profit bigger, it means that element is bigger
        return false; //returns false
    }
    else{
        return false; // if country name bigger, returns false
    }
}

//function that handles file reading operations
void readFile(const string& fileName, Node *array, int size) {
    
    ifstream file; //initialized ifstream variable
    file.open(fileName.c_str()); //opens file

    if (!file) { //checks if file opened or not
        cerr << "File cannot be opened!";
        exit(1); //exits with failure
    }

    string line; //variable that keeps line or word
    getline(file, line); //this reads header line
    Node node =  Node(); //variable that keeps line variables

    for (int i = 0; i < size; i++) {
        getline(file, line, '\t'); //reads country (string)
        node.country=line; //assigns to class

        getline(file, line, '\t'); //reads item type (string)
        node.item_type=line; //assigns to class

        getline(file, line, '\t'); //reads order id (string)
        node.order_id=line; //assigns to class

        file >> node.units_sold; //reads and assigns units sold (integer)

        file >> node.total_profit; //reads and assigns total profit (float)

        getline(file, line, '\n'); //this is for reading the \n character into dummy variable.

        array[i] = node; //adds node to array
    }
    file.close(); //closes file after end of file operation
}

//function that handles file reading operations
void writeFile(const string& fileName, Node *array, int size) {
    ofstream result_file; //initialized ofstream variable
    result_file.open(fileName.c_str()); //opens file
    for (int i = 0; i < size; i++) { //iterates over the array
        //writes each element to file in proper format
        result_file<< array[i].country+'\t'<<array[i].item_type<<'\t'<<array[i].order_id<<'\t'<<
        array[i].units_sold<<'\t'<<fixed<<setprecision(2)<<array[i].total_profit<<'\n';
    }
    result_file.close();
}

// A utility function to swap two nodes
void swapper(Node* element1, Node *element2) {
    Node temp = *element1; //assigns first element to temp variable
    *element1 = *element2; //transfers second element to first element
    *element2 = temp; //assigns temp of first variable to second element
}

//Function that handles pivotting operations
int Partition(Node *array, int size){
    
    int pivot = size-1; //chooses last element as pivot
    int position = 0; //variable that keeps position of pivot 

    for (int i = 0; i < size-1; i++) { //iterates over the array
        if (array[pivot]>array[i]) { //if pivot is bigger than element
            swapper(&array[i], &array[position]); //places element to before the pivot
            position++; //increments position of pivot
        }
    }
    swapper(&array[pivot], &array[position]); //places pivot to proper position

    return position; //returns position of pivot
}
//Main QuickSort function
void QuickSort(Node *array, int size) {
    if (size <= 1) //if size is smaller than 1, array is sorted
        return; //directly returns

    //calls partition function that handles pivotting operations
    int pivot_position = Partition(array,size); 
    
    QuickSort(array, pivot_position); //calls QuickSort for elements smaller than pivot
    QuickSort(array + (pivot_position+1), size - (pivot_position+1)); //calls QuickSort for elements bigger than pivot
}


int main(int argc, char *argv[]) {
    clock_t time; //variable that keeps time
    int N; //keeps input size

    if(argc == 2){ //checks command line argument number 
        N = atoi(argv[1]); //reads argument and transforms to integer
    }else{ //if argument is not in proper format
        cerr << "Command line argument does not given in right format!";
        exit(1); //exits with failure
    }

    Node* array = new Node[N]; //creates array for each line of input
    readFile("sales.txt", array, N); //reads file

    time = clock(); //starts clock
    QuickSort(array, N); //calls QuickSort function
    time = clock() - time; //calculates time that passed

    //prints passed time for QuickSort
    printf("It took me %ld clicks (%f seconds).\n", time, ((float) time) / CLOCKS_PER_SEC);

    writeFile("sorted.txt", array, N); //writes elements to file in sorted format

    delete[] array; //deletes array
    return 0;
}