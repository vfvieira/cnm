#ifndef _DENDROGRAM_H
#define _DENDROGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

struct branch_node{
	int index;
	int branch_nodes;
	int has_child;
	double delta_q;
	
	struct branch_node *left;
	struct branch_node *right;
	struct branch_node *parent;
	
	struct branch_node *queue_next;
};


void GenerateDendrogramScript(int nodes, int **merge, double *merge_heigth, char *prefix_out);


#endif
