#pragma once
#include "Array.h"
#include <cassert>

int max(int a, int b) { return a > b ? a : b; }

template<typename T>
class ArrayStack {
private:
	Array<T> a;
	int capacity;
	int size = 0;

public:
	ArrayStack(int m_capacity):a(m_capacity), capacity(m_capacity) {}

	int getSize() { return size; }

	T get(int index) {
		assert(index >= 0 && index < size);
		return a[index];
	}

	T set(int index , T x) {
		assert(index >= 0 && index < size);
		T y = a[index];
		a[index] = x;
		return y;
	}

	void add(int index, T x) {
		assert(index >= 0 && index <= size);//前提必须是 0 <= index <= size
		if (size + 1 > capacity) { resize(); }
		for (int i = size; i > index; i--) {
			a[i] = a[i - 1];
		}
		a[index] = x;
		size++;
	}

	T remove(int index) {
		assert(index >= 0 && index < size);//前提必须是 0 <= index <= size
		T y = a[index];

		for (int i = index; i < size - 1; i++) {
			a[i] = a[i + 1];
		}
		size--;
		if (3 * size < capacity) { resize(); }

		return y;
	}

	void print(bool haveSize = false) {
		a.print(haveSize,false, size);
		std::cout << "size:" << size <<"  capacity:" <<capacity << std::endl;
	}


private:
	void resize() {
		Array<T> b(max(2 * size, 1));
		for (int i = 0; i < size; i++) {
			b[i] = a[i];
		}
		a = b;//这里的等号的意思是把b的所有权给a
		capacity = a.getLength();
	}
};