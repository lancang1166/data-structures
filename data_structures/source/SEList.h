#pragma once

#include <cassert>
#include <iostream>

#include "Array.h"
#include "ArrayDeque.h"

template <typename T>
class BDeque : public ArrayDeque<T> {
public:
	BDeque(int m_capacity = 0) {
		this->size = 0;
		this->capacity = m_capacity + 1;
		this->head = 0;
		Array<T> temp(m_capacity + 1);//要多留一个空间,SEList本身允许一定的元素数目波动
		this->a = temp;
	}

	~BDeque(){}

	void add(int index, T x) {
		assert(index >= 0 && index <= this->size);
		assert(this->size < this->capacity);

		ArrayDeque<T>::add(index, x);
	}

	bool add(T x) {
		assert(this->size < this->capacity);

		ArrayDeque<T>::add(this->size, x);
		return true;
	}

	int getCapacity() const {
		return this->capacity;
	}

	int getSize() const {
		return this->size;
	}

	void resize() override { }
};

template <typename T>
class Node {
public:
	BDeque<T> deque;
	Node<T>* previous;
	Node<T>* next;

public:
	Node(int m_capacity = 0):deque(m_capacity), previous(nullptr),next(nullptr){}
};


template <typename T>
struct Location {
public:
	Node<T>* theNode;
	int indexInNode;

public:
	Location(Node<T>* m_theNode = nullptr , int m_indexInNode = 0):theNode(m_theNode),indexInNode(m_indexInNode){}
};

//下面是关于SEList的约定
// Unless a block is the last block, then that block contains at least capacity-1 and at most capacity+1 elements
//就是 非末尾块的元素数量b 满足 capacity - 1 <= b <= capacity + 1
template <typename T>
class SEList {
private:
	int blockCapacity;
	int size;
	Node<T> dummy;

private:
	void removeNode(Node<T>* theRemoveNode) {
		assert(theRemoveNode != &dummy);//不能把哨兵节点删了

		theRemoveNode->next->previous = theRemoveNode->previous;
		theRemoveNode->previous->next = theRemoveNode->next;
		delete theRemoveNode;
	}

	Node<T>* addBefore(Node<T>* target) {
		Node<T>* temp = target->previous;

		Node<T>* theAddNode = new Node<T>(blockCapacity);

		//添加节点关系更新
		theAddNode->next = target;
		theAddNode->previous = temp;

		//剩下节点关系更新
		temp->next = theAddNode;
		target->previous = theAddNode;

		return theAddNode;
	}

	void spread(Node<T>* beginNode) {
		Node<T>* temp = beginNode;

		//在beginNode的blockCapacity个块后加一个块
		for (int i = 0; i < blockCapacity; i++) {//temp移到beginNode的后面第blockCapacity个块(就是[beginNode,共计blockCapacity个，再往后一个，因为用的是addBefore)
			temp = temp->next;
		}
		temp = addBefore(temp);

		//移动分配元素
		while (temp != beginNode) {
			while (temp->deque.getSize() < blockCapacity) {
				temp->deque.add(0, temp->previous->deque.remove(temp->previous->deque.getSize() - 1));//不断把左边的尾部加到现在的头部直到当前个数为blockCapacity
			}
			temp = temp->previous;
		}
	}

	void gather(Node<T>* beginNode) {
		Node<T>* temp = beginNode;
		for (int i = 0; i < blockCapacity - 1; i++) {
			while (temp->deque.getSize() < blockCapacity) {
				temp->deque.add(temp->next->deque.remove(0));//不断把右边的头部加到现在的尾部直到当前个数为blockCapacity
			}
			temp = temp->next;
		}
		removeNode(temp);
	}

public:
	SEList(int m_capacity)
		: blockCapacity(m_capacity),size(0),dummy(m_capacity) {
		assert(m_capacity >= 2);//因为目前的spread()、gather() 和装载量检查都基于至少有多个 block 的调整逻辑

		dummy.next = &dummy;
		dummy.previous = &dummy;
	}

	~SEList() {
		Node<T>* temp = dummy.next;

		while (temp != &dummy) {
			Node<T>* next = temp->next;
			delete temp;
			temp = next;
		}
	}

	SEList(const SEList&) = delete;

	SEList& operator=(const SEList&) = delete;

