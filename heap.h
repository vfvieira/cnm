#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifndef _heap_node_H
#define _heap_node_H

struct heap_node{
	int row;
	int col;
	double value;
};

void HeapSwapNodes(int i, int j, struct heap_node *heap,int *deltaQMaxPos);
void HeapGetLeftChild(int current, int *child);
void HeapGetRightChild(int current, int *child);
void HeapGetParent(int current, int *parent);
void HeapBubbleUp(int *current, struct heap_node *heap, int *deltaQMaxPos);
void HeapAddNode(int heap_size, struct heap_node *heap, struct heap_node new_node,int *deltaQMaxPos);
void HeapRemoveNode(int pos, int heap_size, struct heap_node *heap, int *deltaQMaxPos);
void MaxHeapify(int current, int heap_size, struct heap_node *heap, int *deltaQMaxPos);


#endif  /* _AvlTree_H */
