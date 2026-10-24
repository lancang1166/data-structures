#pragma once

#include <cassert>

#include "ArrayQueue.h"

template <typename T>
struct BTNode {
public:
	T data;
	BTNode<T>* parent;
	BTNode<T>* leftChild;
	BTNode<T>* rightChild;

public:
	BTNode() 
		: data{}, parent(nullptr),leftChild(nullptr), rightChild(nullptr) {}

	BTNode(T m_data)
		: data(m_data), parent(nullptr), leftChild(nullptr), rightChild(nullptr) { }
};

template <typename T>
class BinaryTree {
private:
	BTNode<T>* rootNode;
	int size;//记录有多少个节点(每个节点只有一个数据)

public:
	BinaryTree()
		: rootNode(nullptr), size(0) {}

	~BinaryTree() {

		if (rootNode == nullptr) {return;}

		ArrayQueue<BTNode<T>*> sequence;
		sequence.add(rootNode);

		while (!sequence.isEmpty()) {
			BTNode<T>* temp = sequence.remove();
			if (temp->leftChild != nullptr) { sequence.add(temp->leftChild); }
			if (temp->rightChild != nullptr) { sequence.add(temp->rightChild); }
			delete temp;
		}
	}

	BinaryTree(const BinaryTree&) = delete;
	BinaryTree& operator=(const BinaryTree&) = delete;

	BTNode<T>* getRoot() const { return rootNode; }

	BTNode<T>* addRoot(T x) {
		assert(rootNode == nullptr);

		rootNode = new BTNode<T>(x);
		size = 1;

		return rootNode;
	}

	BTNode<T>* addLeftChild(BTNode<T>* parent, T x) {
		assert(parent != nullptr);

		BTNode<T>* theNode = new BTNode<T>(x);
		theNode->parent = parent;
		theNode->leftChild = parent->leftChild;

		parent->leftChild = theNode;
		if (theNode->leftChild != nullptr){ theNode->leftChild->parent = theNode; }
		
		size++;

		return theNode;
	}
	BTNode<T>* addRightChild(BTNode<T>* parent, T x) {
		assert(parent != nullptr);

		BTNode<T>* theNode = new BTNode<T>(x);
		theNode->parent = parent;
		theNode->rightChild = parent->rightChild;

		parent->rightChild = theNode;
		if (theNode->rightChild != nullptr) { theNode->rightChild->parent = theNode; }

		size++;

		return theNode;
	}

	///<summary>
	///<para>以给定节点为根，返回剩下的树高</para>
	///<para>注意: depth只接受本树中有效的非空节点</para>
	///</summary>
	int depth(BTNode<T>* theNode) {

		if (theNode == nullptr) { return 0; };//theNode为空

		int step = 0;
		BTNode<T>* temp = theNode;
		while (temp != rootNode) {
			temp = temp->parent;
			step++;
		}
		return step;
	}

	int size_recursion(BTNode<T>* theNode) {
		if (theNode == nullptr) { return 0; }
		else { return 1 + size_recursion(theNode->leftChild) + size_recursion(theNode->rightChild); }
	}

	//同函数preorderTraverse_iteration的思路
	int size_iteration(BTNode<T>* theNode) {
		int count = 0;

		if (theNode == nullptr) { return count; }

		BTNode<T>* temp = theNode;
		BTNode<T>* come_from = theNode->parent;
		BTNode<T>* next_target;

		while (temp != theNode->parent) {
			if (come_from == temp->parent) {
				if (temp->leftChild != nullptr) { next_target = temp->leftChild; }
				else if (temp->rightChild != nullptr) { next_target = temp->rightChild; }
				else { next_target = temp->parent; }
			}
			else if (come_from == temp->leftChild) {
				if (temp->rightChild != nullptr) { next_target = temp->rightChild; }
				else { next_target = temp->parent; }
			}
			else {//come_from = temp->rightChild
				next_target = temp->parent;
			}

			if (come_from == temp->parent) {//只有从父母来的才访问，这才是前序
				count++;
			}

			come_from = temp;
			temp = next_target;
		}

		return count;
	}

	///<summary>
	///注意: 这本教材把树的 height（高度）定义为从当前节点到最深叶节点的路径所包含的边数
	///</summary>
	int height(BTNode<T>* theNode) {
		if (theNode == nullptr) { return -1; }//-1,因为这样叶节点高度才是 1+ (-1) = 0
		else { 
			int leftHeight = height(theNode->leftChild);
			int rightHeight = height(theNode->rightChild);
			return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);//1+max(左子树高,右子树高)
		}
	}

	void preorderTraverse_recursion(BTNode<T>* theNode) {
		if (theNode == nullptr) { return; }

		visit(theNode);
		preorderTraverse_recursion(theNode->leftChild);
		preorderTraverse_recursion(theNode->rightChild);
	}

	void preorderTraverse_iteration(BTNode<T>* theNode) {
		
		if (theNode == nullptr) { return; }

		BTNode<T>* temp = theNode;
		BTNode<T>* come_from = theNode->parent;
		BTNode<T>* next_target;

		while (temp != theNode->parent) {
			if (come_from == temp->parent) {
				if (temp->leftChild != nullptr) { next_target = temp->leftChild; }
				else if (temp->rightChild != nullptr) { next_target = temp->rightChild; }
				else { next_target = temp->parent; }
			}
			else if (come_from == temp->leftChild) {
				if (temp->rightChild != nullptr) { next_target = temp->rightChild; }
				else { next_target = temp->parent; }
			}
			else{//come_from = temp->rightChild
				next_target = temp->parent;
			}

			if (come_from == temp->parent) {//只有从父母来的才访问，这才是前序
				visit(temp);
			}

			come_from = temp;
			temp = next_target;
		}
	}

	///<summary>广度优先遍历</summary>
	void breadthFirstTraverse(BTNode<T>* theNode) {
		if (theNode == nullptr) { return; }
		
		ArrayQueue<BTNode<T>*> sequence;
		sequence.add(theNode);

		while (!sequence.isEmpty()) {
			BTNode<T>* temp = sequence.remove();
			if (temp->leftChild != nullptr) { sequence.add(temp->leftChild); }
			if (temp->rightChild != nullptr) { sequence.add(temp->rightChild); }
			visit(temp);
		}
	}

	void visit(BTNode<T>* theNode) {
		
	}
};