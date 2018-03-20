/*
*   Versao com o edges_array de inteiros
*   Versao com stack de inteiros
*
*   Problemas: 
*       Ele esta a fazer mal o Tarjan
*       Acho que ele nao esta a tirar os repetidos
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

# define min(a,b) (((a)<(b))?(a):(b))


/* ----------------------------------------------------------------------
                                S T R U C T U R E S
------------------------------------------------------------------------*/
typedef struct vertex{
    int d;
    int low;
    int id;
} *Vertex;

typedef struct int_node{
    int id;
    struct int_node *next;
} Stack;

int Visited;
int n_sccs = 0;

int *stack_array;
int *neighbours_array;
int **edges_array; 
/* Vou fazer assim para mudar os ids logo todos*/

Vertex *vertex_array;
static Stack *stack;

/* ----------------------------------------------------------------------
                                S T A C K
------------------------------------------------------------------------*/
void init_stack(int V){  /*Inicializa a pilha V-> numero de vertices*/
    stack = NULL;
    stack_array = (int *)malloc(sizeof(int)*(V+1));
}

int is_empty_stack(){
    return (stack == NULL);
}

int in_stack(int value){
    return stack_array[value] == value;
}

void push(int id){
    Stack *new;
    new = (struct int_node *) malloc(sizeof(struct int_node));
    new->id = id;
    new->next = stack;
    stack = new;

    stack_array[id] = id; /* Ir a posicao do array e meter la cenas*/
}

int pop(){
    int id;
    Stack *old;
    
    if(!is_empty_stack()){
        id = stack->id;
        old = stack;
        stack = stack->next;
        free(old);

        stack_array[id] = 0; 
        return id;
    }
    else
        return 0;
}

/* ----------------------------------------------------------------------
                C R E A T I O N     O F     A R R A Y S
------------------------------------------------------------------------*/
Vertex create_Vertex(int id){                             /*Construo o vertice*/
    Vertex v;
    v = (struct vertex *)malloc(sizeof(struct vertex));
    v->id = id;
    v->low = INT_MAX;
    v->d = INT_MAX;  
    return v;  
}

void create_Vertex_array(int n_nodes){
    int id;
    vertex_array = (struct vertex **)malloc(sizeof(struct vertex)*(n_nodes+1));
    for (id = 1; id<n_nodes+1; id++){          /*Constroi todos os vertices*/
        vertex_array[id] = create_Vertex(id);
    }
}

void create_edges_array(int n_edges){
    int begin, end;
    int i;
    edges_array =  malloc(sizeof(int*)*n_edges);
    for (i = 0; i<n_edges; i++){
        scanf("%d %d", &begin, &end);
        edges_array[i] = malloc(sizeof(int)*2); 
        edges_array[i][0] = begin;
        edges_array[i][1] = end;
    }
}
/*Nota: Quando eu crio os neighbours ele ja esta ordenado*/
void create_neighbours(int n_edges, int n_nodes){
    int j = 1;
    int i;
    neighbours_array = (int*)malloc((sizeof(int)*n_edges + 1));
    for(i = 0; i<n_edges; i++){
        if (i == n_edges - 1){
            neighbours_array[j] = i;
            break;
        }

        else if(edges_array[i][0] != edges_array[i+1][0]){
            neighbours_array[j] = i;
            j++;
        }
    }
}

/* ----------------------------------------------------------------------
                            S O R T I N G
------------------------------------------------------------------------*/
int compare ( const void *pa, const void *pb ) {
    const int *a = *(const int **)pa;
    const int *b = *(const int **)pb;
    if(a[0] == b[0])
        return a[1] - b[1];
    else
        return a[0] - b[0];
}

void change_ids(int n_edges){
    int i;
    for(i = 0; i<n_edges; i++){
        int id_1 = edges_array[i][0];
        int id_2 = edges_array[i][1];
        edges_array[i][0] = vertex_array[id_1]->id;
        edges_array[i][1] = vertex_array[id_2]->id;

    }
}

