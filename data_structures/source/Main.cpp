#include <iostream>
#include <utility>
#include <map>

#include "Treap.h"

int main(){
	std::map<int, int> counts;

	int cnt1 = 100000;
	int cnt2 = 1000;
	for (int i = 0; i < cnt1; i++) {
		Treap<int> tree;
		for (int i = 0; i < cnt2; i++) {
			tree.add(i);
		}
		counts[tree.theWholeTreeHeight()]++;
	}

	for (const auto& [key, value] : counts) {
		std::cout << key << ": " << value << '\n';
	}

	/*tree.remove(8);

	std::cout << "\nAfter removing 8:\n";
	tree.print();*/
	

	std::cout<<"helloworld!"<<std::endl;
}