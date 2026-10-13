#pragma once

#include <iostream>
#include <cassert>
#include <random>

#include "ArrayQueue.h"

template <typename T>
class RandomQueue : public ArrayQueue<T> {
private:
	std::mt19937 gen{ std::random_device{}() };

private:
	T removeAt(int index) {
		assert(index >= 0 && index < this->size);
		T y = this->a[(this->head + index) % this->capacity];
		this->a[(this->head + index) % this->capacity] = ArrayQueue<T>::front();
		ArrayQueue<T>::remove();
		return y;
	}

public:
	RandomQueue(int m_capacity = 0, int m_head = 0)
		:ArrayQueue<T>(m_capacity ,m_head) {}


	T remove() {
		if (this->size == 0) { std::cout << "现在队列里为空，没有元素" << std::endl; return 0; }

		assert(this->size > 0);

		std::uniform_int_distribution<int> dist(0, this->size - 1);
		int index = dist(gen);

		return RandomQueue<T>::removeAt(index);
	}

};