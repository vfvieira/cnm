#include ${PETSC_DIR}/conf/variables
#include ${PETSC_DIR}/conf/rules

all: runCNM

runCNM: runCNM.o cnm.o avltree.o heap.o dendrogram.o
	mpicc -o runCNM -O3 runCNM.o cnm.o avltree.o heap.o dendrogram.o -O3 -lm

clear:
	rm -f *.o runCNM
