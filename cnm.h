#ifndef _CNM_H
#define _CNM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "avltree.h"
#include "heap.h"

void CNM(AvlTree *AvlNet, double *k, int nodes, int edges, int variation, int display, int hierarchy, char *prefix_out, FILE *out_file);
void GenerateComms(int **merge, int *tagged, int *comms, int comm_id, int current_line);

#endif  /* _CNM_H */
