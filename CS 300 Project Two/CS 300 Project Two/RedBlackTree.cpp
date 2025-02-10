#include <string>
#include <iostream>
#include <vector>
#include "RedBlackTree.hpp"
#include "StringToKey.hpp"
#include <SQLAPI.h>

using namespace std;

// forward declarations
double strToDouble(string str, char ch);
string colorToString(Color color);

//mySQL info
const char* sDBString = "@courses";
const char* sUserID = "root";
const char* sPassword = "password";

// Constructors
Course::Course() 
    : courseId(""), title(""), prereqs() {}
Course::Course(string courseId, string title)
    : courseId(courseId), title(title) {};

Node::Node()
    : Node(Course()) {}

Node::Node(Course aCourse)
    : Node(aCourse, 0) {}

Node::Node(Course aCourse, unsigned int aKey)
    : course(aCourse), key(aKey), left(nullptr), right(nullptr), parent(nullptr), nodeColor(Color::RED) {}

RedBlackTree::RedBlackTree() : root(nullptr) {}

void insertNodeInDatabase(Node* node, SAConnection &con) {
    //fixme: insert courses via json
    SACommand insert(&con, _TSA("INSERT INTO COURSETABLE (ikey, courseId, title, color) VALUES (:1, :2, :3, :4)"));
    if (node == nullptr) return;
    insert << node->key, node->course.courseId, node->course.title, node->nodeColor;
    insert.Param(1).setAsInt64() = node->key;
    insert.Param(2).setAsString() = node->course.courseId.c_str();
    insert.Param(3).setAsString() = node->course.title.c_str();
    insert.Param(4).setAsString() = colorToString(node->nodeColor).c_str();

    insert.Execute();
    insertNodeInDatabase(node->left, con);
    insertNodeInDatabase(node->right, con);
};

void RedBlackTree::exportToDatabase() {
    try {
        // Create a connection object.
        SAConnection con;

        // Connect using SA_MySQL_Client as the client type.
        con.Connect(sDBString, sUserID, sPassword, SA_MySQL_Client);

        // -- Perform database operations here --
        insertNodeInDatabase(RedBlackTree::root, con);


        // Disconnect from the database.
        con.Disconnect();
        std::cout << "Successfully exported to database." << std::endl;
    }
    catch (SAException& x) {
        // Roll back any pending transactions (if necessary)
        try {
            // In case a network error or other issue occurred,
            // you might want to attempt a rollback.
            // (Rollback() itself may throw an exception.)
            // Note: Rollback() is optional if no transaction is pending.
            SAConnection con; // You would normally use the same connection object.
            con.Rollback();
        }
        catch (SAException&) {
            // Ignore additional exceptions from rollback.
        }
        std::cerr << "Error connecting to database: "
            << x.ErrText().GetMultiByteChars() << std::endl;
    }
}

void RedBlackTree::importFromDatabase() {
    try {
        // Create a connection object.
        SAConnection con;

        // Connect using SA_MySQL_Client as the client type.
        con.Connect(sDBString, sUserID, sPassword, SA_MySQL_Client);

        std::cout << "Connected to MySQL database!" << std::endl;

        // -- Perform database operations here --
        SACommand select(&con, _TSA("SELECT courseId, title FROM COURSETABLE"));

        select.Execute();

        while (select.FetchNext()) {
            //SAString sName = select[1].asString();
            //long nAge = select[2].asLong();
            Insert(Course((select[1].asString().GetMultiByteChars()), (select[2].asString().GetMultiByteChars())), stringToKey(select[1].asString().GetMultiByteChars()));
        }

        // Disconnect from the database.
        con.Disconnect();
        std::cout << "Successfully inserted courses from database." << std::endl;
    }
    catch (SAException& x) {
        // Roll back any pending transactions (if necessary)
        try {
            // In case a network error or other issue occurred,
            // you might want to attempt a rollback.
            // (Rollback() itself may throw an exception.)
            // Note: Rollback() is optional if no transaction is pending.
            SAConnection con; // You would normally use the same connection object.
            con.Rollback();
        }
        catch (SAException&) {
            // Ignore additional exceptions from rollback.
        }
        std::cerr << "Error connecting to database: "
            << x.ErrText().GetMultiByteChars() << std::endl;
    }
}

// Free memory
void RedBlackTree::FreeMemory(Node* node) {
    if (node == nullptr) return;

    // Recursively free left and right children
    FreeMemory(node->left);
    FreeMemory(node->right);

    // Delete current node
    delete node;

    // Nullify node to avoid dangling pointers
    node = nullptr;
}


// Destructor
RedBlackTree::~RedBlackTree() {
    FreeMemory(root);
    root = nullptr;
}


// Print Node info (for traversal functions)
void RedBlackTree::PrintNode(Node* n,int depth)
{
    std::cout << "Depth: " << depth << "; Key: " << n->key << "; Title: " << n->course.title << ";  Color: " << colorToString(n->nodeColor) << endl;
}

// In-Order traversal
void RedBlackTree::InOrderTraversal(Node* n, int depth = 0) {
    // Todo: instead of printing, return all of the course information
    if (n == nullptr) return;
    InOrderTraversal(n->left, depth + 1);
    PrintNode(n, depth);
    InOrderTraversal(n->right, depth + 1);
}

