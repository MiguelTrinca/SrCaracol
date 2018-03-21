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

Vertex *vertex_array;
static Stack *stack;

/* ----------------------------------------------------------------------
                                S T A C K
------------------------------------------------------------------------*/
void init_stack(int V){
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

    stack_array[id] = id; 
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
Vertex create_Vertex(int id){                 
    Vertex v;
    v = (struct vertex *)malloc(sizeof(struct vertex));
    v->id = id;
    v->low = INT_MAX;
    v->d = INT_MAX;  
    return v;  
}

void create_Vertex_array(int n_nodes){
    /*
    *   Construimos todos os vertices a priori. Se n_nodes = 5, os vertices serao
    *       1,2,3,4,5.
    */
    int id;
    vertex_array = (struct vertex **)malloc(sizeof(struct vertex)*(n_nodes+1));
    for (id = 1; id<n_nodes+1; id++){       
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

void create_neighbours(int n_edges, int n_nodes){
    /*
    *   O neighbours_array e um array que guarda a posicao inicial de cada vertice,
    *       no edges_array. Como o edges_array esta ordenado sabemos exatamente 
    *       onde estao os vizinhos de um certo vertice. Sabemos tambem que o 
    *           tamanho, ou o grau (degree), e a posicao seguinte no neighbours_
    *           _array. Por exemplo:
    *              edges_array = [[1,2],[1,3],[1,4],[2,4],[3,4],[4,2]]
    *              neighbours_array = [0,0,3,4,5].
    *
    *   Cada posicao do neighbours_array corresponde ao vertice, no mesmo exemplo,
    *       o vertice 1 comeca na posicao 0 do edges_array e o vertice 4 comeca na 
    *       posicao 5 do edges_array.
    */

    int id = 1;
    int i;
    neighbours_array = (int*)malloc((sizeof(int)*n_nodes + 1));
    for(i = 0; i<n_edges; i++){
        if(id == 1){
            neighbours_array[id] = 0;
        }

        if (id != edges_array[i][0]){
            id = edges_array[i][0];
            neighbours_array[id] = i;
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
    /*
    *   Mudar todos os ids, no edges_array, para podermos fazer a impressao
    */
    int i;
    for(i = 0; i<n_edges; i++){
        int id_1 = edges_array[i][0];
        int id_2 = edges_array[i][1];
        edges_array[i][0] = vertex_array[id_1]->id;
        edges_array[i][1] = vertex_array[id_2]->id;

    }
}

void print_output(int n_edges){
    /*
    *   Criamos o array edges_array_aux que vai ter o output segundo as seguintes 
    *       normas:
    *           1) Nao ha Begins e Ends repetidos, ou seja, [[1,1], [4,4]]
    *           2) Nao pode haver edges repetidos, ou seja, [[1,2],[1,2]]
    */
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

void tarjan_visit(Vertex u, int n_nodes, int n_edges, int *scc);
void tarjan_algorithm(int n_nodes, int n_edges){
    init_stack(n_nodes);

    int u;
    int scc[n_nodes+1];
    Visited = 0;

    /* 
    *   Nota: tirei daqui o for que mete os d e lows a infinito porque,
    *       quando construimos os vertices ja o fazemos.
    */

    for(u = 1; u < n_nodes+1; u++){
        if(vertex_array[u]->d == INT_MAX){
            tarjan_visit(vertex_array[u], n_nodes, n_edges, scc);
        }
    }
}

void tarjan_visit(Vertex u, int n_nodes, int n_edges, int *scc){
    int v_id;
    int u_id = u->id;
    u->d = Visited;
    u->low = Visited;
    Visited++;
    
    push(u_id);    

    /*
    *   Sempre que comeco uma visita tenho que mudar o tamanho (tam), que 
    *       corresponde a proxima posicao no neighbours_array.
    *   Existe o caso especial que quando o u_id == n_nodes, significa que estou
    *       na ultima posicao do neighbours_array, logo nao consigo aceder a
    *       neighbours_array[u_id + 1]. Deste modo, sabemos que o tamanho que 
    *       falta percorrer no edges_array e simplesmente o n_edges.
    */

    int tam = neighbours_array[u_id+1]; 
    if(u_id == n_nodes){
        tam = n_edges;
    }
    /*
    *   O primeiro for percorre os vizinhos do vertice u, atraves do neighbours_
    *       _array.
    */
    for(v_id = neighbours_array[u_id]; v_id < tam; v_id++){
        int v = edges_array[v_id][1];   /*Para facilitar a leitura*/ 
        if(vertex_array[v]->d == INT_MAX || in_stack(v)){
            if(vertex_array[v]->d == INT_MAX){
                tarjan_visit(vertex_array[v], n_nodes, n_edges, scc);
            }
            u->low = min(u->low, vertex_array[v]->low);
            
        }
    }
    if(u->d == u->low){
        int id = 0;
        int i = 0;
        int count = 0;

        /*
        *   O while vai tirando os ids da stack ate serem iguai a raiz da scc
        */

        while(id != u_id){
            id = pop();
            scc[i] = id;
            i++;
            count++;
        }

        /*
        *   Nao gosto deste for mas nao sei porque ele em certos casos, nao estava 
        *       a dar o minimo id. Por isso percorro o scc[i], para ter a certeza
        *       que o valor e o minimo.
        */
        
        int min_id = n_nodes;
        for (i = 0; i<count; i++){
            if (scc[i] < min_id){
                min_id = scc[i];
            }
        }


        /*
        *   Este for serve para alterar os ids dos vertices no vertex_array
        */

        for(i = 0; i<count; i++){
            int old_id = scc[i];
            vertex_array[old_id]->id = min_id;
        }

        /*
        *   Depois do Tarjan incremento o numero de sccs
        */
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

    /*Cria as posicoes iniciais dos vertices no edges_array*/
    create_neighbours(n_edges, n_nodes);

    /*Fazer o tarjan*/
    tarjan_algorithm(n_nodes, n_edges);

    /*Alterar o input para quase_output*/
    change_ids(n_edges);
    qsort(edges_array, n_edges, sizeof edges_array[0], compare);
    
    /*Impressao do output*/
    print_output(n_edges);

    return 0;
}

