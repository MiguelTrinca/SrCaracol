#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

# define min(a,b) (((a)<(b))?(a):(b))
# define NIL -1
/*Global variables*/
Link **adj_list; //  Lista de adjacencias (array de links)
static Link *stack; //Stack  de vertexes
int *stack_array;   // Array de vertices como auxilio a stack
Vertex *vertex_array; // Array que guarda vertices para nao os contruir mais que uma vez
int visited;         //Visisted para o algoritmo do tarjan
int n_sccs = 0;     //Este se calhar pode sair daqui

# define item Vertex

/* ----------------------------------------------------------------------
                                S T A C K
------------------------------------------------------------------------*/
void init_stack(int V){  //Inicializa a pilha V-> numero de vertices
    stack = NULL;
    stack_array = (int *)malloc(sizeof(int)*(V+1));
}

int is_empty_stack(){
    return (stack == NULL);
}

int in_stack(int value){
    return stack_array[value] == value;
}

void push(item v){
    Link *new;
    new = (struct node *) malloc(sizeof(struct node));
    new->v = v;
    new->next = stack;
    stack = new;

    int id = v->id;
    stack_array[id] = id; // Ir a posicao do array e meter la cenas
}

item pop(){
    item v;
    Link *old;

    if(!is_empty_stack()){
        v = stack->v;
        old = stack;
        stack = stack->next;
        free(old);

        int id = v->id;
        stack_array[id] = 0;
        return v;
    }
    else
        return NULL;
}


/* ----------------------------------------------------------------------
                                L I S T
------------------------------------------------------------------------*/
void init_list(Link *l){
    l = NULL;
}

int is_empty_list(Link *l){
    return (l==NULL);
}

Link* push_front(Link *l, item v){
    Link *new;
    new = (struct node *) malloc(sizeof(struct node));
    new->v = v;
    new->next = l;
    l = new;
    return l;  // tenho que devolver o l porque nao e global;
}

item pop_list(Link *l){
    item v;
    Link *old;
    if(!is_empty_list(l)){
        v = l->v;
        old = l;
        l = l->next;
        free(old);

        return v;
    }
    else
        return NULL;
}

void print_list(Link *l){
    for(Link *x = l; x!= NULL; x=x->next){
        printf("Vertice: %d\n", x->v->id);
    }
}


/* ----------------------------------------------------------------------
                                G R A P H
------------------------------------------------------------------------*/
void add_edge(int begin, int end);
void init_graph(int nodes, int edges, Vertex *edges_input){
    adj_list = (struct node **)malloc(sizeof(struct node *)*(nodes+1));
    for (int id=0; id < nodes+1; id++) {
        init_list(adj_list[id]);   // Mete todos os ponteiro do array a NULL; Ou seja inicializa as listas
    }
    int begin, end;
    int x = 0;
    for (int i=0; i < edges; i++) {    //Este for e para as linhas do input
   	    scanf("%d %d", &begin, &end);   //leio o primeiro numero e o segundo
        add_edge(begin, end);         //Comeca a construir o grafo segundo o input
        edges_input[x] = vertex_array[begin]; //Escrevo o input no array
        edges_input[x+1] = vertex_array[end]; //Escrevo o input no array
        x += 2;  // vou doia a dois
       }
}

Vertex create_Vertex(int id);
void init_Vertex_array(int v){
    vertex_array = (struct vertex **)malloc(sizeof(struct vertex)*(v+1));
    for (int id = 0; id<v+1; id++){          //Constroi todos os vertices
        vertex_array[id] = create_Vertex(id);
    }
}

void add_edge(int begin, int end){
    Vertex v = vertex_array[end];
    adj_list[begin] = push_front(adj_list[begin], v);     // Ponho a edge no grafo (lista de adjacenicas)
}

Vertex create_Vertex(int id){                             //Construo o vertice
    Vertex v;
    v = (struct vertex *)malloc(sizeof(struct vertex));
    v->id = id;
    v->low = NIL;
    v->d = NIL;
    return v;
}

