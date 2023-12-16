//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : JYour name
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include "CSVparser.hpp"

using namespace std;
using std::vector;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

int DEFAULT_SIZE = 4;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string title;
    vector<string> prereqs;
    Course() {
    }
};

class HashTable {

private:
    // Define structures to hold courses
    struct Node {
        Course course;
        unsigned int key;
        Node* next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a course
        Node(Course aCourse) : Node() {
            course = aCourse;
        }

        // initialize with a course and a key
        Node(Course acourse, unsigned int aKey) : Node(acourse) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Course course);
    void PrintAll();
    void PrintAllAlphanumeric();
    Course Search(string courseId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // FIXME (1): Initialize the structures used to hold courses

    // Initalize node structure by resizing tableSize
    nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // invoke local tableSize to size with this->
    tableSize = size;
    // resize nodes size
    nodes.resize(tableSize);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    // FIXME (2): Implement logic to free storage when class is destroyed

    //loop through each node in the vector
    for (unsigned int i = 0; i < nodes.size(); i++) {
        Node* current = &nodes[i];
        //loop through the linked list of nodes
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // FIXME (3): Implement logic to calculate a hash value

    //returns key modulus tableSize
    //returns a value from 0 to tableSize - 1
    return key % tableSize;
}

/**
 * Insert a course
 *
 * @param course The course to insert
 */
void HashTable::Insert(Course course) {
    // FIXME (5): Implement logic to insert a course
    // create the key for the given course
    int courseKey = hash(int(course.courseId.at(4)));
    // retrieve node using key
    Node* node = &nodes[courseKey];

    // if no entry found for the key
    if (node->key == UINT_MAX) {
        // assign the course to the node
        node->course = course;
    }
    // else find the next open node
    else {
        while (node->next != nullptr)
        {
            node = node->next;
        }
        // add new newNode to end
        node->next = new Node(course);
    }

    //make sure node has the key
    node->key = courseKey;
}

void HashTable::PrintAll()
{
    // for node begin to end iterate
    for (int i = 0; i < nodes.size(); i++) {

        cout << i << ":";
        //   if key not equal to UINT_MAx
        if (nodes[i].key != UINT_MAX) {
            Node* node = &nodes[i];
            // while node not equal to nullptr
            while (node != nullptr) {
                // output key, courseID, title, amount and fund
                Course course = node->course;
                cout << course.courseId << " ";
                // node is equal to next node
                node = node->next;
            }
        }
        cout << endl;
    }
}

void HashTable::PrintAllAlphanumeric()
{
    vector<string> courses;
    // for node begin to end iterate
    for (int i = 0; i < nodes.size(); i++) {
        //   if key not equal to UINT_MAx
        if (nodes[i].key != UINT_MAX) {
            Node* node = &nodes[i];
            // while node not equal to nullptr
            while (node != nullptr) {
                courses.push_back(node->course.courseId);
                // node is equal to next node
                node = node->next;
            }
        }
    }
    sort(courses.begin(), courses.end());
    
    for (int i = 0; i < courses.size(); i++) {
        cout << courses[i] << endl;
    }
}

/**
 * Search for the specified courseId
 *
 * @param courseId The course id to search for
 */
Course HashTable::Search(string courseId) {
    Course course;

    // create the key for the given course
    int hashKey = hash(int(courseId.at(4)));
    // if entry found for the key
    if (nodes.at(hashKey).key != UINT_MAX) {
        //node to search for a matching course
        Node* searchingNode = &nodes.at(hashKey);
        //loop through the nodes
        while (searchingNode != nullptr) {

            //if the node's course matches
            if (searchingNode->course.courseId == courseId) {
                //return node's course
                return searchingNode->course;
            }
            searchingNode = searchingNode->next;
        }
    }

    return course;
}

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    cout << course.courseId << ": " << course.title << " | Prerequisites: ";
    for (int i = 0; i < course.prereqs.size(); i++) {
        cout << course.prereqs[i] << " ";
    }
    cout << endl;
    return;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadcourses(string filePath, HashTable* bst) {
    cout << "Loading file " << filePath << endl;

    ifstream file (filePath);
    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            istringstream iss(line);
            string item;
            char delim = ',';
            vector<string> vec;
            while (getline(iss, item, delim)) {
                vec.push_back(item);
            }

            Course course;
            course.courseId = vec[0];
            course.title = vec[1];

            for (int i = 2; i < vec.size(); i++) {
                course.prereqs.push_back(vec[i]);
            }
            bst->Insert(course);

        }
        file.close();
    }

    else cout << "Unable to open file";
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    string csvPath, courseKey;
    csvPath = "courses.txt";
    courseKey = "CSCI400";

    // Define a timer variable
    clock_t ticks;

    HashTable* ht;
    Course course;
    ht = new HashTable;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Display All Courses Alphabetically" << endl;
        cout << "  4. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:

            // Initialize a timer variable before loading courses
            ticks = clock();

            // Complete the method call to load the courses
            loadcourses(csvPath, ht);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        case 2:
            ht->PrintAll();
            break;
        case 3:
            ht->PrintAllAlphanumeric();
            break;
        case 4:
            ticks = clock();

            course = ht->Search(courseKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!course.courseId.empty()) {
                displayCourse(course);
            }
            else {
                cout << "course Id " << courseKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