	void getLocation(int index, Location<T>& ell) {
		assert(index >= 0 && index < size);//i的范围

		Node<T>* now;
		if (index < size / 2) {//正向遍历
			now = dummy.next;

			while (index >= now->deque.getSize()) {
				index -= now->deque.getSize();
				now = now->next;
			}

			ell.indexInNode = index;
			ell.theNode = now;
		}
		else {//反向遍历
			now = &dummy;
			int idx = size;

			while (index < idx) {
				now = now->previous;
				idx -= now->deque.getSize();
			}

			ell.theNode = now;
			ell.indexInNode = index - idx;
		}
		
	}

	T get(int index) {
		assert(index >= 0 && index < size);

		Location<T> temp;
		getLocation(index,temp);
		return temp.theNode->deque.get(temp.indexInNode);
	}

	T set(int index, T x) {
		assert(index >= 0 && index < size);

		Location<T> temp;
		getLocation(index,temp);
		T y = temp.theNode->deque.set(temp.indexInNode, x);

		return y;
	}

	void add(T x) {
		Node<T>* last = dummy.previous;
		if (last == &dummy || last->deque.getSize() == blockCapacity + 1) {
			last = addBefore(&dummy);
		}
		last->deque.add(x);
		size++;
	}

	void add(int index, T x) {
		assert(index >= 0 && index <= size);

		//情况1:末尾插入
			if (index == size) {
				add(x);
				return;
			}

			Location<T> indexLocation;
			getLocation(index, indexLocation);

			Node<T>* targetNode = indexLocation.theNode;
			Node<T>* temp = indexLocation.theNode;
			int findStep = 0;
			while (findStep < blockCapacity && temp != &dummy && temp->deque.getSize() == blockCapacity + 1) {
				temp = temp->next;
				findStep++;
			}
		//情况2:从当前块开始的连续blockCapacity块中不存在空位
			if (findStep == blockCapacity) {
				spread(targetNode);//扩展，然后化为情况4
				temp = targetNode;
			}

		//情况3:到达链表末尾
			if (temp == &dummy) {
				temp = addBefore(temp);//化为情况4
			}

		//情况4:此时说明从当前块开始的连续blockCapacity块中存在空位
			while (temp != targetNode) {//此时temp指向有空位节点，targetNode指向插入目标节点
				temp->deque.add(0, temp->previous->deque.remove(temp->previous->deque.getSize() - 1));
				temp = temp->previous;
			}

			targetNode->deque.add(indexLocation.indexInNode, x);
			size++;
	}

	T remove(int index) {
		assert(index >= 0 && index < size);

		Location<T> indexLocation;
		getLocation(index, indexLocation);

		//情况1:目标节点去掉一个元素后剩下的元素还是比 blockCapacity-1 大
			if (indexLocation.theNode->deque.getSize() - 1 >= blockCapacity - 1) {
				size--;
				return indexLocation.theNode->deque.remove(indexLocation.indexInNode);
			}

		Node<T>* targetNode = indexLocation.theNode;
		Node<T>* temp = indexLocation.theNode;
		int findStep = 0;
		while (findStep < blockCapacity && temp != &dummy && temp->deque.getSize() == blockCapacity - 1) {
			temp = temp->next;
			findStep++;
		}

		//情况2：从当前块开始的连续blockCapacity块中都是blockCapacity-1个元素
			if (findStep == blockCapacity) {
				gather(targetNode);//聚集，然后化为情况3
			}

		//情况3：从当前块开始的连续blockCapacity块中存在块有大于blockCapacity-1个元素
			temp = targetNode;
			T y = temp->deque.remove(indexLocation.indexInNode);
			while (temp->deque.getSize() < blockCapacity - 1 && temp->next != &dummy) {
				temp->deque.add(temp->next->deque.remove(0));//把右边的块的头元素搬到现在块的尾部
				temp = temp->next;
			}
			if (temp->deque.getSize() == 0) {//特例，尾部块可能被搬空
				removeNode(temp);
			}

			size--;
			return y;
	}

	void print() {
		Node<T>* temp = dummy.next;

		for (int i = 0; temp != &dummy; i++) {
			std::cout << "第" << i << "块:";
			temp->deque.print(true,true);
			std::cout << std::endl;
			temp = temp->next;
		}
		std::cout << std::endl;
	}
};