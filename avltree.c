#include "avltree.h"

/* Código relacionado às operações básicas na AVL
   retirado da internet
   (colocar fonte) */
         
struct AvlNode{
	int col;
	double value;
	AvlTree Left;
	AvlTree Right;
	int Height;
};
        
AvlTree MakeEmpty(AvlTree T){
	if(T != NULL){
    	MakeEmpty(T->Left);
    	MakeEmpty(T->Right);
    	free(T);
	}
	return NULL;
}

Position Find(int X, AvlTree T){
	if(T == NULL) return NULL;
	if(X < T->col) return Find(X, T->Left);
	else if(X > T->col) return Find(X, T->Right);
	else return T;
}

Position FindMin(AvlTree T){
	if(T == NULL) return NULL;
	else if(T->Left == NULL) return T;
	else return FindMin(T->Left);
}

Position FindMax(AvlTree T){
	if(T != NULL)
		while(T->Right != NULL)
			T = T->Right;
	return T;
}

static int Height(Position P){
	if(P == NULL) return -1;
	else return P->Height;
}

static int Max(int Lhs, int Rhs){
	return Lhs > Rhs ? Lhs : Rhs;
}

static Position SingleRotateWithLeft(Position K2){
	Position K1;
	
	K1 = K2->Left;
	K2->Left = K1->Right;
	K1->Right = K2;

	K2->Height = Max( Height( K2->Left ), Height( K2->Right ) ) + 1;
	K1->Height = Max( Height( K1->Left ), K2->Height ) + 1;
	
	return K1;
}

static Position SingleRotateWithRight(Position K1){
	Position K2;
	
	K2 = K1->Right;
	K1->Right = K2->Left;
	K2->Left = K1;

	K1->Height = Max( Height( K1->Left ), Height( K1->Right ) ) + 1;
	K2->Height = Max( Height( K2->Right ), K1->Height ) + 1;

	return K2;
}

static Position DoubleRotateWithLeft(Position K3){
	/* Rotate between K1 and K2 */
	K3->Left = SingleRotateWithRight( K3->Left );

	/* Rotate between K3 and K2 */
	return SingleRotateWithLeft( K3 );
}

static Position DoubleRotateWithRight(Position K1){
	/* Rotate between K3 and K2 */
	K1->Right = SingleRotateWithLeft( K1->Right );

	/* Rotate between K1 and K2 */
	return SingleRotateWithRight( K1 );
}

AvlTree Insert(int X, double value, AvlTree T){

	if(T == NULL){
		/* Create and return a one-node tree */
		T = malloc( sizeof( struct AvlNode ) );
		if(T == NULL) FatalError("Out of space!!!");
		else{
			T->col = X; T->Height = 0;
			T->Left = T->Right = NULL;
			T->value = value;
		}
    }
	else 
	if(X < T->col){
	T->Left = Insert( X, value, T->Left );
        if( Height( T->Left ) - Height( T->Right ) == 2 ){
            if( X < T->Left->col )
                T = SingleRotateWithLeft( T );
            else
                T = DoubleRotateWithLeft( T );
        }
    }
    else
    if( X > T->col ){
    	T->Right = Insert( X, value, T->Right );
        if( Height( T->Right ) - Height( T->Left ) == 2 ){
            if(X > T->Right->col) T = SingleRotateWithRight(T);
            else T = DoubleRotateWithRight(T);
       	}
    }
	/* Else X is in the tree already; we'll do nothing */
	T->Height = Max( Height( T->Left ), Height( T->Right ) ) + 1;
	return T;
}

AvlTree Delete(int X, AvlTree T){
	printf( "Sorry; Delete is unimplemented; %d remains\n", X );
	return T;
}
		
int Retrieve(Position P){
	return P->col;
}
        
int RetrieveCol(Position P){
	return P->col;
}

double RetrieveValue(Position P){
	return P->value;
}
/* **************************************************************
Fim do código relacionado às operações básicas na AVL
   retirado da internet 
   ************************************************************** */
   
double GetDeltaQ(AvlTree T,int col){
	Position P;
	P = Find(col,T);
	return P->value;
}


