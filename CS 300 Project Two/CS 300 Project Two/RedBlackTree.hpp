#ifndef REDBLACKTREE_HPP
#define REDBLACKTREE_HPP

#include <string>
#include <vector>

using namespace std;

enum Color
{
    RED, BLACK
};

// Forward declarations
double strToDouble(string str, char ch);
string colorToString(Color color);

// Define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string title;
    vector<string> prereqs;

    Course();
    Course(string courseId, string title);
};

// Define structures to hold courses
struct Node {
    Course course;
    unsigned int key;
    Color nodeColor;
    Node* left, * right, * parent;

    // Default constructor
    Node();
    // Initialize with a course
    Node(Course aCourse);
    // Initialize with a course and a key
    Node(Course aCourse, unsigned int aKey);
};

class RedBlackTree {
private:
    Node* root;

    Node* Grandparent(Node* n);
    Node* Uncle(Node* n);
    Node* Sibling(Node* n);
    void RotateLeft(Node* n);
    void RotateRight(Node* n);
    void InsertFixup(Node* n);
    void InOrderTraversal(Node* n, int depth);
    void PreOrderTraversal(Node* n, int depth);
    void PrintNode(Node* n, int depth);
    void FreeMemory(Node* node);

public:
    RedBlackTree();
    void Insert(Course course, int key);
    virtual ~RedBlackTree();
    Course* Search(string courseId);
    void PrintInOrderTraversal();
    void PrintPreOrderTraversal();
    Node* GetRoot();
    Node* findNodeByCourse(string courseId);
    void deleteNode(string courseId);
    void exportToDatabase();
    void importFromDatabase();
};

#endif // REDBLACKTREE_HPP
