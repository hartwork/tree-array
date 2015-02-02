/*
 * Copyright (C) 2008, Sebastian Pipping <sebastian@pipping.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
