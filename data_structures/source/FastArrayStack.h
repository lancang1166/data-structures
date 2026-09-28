#pragma once
#include "Array.h"
#include <cassert>
#include <algorithm>

template<typename T>
class FastArrayStack {//这个类只是把resize(),add()和remove()函数中的移动换成了std::copy而已
private:
	Array<T> a;
	int capacity;
	int size = 0;

public:
	FastArrayStack(int m_capacity) :a(m_capacity), capacity(m_capacity) {}

	int getSize() { return size; }

	T get(int index) {
		assert(index >= 0 && index < size);
		return a[index];
	}

	T set(int index, T x) {
		assert(index >= 0 && index < size);
		T y = a[index];
		a[index] = x;
		return y;
	}

	void add(int index, T x) {
		assert(index >= 0 && index <= size);//前提必须是 0 <= index <= size
		if (size + 1 > capacity) { resize(); }
		std::copy_backward(a.begin() + index, a.begin() + size, a.begin() + size + 1);
		a[index] = x;
		size++;
	}

	T remove(int index) {
		assert(index >= 0 && index < size);//前提必须是 0 <= index <= size
		T y = a[index];

		std::copy(a.begin() + index + 1, a.begin() + size - 1, a.begin() + index);
		size--;
		if (3 * size < capacity) { resize(); }

		return y;
	}

	void print(bool haveSize = false) {
		a.print(haveSize, size);
		std::cout << "size:" << size << "  capacity:" << capacity << std::endl;
	}


private:
	void resize() {
		Array<T> b(std::max(2 * size, 1));
		std::copy(a.begin() + 0, a.begin() + size, b.begin() + 0);
		a = b;//这里的等号的意思是把b的所有权给a
		capacity = a.getLength();
	}
};