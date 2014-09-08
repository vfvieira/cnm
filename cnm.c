#include "cnm.h"
#include "dendrogram.h"

void CNM(AvlTree *AvlNet, double *k, int nodes, int edges, int variation, int display, int hierarchy, char *prefix_out, FILE *out_file){
	
	int i; // Iteração
	double max_value; // Máximo valor da linha (para colocar na heap)
	int max_row; // Índice da linha de max_value (para colocar na heap)
	int max_col; // Índice da coluna de max_heap (para colocar na heap)

	int ncomms;	// Número de comunidades corrente
	
	double *a; // Estrutura a (Equation 9 do paper)
	a = (double*) malloc(sizeof(double)*nodes); // free OK
	for(i = 0 ; i < nodes ; i++){
		a[i] = (double) k[i] / (double) (2*edges);
	}
	
	int *comms; // Estrutura que guarda as comunidades de cada nó
	comms = (int*) malloc(sizeof(int)*nodes); // free OK
	for(i = 0 ; i < nodes ; i++){
		comms[i] = 0;
	}
	
	double *nic;
	if(variation == 2 || variation == 3){
		nic = (double*) malloc(sizeof(double)*nodes); // free OK
		for(i = 0 ; i < nodes ; i++){
			nic[i] = 1;
		}
	}
	
	int *deltaQMaxPos;
    deltaQMaxPos = (int*) malloc(sizeof(int)*nodes);
		
	// Max-Heap para guardar o DeltaQ máximo em cada linha
	// **** não sei se liberei certo a memória ***
	int heap_size;
	heap_size = 0;	
	struct heap_node *heap;
	heap = (struct heap_node*) malloc(sizeof(struct heap_node) * nodes);
	
	struct heap_node new_node;
		
	double k_i; // Grau da linha corrente (para inicializar a AVL)
	
	int left_index, right_index;
	int current_index;
	
	ncomms = nodes; // Começa com n comunidades unárias
	for(i = 0 ; i < nodes ; i++){
		max_value = -INFINITY;
		max_row = i;
		max_col = 0;
		
		k_i = k[i]; // Grau da linha corrende
		
		// Percorre a AVLTree com uma busca em profundidada preorder para inicializar cada valor de DeltaQ
		InitializeDeltaQ(AvlNet[i], i, k_i, k, edges, &max_col, &max_value);

		// Atualiza a Max-Heap com o valor máximo de cada linha
		new_node.row = max_row;
		new_node.col = max_col;
		new_node.value = max_value;
		HeapAddNode(heap_size,heap,new_node,deltaQMaxPos);
		heap_size++;
	}
	
	// Estruturas de dados DeltaQ (AVL), a (array) e heap (max-heap) criadas e inicializadas.
	
	//Agora tem que pegar a combinação de comunidades (comm1 e comm2) de melhor modularidade na max-heap e fazer a união de comunidades.
	int comm1, comm2;
	double deltaQ, Q, Q_dendrogram;
		
	Q = 0;
	Q_dendrogram = 0;
	
	double den = (double) 1 / ((double) 2 * edges);
	for(i = 0 ; i < nodes ; i++){
		Q += -1.0*(k[i]*den)*(k[i]*den);
	}
	
	double mv;
	int mc, mr;
	int pos = 0;
	int aux;
	
	comm1 = heap[0].row;
	comm2 = heap[0].col;
	
	comms[comm2] = comms[comm1];
	
	if(variation == 1 || variation == 2 || variation == 3){
		deltaQ = GetDeltaQ(AvlNet[comm1], comm2);
	}
	else{
		deltaQ = heap[0].value;
	}
	
	HeapRemoveNode(0,heap_size,heap,deltaQMaxPos);
	heap_size--;
	
	max_value = -INFINITY;
	max_row = 0;
	max_col = 0;
	
	int **merge;
	merge = (int**) malloc(sizeof(int*)*(ncomms-1)); // free OK
	for(i = 0 ; i < (ncomms-1) ; i++){
		merge[i] = (int*) malloc(sizeof(double)*3); // free OK
	}
	
	int *current_ref;
	current_ref = (int*) malloc(sizeof(int)*(nodes)); // free OK
	
	for(i = 0 ; i < nodes ; i++){
		current_ref[i] = -1;
	}
	
	double *merge_heigth;
	merge_heigth = (double*) malloc(sizeof(double)*(nodes)); // free OK
	
	int merge_index_comm1 = -1;
	int merge_index_comm2 = -1;
	int parent_index;
	int j;
	int line_index;
	int mcount = 0;
	int count_leaf = 1;
	
	int count = 0;
	
	double value;
	
	double current_heigth = 0;
	double least_deltaQ = 0;
	
	while(deltaQ > 0.0000001){
	
		if(display > 1){
			//printf("\n Joining %d and %d... (deltaQ = %f)",comm1,comm2,deltaQ);
		}
		
		ncomms--;
		//current_heigth = Q + (1 - deltaQ);
		Q = Q + deltaQ;	
		
		printf("%f\n", deltaQ);
		
		if(hierarchy == 1){	// DENDROGRAMA
			Q_dendrogram = Q_dendrogram + deltaQ;
			
			if(deltaQ < least_deltaQ){
				least_deltaQ = deltaQ;
			}
			
			current_heigth = current_heigth + (1/deltaQ);
			// ***************************************************************
			// Início da montagem do merge
			// ***************************************************************
		
			if(current_ref[comm1] == -1){ //comm1 eh folha
				merge_index_comm1 = -count_leaf;
				count_leaf++;
				current_ref[comm1] = mcount+1;
			}
			else{
				merge_index_comm1 = current_ref[comm1];
				current_ref[comm1] = mcount+1;
			}
		
			if(current_ref[comm2] == -1){ //comm2 eh folha
				merge_index_comm2 = -count_leaf;
				count_leaf++;
				current_ref[comm2] = mcount+1;
			}
			else{
				merge_index_comm2 = current_ref[comm2];
				current_ref[comm2] = mcount+1;
			}
		
			merge[mcount][0] = merge_index_comm1;
			merge[mcount][1] = merge_index_comm2;
			
			//merge_heigth[mcount] = Q_dendrogram;
			merge_heigth[mcount] = current_heigth;
		
			mcount++;
		
			// ***************************************************************
			//*/// FIM DO DENDROGRAMA
		}
		
		if(display > 2){
			//printf("\n merge[0] = %d | merge[1] = %d (deltaQ = %f)",merge[mcount-1][0],merge[mcount-1][1],deltaQ);
		}
		
		max_value = -INFINITY;
		PostOrderTraversalJoinComm1(AvlNet,AvlNet[comm1],AvlNet[comm2],a,comm1,comm2);
		PostOrderTraversalJoinComm2(AvlNet,AvlNet[comm1],AvlNet[comm2],a,comm1,comm2);
		MakeZero(AvlNet[comm1], comm2);
		AvlNet[comm2] = MakeEmpty(AvlNet[comm2]);
				
		// Join feito. Atualizar estruturas auxiliares
		
		// Linha relativa à comm2
		pos = deltaQMaxPos[comm2];
		HeapRemoveNode(pos,heap_size,heap,deltaQMaxPos);
		heap_size--;
				
		// Linha relativa à comm1
		max_row = 0;
		max_col = 0;
		max_value = -INFINITY;
		PostOrderTraversalMaxValue(AvlNet[comm1],comm1,k,nic,variation,&max_value,&max_row,&max_col);
		new_node.row = comm1;
		new_node.col = max_col;
		new_node.value = max_value;
				
		HeapAddNode(heap_size,heap,new_node,deltaQMaxPos);
		heap_size++;
						
		// Demais linhas
		UpdateHeap(AvlNet,AvlNet[comm1],a,heap,heap_size,comm1,comm2,k,nic,variation,deltaQMaxPos);
				
		// Atualizando o vetor a
		a[comm1] = a[comm1] + a[comm2];
		a[comm2] = 0;
		
		if(variation == 1){ // DDA
			k[comm1] = k[comm1] + k[comm2];
			k[comm2] = 0;
		}
		
		if(variation == 2 || variation == 3){ // WT1 WT2
			nic[comm1] = nic[comm1] + nic[comm2];
			nic[comm2] = 0;
		}
		
		// Preparando a nova iteração...
		
		// Escolhendo as melhores comunidades...		
		comm1 = heap[0].row;
		comm2 = heap[0].col;
		
		
		
		if(variation == 1 || variation == 2 || variation == 3){
			deltaQ = GetDeltaQ(AvlNet[comm1], comm2);
		}
		else{
			deltaQ = heap[0].value;
		}
		
		// Retirando nó da heap
		HeapRemoveNode(0,heap_size,heap,deltaQMaxPos);
		heap_size--;
	}
	
	// FIM DO CNM DE VERDADE
	// DAQUI PRA BAIXO É SO POR CAUSA DO DENDROGRAMA
	
	printf("\n\t [Final Modularity: %f (%d communities)] \n", Q, ncomms);
	
	fprintf(out_file,"Final Modularity: %f \n", Q);
	fprintf(out_file,"Number of communities found: %d \n", ncomms);
	
	if(hierarchy == 1){
		int *tagged;
		tagged = (int*) malloc(sizeof(int)*mcount);
		for(i = 0 ; i < mcount ; i++){
			tagged[i] = 0;
		}
	
		int not_tagged;
		for(i = 1 ; i <= ncomms ; i++){
			for(j = mcount - 1 ; j > 0 ; j--){
				if(tagged[j] == 0){
					not_tagged = j;
					break;
				}
			}
			GenerateComms(merge,tagged,comms,i,not_tagged);
		}
		
		
		char comms_filename[100];
		sprintf(comms_filename,"out/%s_comms.txt",prefix_out);
		
		FILE *comms_file;
		comms_file = fopen(comms_filename, "w");
	
		if(comms_file == NULL){
			printf("Warning: Error writing file in %s.\n", comms_filename);
		}
	
		for(i = 0 ; i < nodes ; i++){
			fprintf(comms_file,"%d\n", comms[i]);
		}
	
		max_row = 0;
		max_col = 0;
		max_value = -INFINITY;
		for(i = 0 ; i < nodes ; i++){
			PostOrderTraversalMaxValue2(AvlNet[i],i,k,nic,variation,&max_value,&max_row,&max_col);
		}
		comm1 = max_row;
		comm2 = max_col;
		deltaQ = max_value;
		
		free(tagged);
	
	}
	printf("\t\t ==== least: %f", least_deltaQ);
	if(hierarchy == 1){
	
		while(ncomms > 1){
			
			//current_heigth = Q_dendrogram + (1 - 0.00001);
			
			//current_heigth = 1.5 * current_heigth;
			
			current_heigth += (1 / 0.01); //0.5 * least_deltaQ;
			
			Q_dendrogram += 0.5;
						
			if(display > 1){
				printf("\n\t (Just for dendrogram) Joining %d and %d... (deltaQ = %f)",comm1,comm2,deltaQ);
			}
				
			// ***************************************************************
			// Início da montagem do merge
			// ***************************************************************
	
			if(current_ref[comm1] == -1){ //comm1 eh folha
				merge_index_comm1 = -count_leaf;
				count_leaf++;
				current_ref[comm1] = mcount+1;
			}
			else{
				merge_index_comm1 = current_ref[comm1];
				current_ref[comm1] = mcount+1;
			}
	
			if(current_ref[comm2] == -1){ //comm2 eh folha
				merge_index_comm2 = -count_leaf;
				count_leaf++;
				current_ref[comm2] = mcount+1;
			}
			else{
				merge_index_comm2 = current_ref[comm2];
				current_ref[comm2] = mcount+1;
			}
	
			merge[mcount][0] = merge_index_comm1;
			merge[mcount][1] = merge_index_comm2;
			
			//merge_heigth[mcount] = Q_dendrogram;
			merge_heigth[mcount] = current_heigth;
			//merge_heigth[mcount] = Q_dendrogram;
	
			mcount++;
	
			// ***************************************************************
			//*/// FIM DO DENDROGRAMA
			
			ncomms--;
							
			max_value = -INFINITY;
			PostOrderTraversalJoinComm1(AvlNet,AvlNet[comm1],AvlNet[comm2],a,comm1,comm2);
			PostOrderTraversalJoinComm2(AvlNet,AvlNet[comm1],AvlNet[comm2],a,comm1,comm2);
			MakeZero(AvlNet[comm1], comm2);
			MakeZero(AvlNet[comm2], comm1);
			AvlNet[comm2] = MakeEmpty(AvlNet[comm2]);
				
			// Join feito. Atualizar estruturas auxiliares
		
			// Atualizando o vetor a
			a[comm1] = a[comm1] + a[comm2];
			a[comm2] = 0;
		
			if(variation == 1){ // DDA
				k[comm1] = k[comm1] + k[comm2];
				k[comm2] = 0;
			}
		
			if(variation == 2 || variation == 3){ // WT1 WT2
				nic[comm1] = nic[comm1] + nic[comm2];
				nic[comm2] = 0;
			}
		
			// Preparando a nova iteração...
		
			// Escolhendo as melhores comunidades...		
			max_row = 0;
			max_col = 0;
			max_value = -INFINITY;
			for(i = 0 ; i < nodes ; i++){
				PostOrderTraversalMaxValue2(AvlNet[i],i,k,nic,variation,&max_value,&max_row,&max_col);
			}
			comm1 = max_row;
			comm2 = max_col;
			deltaQ = max_value;
		}
	}
	
	if(hierarchy == 1){// DENDROGRAMA
		//for(i = 0 ; i < nodes ; i++){
		//	merge_heigth[i] = i + 1;
		//}
		GenerateDendrogramScript(nodes, merge, merge_heigth, prefix_out);
	}
	
	for(i = 0 ; i < nodes-1 ; i++){
		printf("%d\t%d\t%d\t%f\n", i+1, merge[i][0], merge[i][1], merge_heigth[i]);
	}
	
	
	// Liberando a memória
	free(a);
	free(deltaQMaxPos);
	free(comms);
	
	free(current_ref);
	free(merge_heigth);
	
	free(heap);
	
	for(i = 0 ; i < ncomms-1 ; i++){
		free(merge[i]);
	}
	free(merge);
	
	if(variation == 2 || variation == 3){
		free(nic);
	}
}


void GenerateComms(int **merge, int *tagged, int *comms, int comm_id, int current_line){
	tagged[current_line] = 1;
	
	if(merge[current_line][0] < 0){ // Achou uma folha
		comms[ (-merge[current_line][0]) - 1] = comm_id;
	}
	else{
		GenerateComms(merge,tagged,comms,comm_id,merge[current_line][0]-1);
	}
	
	if(merge[current_line][1] < 0){ // Achou uma folha
		comms[ (-merge[current_line][1]) - 1] = comm_id;
	}
	else{
		GenerateComms(merge,tagged,comms,comm_id,merge[current_line][1]-1);
	}
}
