#include "fatal.h"
#define ZERO 1e-20

#ifndef _AvlTree_H
#define _AvlTree_H

typedef int ElementType;


struct AvlNode;
struct heap_node;

typedef struct AvlNode *Position;
typedef struct AvlNode *AvlTree;

#include "cnm.h"
#include "heap.h"
#include <stdlib.h>


AvlTree MakeEmpty( AvlTree T );
Position Find( int X, AvlTree T );
Position FindMin( AvlTree T );
Position FindMax( AvlTree T );
AvlTree Insert( int X, double value, AvlTree T );
AvlTree Delete( int X, AvlTree T );
int RetrieveCol( Position P );
int Retrieve( Position P );
double RetrieveValue( Position P );

double GetDeltaQ(AvlTree T, int col);
void PostOrderTraversal(AvlTree T);
void InitializeDeltaQ(AvlTree T, int row, double k_i, double *k, int edges, int *max_col, double *max_value);
void PostOrderTraversalJoin(AvlTree *AvlNet, AvlTree T_comm1, AvlTree T_comm2, double *a,struct heap_node *heap, int heap_size,int comm1, int comm2, int insert);
void PostOrderTraversalMaxValue(AvlTree T_commk, int commk,double *k, double *nic, int variation, double *max_value,int *max_row,int *max_col);
void PostOrderTraversalJoinComm1(AvlTree *AvlNet, AvlTree T_comm1, AvlTree T_comm2, double *a, int comm1, int comm2);
void PostOrderTraversalJoinComm2(AvlTree *AvlNet, AvlTree T_comm1, AvlTree T_comm2, double *a, int comm1, int comm2);
void UpdateHeap(AvlTree *AvlNet, AvlTree T_comm1, double *a, struct heap_node *heap, int heap_size, int comm1, int comm2, double *k, double *nic, int variation, int *deltaQMaxPos);

void ReadNetworkToAVL(FILE *f, int nodes, int edges, int display, AvlTree *AvlNet, double *k);
void MakeZero(AvlTree T, int pos);

void PostOrderTraversalMaxValue2(AvlTree T_commk, int commk,double *k, double *nic, int variation, double *max_value,int *max_row,int *max_col);

#endif  /* _AvlTree_H */
