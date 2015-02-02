CPPFLAGS += -DTESTING
LDFLAGS += -lstdc++

all: tree-array-demo

tree-array-demo: main.o ArraySearchTree.o
	$(LINK.o) $^ -o $@

main.o: main.cpp ArraySearchTree.h

ArraySearchTree.o: ArraySearchTree.cpp ArraySearchTree.h

clean:
	$(RM) *.o tree-array-demo

.PHONY: clean
