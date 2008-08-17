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

#include "ArraySearchTree.h"
#include <cstdlib>
#include <cassert>

enum Side {
	LEFT = 0,
	RIGHT = 1
};

enum Relation {
	LESS = 0,
	GREATER = 1,
	EQUAL = 2
};

class Node {
#ifdef TESTING
public:
#else
	friend class ArraySearchTree;
	friend class ArraySearchTreePrivate;
#endif
	int m_value;
	int m_leftChildCount;
	Node * m_children[2];
	Node * m_parent;

public:
	Node(int key, Node * parent)
			: m_value(key), m_leftChildCount(0), m_parent(parent) {
		m_children[LEFT] = NULL;
		m_children[RIGHT] = NULL;
	}
};

class ArraySearchTreePrivate {
	friend class ArraySearchTree;

	Node * m_root;
	int m_count;

	ArraySearchTreePrivate()
			: m_root(NULL), m_count(0) {

	}

};

namespace {
	Relation compare(int a, int b) {
		return (a < b)
				? LESS
				: ((a > b)
					? GREATER
					: EQUAL);
	}
} // anon namespace

ArraySearchTree::ArraySearchTree()
		: d(new ArraySearchTreePrivate()) {
	assert(static_cast<int>(LESS) == static_cast<int>(LEFT));
	assert(static_cast<int>(GREATER) == static_cast<int>(RIGHT));
}

ArraySearchTree::~ArraySearchTree() {
	Node * walker = d->m_root;
	while (walker != NULL) {
		// Jump to either child
		for (int i = 0; i < 2; i++) {
			Node * & child = walker->m_children[i];
			if (child != NULL) {
				walker = child;
				break;
			}
		}

		Node * & parent = walker->m_parent;
		if (parent == NULL) {
			break;
		}

		// No more children
		for (int j = 0; j < 2; j++) {
			Node * & candidate = parent->m_children[j];
			if (candidate == walker) {
				delete candidate;
				candidate = NULL;
				walker = parent;
				break;
			}
		}
	}

	delete d;
}

int
ArraySearchTree::count() const {
	return d->m_count;
}

int
ArraySearchTree::get(int index) const {
	Node * walker = d->m_root;
	int offset = 0;
	while (walker != NULL) {
		const int nodeIndex = walker->m_leftChildCount + offset;
		// std::cout << std::endl << "at " << nodeIndex << std::endl;
		const Relation rel = compare(index, nodeIndex);
		switch (rel) {
		case EQUAL:
			return walker->m_value;
		case GREATER:
			offset += walker->m_leftChildCount + 1;
			// fall trough
		case LESS:
			walker = walker->m_children[rel];
			break;
		default:
			assert(false);
		}
	}
	return -1;
}

bool
ArraySearchTree::insert(int index, int value) {
	if (d->m_root == NULL) {
		d->m_root = new Node(value, NULL);
		d->m_count++;
		return true;
	}

	if ((index < 0) || (index > d->m_count)) {
		return false;
	}

	Node * walker = d->m_root;
	int offset = 0;
	while (walker != NULL) {
		const int nodeIndex = walker->m_leftChildCount + offset;
		const Side side = (index < nodeIndex + 1) ? LEFT : RIGHT;
		// std::cout << std::endl << " at " << nodeIndex << " side " << side << std::endl;
		if (side == RIGHT) {
			offset += walker->m_leftChildCount + 1;
		}
		Node * & child = walker->m_children[side];
		if (child == NULL) {
			child = new Node(value, walker);

			// Increase child counters
			Node * cur = child;
			Node * parent = walker;
			while (parent != NULL) {
				if (parent->m_children[LEFT] == cur) {
					parent->m_leftChildCount++;
				}
				cur = parent;
				parent = parent->m_parent;
			}

			d->m_count++;
			return true;
		}
		walker = child;
	}
	assert(false);
}

bool
ArraySearchTree::remove(int index) {
	Node * walker = d->m_root;
	int offset = 0;
	while (true) {
		if (walker == NULL) {
			return false;
		}

		const int nodeIndex = walker->m_leftChildCount + offset;
		const Relation rel = compare(index, nodeIndex);
		switch (rel) {
		case EQUAL:
			{
				Node * nodeToDelete = walker;
				while (true) {
					Node * & left = nodeToDelete->m_children[LEFT];
					Node * & right = nodeToDelete->m_children[RIGHT];
					if ((left != NULL) && (right != NULL)) {
						// Two children -> replace with next in order
						Node * nextInOrder = right;
						while (nextInOrder->m_children[LEFT] != NULL) {
							nextInOrder = nextInOrder->m_children[LEFT];
						}
						nodeToDelete->m_value = nextInOrder->m_value;
						nodeToDelete = nextInOrder;
					} else {
						// No or just a single child
						Node * & parentChild = (nodeToDelete->m_parent == NULL)
								? d->m_root
								: ((nodeToDelete->m_parent->m_children[LEFT]
										== nodeToDelete)
									? nodeToDelete->m_parent->m_children[LEFT]
									: nodeToDelete->m_parent->m_children[RIGHT]);

						// Decrease child counters
						Node * cur = nodeToDelete;
						Node * parent = nodeToDelete->m_parent;
						while (parent != NULL) {
							if (parent->m_children[LEFT] == cur) {
								parent->m_leftChildCount--;
							}
							cur = parent;
							parent = parent->m_parent;
						}

						// Unlink node
						if ((left != NULL) && (right == NULL)) {
							// One child -> replace with child
							parentChild = left;
							left->m_parent = nodeToDelete->m_parent;
						} else if ((left == NULL) && (right != NULL)) {
							// One child -> replace with child
							parentChild = right;
							right->m_parent = nodeToDelete->m_parent;
						} else if ((left == NULL) && (right == NULL)) {
							// No children -> Just delete
							parentChild = NULL;
						} else {
							assert(false);
						}

						delete nodeToDelete;
						d->m_count--;
						return true;
					}
				}
			}
			assert(false);
			break;
		case GREATER:
			offset += walker->m_leftChildCount + 1;
			// fall through
		case LESS:
			walker = walker->m_children[rel];
			break;
		default:
			assert(false);
		}
	}
}

#ifdef TESTING
/**
 * C++ version std::string style "itoa":
 * Copyright (C) Dr Stuart Lowe <stuart.lowe@manchester.ac.uk>
 */
std::string itoa(int value, int base) {
	enum { kMaxDigits = 35 };
	std::string buf;
	buf.reserve( kMaxDigits ); // Pre-allocate enough space.

	// Check that the base is valid
	if (base < 2 || base > 16) return buf;
	int quotient = value;

	// Translating number to string with base:
	do {
		buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
		quotient /= base;
	} while ( quotient );

	// Append the negative sign for base 10
	if ( value < 0 && base == 10) buf += '-';

	std::reverse( buf.begin(), buf.end() );
	return buf;
}

std::string
serializeInOrder(const Node * node) {
	if (node == NULL) {
		return std::string();
	}
	const Node * const & left = node->m_children[LEFT];
	const Node * const & right = node->m_children[RIGHT];
	return "(" + ((left != NULL) ? serializeInOrder(left) : "")
			+ "["
			+ itoa(node->m_leftChildCount, 10)
			+ "]"
			+ ","
			+ itoa(node->m_value, 10)
			+ ","
			+ ((right != NULL) ? serializeInOrder(right) : "") + ")";
}

std::string
ArraySearchTree::serialize() const {
	return "<" + serializeInOrder(d->m_root) + ">";
}
#endif