void PostOrderTraversal(AvlTree T){
	if(T != NULL){
		PostOrderTraversal(T->Left);
		PostOrderTraversal(T->Right);
		// Aqui vai qualquer operação que eu quero que seja feita em cada nó
		printf("[%d] = %.10f | ", T->col, T->value);
	}
}

void ReadNetworkToAVL(FILE *f, int nodes, int edges, int display, AvlTree *AvlNet, double *k){
	int i;
	
	for(i = 0 ; i < nodes ; i++){
		AvlNet[i] = MakeEmpty(NULL);
		k[i] = 0;
	}
	
	char interaction[150]; // Ficar de olho na quantidade de caracteres que podem aparecer em cada linha
	
	char *from_node_tok, *to_node_tok, *value_tok, *token;
	int from_node, to_node;
	double value;
    
	if(display >= 1){
		printf("Reading network ... ");
	}
	
	// Estruturas que vao guardar as strings lidas dos arquivos
	// Ficar de olho na quantidade de caracteres que podem aparecer
	from_node_tok = (char*) malloc(sizeof(char)*100); //destruida
    to_node_tok = (char*) malloc(sizeof(char)*100); //destruida
	value_tok = (char*) malloc(sizeof(char)*100); //destruida
	
	while(fgets(interaction, sizeof interaction, f) != NULL){
	
		token = strtok(interaction, "\t"); // Leu o from
		strcpy(from_node_tok, (const char *) token);
		from_node = (int) atoi(from_node_tok);
		
		token = strtok(NULL, "\t"); // Leu o to
		strcpy(to_node_tok, (const char *) token);
		to_node = (int) atoi(to_node_tok);
		
		token = strtok(NULL, "\n"); // Leu o valor
		strcpy(value_tok, (const char *) token);
		value = (const double) atof(value_tok);
		
		AvlNet[from_node-1] = Insert(to_node-1, value, AvlNet[from_node-1]);
		k[from_node-1]++;
		
		AvlNet[to_node-1] = Insert(from_node-1, value, AvlNet[to_node-1]);
		k[to_node-1]++;
		
		}
	if(display >= 1){
		printf("OK!\n");
	}
	
	// Liberando a memória
	free(from_node_tok);
	free(to_node_tok);
	free(value_tok);
}

void InitializeDeltaQ(AvlTree T, int row, double k_i, double *k, int edges, int *max_col, double *max_value){
	double value;
	int col;
	double M = (double) 0.5 / (double) edges;
	if(T != NULL){
   		InitializeDeltaQ(T->Left, row, k_i, k, edges, max_col, max_value);
   		InitializeDeltaQ(T->Right, row, k_i, k, edges, max_col, max_value);
		col = T->col;
		
		value = 2 * M * (1 - k_i * k[col] * M);
		T->value = value;
		
		if((value > *max_value) && (row != col)){ // Se valor for maior && linha for diferente de coluna
			*max_value = value;
			*max_col = col;
		}
	}
}

