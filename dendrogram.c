#include "dendrogram.h"

void GenerateDendrogramScript(int nodes, int **merge, double *merge_heigth, char *prefix_out){
	
	int i;
	char r_script_filename[100];
	sprintf(r_script_filename,"out/%s_dendrogram.R",prefix_out);
	
	FILE *r_script_file; //Input network file
	r_script_file = fopen(r_script_filename, "w");
	
	if(r_script_file == NULL){
		printf("Warning: Error while reading network from %s.\n", r_script_filename);
	}
	
	fprintf(r_script_file,"a <- list()\n");
	fprintf(r_script_file,"a$merge <- matrix(c(");
	for(i = 0 ; i < nodes-1 ; i++){
		fprintf(r_script_file,"%d,%d", merge[i][0], merge[i][1]);
		if(i != nodes-2){
			fprintf(r_script_file,",");
			fprintf(r_script_file,"\n");	
		}
		
	}
	fprintf(r_script_file,"), nc=2, byrow=TRUE )\n");
	
	fprintf(r_script_file,"a$height <-  c(");
	for(i = 0 ; i < nodes-1 ; i++){
		fprintf(r_script_file,"%f", merge_heigth[i]);
		if(i != nodes-2){
			fprintf(r_script_file,",");
		}
	}
	fprintf(r_script_file,")\n");
	
	fprintf(r_script_file,"a$order <- 1:%d\n",nodes);
	fprintf(r_script_file,"a$labels <- c(");
	for(i = 0 ; i < nodes ; i++){
		fprintf(r_script_file,"'%d'",i+1);
		if(i != nodes-1){
			fprintf(r_script_file,",");
		}
	}
	fprintf(r_script_file,")\n");
	fprintf(r_script_file,"class(a) <- \"hclust\"\n");
	fprintf(r_script_file,"hc <- as.dendrogram(a)\n");
	fprintf(r_script_file,"plot(hc, edgePar = list(pch = c(1,NA), cex = 5, lab.cex = 1))\n");


	fclose(r_script_file);
	
	/*for(i = 0 ; i < nodes-1 ; i++){
		free(merge[i]);
	}
	free(merge);
	
	free(merge_heigth);
	*/
	
	

}
