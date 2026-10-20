#pragma once

#include<cassert>

template <typename T>
class DLList {//Doubly-Linked List
private:
	struct Node {
		T data;
		Node* previous;
		Node* next;
	};

	Node dummy;
	int size;

public:
	DLList() {
		dummy.next = &dummy;
		dummy.previous = &dummy;
		size = 0;
	}

	~DLList() {
		while (dummy.next != &dummy) {
			removeNode(dummy.next);
		}
	}

	DLList(const DLList&) = delete;

	DLList& operator=(const DLList&) = delete;

	Node* getNode(int index) {
		//index得合法,0<=index<size;
		assert(index >= 0 && index < size);

		//如果靠近头就正着找，否则倒着找
		if (index < size / 2) {
			//注意0号位是dummy.next
			Node* now = dummy.next;
			for (int i = 0; i < index; i++) {
				now = now->next;
			}
			return now;
		}
		else {
			Node* now = dummy.previous;
			for (int i = 0; i < size - index - 1; i++) {
				now = now->previous;
			}
			return now;
		}
	}

	T get(int index) { return getNode(index)->data; }

	T set(int index , T x) {
		Node* theNode = getNode(index);
		T y = theNode->data;
		theNode->data = x;

		return y;
	}

	Node* addBefore(Node* target, T x) {
		Node* temp = target->previous;

		Node* theAddNode = new Node;
		theAddNode->data = x;

		//添加节点关系更新
		theAddNode->next = target;
		theAddNode->previous = temp;

		//剩下节点关系更新
		temp->next = theAddNode;
		target->previous = theAddNode;

		size++;
		return theAddNode;
	}

	void add(int index, T x) {
		assert(index >= 0 && index <= size);

		Node* target = (index == size)? &dummy: getNode(index);//如果在表尾加有一个分支

		addBefore(target, x);
	}

	void removeNode(Node* theRemoveNode) {
		assert(theRemoveNode != &dummy);//不能把哨兵节点删了

		theRemoveNode->next->previous = theRemoveNode->previous;
		theRemoveNode->previous->next = theRemoveNode->next;
		delete theRemoveNode;
		size--;
	}

	T remove(int index) {
		T y = getNode(index)->data;
		removeNode(getNode(index));
		return y;
	}
};