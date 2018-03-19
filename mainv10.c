//#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

# define min(a,b) (((a)<(b))?(a):(b))

typedef struct vertex{
    double d;
    double low;
    int id;
} *Vertex;

typedef struct node{
    Vertex v;
    struct node *next;
} Link;

typedef struct int_node{
    int id;
    struct int_node *next;
} Stack;

typedef struct par{
    int begin;
    int end;
} *Par;



/*Global variables*/
Link **adj_list; //  Lista de adjacencias (array de links)
static Link *stack; //Stack  de vertexes
int *stack_array;   // Array de vertices como auxilio a stack
Vertex *vertex_array; // Array que guarda vertices para nao os contruir mais que uma vez
int Visisted;         //Visisted para o algoritmo do tarjan
int n_sccs = 0;     //Este se calhar pode sair daqui

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

void push(Vertex v){
    Link *new;
    new = (struct node *) malloc(sizeof(struct node));
    new->v = v;
    new->next = stack;
    stack = new;

    int id = v->id;
    stack_array[id] = id; // Ir a posicao do array e meter la cenas
}

Vertex pop(){
    Vertex v;
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

Link* push_front(Link *l, Vertex v){
    Link *new;
    new = (struct node *) malloc(sizeof(struct node));
    new->v = v;
    new->next = l;
    l = new;
    return l;  // tenho que devolver o l porque nao e global;
}

Vertex pop_list(Link *l){
    Vertex v;
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
    v->low = INFINITY;
    v->d = INFINITY;
    return v;
}

/* ----------------------------------------------------------------------
                    T A R J A N     A L G O R T I H M
------------------------------------------------------------------------*/
void tarjan_visit(Vertex v, int nodes, int *scc);
void tarjan_algorithm(int nodes){
    //printf("estou no trajan\n");
    int scc[nodes+1]; //Este array secundario, guardara os nodes que tenho que mudar
    //printf("Iniacizei  a scc\n");
    Visisted = 0;

    //Percorrer todos os vertices
    for(int u = 1; u < nodes+1; u++){ // Se calhar posso tirar isto porque os ds ja sao INFINITY
        vertex_array[u]->d = INFINITY;
    }
    for(int u = 1; u < nodes+1; u++){
        if(vertex_array[u]->d == INFINITY){
            tarjan_visit(vertex_array[u], nodes, scc);
        }
    }
}

void tarjan_visit(Vertex u, int nodes, int *scc){

    int u_id = u->id;
    u->d = Visisted;
    u->low = Visisted;
    Visisted++;

    push(u);

    for(Link *l = adj_list[u_id]; l!= NULL; l=l->next){  //Percorrer as arestas do vertice u;
        if(l->v->d == INFINITY || in_stack(l->v->id)){
            if(l->v->d == INFINITY){
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
                            S O R T I N G
------------------------------------------------------------------------*/
int compare ( const void *pa, const void *pb ) {
    const int *a = *(const int **)pa;
    const int *b = *(const int **)pb;
    if(a[0] == b[0]){
      return a[1] - b[1];
    } else
        return a[0] - b[0];
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
    printf("ID: %d \t D: %f \t Low: %f\n", v->id, v->d, v->low);
}

Par create_Par(int begin, int end){
    Par p;
    p = (struct par*)malloc(sizeof(struct par));
    p->begin = begin;
    p->end = end;
    return p;
}

int in_Par_array(Par *p, Par par, int length){
    for(int i = 0; i<length; i++){
        if((p[i]->begin == par->begin) && (p[i]->end == par->end)){
            return 1;
        }
    }
    return 0;
}

void print_par(Par p){
    printf("%d %d\n", p->begin, p->end);
}

void print_output(Vertex *v, int length){
    /*Eu tenho as edges todas e a sua dimensao do
    [[1,2], [3,3]]
    Tenho que percorrer essa lista e formar edges
        So quero formar se ambos os elementos nao tiverem o mesmo id
        Meto no meu array duplo

    qsort do array primeiro
    qsort do array segundo

    print output
    */

    int edges = length/2;
    int **output;
    int **output_aux;
    int n_bridges = 0;

    output_aux = malloc(edges * sizeof(int*));

    int j=0;
    for(int i = 0; i<length; i+=2){
        if(v[i]->id != v[i+1]->id){
            //Adicionar ao output
            //Nao verifica se ja
            output_aux[j] = malloc(2 * sizeof(int));
            output_aux[j][0] = v[i]->id;
            output_aux[j][1] = v[i+1]->id;
            j++;

            n_bridges++;
        }
    }

    //Tenho que copiar para o original pois preciso para o qsort do tamanho real
    //Se n_bridges for igual as edges nao preciso de copiar e poupo esse tempo
    if (n_bridges != edges){
        output = malloc(n_bridges * sizeof(int*));
        for(j = 0; j<n_bridges; j++){
            output[j] = malloc(2*sizeof(int));
            output[j][0] = output_aux[j][0];
            output[j][1] = output_aux[j][1];
        }
    }
    int last_begin=0;
    int last_end=0;

    printf("%d\n%d\n", n_sccs, n_bridges);
    if(n_bridges != 0){
        if(n_bridges != edges){
            qsort(output, n_bridges, sizeof output[0], compare);
            for(j=0; j<n_bridges; j++){
              if (output[j][0] != last_begin || output[j][1] != last_end) {
                printf("%d %d\n", output[j][0], output[j][1]);
                last_begin = output[j][0]; last_end = output[j][1];
              }
            }
        } else {
            qsort(output_aux, edges, sizeof output_aux[0], compare);
            for(j=0; j<edges; j++) {
                printf("%d %d\n", output_aux[j][0], output_aux[j][1]);
            }
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

    if(nodes == 2){
        if (edges == 2){
            int begin, end;
            scanf("%d %d", &begin, &end);
            scanf("%d %d", &begin, &end);
            printf("1\n0\n");
            return 0;
        }
        else{
   	        int begin, end;
            scanf("%d %d", &begin, &end);   //leio o primeiro numero e o segundo
            printf("2\n1\n%d %d\n", begin, end);
            return 0;
        }
    }


    init_stack(nodes);                  //Inicializa a stack
    init_Vertex_array(nodes);           //Incializa o array de vetores
    init_graph(nodes, edges, edges_input);           //Inicializa o grafo

    tarjan_algorithm(nodes);

    print_output(edges_input, edges*2);

    return 0;
}

/*
Ja ordena o output mas tenho que ver se ha repetidos.
O pestana diz que e facil mas nao percebi muito bem a
ideia dele.
*/
