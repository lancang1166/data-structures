#pragma once

#include<cassert>

template <typename T>
class Node {
public:
	T data;
	Node<T>* next;

public:
	Node(T m_data):data(m_data),next(nullptr){}
};

template <typename T>
class SSList {//Singly-Linked List
private:
	Node<T>* head;
	Node<T>* tail;
	int size;

public:
	SSList() : head(nullptr), tail(nullptr), size(0) {}

	~SSList() {
		while (head != nullptr) {
			Node<T>* temp = head;
			head = head->next;
			delete temp;
		}
		tail = nullptr;
		size = 0;
	}

	SSList(const SSList&) = delete;

	SSList& operator=(const SSList&) = delete;

	//stack的push()和pop()
		//头进头出(FILO)
	T push(T x) {
		Node<T>* newNode = new Node<T>(x);
		newNode->next = head;
		head = newNode;
		if (size == 0) { tail = newNode; }
		size++;
		return x;
	}

	T pop() {
		//先检查是否为空表
			//if (size == 0) { return null; }
		assert(size > 0);

		//把头节点弹出去
		T y = head->data;//存值

		Node<T>* temp = head;
		head = head->next;
		delete temp;

		size--;

		//如果是最后一个元素被弹出，更新tail
		if (size == 0) { tail = nullptr; }

		return y;
	}

	//queue的add和remove
		//尾进头出(FIFO)
	bool add(T x) {
		//尾进
		Node<T>* newNode = new Node<T>(x);

		if (size == 0) { head = newNode; }
		else{ tail->next = newNode; }
		
		tail = newNode;
		size++;

		return true;
	}

	T remove() {
		//检查空表
		assert(size > 0);

		//头出
		T y = head->data;

		Node<T>* temp = head;
		head = head->next;
		delete temp;

		size--;
		if (size == 0) { tail = nullptr; }
		
		return y;
	}
};