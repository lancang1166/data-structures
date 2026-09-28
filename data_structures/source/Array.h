#pragma once
#include <iostream>
#include <cassert>

template <typename T>
class Array {
private:
	T* point;
	int length;

public:
	Array(int m_length):length(m_length){
		point = new T[length]();//这里的"()"就是将值初始化了
	}

	~Array(){
		delete[] point;
	}

	T& operator[] (int number) {
		assert(number >= 0 && number < length);
		return point[number];
	}

	Array<T>& operator=(Array<T>& other) {
		if (this == &other) {
			return *this;
		}

		if (point != nullptr) {delete[] point;}
		point = other.point;
		other.point = nullptr;
		length = other.length;
		return *this;
	}

	int getLength() { return length; }

	T* begin() { return point; }

	void print(bool haveSize = false , bool haveHead = false, int size = 0, int head = 0) {
		if (haveHead&&!haveSize) {
			for (int i = 0; i < head; i++) {
				std::cout << point[i] << "  ";
			}
			std::cout << " -> ";
			for (int i = head; i < length; i++) {
				std::cout << point[i] << "  ";
			}
		}

		if (haveSize&&!haveHead) {
			for (int i = 0; i < size; i++) {
				std::cout << point[i] << "  ";
			}
			std::cout << "| ";
			for (int i = size; i < length; i++) {
				std::cout << point[i] << "  ";
			}
		}

		if (!haveHead && !haveSize) {
			for (int i = 0; i < length; i++) {
				std::cout << point[i] << "  ";
			}
		}

		if (haveHead && haveSize) {
			if (head + size > length) {
				for (int i = 0; i < size - (length - head); i++) {
					std::cout << point[i] << "  ";
				}
				std::cout << "| ";
				for (int i = size - (length - head); i < head; i++) {
					std::cout << point[i] << "  ";
				}
				std::cout << " -> ";
				for (int i = head; i < length; i++) {
					std::cout << point[i] << "  ";
				}
			}
			else {
				for (int i = 0; i < head; i++) {
					std::cout << point[i] << "  ";
				}
				std::cout << " -> ";
				for (int i = head; i < size + size; i++) {
					std::cout << point[i] << "  ";
				}
				std::cout << "| ";
				for (int i = size; i < length; i++) {
					std::cout << point[i] << "  ";
				}
			}
		}
	}
};