void PostOrderTraversalJoinComm1(AvlTree *AvlNet, AvlTree T_comm1, AvlTree T_comm2, double *a, int comm1, int comm2){
    // Nesta função eu percorro a comm1 e, para cada nó, eu vejo se existe na comm2.
    // Se existir, eu uso a Eq 10a. Senão, eu uso a Eq 10c
    
    int ind_k;
	Position P;
	Position P_k;
	double value;
			
	if(T_comm1 != NULL){
		PostOrderTraversalJoinComm1(AvlNet,T_comm1->Left,T_comm2,a,comm1,comm2);
        PostOrderTraversalJoinComm1(AvlNet,T_comm1->Right,T_comm2,a,comm1,comm2);
        
    	if(fabs(T_comm1->value) > ZERO){
        
        	ind_k = T_comm1->col; // Coluna de comm1 considerada no momento
		    if(ind_k != comm1){
		    	// Tento achar ind_k na comm2
				P = Find(ind_k,T_comm2);
				// Nó da coluna k (P) existe na comm1 e na comm2 (Eq 10a)
				if( (P != NULL) && (fabs(P->value) > ZERO) ){
					T_comm1->value = T_comm1->value + P->value;
					// Se achei um nó que está ligado a comm1 e a comm2, então eu sei que na linha dele eu vou achar um nó de coluna comm1 e um nó de coluna comm2.
					// Eu já posso pegar a linha de k e na coluna de comm1 aplicar a Eq 10a (que é o T_comm1->value que eu acabei de calcular)
					P_k = Find(comm1, AvlNet[ind_k]); // O nó é o P_k
					if( (P_k != NULL) && (fabs(P_k->value) > ZERO) ){
						P_k->value = T_comm1->value;
					}
					else{
						printf("\nError: [1]PostOrderTraversalJoinComm1: A linha k não poderia ser NULL na coluna comm1, mas é... (ou poderia?)\n");
						printf("\nAborting...\n");
						exit(0);
					}
		
					// E na coluna de comm2 eu posso colocar 0 (porque essa coluna vai sumir)
					P_k = Find(comm2, AvlNet[ind_k]);
					if( (P_k != NULL) && (fabs(P_k->value) > ZERO) ){
						P_k->value = 0.0;
					}
					else{
						printf("\nError: [2]PostOrderTraversalJoinComm1: A linha k não poderia ser NULL na coluna comm2, mas é... (ou poderia?)\n");
						printf("\nAborting...\n");
						exit(0);
					}
				}
				else{ // P == NULL, ou seja, nó existe na comm1 mas não na comm2 (Eq 10c)
					value = T_comm1->value - (2*a[comm2]*a[ind_k]);
		
					// Aqui eu achei um nó que está ligado a comm1 mas não a comm2
					// Então eu sei que na linha dele eu vou achar um nó de coluna comm1
					P_k = Find(comm1, AvlNet[ind_k]);
					if( (P_k != NULL) && (fabs(P_k->value) > ZERO) && (fabs(T_comm1->value) > ZERO) ){
						// Então eu pego a linha de k e na coluna de comm1 eu posso aplicar a Eq 10c (que é o T_comm1->value que eu acabei de calcular)
						P_k->value = value;
					}
					else{
						printf("\nError: [4]PostOrderTraversalJoinComm1: Não existe coluna comm1 (%d) na linha k (%d), mas deveria... \n",comm1,ind_k);
						printf("\nAborting...\n");
						exit(0);
					}
					T_comm1->value = value;
				}
			}
		}
		//else{
				// Eu pensei que não era vazio por não ser NULL mas o valor é menor que ZERO...
		//}
		
	}
}

void PostOrderTraversalJoinComm2(AvlTree *AvlNet, AvlTree T_comm1, AvlTree T_comm2, double *a, int comm1, int comm2){
	
	// Nesta função eu percorro cada nó da comm2 e, se ele não existir na comm1 eu aplico a Eq. 10b (se ele existir, a Eq. 10a já foi aplicada)
    int ind_k;
    double value;
	Position P;
	Position P_k;
	
	int found_k = 0;
					
	if(T_comm2 != NULL){
		PostOrderTraversalJoinComm2(AvlNet,AvlNet[comm1],T_comm2->Left,a,comm1,comm2);
		PostOrderTraversalJoinComm2(AvlNet,AvlNet[comm1],T_comm2->Right,a,comm1,comm2);
		
		if(fabs(T_comm2->value) > ZERO){
			
			ind_k = T_comm2->col;
			// Tento achar ind_k na comm1
			
			P = Find(ind_k,AvlNet[comm1]);
			if( (P == NULL) || ((P!= NULL)&&(fabs(P->value) < ZERO)) ){ // Nó existe na comm2 mas não na comm1 (Eq 10b)
				value = T_comm2->value - (2*a[ind_k]*a[comm1]);
				// Como não existe na T_comm1, terá que ser inserido na T_comm1
				AvlNet[comm1] = Insert(ind_k, value, AvlNet[comm1]);
				
				// Inserir uma coluna comm1 na linha k
				AvlNet[ind_k] = Insert(comm1, value, AvlNet[ind_k]);
							
				// Se eu achei um nó que está ligado a comm2
				// então na linha dele eu sei que vou achar um nó na coluna comm2
				P_k = Find(comm2, AvlNet[ind_k]);
				if(P_k != NULL){
					// E na coluna do comm2 eu posso colocar 0 (porque essa coluna vai sumir)
					P_k->value = 0;
				}
				else{
					printf("Error: [6]PostOrderTraversalJoinComm2: Existe uma coluna [comm2][commk] mas não existe [commk][comm2]");
					printf("\nAborting...\n");
					exit(0);
				}
			}
		}
	}
}

