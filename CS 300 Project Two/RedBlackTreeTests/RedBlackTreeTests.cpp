#include "pch.h"
#include "CppUnitTest.h"
#include "RedBlackTree.hpp"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RedBlackTreeTests
{
	TEST_CLASS(RedBlackTreeTests)
	{
	public:
		
		//Example tree for testing
		std::unique_ptr<RedBlackTree> testTree() {
			auto rbt = std::make_unique<RedBlackTree>();
			Course c1, c2, c3, c4, c5;
			rbt->Insert(c1, 1);
			rbt->Insert(c2, 2);
			rbt->Insert(c3, 3);
			rbt->Insert(c4, 4);
			rbt->Insert(c5, 5);
			return rbt;
		}
		TEST_METHOD(TestRootIsNull){
			RedBlackTree rbt;
			Assert::IsNull(rbt.GetRoot());
		}

		TEST_METHOD(TestInsertOne) {
			RedBlackTree rbt;
			rbt.Insert(Course(), 1);
			Assert::IsTrue(rbt.GetRoot()->key == 1);
		}

		// Properties
		// 1. Node Color: Each node is either red or black
		// 2. Root Property: The root of the tree is always black
		// 3. Red property: Red nodes cannot have red children
		// 4. Black property: Every path from a node to its leaves has the same
		//		number of black nodes

		//Property 1
		void ValidateNodeColors(Node* node) {
			if (node == nullptr) return;
			Assert::IsTrue(node->nodeColor == Color::RED || node->nodeColor == Color::BLACK);
			ValidateNodeColors(node->left);
			ValidateNodeColors(node->right);
		}
		TEST_METHOD(TestAllNodesHaveColor) {
			//FIXME: remove test cout
			auto rbt = testTree();
			ValidateNodeColors(rbt->GetRoot());
			rbt->PrintPreOrderTraversal();
		}

		
		//Property 2
		TEST_METHOD(TestRootIsBlack) {
			auto rbt = testTree();
			Assert::IsTrue(rbt->GetRoot()->nodeColor == Color::BLACK);
		}

		//Property 3

		void ValidateRedChildren(Node* node) {
			if (node == nullptr) return;
			if (node->nodeColor == Color::RED) { // No need to check reds if the node is black
				Assert::IsTrue(node->left == nullptr || node->left->nodeColor == Color::BLACK);
				Assert::IsTrue(node->right == nullptr || node->right->nodeColor == Color::BLACK);
			}
			ValidateNodeColors(node->left);
			ValidateNodeColors(node->right);
		}
		TEST_METHOD(TestConsecutiveRedNodes) {
			auto rbt = testTree();
			ValidateRedChildren(rbt->GetRoot());
		}

		//Property 4
		int CheckBlackHeight(Node* node) {
			if (node == nullptr) {
				return 1; // Null nodes count as black
			}

			// Recursively check the left and right subtrees
			int leftBlackHeight = CheckBlackHeight(node->left);
			int rightBlackHeight = CheckBlackHeight(node->right);

			// Ensure the left and right subtrees have the same black height
			Assert::IsFalse(leftBlackHeight != rightBlackHeight);

			// Include the current node in the black height if it's black
			return leftBlackHeight + (node->nodeColor == Color::BLACK ? 1 : 0);
		}
		TEST_METHOD(TestBlackHeight) {
			auto rbt = testTree();
			CheckBlackHeight(rbt->GetRoot());
		}
		

	};
}
