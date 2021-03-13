/* @Author
Student Name: Başar Demir
Student ID: 150180080
Date: 25/12/2020  */

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <queue>
#include <ctime>
#include <stdlib.h>
#include <string>

using namespace std;

#define LONGITUDE 33.40819
#define LATITUDE 39.19001

//Function for calculating Euclidean Distance
double euclideanDistance(double longitude, double latitude) {
    return sqrt(pow(LONGITUDE - longitude, 2) + pow(LATITUDE - latitude, 2));
}

//Priority Queue Class
class PriorityQueue {
private:
    vector<double> taxis; //keeps distances of taxis for heap
public:
    double getNearestTaxi(); //Function that get minimum element of heap

    void decreaseTaxiDistance(); //Function for decrease random node value of heap

    void addTaxi(double distance); //Function for adding new element to the heap

    void minHeapify(int index); //MinHeapify function of PriorityQueue

    int getParent(int index); //Function that finds index of parent

    int getLeft(int index); //Function that finds index of left child

    int getRight(int index); //Function that finds index of right child
};

//MinHeapify function of PriorityQueue
void PriorityQueue::minHeapify(int index) {
    int smallest_index = index; //keeps index of smallest node
    double smallest_element = this->taxis[index]; //gets current node's value
    unsigned left_index = this->getLeft(index); //gets index of left element
    unsigned right_index = this->getRight(index); //gets index of right element

    //if left node exists and left node smaller than smallest node value
    if (left_index < this->taxis.size() && smallest_element > this->taxis[left_index]) {
        smallest_index = left_index; //updates smallest node's index
        smallest_element = this->taxis[left_index]; //updates smallest node's value
    }
    //if right node exists and right node smaller than smallest node value
    if (right_index < this->taxis.size() && smallest_element > this->taxis[right_index]) {
        smallest_index = right_index;//updates smallest node's index
        smallest_element = this->taxis[right_index]; //updates smallest node's value
    }
    //Checks child nodes (left, right) smaller than current node or not
    if (smallest_index != index) {
        //Gets smaller node to the upper node
        swap(this->taxis[index], this->taxis[smallest_index]);
        //Calls MinHeapify for swapped node
        this->minHeapify(smallest_index);
    }
}

//Function that get minimum element of heap
double PriorityQueue::getNearestTaxi() {
    //If heap is empty, throws error
    if (this->taxis.size() < 1) {
        cout<< "There is not any taxi!"<<endl;
        exit(1);
    }
    double min_element = this->taxis[0]; //Takes smallest value of the heap
    this->taxis[0] = this->taxis[taxis.size() - 1]; //Places last element of heap as a root
    this->taxis.pop_back(); //Decreases size of heap
    this->minHeapify(0); //Calls MinHeapify for root
    return min_element; //Returns minimum element
}

//Function for decrease node value of heap
void PriorityQueue::decreaseTaxiDistance() {
    int index = 0;
    if(this->taxis.size()<=0){
        return;
    }
    if(this->taxis.size()>1){
        //Randomly chooses index from heap
        index = rand() % (this->taxis.size());
    }
    //if node's value smaller than 0.01
    if(this->taxis[index]<0.01){
        //Directly sets as 0
        this->taxis[index] = 0;
    }else{
        //decreases value with 0.01
        this->taxis[index] -= 0.01;
    }
    //if it is not a root and it is smaller than its parent
    while (index > 0 && this->taxis[getParent(index)] > this->taxis[index]) {
        //Places to parent node's position
        swap(this->taxis[getParent(index)], this->taxis[index]);
        index = getParent(index); //Updates index
    }
}

//Function for adding new element to the heap
void PriorityQueue::addTaxi(double distance) {
    //Adds new value to the end of the heap
    this->taxis.push_back(distance);

    int index = taxis.size() - 1; //takes index value of the last element

    //if it is not a root and it is smaller than its parent
    while (index > 0 && this->taxis[getParent(index)] > this->taxis[index]) {
        //Places to parent node's position
        swap(this->taxis[getParent(index)], this->taxis[index]);
        index = getParent(index); //Updates index
    }
}

//Function that finds index of parent
int PriorityQueue::getParent(int index) {
    return (index-1) >> 1;
}

//Function that finds index of left child
int PriorityQueue::getLeft(int index) {
    return (index << 1)+1;
}

//Function that finds index of right child
int PriorityQueue::getRight(int index) {
    return (2) + (index << 1);
}

//Function that reads file
void readFile(queue<double> *taxi_queue, int m) {
    ifstream file; //defines file pointer
    file.open("locations.txt"); //opens file

    if (!file) { //if it is not exist
        cerr << "File cannot be opened!";
        exit(1);
    }

    double longitude; //variable that keeps longitude value
    double latitude; //variable that keeps latitude value
    string line; //variable that keeps line of file
    int counter =0;
    getline(file, line); //reads header file
    while (counter<m) {
        file >> longitude; //reads longitude
        file >> latitude; //reads latitude
        //pushes the distance of taxi to priority queue
        taxi_queue->push(euclideanDistance(longitude, latitude));
        getline(file, line, '\n'); //reads breakline
        counter++;
    }
    file.close(); //closes file
}

int main(int argc, char *argv[]) {

    srand(time(NULL)); //seeds random function

    int m; //keeps input size
    double p; //probability of update

    if(argc == 3){ //checks command line argument number
        m = atoi(argv[1]); //reads argument and transforms to integer
        p = atof(argv[2]); //reads argument and transforms to double
    }else{ //if argument is not in proper format
        cerr << "Command line argument does not given in right format!";
        exit(1); //exits with failure
    }

    queue<double> input_queue; //Stores inputs
    readFile(&input_queue, m+5); //reads input file

    PriorityQueue *taxi_pq = new PriorityQueue(); //Initialized priority queue
    
    for(int i=0; i<5;i++){ //pq initialized with 5 taxis
        taxi_pq->addTaxi(input_queue.front()); //reads from input
        input_queue.pop(); //pops from input queue
    }

    int counter = 0; //counter keeps operation number

    int addition_counter = 0; //counter keeps addition number
    int update_counter = 0; //counter keeps update number

    vector<double> called_taxis; //vector that keeps distances of called taxis

    time_t timer = clock(); //starts time
    while (m--) {
        if (counter == 100) { //if it is 100th operation
            double taxi = taxi_pq->getNearestTaxi();
            called_taxis.push_back(taxi); //gets taxi and pushes to array
            counter = 0; //sets counter as 0
        }
        if (rand() % 100+1 <= p * 100 ) {
            taxi_pq->decreaseTaxiDistance(); //calls decrease taxi distance
            update_counter++; //increments update counter by 1
            counter++; //increments counter by 1
        } else {
            taxi_pq->addTaxi(input_queue.front()); //reads from input
            input_queue.pop(); //pops from input queue
            addition_counter++; //increments addition counter by 1
            counter++; //increments counter by 1
        }
    }
    timer = clock() - timer; //calculates time that is passed

    //prints called taxi distances
    for(unsigned i=0; i<called_taxis.size();i++){
        cout<<"Taxi called. Distance: "<<called_taxis[i]<<endl;
    }
    //prints expected outputs
    cout <<"Number of addition operations: "<< addition_counter <<endl;
    cout <<"Number of update operations: " << update_counter << endl;
    cout <<"Time that is passed: "<<timer <<" ms"<<endl;

    delete taxi_pq; //deallocates priority queue
    return 0;
}
