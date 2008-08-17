#include <iostream>
#include "ArraySearchTree.h"

void dump(const ArraySearchTree & tree) {
	std::cout << tree.serialize() << std::endl;
	for (int i = 0; i < tree.count(); i++) {
		std::cout << "tree[" << i << "] == " << tree.get(i) << std::endl;
	}
}

int main() {
	ArraySearchTree tree;
	tree.insert(1, 20);
	tree.insert(1, 50);
	tree.insert(0, 10);
	tree.insert(2, 40);
	tree.insert(2, 30);
	dump(tree);

	tree.remove(2);
	dump(tree);
	tree.remove(1);
	dump(tree);
	return 0;
}
