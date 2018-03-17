#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <stdio.h>
#include <stack>
//#include <math.h>

//#define NIL /*implementation defined*/
# define min(a,b) (((a)<(b))?(a):(b))
# define NIL -1

// auxiliary function - adds padding
std::string padding(int pad) {
	std::string res;
	while (pad > 0) {
		res+="-";
		pad--;
	}
	return res;
}


class CustomStack : public std::stack<int> {
public:
	CustomStack(){};

	void push(const int id) {
		std::stack<int>::push(id);
		//in_stack[id] = true;
	}

	int pop() {
		std::cout << "STACK SIZE BEFORE POP "<< std::stack<int>::size() << "\n";
		int id = std::stack<int>::top();
		//in_stack[id] = false;
		std::stack<int>::pop();
		std::cout << "STACK SIZE AFTER  POP "<< std::stack<int>::size() << "\n";
		return id;     // major bug. Nao tinhamos este return
	}
};


class Graph {
private:
	int _nodes_num;
  int _edges_num;
	std::list<int> *adj_list;  //Array de listas

public:
	Graph(int nodes_num, int edges_num) {
		_nodes_num = nodes_num;
		_edges_num = edges_num;

		adj_list = new std::list<int>[_nodes_num+1]; //Criar o array com dimensao nodes + 1
	}

	void addEdge(int beg, int end) {
		adj_list[beg].push_front(end);  //Criar a edge.
	}

	void SCC();
	void SCCvisit(int u, CustomStack *stack, bool *in_stack, int *d, int *low, int lv);
};

void Graph::SCC() {
	int lv = 0; // recursion level
	bool *in_stack = new bool[_nodes_num+1];
	int *d = new int[_nodes_num+1];
	int *low = new int[_nodes_num+1];
	CustomStack stack;

	int u;
	for (u=1; u < _nodes_num+1; u++)
		d[u] = NIL;
		in_stack[u] = false;

	std::cout << "====== Initiating Tarjan ======" << '\n';
	for (u=1; u < _nodes_num+1; u++) {
		if (d[u] == NIL)
			SCCvisit(u, &stack, in_stack, d, low, lv);
		std::cout << "\n";
	}

	/*std::cout << "printing result "<< scc.size() << '\n';
	while (!scc.empty()) {
		std::cout << scc.front() << "_";
		scc.pop_front();
	}*/

	//return scc;
}


void Graph::SCCvisit(int u, CustomStack *stack, bool *in_stack, int *d, int *low, int lv) {
	static int visited = 0;
	d[u] = visited;
	low[u] = visited;
	visited++;

	// debugging
	std::cout << padding(lv) << u << '\n';


	stack->push(u);  //Push int node to the stack
	in_stack[u] = true;

	int v;

	std::list<int>::iterator it;
	std::list<int> neighbours = adj_list[u]; // List of edges (neighbours of the node id)
	for (it = neighbours.begin(); it != neighbours.end(); it++) { //For every neighbour
		v = *it;
		if (d[v] == NIL || in_stack[v]) {
			if (d[v] == NIL)
				SCCvisit(v, stack, in_stack, d, low, lv+1);
			d[v] = min(low[v], low[u]);
		} /*else if (!in_stack[v]) {
			// this is the case in which we find cross-edges, which is what we want
		}*/
	}

	if (d[u] == low[u]){
		// starts a new scc
		std::list<int> scc;
		do { // check if it is a do while
			v = stack->pop();
			in_stack[v] = false;
			scc.push_front(v);
			std::cout << "U:"<< u << "V: "<< v << " ";
		} while (v != u);

		// printing the SCC


		std::cout << "SCC: de size "<< scc.size() << " ";
		for (int i=0; i<scc.size();i++){
			std::cout << scc.front() << "_";
			scc.pop_front();
		}
		std::cout << "\n";
	}
}
/*
5
6
1 2
2 3
3 1
3 4
4 5
5 4
1
-2
--3
---4
----5
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
	 g.SCC();

   //std::cout << "number of nodes: " << nodes << "\n";
   //std::cout << "number of edges: " << edges << "\n";

	 return 0;
}