// Pre-Order Traversal
void RedBlackTree::PreOrderTraversal(Node* n, int depth = 0) {
    // Todo: instead of printing, return all of the course information
    if (n == nullptr) return;
    PrintNode(n, depth);
    PreOrderTraversal(n->left, depth + 1);
    PreOrderTraversal(n->right, depth + 1);
}

// Print In-order
void RedBlackTree::PrintInOrderTraversal() {
    InOrderTraversal(root, 0);
}

// Print Pre-order
void RedBlackTree::PrintPreOrderTraversal() {
    PreOrderTraversal(root, 0);
}

Node* RedBlackTree::GetRoot()
{
    return root;
}

// Insert fixup
void RedBlackTree::InsertFixup(Node* n) {
    while (n != root && n->parent->nodeColor == Color::RED) {
        Node* u = Uncle(n);
        Node* g = Grandparent(n);

        if (u != nullptr && u->nodeColor == Color::RED) { // Case 1: Uncle is RED
            n->parent->nodeColor = Color::BLACK;
            u->nodeColor = Color::BLACK;
            g->nodeColor = Color::RED;
            n = g;
        }
        else { // Uncle is BLACK
            if (n->parent == g->left) {
                if (n == n->parent->right) { // Case 2: Node is right child
                    n = n->parent;
                    RotateLeft(n);
                }
                // Case 3: Node is left child
                n->parent->nodeColor = Color::BLACK;
                g->nodeColor = Color::RED;
                RotateRight(g);
            }
            else {
                if (n == n->parent->left) { // Case 2 (mirror): Node is left child
                    n = n->parent;
                    RotateRight(n);
                }
                // Case 3 (mirror): Node is right child
                n->parent->nodeColor = Color::BLACK;
                g->nodeColor = Color::RED;
                RotateLeft(g);
            }
        }
    }
    root->nodeColor = Color::BLACK;
}

// Insert
void RedBlackTree::Insert(Course course, int key) {
    Node* new_node = new Node(course, key);
    new_node->nodeColor = Color::RED;
    Node* current = root;
    Node* parent = nullptr;

    while (current != nullptr) {
        parent = current;
        if (new_node->key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

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

    InsertFixup(new_node);
}

// Search
Course* RedBlackTree::Search(string courseId) {
    Node* node = findNodeByCourse(courseId);
    if (node == nullptr) return nullptr;
    return &node->course;
}

// Rotate left
void RedBlackTree::RotateLeft(Node* n) {
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

// Rotate right
void RedBlackTree::RotateRight(Node* n) {
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

// Grandparent
Node* RedBlackTree::Grandparent(Node* n) {
    if (n != nullptr && n->parent != nullptr) {
        return n->parent->parent;
    }
    return nullptr;
}

// Uncle
Node* RedBlackTree::Uncle(Node* n) {
    Node* g = Grandparent(n);
    if (g == nullptr) return nullptr;
    if (n->parent == g->left) {
        return g->right;
    }
    else {
        return g->left;
    }
}

// Sibling
Node* RedBlackTree::Sibling(Node* n) {
    if (n == nullptr || n->parent == nullptr) {
        return nullptr;
    }
    if (n == n->parent->left) {
        return n->parent->right;
    }
    else {
        return n->parent->left;
    }
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
    return"NIL";
}

Node* RedBlackTree::findNodeByCourse(string courseId) {
    Node* current = root;
    int key = stringToKey(courseId);
    while (current != nullptr) {
        if (current->key == key) {
            return current;
        }
        else if (key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return nullptr;
}

Node* getSuccessor(Node* curr) {
    curr = curr->right;
    while (curr != nullptr && curr->left != nullptr)
        curr = curr->left;
    return curr;
}

void RedBlackTree::deleteNode(string courseId) {
    Node* nodeToDelete = findNodeByCourse(courseId);
    Node* replacementNode = nullptr;
    if (nodeToDelete == nullptr) return;

    //
    // Step 1: follow BST deletion rules
    //

    if (nodeToDelete->left != nullptr && nodeToDelete->right != nullptr) {
        replacementNode = getSuccessor(nodeToDelete);

        //Ensure hanging porinters to replacementNode are prevented
        if (replacementNode->parent != nodeToDelete) {
            if (replacementNode == replacementNode->parent->left) {
                replacementNode->parent->left = nullptr;
            }
            else {
                replacementNode->parent->right = nullptr;
            }
        }
    }
    else if (nodeToDelete->left != nullptr) {
        replacementNode = nodeToDelete->left;
    }
    else if (nodeToDelete->right != nullptr) {
        replacementNode = nodeToDelete->right;
    }

    // Ensure nodeToDelete's parent knows of nodeToDelete's replacement
    if (nodeToDelete == root) {
        root = replacementNode;
    }
    else if (nodeToDelete == nodeToDelete->parent->left) {
        nodeToDelete->parent->left = replacementNode;
    }
    else {
        nodeToDelete->parent->right = replacementNode;
    }

    //
    // Step 2: Delete node and fix if neccecary
    //
    InsertFixup(nodeToDelete->parent);
    delete nodeToDelete;
    if (replacementNode != nullptr) {
        InsertFixup(replacementNode);
    }
}

