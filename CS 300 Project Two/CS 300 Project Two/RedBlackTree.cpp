#include <string>
#include <iostream>
#include <vector>
#include "RedBlackTree.hpp"

using namespace std;

// forward declarations
double strToDouble(string str, char ch);
string colorToString(Color color);

// Constructors
Course::Course() : courseId(""), title(""), prereqs() {}

Node::Node()
{
}

Node::Node(Course aCourse)
{
}

Node::Node(Course aCourse, unsigned int aKey)
    : course(aCourse), key(aKey), left(nullptr), right(nullptr), parent(nullptr), nodeColor(Color::RED) {}

RedBlackTree::RedBlackTree() : root(nullptr) {}

// Destructor
RedBlackTree::~RedBlackTree() {
    FreeMemory(root);
}

// Print Node info (for traversal functions)
void RedBlackTree::PrintNode(Node* n,int depth)
{
    std::cout << depth << " " << n->key << " | " << n->course.title << " " << colorToString(n->nodeColor) << endl;
}

// In-Order traversal
void RedBlackTree::InOrderTraversal(Node* n, int depth = 0) {
    if (n == nullptr) {
        return;
    }
    InOrderTraversal(n->left, depth + 1);
    PrintNode(n, depth);
    InOrderTraversal(n->right, depth + 1);
}

// Pre-Order Traversal
void RedBlackTree::PreOrderTraversal(Node* n, int depth = 0) {
    if (n == nullptr) {
        return;
    }
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
Course RedBlackTree::Search(string courseId) {
    Node* current = root;
    while (current != nullptr) {
        if (current->course.courseId == courseId) {
            return current->course;
        }
        else if (courseId < current->course.courseId) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return Course(); // Return a default Course if not found
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

// Free memory
void RedBlackTree::FreeMemory(Node* node) {
    if (node == nullptr) return;
    FreeMemory(node->left);
    FreeMemory(node->right);
    delete node;
}

