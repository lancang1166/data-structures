#pragma once
#include "Array.h"
#include <cassert>


template<typename T>
class ArrayDeque {
private:
	Array<T> a;
	int size = 0;
	int capacity;
	int head;

public:
	ArrayDeque(int m_capacity = 0, int m_head = 0):a(m_capacity),capacity(m_capacity),head(m_head){}

	T get(int index) {
		assert(index >= 0 && index < size);
		return a[(head + index) % capacity];
	}

	T set(int index, T x) {
		assert(index >= 0 && index < size);
		T y = a[(head + index) % capacity];
		a[(head + index) % capacity] = x;
		return y;
	}

	//调试悟出的道理，循环数组下标用加法不用减法，减法有可能变成负数，但是加法不会
	void add(int index, T x) {
		assert(index >= 0 && index <= size);
		if (size + 1 > capacity)(resize());
		if (index <= size / 2) {
			head = ( (head == 0) ? capacity - 1 : head - 1);
			for (int i = 0; i < index; i++) {
				a[(head + i) % capacity] = a[(head + i + 1) % capacity];
			}
		}
		else {
			for (int i = size - 1; i > index - 1; i--) {
				a[(head + i + 1) % capacity] = a[(head + i) % capacity];
			}
		}
		a[(head + index) % capacity] = x;
		size++;
	}

	T remove(int index) {
		assert(index >= 0 && index < size && size > 0);
		T y = a[(head + index) % capacity];
		if (index < size / 2) {
			for (int i = index - 1; i >= 0; i--) {
				a[(head + i + 1) % capacity] = a[(head + i) % capacity];
			}
			head = ( (head == capacity - 1) ? 0 : head + 1);
		}
		else {
			for (int i = index; i < size - 1; i++) {
				a[(head + i) % capacity] = a[(head + i + 1) % capacity];
			}
		}
		size--;
		if (3 * size < capacity) { resize(); }

		return y;
	}

	void print(bool haveHead = false , bool haveSize = false) {
		a.print(haveSize, haveHead, size, head);
		std::cout << "head:" << head << "  size:" << size << "  capacity:" << capacity << std::endl;
	}

private:
	void resize() {
		Array<T> b((2 * size > 1 ? 2 * size : 1));
		for (int i = 0; i < size; i++) {
			b[i] = a[(head + i) % capacity];
		}
		a = b;
		capacity = a.getLength();
		head = 0;
	}
};