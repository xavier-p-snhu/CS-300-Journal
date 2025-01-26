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

enum Color
{
    RED, BLACK
};

// forward declarations
double strToDouble(string str, char ch);
string colorToString(Color color);

// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string title;
    vector<string> prereqs;
    Course() {
    }
};

class RedBlackTree {

private:
    // Define structures to hold courses
    struct Node {
        Course course;
        unsigned int key;
        Color nodeColor;
        Node * left, * right, * parent;

        // default constructor
        Node() {
            key = UINT_MAX;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            nodeColor = Color::RED;
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

    Node* root;

    Node* Grandparent(Node* n) {
        if (n->parent != nullptr) {
            return n->parent;
        }
        return nullptr;
    }

    Node* Uncle(Node* n) {
        Node* g = Grandparent(n);
        if (g->parent == NULL) {
            return nullptr;
        }
        else if (g->left == n->parent) {
            return g->right;
        }
        else {
            return g->left;
        }
        return nullptr;
    }

    Node* Sibling(Node* n) {
        if (n == nullptr || n->parent == nullptr) {
            return nullptr;
        }
        if (n == n->parent->left) {
            return n->parent->right;
        }
        else {
            return n->parent->left;
        }
        return nullptr;
    }

    void RotateLeft(Node* n) {
        Node* r = n->right;
        if (r == nullptr) {
            return; // Cannot rotate left if there is no right child
        }

        n->right = r->left;
        if (r->left != nullptr) {
            r->left->parent = n;
        }

        r->parent = n->parent;
        if (n->parent == nullptr) {
            root = r; // Update the root if n was the root
        }
        else if (n == n->parent->left) {
            n->parent->left = r;
        }
        else {
            n->parent->right = r;
        }

        r->left = n;
        n->parent = r;
    }


    void RotateRight(Node* n) {
        Node* l = n->left;
        if (l == nullptr) {
            return; // Cannot rotate right if there is no left child
        }

        n->left = l->right;
        if (l->right != nullptr) {
            l->right->parent = n;
        }

        l->parent = n->parent;
        if (n->parent == nullptr) {
            root = l; // Update the root if n was the root
        }
        else if (n == n->parent->left) {
            n->parent->left = l;
        }
        else {
            n->parent->right = l;
        }

        l->right = n;
        n->parent = l;
    }

    void InsertFixup(Node* n) {
        while (n != root && n->parent->nodeColor == Color::RED) {
            Node* u = Uncle(n);
            if (u != nullptr && n->parent->nodeColor == Color::RED) {
                //Case 1: uncle is red
                n->parent->nodeColor = Color::BLACK;
                u->nodeColor = Color::BLACK;
                Grandparent(n)->nodeColor = Color::BLACK;
                n = Grandparent(n);
            }
            else {
                if (n->parent == Grandparent(n)->left) {
                    //Uncle is black, parent is left child
                    if (n == n->parent->right) {
                        n = n->parent;
                        RotateLeft(n);
                    }
                    n->parent->nodeColor = Color::BLACK;
                    Grandparent(n)->nodeColor = Color::RED;
                    RotateLeft(Grandparent(n));
                }
                else {
                    //Uncle is black, parent is right child
                    if (n == n->parent->left) {
                        n = n->parent;
                        RotateRight(n);
                    }
                    n->parent->nodeColor = Color::BLACK;
                    Grandparent(n)->nodeColor = Color::RED;
                    RotateLeft(Grandparent(n));
                }
            }
        }
        root->nodeColor = Color::BLACK;
    }

    void InOrderTraversal(Node* n, int depth = 0) {
        if (n == nullptr) {
            return;
        }
        InOrderTraversal(n->left);
        std::cout << n->key << " | " << n->course.title << " " << colorToString(n->nodeColor) << endl;
        InOrderTraversal(n->right);
    }

public:
    RedBlackTree();
    void Insert(Course course, int key);
    virtual ~RedBlackTree();
    Course Search(string courseId);
    void printInOrderTraversal();
};

/**
 * Default constructor
 */
RedBlackTree::RedBlackTree() {
    // FIXME (1): Initialize the structures used to hold courses
    // FIXME (1.5) Am I supposed to put something here?
}

/**
 * Destructor
 */
RedBlackTree::~RedBlackTree() {
    // FIXME (2): Implement logic to free storage when class is destroyed

    //loop through each node in the vector

    /*
    for (unsigned int i = 0; i < nodes.size(); i++) {
        Node* current = &nodes[i];
        //loop through the linked list of nodes
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
    */
}

//FIXME do the comment thing like above deconstructor
void RedBlackTree::Insert(Course course, int key) {
    Node* new_node = new Node(course, key);
    Node* current = root;
    Node* parent = nullptr;

    //Find the correct position to insert the node
    while (current != nullptr) {
        parent = current;
        if (new_node->key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    //Insert the node
    new_node->parent = parent;
    if (parent == nullptr) {
        root = new_node;
    }
    else if (new_node->key < parent->key) {
        parent->left = new_node;
    }
    else {
        parent->right = new_node;
    }
}

/**
 * Search for the specified courseId
 *
 * @param courseId The course id to search for
 */
Course RedBlackTree::Search(string courseId) {
    //FIXME rework for hashtable
    Course course;
    /*
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
    */

    return course;
}

void RedBlackTree::printInOrderTraversal()
{
    InOrderTraversal(root);
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
void loadcourses(string filePath, RedBlackTree* bst) {
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

            bst->Insert(course, stoi(course.courseId.substr(vec[0].length() - 3)));

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

string colorToString(Color color)
{
    switch (color)
    {
    case RED:
        return "RED";
        break;
    case BLACK:
        return "BLACK";
        break;
    }
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    string csvPath, courseKey;
    csvPath = "courses.txt";
    courseKey = "400";

    // Define a timer variable
    clock_t ticks;

    RedBlackTree* rbt;
    Course course;
    rbt = new RedBlackTree;

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
            loadcourses(csvPath, rbt);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        case 2:
            rbt->printInOrderTraversal();
            break;
        case 3:
            //FIXME: remove case 3, or change it
            cout << "Not implemented :(" << endl;
            //rbt->PrintAllAlphanumeric();
            break;
        case 4:
            ticks = clock();

            course = rbt->Search(courseKey);

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
