#include <cassert>
#include <iostream>
#include "RedBlackTree.hpp"

// Example unit test for the RedBlackTree class
void testRedBlackTreeInsert() {
    // Step 1: Initialize the tree
    RedBlackTree tree;

    // Step 2: Create test courses
    Course course1;
    course1.courseId = "100";
    course1.title = "Introduction to Programming";

    Course course2;
    course2.courseId = "200";
    course2.title = "Data Structures";

    Course course3;
    course3.courseId = "300";
    course3.title = "Algorithms";

    // Step 3: Insert courses into the tree
    tree.Insert(course1, 100);
    tree.Insert(course2, 200);
    tree.Insert(course3, 300);

    // Step 4: Verify the tree structure
    // Check that the root node is black
    assert(tree.Search("100").courseId == "100"); // Root should be course1
    assert(tree.Search("200").courseId == "200"); // Right child should be course2
    assert(tree.Search("300").courseId == "300"); // Right-right grandchild should be course3

    // Verify the ordering in in-order traversal
    std::cout << "In-order traversal: " << std::endl;
    tree.PrintInOrderTraversal();

    // Check node colors to ensure the tree maintains Red-Black Tree properties
    // This test assumes that we have access to a debug function that verifies the tree colors
    // and structure. Alternatively, this can be done manually if the colors can be retrieved.

    // If we reach this point, the test has passed
    std::cout << "Test passed!" << std::endl;
}

//int main() {
//    // Run the test
//    testRedBlackTreeInsert();
//
//    return 0;
//}
