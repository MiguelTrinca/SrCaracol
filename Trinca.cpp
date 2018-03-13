#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <stdio.h>
#include <stack>
#include <math.h>

//#define INFINITY /*implementation defined*/
#define min(a,b) (((a)<(b))?(a):(b))


class Node {
private:
	int _id = 0;
	double _d, _low;
	bool _in_stack;
public:
	Node(int id){
		_id = id;
		_d = INFINITY;
		_low = 0; // Antes estava infinity
		_in_stack = false; // tracks whether or not th Node is in the stack
	}
	bool inStack() { return _in_stack; }
	void setStack(bool in_stack) { _in_stack = in_stack;	}
	int getID() {return _id;}

	double get_d() {return _d;}
	double set_d(double val) {
		_d = val;
		std::cout << "d set to " << _d << "\n";
	}

	double get_low() {return _low;}
	double set_low(double val) {
		_low = val;
	 	std::cout << "low set to " << _low << "\n";
	}
};

class CustomStack : public std::stack<int> {
private:
	Node *nodes_array; //Array de nodes
	bool *in_stack;


public:
	CustomStack(Node *nod_array, bool *instack) { // /!\ don't know if it should be created internally or externaly
		in_stack = instack;
		nodes_array = nod_array;
	}

	void push(const int id) {
		std::stack<int>::push(id);
		in_stack[id] = true;
		nodes_array[id].setStack(true);
	}

	int pop() {
		int id = std::stack<int>::top();
		in_stack[id] = false;
		nodes_array[id].setStack(false);
		std::stack<int>::pop();
		return id;     // major bug. Nao tinhamos este return
	}

	Node popNode() {
		int id = std::stack<int>::top();
		std::stack<int>::pop();
		nodes_array[id].setStack(false);
		return nodes_array[id];
	}

	/* Checks if present in the stack in linear time (/!\ Hopefully)
	 */
	bool inStack(int id){
		return nodes_array[id].inStack();  //Devolve true se estiver la dentro
	}
};


class Graph {
private:
	int _nodes_num;
  int _edges_num;
	std::list<int> *adj_list;  //Array de listas
	Node *nodes_array; //Array de nodes

public:
	Graph(int nodes_num, int edges_num) {
		_nodes_num = nodes_num;
		_edges_num = edges_num;

		adj_list = new std::list<int>[_nodes_num+1]; //Criar o array com dimensao nodes + 1
		nodes_array[_nodes_num + 1];				//Criar o array com dimensao nodes + 1

		/*Criacao de todos os nodes quando e chamado o construtor do grafo
		/		estamos a supor que os id dos nodes sao sequenciais ou seja
		/		Se tivermos 3 nodes, sao 1,2 e 3.
		*/
		for(int id = 0; id<_nodes_num+1; id++)
			createNode(id);
	}

	void addEdge(int beg, int end) {
		adj_list[beg].push_front(end);  //Criar a edge.
	}

	/*Criacao de um node, caso o nosso postulado esteja errado*/
	void createNode(int id){
		//if(nodes_array[id].getID() == 0){ //Quero construir quando tenho NULL
			Node *n = new Node(id); //Crio o node
			nodes_array[id] = *n;          // Meto no array
		//}
	}

	void listNodes() {
		std::cout << "listing nodes" << "\n";
		for(int id = 0; id<_nodes_num+1; id++)
			std::cout << nodes_array[id].getID() << "\n";
	}

	void SCC();
	void SCCvisit(int u, CustomStack *stack, bool *in_stack, double *d, double *low, std::list<int> scc);
};

void Graph::SCC() {
	
	bool *in_stack = new bool[_nodes_num+1];
	double *d = new double[_nodes_num+1];
	double *low = new double[_nodes_num+1];
	CustomStack *stack = new CustomStack(nodes_array,in_stack);

	std::list<int> scc; // return value

	int u;
	for (u=1; u < _nodes_num+1; u++)
		d[u] = INFINITY;
		in_stack[u] = false;

	for (u=1; u < _nodes_num+1; u++) {
		if (d[u] == INFINITY)
			SCCvisit(u, stack, in_stack, d, low, scc);
		std::cout << "\n";
	}

	//return scc;
}


void Graph::SCCvisit(int u, CustomStack *stack, bool *in_stack, double *d, double *low, std::list<int> scc) {
	static int visited = 0;

	/*int id = node.getID();
	node.set_d(visited);
	node.set_low(visited);
	(visited)++;*/
	d[u] = visited;
	low[u] = visited;
	visited++;

	stack->push(u);  //Push int node to the stack
	int v;

	//Node *v_node; //Pointer to a node
	std::list<int>::iterator it;
	std::list<int> neighbours = adj_list[u]; // List of edges (neighbours of the node id)
	for (it = neighbours.begin(); it != neighbours.end(); it++) { //For every neighbour
		v = *it;
		//std::cout << "visiting " << v << "\n"; // debugging
		//*v_node = nodes_array[*v];
		if (d[v] == INFINITY || in_stack[v]) {
			if (d[v] == INFINITY)
				SCCvisit(v, stack, in_stack, d, low, scc);
			d[v] = min(low[v], low[u]);
		}
	}

	if (d[u] == low[u]){
		while (v != u){
			v = stack->pop();
			std::cout << v << "_";
			//scc.push_front(v);
		}
	}
	//std::cout << "\n";
}

/*Comentarios
Porque *v_node = nodes_array[*v]; e nao v_node = nodes_array[v];
Nao percebo porque fazemos *v_node sempre
Nao percebo o ultimo if comentei e escrevi um novo, acho que nao precisamos do node basta o inteiro
	podemos devolver um array de inteiros que e o scc
*/




int main(int argc, char** argv) {

   int nodes;
   int edges;

   scanf("%d", &nodes);
   scanf("%d", &edges);

   Graph g(nodes, edges);

   int beg, end;
   for (int i=0; i < edges; i++) {
   	scanf("%d %d", &beg, &end);
   	g.addEdge(beg, end);
   }
	 //g.listNodes();
	 g.SCC();

   std::cout << "number of nodes: " << nodes << "\n";
   std::cout << "number of edges: " << edges << "\n";

	 return 0;
}
