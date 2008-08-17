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

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H 1

#ifdef TESTING
# include <string>
#endif

class ArraySearchTreePrivate;

class ArraySearchTree {
	ArraySearchTreePrivate * const d;

public:
	ArraySearchTree();
	~ArraySearchTree();

	int count() const;
	int get(int index) const;

	bool insert(int index, int value);
	bool remove(int index);

#ifdef TESTING
	std::string serialize() const;
#endif
};

#endif // BINARY_SEARCH_TREE_H