/* ----------------------------------------------------------------------
                    T A R J A N     A L G O R T I H M
------------------------------------------------------------------------*/
void tarjan_visit(Vertex v, int nodes, int *scc);
void tarjan_algorithm(int nodes){
    int scc[nodes+1]; //Este array secundario, guardara os nodes que tenho que mudar

    visited = 0;
    //Percorrer todos os vertices
    for(int u = 1; u < nodes+1; u++){ // Se calhar posso tirar isto porque os ds ja sao NIL
        vertex_array[u]->d = NIL;
    }
    for(int u = 1; u < nodes+1; u++){
        if(vertex_array[u]->d == NIL){
            tarjan_visit(vertex_array[u], nodes, scc);
        }
    }
}

void tarjan_visit(Vertex u, int nodes, int *scc){

    int u_id = u->id;
    u->d = visited;
    u->low = visited;
    printf("d[u]=%d low[u]=%d visited=%d\n", u->d, u->low, visited);
    visited++;

    push(u);

    for(Link *l = adj_list[u_id]; l!= NULL; l=l->next){  //Percorrer as arestas do vertice u;
        if(l->v->d == NIL || in_stack(l->v->id)){
            if(l->v->d == NIL){
                tarjan_visit(l->v, nodes, scc);
            }
            u->low = min(u->low, l->v->low);
        }
    }
    if(u->d == u->low){
        Vertex v;
        int i = 0;
        int count = 0;  // Serve para ter a certeza que nao saimos do array quando estoua mudar o id dos vertices
        int new_id;     //Guarda o novo id que tenho que mudar para os vertices
        while(v->id != u->id){
            v = pop(); //Sai todos os vertices que da stact que pertencem a scc
            scc[i] = v->id; // Ponho no array;
            i++;
            count++;
        }
        new_id = v->id;  // Este e o valor que eu tenho que mudar

        for(int u = 0; u<count; u++){
            int old_id = scc[u];
            vertex_array[old_id]->id = new_id;
        }

        n_sccs++; //Incrementa o numero de sccs (Precisamos para o output)
    }
}

/* ----------------------------------------------------------------------
                                O T H E R
------------------------------------------------------------------------*/
//Funcoes auxiliares de debug e para imprimir o output

void print_int_array(int *a, int length){
    for(int i = 1; i<length; i++){
        printf("-- Imprimindo Stack --\n");
        printf("Posicao: %d \t Valor: %d\n", i, a[i]);
    }
}

void print_Vertex_array(Vertex *v, int length){
    for(int i = 0; i<length; i++){
        printf("-- Imprimindo Vertice --\n");
        printf("Posicao: %d \t Valor: %d\n", i, v[i]->id);
    }
}

void print_vertex(Vertex v){
    printf("ID: %d \t D: %d \t Low: %d\n", v->id, v->d, v->low);
}

void print_output(Vertex *v, int length){
    int i = 0;
    int j = 0;
    int n_bridges = 0;
    printf("%d\n", n_sccs);
    while(j<length){
        if( v[j]->id != v[j+1]->id){
            n_bridges++;
        }
        j += 2;
    }
    printf("%d\n", n_bridges);
    if(n_bridges != 0){
        while(i<length){
            if( v[i]->id != v[i+1]->id){
                printf("%d %d\n", v[i]->id, v[i+1]->id);
            }
            i += 2;
        }
    }
}
/* ----------------------------------------------------------------------
                                 M A I N
------------------------------------------------------------------------*/
int main(){
    int nodes;
    int edges;
    scanf("%d", &nodes);
    scanf("%d", &edges);

    Vertex edges_input[edges*2];        //Array que guarda os inputs

    init_stack(nodes);                  //Inicializa a stack
    init_Vertex_array(nodes);           //Incializa o array de vetores
    init_graph(nodes, edges, edges_input);           //Inicializa o grafo

    tarjan_algorithm(nodes);

    print_output(edges_input, edges*2);

    return 0;
}

/*Tenho que ordenar o output, caso
3
3
1 2
2 3
1 3
Output tem de ser
1 2
1 3
2 3

Ver porque e que para o terceiro input estou a imprimir mais cenas
*/