void MakeZero(AvlTree T, int pos){
	Position P;
	P = Find(pos, T);
	
	if(P != NULL){
		P->value = 0;
	}
}

void PostOrderTraversalMaxValue(AvlTree T_commk, int commk, double *k, double *nic, int variation, double *max_value,int *max_row,int *max_col){
    // Essa função varre a linha de k e descobre qual é o valor máximo.
    // (usada se eu não quiser usar a heap ou se eu for obrigado a percorrer a linha)
	double ratio;
	if(T_commk != NULL){
		PostOrderTraversalMaxValue(T_commk->Left,commk,k,nic,variation,max_value,max_row,max_col);
		PostOrderTraversalMaxValue(T_commk->Right,commk,k,nic,variation,max_value,max_row,max_col);
		
		// Trecho para CNM puro
		if(variation == 0){
			if( (T_commk->value > (*max_value)) && (commk != T_commk->col) ){
				(*max_value) = T_commk->value;
				(*max_row) = commk;
				(*max_col) = T_commk->col;
			}
		}
		
		// Trecho para DDA
		if(variation == 1){
			if( (T_commk->value/k[commk] > (*max_value)) && (commk != T_commk->col) ){
				(*max_value) = T_commk->value/k[commk];
				(*max_row) = commk;
				(*max_col) = T_commk->col;
			}
		}
				
		// Trecho para WT (1)
		if(variation == 2 || variation == 3){
			printf("\nNot implemented.\n");
			exit(0);
		}
		
		/*
		if(variation == 2){
			if( (k[commk]/k[T_commk->col]) < (k[comm2]/k[T_commk->col])){
				ratio = (k[commk]/k[T_commk->col]);
			}
			else{
				ratio = (k[T_commk->col]/k[commk]);
			}
			if( ( (T_commk->value) * ratio > (*max_value)) && (commk != T_commk->col) ){
				(*max_value) = T_commk->value * ratio;
				(*max_row) = commk;
				(*max_col) = T_commk->col;
			}
		}
		
		// Trecho para WT (2)
		if(variation == 3){		
			if( (nic[commk]/nic[T_commk->col]) < (nic[comm2]/nic[T_commk->col])){
				ratio = (nic[commk]/nic[T_commk->col]);
			}
			else{
				ratio = (nic[T_commk->col]/nic[commk]);
			}
			if( ( (T_commk->value * ratio) > (*max_value)) && (commk != T_commk->col) ){
				(*max_value) = T_commk->value * ratio;
				(*max_row) = commk;
				(*max_col) = T_commk->col;
			}
		}
		*/
	}
}

