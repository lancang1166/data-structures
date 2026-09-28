#pragma once
#include "ArrayStack.h"
#include <cassert>

template<typename T>
class DualArrayDeque {
private:
	ArrayStack<T> front;
	ArrayStack<T> back;

public:
	DualArrayDeque(int m_capacity = 0):front(m_capacity/2),back(m_capacity - (m_capacity/2)){}

	int getSize() { return front.getSize() + back.getSize(); }

	T get(int index) {
		assert(index >= 0 && index < this->getSize());
		if (index < front.getSize()) {
			return front.get(front.getSize() - index - 1);
		}
		else {
			return back.get(index - front.getSize());
		}
	}

	T set(int index, T x) {
		assert(index >= 0 && index < this->getSize());
		if (index < front.getSize()) {
			return front.set(front.getSize() - index - 1 , x);
		}
		else {
			return back.set(index - front.getSize() , x);
		}
	}

	void add(int index, T x) {
		assert(index >= 0 && index <= this->getSize());
		if (index < front.getSize()) {
			//front逻辑位置i对应size-i-1,但是add是在逻辑位置 i 前面插入一个新元素，所以插入位置应该是原元素对应位置的前面，即size-i
			front.add(front.getSize() - index, x);
		}
		else {
			back.add(index - front.getSize(), x);
		}
		balance();
	}

	T remove(int index) {
		assert(index >= 0 && index < this->getSize());
		T y = get(index);
		if (index < front.getSize()) {
			front.remove(front.getSize() - index - 1);
		}
		else {
			back.remove(index - front.getSize());
		}
		balance();
		return y;
	}

	void print() {
		std::cout << "Front: ";
		front.print();
		std::cout << "Back: ";
		back.print();
		std::cout << "现在的逻辑顺序: ";
		for (int i = 0; i < front.getSize(); i++) {
			std::cout << get(i) << " ";
		}
		std::cout << " |";
		for (int i = front.getSize(); i < getSize(); i++) {
			std::cout << get(i) << " ";
		}
		std::cout << std::endl << std::endl;
	}


private:
	int max(int a, int b) { return a > b ? a : b; }

	void balance() {//balance()会确保除非this->getSize() < 2, front.getSize()和back.getSize()之间的权重差异不会超过 3 倍。
		if (3 * front.getSize() < back.getSize() || 3 * back.getSize() < front.getSize()) {
			int newSize = getSize();

			int newSizeA = newSize / 2;
			ArrayStack<T> A(max(2*newSizeA , 1));
			for (int i = 0; i < newSizeA; i++) {//将this(newSizeA-1),...,this(0)放入A
				A.add(i, get(newSizeA - 1 - i));
			}

			int newSizeB = newSize - newSizeA;
			ArrayStack<T> B(max(2 * newSizeB, 1));
			for (int i = 0; i < newSizeB; i++) {//将剩下的顺着放进去
				B.add(i, get(i + newSizeA));
			}

			front = A;
			back = B;
		}
	}
};

#if 0
AI帮我找到的下标问题，然后总结:

现在把正确公式整理成一张表

这个表你真的可以记一下，因为这就是 DualArrayDeque 最容易出错的地方。

设：

F = front.size()
B = back.size()

逻辑序列：

[front][back]

那么：

操作	front 中的物理位置	back 中的物理位置
get(i)	    F - i - 1	        i - F
set(i, x)	F - i - 1	    i - F
remove(i)	F - i - 1	    i - F
add(i, x)	F - i	        i - F

唯一特殊的是：

front 的 add 是 F - i，其他 front 操作是 F - i - 1。

#endif