#include <iostream>
#include "ArrayDeque.h"

int main() {

	ArrayDeque<int> a;
	a.add(0 ,1);
	a.print(true,true);
	a.add(0 ,2);
	a.print(true,true);
	a.add(0 ,3);
	a.print(true,true);
	a.add(1 ,4);
	a.print(true,true);
	a.add(2,5);
	a.print(true,true);
	a.add(0,6);
	a.print(true,true);

	a.remove(0);
	a.print(true,true);
	a.remove(3);
	a.print(true,true);

	a.add(4,7);
	a.print(true,true);
	a.add(4,8);
	a.print(true,true);
	a.add(0,9);
	a.print(true,true);

	a.remove(6);
	a.print(true,true);
	a.remove(5);
	a.print(true,true);
	a.remove(1);
	a.print(true,true);

	std::cout << "hello world!" << std::endl;
}