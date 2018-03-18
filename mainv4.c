/*
  Basic idea for the program: PANFUL Way of DOING THINGS

*/
# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# define key(i) (i*2)
# define d(i) (i*2)
# define low(i) (i*2+1)
# define min(a,b) (((a)<(b))?(a):(b))
# define NIL -1
# define Item

typedef int bool;
// Stores information about a node
typedef struct node_info {
  double d;
  double low;
} Node_info;

typedef struct stack {
  bool *in_stack; // array that stores if a given node is in the stack
  int sp; // stack pointer
  int *stack;
  int max;
} Stack;

typedef struct edge {
  int x;
  int y;
} Edge;

typedef struct graph {
  Edge *edges;
  int *edges_index;
  int edges_n;
  int nodes_n;
} Graph;


//Global elite
int Visisted;
int n_sccs=0;

/* ----------------------------------------------------------------------
                              E D G E S
------------------------------------------------------------------------*/
Edge *create_edges_array(int size){
  return (Edge*) malloc(sizeof(Edge)*size);
}

/* Parses edges and with count sort puts them in order
 */
void parse_edges(Edge *edges, int edges_n, int *edges_cnt) {
  Edge *edges_input = create_edges_array(edges_n);  // TODO one of these is
  Edge *edges_tmp  = create_edges_array(edges_n);   // uncessessary TODO
  for (int i=0; i<edges_n; i++)
    scanf("%d %d", &(edges_tmp[i].x), &(edges_tmp[i].y)); // pointer arithmetic

  for (int i=0; i<edges_n;i++) {
    printf("|%d %d|\n", edges_tmp[i].x, edges_tmp[i].y);
  }

  // SORTING IN Y
  /*int i, j, cnt[edges_n+1];
  for (j = 0; j<edges_n; j++) // reset count
    cnt[j] = 0;
  for (i=0; i<edges_n; i++)
    cnt[edges_input[i].y+1]++;
  for (j = 1; j<=edges_n; j++)
    cnt[j] += cnt[j-1];
  for (i=0; i<edges_n; i++) {
    edges_tmp[cnt[edges_input[i].y]].x = edges_input[i].x; // copy end
    edges_tmp[cnt[edges_input[i].y]++].y = edges_input[i].y; // copy beg
  }*/

  // SORTING IN X
  int i, j, cnt[edges_n+1];
  for (j = 0; j<edges_n; j++) // reset count
    cnt[j] = 0;
  for (i=0; i<edges_n; i++)
    cnt[edges_tmp[i].x+1]++;
  for (j = 1; j<=edges_n; j++)
    cnt[j] += cnt[j-1];
  for (j=0; j<edges_n; j++)
    edges_cnt[j] = cnt[j]; // copia o cnt para edges_cnt
  for (i=0; i<edges_n; i++) {
    edges[cnt[edges_tmp[i].x]].y = edges_tmp[i].y; // copy end
    edges[cnt[edges_tmp[i].x]++].x = edges_tmp[i].x; // copy beg
  }
  free(edges_tmp);
  free(edges_input);

}

/* ----------------------------------------------------------------------
                              S T A C K
------------------------------------------------------------------------*/
/*
 * The stack is an array of size nodes_n given that the tarjan algorithm
 * will not put repeated nodes in the stack (verify this!!!TODO)
 */
Stack* init_stack(int nodes_n){
  Stack *s_struct = (Stack*) malloc(sizeof(Stack));
  s_struct->stack = (int*) malloc(sizeof(int)*(nodes_n+1)); // TODO - why +1
  s_struct->in_stack = (int*) malloc(sizeof(int)*(nodes_n+1));
  s_struct->sp = 0;
  s_struct->max = nodes_n;
  return s_struct;
}

int is_empty_stack(Stack *s){
    return (s->sp == 0);
}

void push(Stack *s, int v) {
  if (s->sp > s->max) {
    printf("ERROR: reached stack limit\n");
    return;
  }
  s->stack[(s->sp)++] = v;
  s->in_stack[v] = 1;
}

int pop(Stack *s) {
  int v = s->stack[--(s->sp)];
  s->in_stack[v] = 0;
  return v;
}

int in_stack(Stack *s, int v) {
  return s->in_stack[v];
}

void print_stack(Stack *s) {
  int sp = s->sp;
  sp--;
  printf("printing Stack: >");
  while (sp >= 0) {
    printf("%d|",s->stack[sp--]);
  }printf("\n");
}

