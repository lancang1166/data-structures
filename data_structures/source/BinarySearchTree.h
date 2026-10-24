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
		: data{}, parent(nullptr), leftChild(nullptr), rightChild(nullptr) {
	}

	BTNode(T m_data)
		: data(m_data), parent(nullptr), leftChild(nullptr), rightChild(nullptr) {
	}
};

template <typename T>
class BinarySearchTree {
private:
	BTNode<T>* rootNode;
	int size;//记录有多少个节点(每个节点只有一个数据)

public:
	int getSize() const { return size; }

	///<summary>
	///<para>从整棵树的根节点到该节点的路径长度。</para>
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
	/// <para>从该节点到其子树最深叶节点的路径长度。</para>
	/// <para>注意: 这本教材把树的 height（高度）定义为从当前节点到最深叶节点的路径所包含的边数</para>
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
			else {//come_from = temp->rightChild
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

	/// <summary>
	///<para>如果树为空，返回 nullptr。</para>
	///<para>如果 x 不存在，返回搜索路径上最后一个非空节点，也就是准备插入位置的父节点。</para>
	///<para>如果 x 已经存在，返回该节点本身。</para>
	/// </summary>
	/// <param name="x">节点编号</param>
	/// <returns>找到的节点指针</returns>
	BTNode<T>* findLastNode(T x) {
		BTNode<T>* temp = rootNode;
		BTNode<T>* previous = nullptr;

		while (temp != nullptr) {
			previous = temp;
			if (x < temp->data) { temp = temp->leftChild; }
			else if (x > temp->data) { temp = temp->rightChild; }
			else { break; }//按理说这个不会触发
		}
		return previous;
	}

	/// <summary>
	/// <para>将给定节点从树上摘下来</para>
	/// <para>注意: 只适用于没孩子或只有一个孩子的节点</para>
	/// <para>要求:给定的节点属于当前的这棵树</para>
	/// <para>不负责释放节点，也不修改 size。</para>
	/// </summary>
	/// <param name="theNode">给定节点</param>
	void splice(BTNode<T>* theNode) {
		assert(theNode != nullptr);
		assert(!(theNode->leftChild != nullptr && theNode->rightChild != nullptr));


		BTNode<T>* child = (theNode->leftChild != nullptr) ? theNode->leftChild : theNode->rightChild;

		BTNode<T>* parent = theNode->parent;

		// 更新父节点或根节点
		if (parent == nullptr) {//此时theNode就是rootNode
			rootNode = child;
		}
		else if (parent->leftChild == theNode) {
			parent->leftChild = child;
		}
		else {
			assert(parent->rightChild == theNode);
			parent->rightChild = child;
		}

		// 更新孩子的父指针
		if (child != nullptr) {
			child->parent = parent;
		}

		theNode->parent = nullptr;
		theNode->leftChild = nullptr;
		theNode->rightChild = nullptr;
	}

public:
	BinarySearchTree()
			: rootNode(nullptr), size(0) {
		}

	~BinarySearchTree() {

			if (rootNode == nullptr) { return; }

			ArrayQueue<BTNode<T>*> sequence;
			sequence.add(rootNode);

			while (!sequence.isEmpty()) {
				BTNode<T>* temp = sequence.remove();
				if (temp->leftChild != nullptr) { sequence.add(temp->leftChild); }
				if (temp->rightChild != nullptr) { sequence.add(temp->rightChild); }
				delete temp;
			}
		}

	BinarySearchTree(const BinarySearchTree&) = delete;
	BinarySearchTree& operator=(const BinarySearchTree&) = delete;

	BTNode<T>* getRoot() const { return rootNode; }

	/// <summary>
	///找相等的元素
	/// </summary>
	/// <param name="x">要找的数</param>
	/// <returns>指向找到的数所在节点的指针,没找到就是nullptr</returns>
	BTNode<T>* findEQ(const T& x) {
			BTNode<T>* temp = rootNode;
			while (temp != nullptr) {
				if (x == temp->data) { break; }
				else if (x < temp->data) { temp = temp->leftChild; }
				else{ temp = temp->rightChild; }//(x > temp->data) 
			}
			return temp;
		}

	/// <summary>
	/// 找大于等于x的元素
	/// </summary>
	/// <param name="x">参数x</param>
	/// <returns>指向找到的数所在节点的指针,没找到就是nullptr</returns>
	BTNode<T>* find(const T& x) {
			BTNode<T>* temp = rootNode;
			BTNode<T>* targetNode = nullptr;
			while (temp != nullptr) {
				if (x <= temp->data) { 
					targetNode = temp;
					temp = temp->leftChild; 
				}
				else { temp = temp->rightChild; }//(x > temp->data) 
			}
			return targetNode;
		}

	/// <summary>
	/// 在y(y>=x)后面添加x
	/// </summary>
	bool add(T x) {
			if (findEQ(x) != nullptr) { return true; }//已经存在x就不用插入了

			BTNode<T>* theNewNode = new BTNode<T>(x);
			BTNode<T>* lastNode = findLastNode(x);

			if (lastNode == nullptr) {
				rootNode = theNewNode;//空树则直接成为根节点
				size++;
				return true;
			}

			theNewNode->parent = lastNode;
			if (x < lastNode->data) {
				lastNode->leftChild = theNewNode;
			}
			else {
				lastNode->rightChild = theNewNode;
			}

			size++;

			return true;
		}

	/// <summary>
	/// <para>将给定的节点删掉</para>
	/// <para>要求:给定的节点属于当前的这棵树</para>
	/// </summary>
	/// <param name="theNode">给定节点的指针</param>
	void remove(BTNode<T>* theNode) {
		assert(theNode != nullptr);

		if (!(theNode->leftChild != nullptr && theNode->rightChild != nullptr)) {
			splice(theNode);
			delete theNode;
			size--;
			return;
		}

		//首先找到比theNode大的数里的最小的节点
		BTNode<T>* successor = theNode->rightChild;

		while (successor->leftChild != nullptr) {
			successor = successor->leftChild;
		}

		BTNode<T>* theSmallest_butAboveTheNode = successor;

		//theSmallest_butAboveTheNode只有两种情况
		//要么是叶子，要么有一个有孩子，均可以依靠splice解决
		splice(theSmallest_butAboveTheNode);
		theNode->data = theSmallest_butAboveTheNode->data;
		delete theSmallest_butAboveTheNode;
		size--;
	}
};