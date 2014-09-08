#include "heap.h"

void HeapSwapNodes(int i, int j, struct heap_node *heap,int *deltaQMaxPos){
	struct heap_node node_aux;
	node_aux = heap[j];
	heap[j] = heap[i];
	deltaQMaxPos[heap[j].row] = j;
	
	heap[i] = node_aux;
	deltaQMaxPos[heap[i].row] = i;
}
void HeapGetLeftChild(int current, int *child){
	*child = 2*current + 1;
}
void HeapGetRightChild(int current, int *child){
	*child = 2*current + 2;
}
void HeapGetParent(int current, int *parent){
	*parent = (current-1)/2;
}
void HeapBubbleUp(int *current, struct heap_node *heap, int *deltaQMaxPos){
    int parent;

    HeapGetParent(*current,&parent);
    while(*current > 0 && heap[*current].value > heap[parent].value){
    	HeapSwapNodes(*current,parent,heap,deltaQMaxPos);
    	*current = parent;
    	HeapGetParent(*current,&parent);
    }
}
void HeapAddNode(int heap_size, struct heap_node *heap, struct heap_node new_node, int *deltaQMaxPos){
	int current;
	current = heap_size;
	heap[current] = new_node;
	deltaQMaxPos[new_node.row] = current;
	
	HeapBubbleUp(&current,heap,deltaQMaxPos);
}

void HeapRemoveNode(int pos, int heap_size, struct heap_node *heap, int *deltaQMaxPos){
	heap[pos].value = heap[heap_size-1].value;
	heap[pos].row = heap[heap_size-1].row;
	heap[pos].col = heap[heap_size-1].col;
	deltaQMaxPos[heap[pos].row] = pos;
	heap_size--;
	
	MaxHeapify(pos, heap_size, heap, deltaQMaxPos);
}

void MaxHeapify(int current, int heap_size, struct heap_node *heap, int *deltaQMaxPos){
	int left_child;
	int right_child;
	int largest_child;
		
	HeapGetLeftChild(current,&left_child);
	HeapGetRightChild(current,&right_child);
	
	
	largest_child = current;
	
	if(left_child < (heap_size) && largest_child < (heap_size)){
		if(heap[left_child].value > heap[largest_child].value){
			largest_child = left_child;
		}
	}
	if(right_child < (heap_size) && largest_child < (heap_size)){
		if(heap[right_child].value > heap[largest_child].value){
			largest_child = right_child;
		}
	}
	if(largest_child != current && largest_child < (heap_size)){
		HeapSwapNodes(current,largest_child,heap,deltaQMaxPos);
		MaxHeapify(largest_child, heap_size, heap,deltaQMaxPos);
	}
}