void UpdateHeap(AvlTree *AvlNet, AvlTree T_comm1, double *a, struct heap_node *heap, int heap_size, int comm1, int comm2, double *k, double *nic, int variation, int *deltaQMaxPos){
	
	// Agora que a comm1 está pronta, todos que tem algum nó dela na .col na heap será atualizado...
	int ind_k, pos;
	int max_row_k, max_col_k;
	double max_value_k = -INFINITY;
	double value;
	
	if(T_comm1 != NULL){
		UpdateHeap(AvlNet, T_comm1->Left,a,heap,heap_size,comm1,comm2,k,nic,variation,deltaQMaxPos);
		UpdateHeap(AvlNet, T_comm1->Right,a,heap,heap_size,comm1,comm2,k,nic,variation,deltaQMaxPos);
	
		ind_k = T_comm1->col;
		
		if( fabs(T_comm1->value) > ZERO && T_comm1-> col != comm1){
			pos = deltaQMaxPos[ind_k];
			
			if( (heap[pos].col == comm1 || heap[pos].col == comm2)){// || heap[pos].row == ind_k){
				value = heap[pos].value;
				max_row_k = -1;
				max_col_k = -1;
				max_value_k = -INFINITY;
				
				
				/* Usando esse trecho o resultado fica certo mas demora muito
				PostOrderTraversalMaxValue(AvlNet[ind_k],ind_k,k,nic,variation,&max_value_k,&max_row_k,&max_col_k);
				heap[pos].row = ind_k;
				heap[pos].col = max_col_k;
				heap[pos].value = max_value_k;
				if(max_value_k > value){
					HeapBubbleUp(&pos,heap,deltaQMaxPos);
				}
				else{
					MaxHeapify(pos, heap_size, heap, deltaQMaxPos);
				}
				//*/
				
				///* Usando esse trecho roda mais rápido mas dá errado
				HeapRemoveNode(pos,heap_size,heap,deltaQMaxPos);
				heap_size--;
				
				max_row_k = -1;
				max_col_k = -1;
				max_value_k = -INFINITY;
				PostOrderTraversalMaxValue(AvlNet[ind_k],ind_k,k,nic,variation,&max_value_k,&max_row_k,&max_col_k);				
				struct heap_node new_node;
				new_node.row = max_row_k;
				new_node.col = max_col_k;
				new_node.value = max_value_k;
				
				HeapAddNode(heap_size,heap,new_node,deltaQMaxPos);
				heap_size++;
				//*/
			}
		}
	}
}


void PostOrderTraversalMaxValue2(AvlTree T_commk, int commk, double *k, double *nic, int variation, double *max_value,int *max_row,int *max_col){
    // Essa função varre a linha de k e descobre qual é o valor máximo.
    // (usada se eu não quiser usar a heap ou se eu for obrigado a percorrer a linha)
	double ratio;
	if(T_commk != NULL){
		PostOrderTraversalMaxValue2(T_commk->Left,commk,k,nic,variation,max_value,max_row,max_col);
		PostOrderTraversalMaxValue2(T_commk->Right,commk,k,nic,variation,max_value,max_row,max_col);
		
		// Trecho para CNM puro
		if(variation == 0){
			if( (T_commk->value > (*max_value)) && (commk != T_commk->col) && (fabs(T_commk->value) > ZERO)   ){
				(*max_value) = T_commk->value;
				(*max_row) = commk;
				(*max_col) = T_commk->col;
			}
		}
		
		// Trecho para DDA
		if(variation == 1){
			if( (T_commk->value/k[commk] > (*max_value)) && (commk != T_commk->col) && (fabs(T_commk->value/k[commk]) > ZERO)   ){
				(*max_value) = T_commk->value/k[commk];
				(*max_row) = commk;
				(*max_col) = T_commk->col;
			}
		}
				
		// Trecho para WT (1)
		if(variation == 2 || variation == 3){
			printf("\nNot implemented.\n");
			exit(0);
		}
		
		/*
		if(variation == 2){
			if( (k[commk]/k[T_commk->col]) < (k[comm2]/k[T_commk->col])){
				ratio = (k[commk]/k[T_commk->col]);
			}
			else{
				ratio = (k[T_commk->col]/k[commk]);
			}
			if( ( (T_commk->value) * ratio > (*max_value)) && (commk != T_commk->col) ){
				(*max_value) = T_commk->value * ratio;
				(*max_row) = commk;
				(*max_col) = T_commk->col;
			}
		}
		
		// Trecho para WT (2)
		if(variation == 3){		
			if( (nic[commk]/nic[T_commk->col]) < (nic[comm2]/nic[T_commk->col])){
				ratio = (nic[commk]/nic[T_commk->col]);
			}
			else{
				ratio = (nic[T_commk->col]/nic[commk]);
			}
			if( ( (T_commk->value * ratio) > (*max_value)) && (commk != T_commk->col) ){
				(*max_value) = T_commk->value * ratio;
				(*max_row) = commk;
				(*max_col) = T_commk->col;
			}
		}
		*/
	}
}

