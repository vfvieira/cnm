#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "avltree.h"
#include "cnm.h"
#include "heap.h"

void parse_args(int argc, char **argv, int *nodes, int *edges, int *starting_node, int *display, int *variation, int *hierarchy, char *filename_in, char *prefix_out){
		
	*nodes = -1;
	*edges = -1;
	*starting_node = -1;
	*display = -1;
	*variation = 0;
	*hierarchy = 0;
	sprintf(prefix_out,"%s","output");
		
	if(argc < 2){
		printf("\nError: Missing fiename.");
		sprintf(filename_in,"bases/zachary.txt");
		printf("\nUsing Zachary as default.");
		printf("\nPress any key to continue...\n");
		getchar();
	}
	if(argc % 2 == 0){
		printf("\nWrong usage.\n");
		exit(0);
	}

	int i;
	for(i = 1 ; i < argc ; i++){
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
				case 'n': // setando número de nós
					*nodes = atoi(argv[i+1]);
					i++;
					break;
				case 'e':
					*edges = atoi(argv[i+1]);
					i++;
					break;
				case 's':
					*starting_node = atoi(argv[i+1]);
					i++;
					break;
				case 'i':
					sprintf(filename_in,"%s",argv[i+1]);
					i++;
					break;
				case 'd':
					*display = atoi(argv[i+1]);
					i++;
					break;
				case 'o':
					sprintf(prefix_out,"%s",argv[i+1]);
					i++;
					break;
				case 'v':
					*variation = atoi(argv[i+1]);
					i++;
					break;
				case 'h':
					*hierarchy = atoi(argv[i+1]);
					i++;
					break;
				default:
					printf("\nUnknown option -%s\n",argv[i+1]);
					i++;
					break;
			}
		}
	}
	
	
	if(*display == -1){*display = 1;}
	if(*starting_node == -1){*starting_node = 1;}
	
	
	
		
	if(*nodes == -1 || *edges == -1){
		printf("\n Network not configured yet. Missing information (nodes and/or edges)\n");
		exit(0);
	}
		
	
}

int main(int argc, char *argv[]){
	
	FILE *net_file; //Input network file
	char filename_in[100]; //Input network file path
	char prefix_out[100]; //Input network file path

	int nodes, edges, starting_node, variation, hierarchy;
	
	int display = 2;
		
	struct timeval start, end;
	gettimeofday(&start, NULL);
	
	float diff;
	
	parse_args(argc,argv,&nodes,&edges,&starting_node,&display,&variation,&hierarchy,filename_in,prefix_out);
	
	net_file = fopen(filename_in, "r");
	if(display >= 1){
		printf("Reading network from file %s...\n", filename_in);
	}
	if (net_file == NULL){
		printf("Error while reading network from %s.\n", filename_in);
		return 0;
	}
	
		
	if(display >= 1){
		printf("Network successfully read: %d nodes and %d edges. ", nodes, edges);
		printf("Here we go!\n");
	 	  	printf("Generating data structures (AVL)... \n");
   	}
    
    double *k;
    k = (double*) malloc(sizeof(double)*nodes); // free OK
    int i;
    for(i = 0 ; i < nodes ; i++){
    	k[i] = 0;
    }
    
  	AvlTree *AvlNet;
	AvlNet = (AvlTree*) malloc(sizeof(AvlTree) * nodes); // free OK
	ReadNetworkToAVL(net_file, nodes, edges, display, AvlNet,k);
	
	printf("Running CNM... \n");
	
	char out_filename[100];
	sprintf(out_filename,"out/%s_out.txt",prefix_out);
	FILE *out_file;
	out_file = fopen(out_filename, "w");
	
	CNM(AvlNet,k,nodes,edges,variation,display,hierarchy,prefix_out,out_file);
   
   	fclose(net_file);
   	
	   
    gettimeofday(&end, NULL);
    diff = end.tv_sec - start.tv_sec + (double)(end.tv_usec - start.tv_usec)/CLOCKS_PER_SEC;
    
    if(display >= 1){
    	printf("Done. (Total time: %.2f seconds)\n", diff);
	}
	
	fprintf(out_file,"Done. Total time: %.2f seconds \n", diff);
	
	
	fclose(out_file);
		
	
	for(i = 0 ; i < nodes ; i++){
		MakeEmpty(AvlNet[i]);
	}
	free(AvlNet);
	free(k);
	return 0;
}
