/*
  Basic idea for the program: PANFUL Way of DOING THINGS

*/
# include <stdio.h>
# include <stdlib.h>

# define key(i) (i*2)
# define d(i) (i*2)
# define low(i) (i*2+1)
# define min(a,b) (((a)<(b))?(a):(b))
# define NIL -1
# define Item

typedef int bool;
// Stores information about a node
typedef struct node_info {
  int d;
  int l;
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
  for (int i=0; i<edges_n; i++){
    scanf("%d %d", &(edges_input[i].x), &(edges_input[i].y)); // pointer arithmetic
    //scanf("%d %d", &k, &p); // pointer arithmetic
    //printf("%d: %d %d\n",i, edges_input[i].x, edges_input[i].y );
  }

  for (int i=0; i<edges_n;i++) {
    printf("|%d %d|\n", edges_input[i].x, edges_input[i].y);
  }

  // SORTING IN Y
  int i, j, cnt[edges_n+1];
  for (j = 0; j<edges_n; j++) // reset count
    cnt[j] = 0;
  for (i=0; i<edges_n; i++)
    cnt[edges_input[i].y+1]++;
  for (j = 1; j<=edges_n; j++)
    cnt[j] += cnt[j-1];
  for (j=0; j<edges_n; j++)
    edges_cnt[j] = cnt[j]; // copia o cnt para edges_cnt
  for (i=0; i<edges_n; i++) {
    edges_tmp[cnt[edges_input[i].y]].x = edges_input[i].x; // copy end
    edges_tmp[cnt[edges_input[i].y]++].y = edges_input[i].y; // copy beg
  }

  // SORTING IN X
  //int i, j, cnt[edges_n+1];
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
  int *stack = (int*) malloc(sizeof(int)*(nodes_n+1)); // TODO - why +1
  int *in_stack = (int*) malloc(sizeof(int)*(nodes_n+1));
  Stack *s_struct = (Stack*) malloc(sizeof(Stack));
  s_struct->stack = stack;
  s_struct->in_stack = in_stack;
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

int instack(Stack *s, int v) {
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
    for (int i=0; i<edges_n;i++) {
      printf("%d %d\n", edges[i].x, edges[i].y);
    }

    // nodes (starts at 1)
    Node_info *nodes_info = malloc(sizeof(Node_info)*(nodes_n+1)); // node [d,low]
    for (int i=1; i<=nodes_n;i++) {
      nodes_info[i].d = NIL;   // set d
      nodes_info[i].l = NIL; // set low
    }

    // DEBUG Printing de todos os neighbours
    for (int i=1;i<=nodes_n;i++){
      printf("\nprinting neighbours of %d\n", i);
      int first = first_neighbour(i, edges_index);
      int last = last_neighbour(i,edges_index,nodes_n, edges_n);
      for (int j=first; j<last; j++)
        printf("|%d", edges[j].y);
    }

    //init_Vertex_array(nodes);           //Incializa o array de vetores
    //init_graph(nodes, edges, edges_input);           //Inicializa o grafo

    //tarjan_algorithm(nodes);

    //print_output(edges_input, edges*2);

    return 0;
}
