/*
  Basic idea for the program: PANFUL Way of DOING THINGS

*/
# include <stdio.h>
# include <stdlib.h>

# define key(i) (i*2)
# define d(i) (i*2)
# define low(i) (i*2+1)
# define NIL -1

typedef struct node {
  int d;
  int l;
} Node;

typedef struct edge {
  int x;
  int y;
} Edge;

Edge *create_edges_array(int size){
  return (Edge*) malloc(sizeof(Edge)*size);
}

/* Parses edges and with count sort puts them in order
 */
void parse_edges2(Edge *edges, int edges_n, int *edges_cnt) {
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

// NODE OPERATIONS

/* ----------------------------------------------------------------------
                                 M A I N
------------------------------------------------------------------------*/
int main(){
    int nodes_n;
    int edges_n;
    scanf("%d", &nodes_n);
    scanf("%d", &edges_n);

    //stack
    int stack[nodes_n]; // bigger than it needs
    int *sp = stack;  // stack pointer

    // edges
    int *edges = malloc(sizeof(int)*2*edges_n); // Array que guarda os inputs
    int *edges_index = malloc(sizeof(int)*nodes_n+1); // Array do count

    //parse_edges(edges, edges_n, edges_index); // parses and sorts
    // edges_index is the index of the edges's beginning

    // trying edges2
    //Edge *edges2 = create_edges_array(edges_n);
    Edge *edges2 = create_edges_array(edges_n);
    parse_edges2(edges2, edges_n, edges_index);
    for (int i=0; i<edges_n;i++) {
      printf("%d %d\n", edges2[i].x, edges2[i].y);
    }

    // nodes (starts at 1)
    int *nodes = malloc(sizeof(int)*2*(nodes_n+1)); // node [d,low]
    for (int i=1; i<=nodes_n;i++) {
      nodes[d(i)] = NIL;   // set d
      nodes[low(i)] = NIL; // set low
    }

    //init_stack(nodes);                  //Inicializa a stack
    //init_Vertex_array(nodes);           //Incializa o array de vetores
    //init_graph(nodes, edges, edges_input);           //Inicializa o grafo

    //tarjan_algorithm(nodes);

    //print_output(edges_input, edges*2);

    return 0;
}
