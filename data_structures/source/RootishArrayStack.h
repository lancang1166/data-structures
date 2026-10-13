#pragma once

#include <cmath>
#include <cassert>

#include "ArrayStack.h"

template <typename T>
class RootishArrayStack {
private:
	ArrayStack<T*> blocks;
	int capacity;
	int size;

private:
	int getBlockIndex(int index) {
		double db = (-3.0 + std::sqrt(9.0 + 8.0 * index)) / 2.0;
		int theBlockIndex = (int)std::ceil(db);
		return theBlockIndex;
	}

	void refreshCapacity() {
		int r = blocks.getSize();
		capacity = r * (r + 1) / 2;
	}

	void grow() {
		//这里往blocks后面添加一个数组，数组长度恰好就是blocks的长度+1
		int r = blocks.getSize();
		blocks.add(r, new T[r + 1]());
		refreshCapacity();
	}

	//这个是我原来自己写的，对应的是r(r-1)/2这个阈值
	/*void shrink() {
		delete[] blocks.get(blocks.getSize() - 1);
		blocks.remove(blocks.getSize() - 1);
	}*/

	//这个是教材的，使用的是(r-1)(r-2)/2这个阈值
	void shrink() {
		int r = blocks.getSize();
		while (r > 0 && (r - 2) * (r - 1) / 2 >= size) {
			delete[] blocks.remove(blocks.getSize() - 1);
			r--;
		}
		refreshCapacity();
	}

public:
	~RootishArrayStack() {
		for (int i = 0; i < blocks.getSize(); i++) {
			delete[] blocks.get(i);
		}
	}

	int getSize() { return size; }

	T get(int index) {
		assert(index >= 0 && index < size);
		int indexOfBlock = getBlockIndex(index);
		int indexInBlock = index - (indexOfBlock * (indexOfBlock + 1)) / 2;
		return *(blocks.get(indexOfBlock) + indexInBlock);
	}

	T set(int index, T x) {
		assert(index >= 0 && index < size);
		T y = get(index);
		int indexOfBlock = getBlockIndex(index);
		int indexInBlock = index - (indexOfBlock * (indexOfBlock + 1)) / 2;
		*(blocks.get(indexOfBlock) + indexInBlock) = x;
		return y;
	}

	void add(int index, T x) {
		assert(index >= 0 && index <= size);

		if (size + 1 > capacity) { grow(); }
		size++;

		for (int i = size - 1; i > index; i--) {
			set(i, get(i - 1));
		}
		set(index, x);
	}

	T remove(int index) {
		assert(index >= 0 && index < size);

		T y = get(index);

		for (int i = index; i < size - 1; i++) {
			set(i, get(i + 1));
		}
		size--;

		//if (size <= capacity - blocks.getSize()) { shrink(); }//我的阈值为r(r-1)/2
		int r = blocks.getSize();
		if ((r - 2) * (r - 1) / 2 >= size) shrink();//教材的阈值为(r-1)(r-2)/2

		return y;
	}

	void print() {
		std::cout << "Block: " << std::endl;
		int index = 0;
		for (int i = 0; i < blocks.getSize(); i++) {
			std::cout << "第" << i << "个block: ";
			for (int j = index; j < (i + 1) + index && j < size; j++) {
				std::cout<<get(j)<<" ";
			}
			std::cout << std::endl;
			index += i + 1;
		}
	}
};