void print_output(int n_edges){
    int **edges_array_aux;
    int last_begin = 0;
    int last_end = 0;
    int n_bridges = 0;
    int j = 0;
    int i;

    edges_array_aux =  malloc(sizeof(int*)*n_edges);
    for(i = 0; i<n_edges; i++){
        if(edges_array[i][0] != edges_array[i][1]){
            if (edges_array[i][0] != last_begin || edges_array[i][1] != last_end) {
                edges_array_aux[j] = calloc(2,sizeof(int)); 
                edges_array_aux[j][0] = edges_array[i][0];
                edges_array_aux[j][1] = edges_array[i][1];
                last_begin = edges_array[i][0]; last_end = edges_array[i][1];
                n_bridges++;
                j++;
            }
        }
    }
    printf("%d\n%d\n", n_sccs, n_bridges);  
    if(n_bridges != 0){      
        for(i = 0; i<n_bridges; i++){
            printf("%d %d\n", edges_array_aux[i][0], edges_array_aux[i][1]);            
        }
    }
}

/* ----------------------------------------------------------------------
                    T A R J A N     A L G O R T I H M
------------------------------------------------------------------------*/
/*
*   Em vez de mandar o int tambem posso mandar o vertex, mas acho que vai
*       dar ao mesmo.
*   Por outro lado a stack tambem tem o vertex, por isso se calhar tb posso
*       mudar.
*   Nota: Se calhar devia mandar-lhe o vertex e nao o id, porque o codigo
*       fica pesado
*   Nota2: Fazer outra versao so com inteiros. so o vertex_array a dar a
*       a informacao sobre os d e lows
*/

void tarjan_visit(Vertex u, int n_nodes, int *scc);
void tarjan_algorithm(int n_nodes){
    init_stack(n_nodes);

    int u;
    int scc[n_nodes+1];
    Visited = 0;

    /*Este for nao e e necessario pois ele ja tem os ds a infinity*/
    for(u = 1; u < n_nodes+1; u++){
        vertex_array[u]->d = INT_MAX;
    }

    for(u = 1; u < n_nodes+1; u++){
        if(vertex_array[u]->d == INT_MAX){
            tarjan_visit(vertex_array[u], n_nodes, scc);
        }
    }
}

void tarjan_visit(Vertex u, int n_nodes, int *scc){
    int u_id = u->id;
    u->d = Visited;
    u->low = Visited;
    Visited++;

    /*Push do vertice para a stack, a stack e de vertex;*/
    push(u_id);    
    
    int v_id;
    /*Percorrer os vizinhos do vertice u;*/
    for(v_id = neighbours_array[u_id-1]; v_id < neighbours_array[u_id]+1; v_id++){
        int v = edges_array[v_id][1]; /*Novo vizinho*/
        if(vertex_array[v]->d == INT_MAX || in_stack(v)){
            if(vertex_array[v]->d == INT_MAX){
                tarjan_visit(vertex_array[v], n_nodes, scc);
            }
            u->low = min(u->low, vertex_array[v]->low);
        }
    }
    if(u->d == u->low){
        int id = 0;
        int i = 0;
        int count = 0;
        int new_id;
        while(id != u_id){
            id = pop();
            scc[i] = id;
            i++;
            count++;
        }
        new_id = id;
        for(i = 0; i<count; i++){
            int old_id = scc[i];
            vertex_array[old_id]->id = new_id;
        }
        n_sccs++;
    }
}


/* ----------------------------------------------------------------------
                                 M A I N
------------------------------------------------------------------------*/
int main(){
    int n_nodes, n_edges;
    scanf("%d", &n_nodes);
    scanf("%d", &n_edges);

    /*Cria os vertices todos*/
    create_Vertex_array(n_nodes);
    
    /*Cria os edges e organiza [[1,2],[2,3],[3,1]] por exemplo*/
    create_edges_array(n_edges);
    qsort(edges_array, n_edges, sizeof edges_array[0], compare);

    /*Cria as posicoes de visinhos dos vertices*/
    create_neighbours(n_edges, n_nodes);

    /*Fazer o tarjan*/
    tarjan_algorithm(n_nodes);

    /*Alterar o input para quase_output*/
    change_ids(n_edges);
    qsort(edges_array, n_edges, sizeof edges_array[0], compare);
    
    print_output(n_edges);

    return 0;
}