/* ----------------------------------------------------------------------
                      N O D E   O P E R A T I O N
------------------------------------------------------------------------*/
/*
 * Returns the index of the first neighbour
 */
int first_neighbour(int node_id, int* edges_index) {
  return edges_index[node_id];
}

/*
 * Returns the index of the last neighbour
 */
int last_neighbour(int node_id, int* edges_index, int nodes_n, int edges_n) {
  if (node_id == nodes_n)
    return edges_n; // if it is the last node
  return edges_index[node_id+1];
}

/* ----------------------------------------------------------------------
                    T A R J A N     A L G O R T I H M
------------------------------------------------------------------------*/
void tarjan_visit(int v, int nodes, int edges, int *scc, Node_info *node_info, Stack *stack, int *edges_index, Edge *edges_array);
void tarjan_algorithm(int nodes, int edges,  Node_info *node_info, Stack *stack, int *edges_index, Edge *edges_array){
    int scc[nodes+1]; //Este array secundario, guardara os nodes que tenho que mudar
    Visisted = 0;

    //Percorrer todos os vertices
    /*for(int u = 1; u < nodes+1; u++){ // Se calhar posso tirar isto porque os ds ja sao INFINITY
        vertex_array[u]->d = INFINITY;
    }*/
    for(int u = 1; u < nodes+1; u++){
        if(node_info[u].d == INFINITY){
            tarjan_visit(u, nodes, edges, scc, node_info, stack, edges_index, edges_array);
        }
    }
}

void tarjan_visit(int u, int nodes, int edges, int *scc, Node_info *node_info, Stack *stack, int *edges_index, Edge *edges_array){

    node_info[u].d = Visisted;
    node_info[u].low = Visisted;
    Visisted++;

    push(stack, u);

    int first = first_neighbour(u, edges_index);
    int last = last_neighbour(u, edges_index, nodes, edges);
    for (int j=first; j<last; j++){
      if (node_info[j].d == INFINITY || in_stack(stack, j)){
        if(node_info[j].d == INFINITY){
          tarjan_visit(j, nodes, edges, scc, node_info, stack, edges_index, edges_array);
        }
      }
      node_info[u].low = min(node_info[u].low, node_info[j].low);
    }


    if(node_info[u].d == node_info[u].low){
        int v = 0;
        int i = 0;
        int count = 0;
        int new_id;     //Guarda o novo id que tenho que mudar para os vertices
        while(v != u){
            v = pop(stack); //Sai todos os vertices que da stact que pertencem a scc
            scc[i] = v; // Ponho no array;
            i++;
            count++;
        }
        new_id = v;  // Este e o valor que eu tenho que mudar

        for(int u = 0; u<count; u++){
            int old_id = scc[u];
            for(int i = 0; i<nodes+1; i++){
              if (edges_array[i].y == old_id) edges_array[u].y = new_id;
              if (edges_array[i].x == old_id) edges_array[u].x = new_id;
            }
        }
        n_sccs++; //Incrementa o numero de sccs (Precisamos para o output)
    }
}



/* ----------------------------------------------------------------------
                                 M A I N
------------------------------------------------------------------------*/
int main(){
    int nodes_n;
    int edges_n;
    scanf("%d", &nodes_n);
    scanf("%d", &edges_n);

    //stack
    Stack *stack = init_stack(nodes_n); //Inicializa a stack


    // edges
    int *edges_index = malloc(sizeof(int)*nodes_n+1); // Array do count
    Edge *edges = create_edges_array(edges_n);
    parse_edges(edges, edges_n, edges_index);
    // TODO verificar edges_index se começa em 1 /!\ TODO

    for (int i=0; i<edges_n;i++) {
      printf("%d %d\n", edges[i].x, edges[i].y);
    }

    // nodes (starts at 1)
    Node_info *node_info = malloc(sizeof(Node_info)*(nodes_n+1)); // node [d,low]
    for (int i=1; i<=nodes_n;i++) {
      node_info[i].d = INFINITY;   // set d
      node_info[i].low = INFINITY; // set low
    }

    // DEBUG Printing de todos os neighbours
    for (int i=1;i<=nodes_n;i++){
      printf("\nprinting neighbours of %d\n", i);
      int first = first_neighbour(i, edges_index);
      int last = last_neighbour(i,edges_index,nodes_n, edges_n);
      for (int j=first; j<last; j++)
        printf("|%d", edges[j].y);
    }

    tarjan_algorithm(nodes_n, edges_n, node_info, stack, edges_index, edges);

    return 0;
}
