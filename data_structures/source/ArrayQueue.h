#pragma once
#include "Array.h"
#include <cassert>

template<typename T>
class ArrayQueue {
private:
	Array<T> a;
	int size = 0;
	int capacity;
	int head;

public:
	ArrayQueue(int m_capacity = 0, int m_head = 0):a(m_capacity),capacity(m_capacity),head(m_head){}

	int getSize() { return size; }

	bool add(T x) {
		if (size + 1 > capacity) { resize(); }
		a[(head + size) % capacity] = x;
		size++;
		return true;
	}

	T remove() {
		assert(size > 0);
		T y = a[head];
		head = (head + 1) % capacity;
		size--;
		if (3 * size < capacity) { resize(); }
		return y;
	}

	T front() {
		assert(size > 0);
		return a[head]; 
	}

	void print(bool haveHead = false) {
		a.print(false, haveHead, 0, head);
		std::cout << "head:" << head << "  size:" << size << "  capacity:" << capacity << std::endl;
	}

private:
	void resize() {
		Array<T> b( (2 * size > 1 ? 2 * size : 1) );
		for (int i = 0; i < size; i++) {
			b[i] = a[(head + i) % capacity];
		}
		a = b;
		capacity = a.getLength();
		head = 0;
	}
};