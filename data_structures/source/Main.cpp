#include<iostream>
#include"DualArrayDeque.h"

#include <Windows.h>

int main(){
	DualArrayDeque<int>a;
	a.add(0,1);
	a.print();
	a.add(0,2);
	a.print();
	a.add(0,3);
	a.print();
	a.add(1,4);
	a.print();
	a.add(2,5);
	a.print();
	a.add(0,6);
	a.print();

	a.remove(0);
	a.print();
	a.remove(3);
	a.print();

	a.add(4,7);
	a.print();
	a.add(4,8);
	a.print();
	a.add(0,9);
	a.print();

	a.remove(6);
	a.print();
	a.remove(5);
	a.print();
	a.remove(1);
	a.print();

	std::cout<<"helloworld!"<<std::endl;
}