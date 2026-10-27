#pragma once

#include <cassert>
#include <utility>
#include <random>
#include <climits>
#include <iostream>

#include "BinarySearchTree.h"
#include "ArrayQueue.h"

template <typename T>
struct TreapNode {
public:
	T data{};

	TreapNode<T>* parent = nullptr;
	TreapNode<T>* leftChild = nullptr;
	TreapNode<T>* rightChild = nullptr;

	std::pair<int, int> priority;

public:
	TreapNode()
		: data{}, parent(nullptr), leftChild(nullptr), rightChild(nullptr), priority({0 , 0}) {
	}

	TreapNode(T m_data)
		: data(m_data), parent(nullptr), leftChild(nullptr), rightChild(nullptr), priority({ 0 , 0 }) {
	}
};

template <typename T>
class Treap
{
private:
	TreapNode<T>* rootNode;
	int size;

private:
	std::mt19937 gen{ std::random_device{}() };
	std::uniform_int_distribution<int> dist{ 0, INT_MAX };
	int uniqueID = 0;

private:
	int generateNumber() {
		return dist(gen);
	}

private://从BinarySearchTree搬来的函数

	///<summary>
	/// <para>从该节点到其子树最深叶节点的路径长度。</para>
	/// <para>注意: 这本教材把树的 height（高度）定义为从当前节点到最深叶节点的路径所包含的边数</para>
	///</summary>
	int height(TreapNode<T>* theNode) {
		if (theNode == nullptr) { return -1; }//-1,因为这样叶节点高度才是 1+ (-1) = 0
		else {
			int leftHeight = height(theNode->leftChild);
			int rightHeight = height(theNode->rightChild);
			return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);//1+max(左子树高,右子树高)
		}
	}

	/// <summary>
	///<para>如果树为空，返回 nullptr。</para>
	///<para>如果 x 不存在，返回搜索路径上最后一个非空节点，也就是准备插入位置的父节点。</para>
	///<para>如果 x 已经存在，返回该节点本身。</para>
	/// </summary>
	/// <param name="x">节点编号</param>
	/// <returns>找到的节点指针</returns>
	TreapNode<T>* findLastNode(T x) {
		TreapNode<T>* temp = rootNode;
		TreapNode<T>* previous = nullptr;

		while (temp != nullptr) {
			previous = temp;
			if (x < temp->data) { temp = temp->leftChild; }
			else if (x > temp->data) { temp = temp->rightChild; }
			else { break; }//按理说这个不会触发
		}
		return previous;
	}

	/// <summary>
	///找相等的元素
	/// </summary>
	/// <param name="x">要找的数</param>
	/// <returns>指向找到的数所在节点的指针,没找到就是nullptr</returns>
	TreapNode<T>* findEQ(const T& x) {
		TreapNode<T>* temp = rootNode;
		while (temp != nullptr) {
			if (x == temp->data) { break; }
			else if (x < temp->data) { temp = temp->leftChild; }
			else { temp = temp->rightChild; }//(x > temp->data) 
		}
		return temp;
	}

	/// <summary>
	/// <para>在y(y>=x)后面添加x</para>
	/// <para>给新节点增加了随机的优先级</para>
	/// </summary>
	TreapNode<T>* add_binarySearchTree(T x) {

		TreapNode<T>* findTemp = findEQ(x);
		if (findTemp != nullptr) { return findTemp; }//已经存在x就不用插入了

		TreapNode<T>* theNewNode = new TreapNode<T>(x);

		theNewNode->priority = { generateNumber(), uniqueID };
		uniqueID++;

		TreapNode<T>* lastNode = findLastNode(x);

		if (lastNode == nullptr) {
			rootNode = theNewNode;//空树则直接成为根节点
			size++;
			return theNewNode;
		}

		theNewNode->parent = lastNode;
		if (x < lastNode->data) {
			lastNode->leftChild = theNewNode;
		}
		else {
			lastNode->rightChild = theNewNode;
		}

		size++;

		return theNewNode;
	}

	/// <summary>
	/// <para>将给定节点从树上摘下来</para>
	/// <para>注意: 只适用于没孩子或只有一个孩子的节点</para>
	/// <para>要求:给定的节点属于当前的这棵树</para>
	/// <para>不负责释放节点，也不修改 size。</para>
	/// </summary>
	/// <param name="theNode">给定节点</param>
	void splice(TreapNode<T>* theNode) {
		assert(theNode != nullptr);
		assert(!(theNode->leftChild != nullptr && theNode->rightChild != nullptr));


		TreapNode<T>* child = (theNode->leftChild != nullptr) ? theNode->leftChild : theNode->rightChild;

		TreapNode<T>* parent = theNode->parent;

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
	Treap()
		:rootNode(nullptr),size(0){ }

	~Treap() {
		if (rootNode == nullptr) { return; }

		ArrayQueue<TreapNode<T>*> sequence;
		sequence.add(rootNode);

		while (!sequence.isEmpty()) {
			TreapNode<T>* temp = sequence.remove();
			if (temp->leftChild != nullptr) { sequence.add(temp->leftChild); }
			if (temp->rightChild != nullptr) { sequence.add(temp->rightChild); }
			delete temp;
		}
	}

	Treap(const Treap&) = delete;

	Treap& operator=(const Treap&) = delete;

	/// <summary>
	/// <para>左旋函数</para>
	/// <para>提升传进来的节点theNode</para>
	/// <para>传进来的节点theNode得是右孩子</para>
	/// </summary>
	/// <param name="theNode">要提升的节点</param>
	void rotateLeft(TreapNode<T>* theNode) {

		if (theNode == nullptr) { return; }//theNode不能为空

		TreapNode<T>* oldParent = theNode->parent;

		if (oldParent == nullptr) { return; }//说明theNode是根节点，左旋没有意义且会爆炸

		assert(oldParent->rightChild == theNode);//theNode得是右孩子

		TreapNode<T>* oldChild = theNode->leftChild;

		theNode->parent = oldParent->parent;
		oldParent->parent = theNode;
		if (oldChild != nullptr) { oldChild->parent = oldParent; }

		if (theNode->parent != nullptr) {//oldParent原来不是根节点则更新theNode的父节点
			if (theNode->parent->leftChild == oldParent) { theNode->parent->leftChild = theNode; }
			else{ theNode->parent->rightChild = theNode; }
		}
		else {
			rootNode = theNode;
		}

		oldParent->rightChild = oldChild;
		theNode->leftChild = oldParent;
	}

	/// <summary>
	/// <para>右旋函数</para>
	/// <para>提升传进来的节点theNode</para>
	/// <para>传进来的节点theNode得是左孩子</para>
	/// </summary>
	/// <param name="theNode">要提升的节点</param>
	void rotateRight(TreapNode<T>* theNode) {

		if (theNode == nullptr) { return; }//theNode不能为空

		TreapNode<T>* oldParent = theNode->parent;

		if (oldParent == nullptr) { return; }//说明theNode是根节点，右旋没有意义且会爆炸

		assert(oldParent->leftChild == theNode);//theNode得是左孩子

		TreapNode<T>* oldChild = theNode->rightChild;

		theNode->parent = oldParent->parent;
		oldParent->parent = theNode;
		if (oldChild != nullptr) { oldChild->parent = oldParent; }

		if (theNode->parent != nullptr) {//oldParent原来不是根节点则更新theNode的父节点
			if (theNode->parent->leftChild == oldParent) { theNode->parent->leftChild = theNode; }
			else { theNode->parent->rightChild = theNode; }
		}
		else {
			rootNode = theNode;
		}

		oldParent->leftChild = oldChild;
		theNode->rightChild = oldParent;
	}

	/// <summary>
	/// <para>字面意思，在当前树中添加一个含x的节点</para>
	/// <para>如果当前树已经有含x的节点，就失败了不添加</para>
	/// </summary>
	/// <param name="x">要加的数据</param>
	/// <returns>一个bool值，判断add成功与否</returns>
	bool add(T x) {
		if (findEQ(x) != nullptr) { return false; }//已经存在含x的节点
		
		TreapNode<T>* theNewNode = add_binarySearchTree(x);

		while (theNewNode->parent != nullptr && theNewNode->priority < theNewNode->parent->priority) {
			if (theNewNode == theNewNode->parent->leftChild) { rotateRight(theNewNode); }
			else { rotateLeft(theNewNode); }
		}

		if (theNewNode->parent == nullptr) { rootNode = theNewNode; }

		return true;
	}

	/// <summary>
	/// <para>字面意思，在当前树中删除一个含x的节点</para>
	/// <para>如果当前树已经没有含x的节点，就失败了不添加</para>
	/// </summary>
	/// <param name="x">要删的数据</param>
	/// <returns>一个bool值，判断remove成功与否</returns>
	bool remove(T x) {
		TreapNode<T>* theNode = findEQ(x);
		if (theNode == nullptr) { return false; }//不存在含x的节点,没有删除的意义

		while (theNode->leftChild != nullptr && theNode->rightChild != nullptr) {//只要出来theNode不含有两个孩子就行
			if (theNode->leftChild->priority < theNode->rightChild->priority) {
				rotateRight(theNode->leftChild); //左孩子优先级高，右旋提升
			}
			else {
				rotateLeft(theNode->rightChild); //右孩子优先级高，左旋提升
			}
		}

		splice(theNode);
		delete theNode;
		size--;

		return true;
	}

	void print() {
		if (rootNode == nullptr) {
			std::cout << "========== Treap ==========\n";
			std::cout << "Tree is empty.\n";
			std::cout << "Size: " << size << '\n';
			std::cout << "===========================\n";
			return;
		}

		struct PrintItem {
			TreapNode<T>* node;
			int level;
			const char* side;
		};

		ArrayQueue<PrintItem> q;
		q.add({ rootNode, 0, "ROOT" });

		int currentLevel = -1;

		std::cout << "========== Treap ==========\n";
		std::cout << "Size: " << size << "\n\n";

		while (!q.isEmpty()) {
			PrintItem item = q.remove();
			TreapNode<T>* node = item.node;

			if (item.level != currentLevel) {
				currentLevel = item.level;
				std::cout << "\nLevel " << currentLevel << ":\n";
			}

			std::cout << "  [" << item.side << "] "
				<< "data=" << node->data
				<< "  priority=("
				<< node->priority.first << ", "
				<< node->priority.second << ")";

			if (node->parent != nullptr) {
				std::cout << "  parent=" << node->parent->data;
			}
			else {
				std::cout << "  parent=null";
			}

			std::cout << '\n';

			if (node->leftChild != nullptr) {
				q.add({ node->leftChild, item.level + 1, "L" });
			}

			if (node->rightChild != nullptr) {
				q.add({ node->rightChild, item.level + 1, "R" });
			}
		}

		std::cout << "\n===========================\n";
	}

	int theWholeTreeHeight() {
		return height(rootNode);
	}

};