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
		_low = INFINITY;
		_in_stack = false; // tracks whether or not th Node is in the stack
	}
	bool inStack() { return _in_stack; }
	void setStack(bool in_stack) { _in_stack = in_stack;	}
	int getID() {return _id;}

	double get_d() {return _d;}
	double set_d(double val) { _d = val; }
	double get_low() {return _low;}
	double set_low(double val) { _low = val; }
};

class CustomStack : public std::stack<int> {
private:
	Node *nodes_array; //Array de nodes
public:
	CustomStack(Node *nod_array) { // /!\ don't know if it should be created internally or externaly
		nodes_array = nod_array;
	}

	void push(const int id) {
		std::stack<int>::push(id);
		nodes_array[id].setStack(true);
	}

	int pop() {
		int id = std::stack<int>::top();
		std::stack<int>::pop();
		nodes_array[id].setStack(false);
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

	void /*std::list<int>*/ tarjanSCC() {
		int visited = 0;
		CustomStack *stack = new CustomStack(nodes_array);
		//result = new std::list<int>;

		for (int id=1; id < _nodes_num+1; id++) {
			if (nodes_array[id].get_d() == INFINITY)
				tarjan_visit(nodes_array[id], &visited, stack);
		}
		//return result
	}

	void tarjan_visit(Node node, int *visited, CustomStack *stack) {
		int id = node.getID();
		node.set_d(*visited);
		node.set_low(*visited);
		(*visited)++;

		stack->push(node.getID());

		Node *v_node; //Pointer to a node
 		std::list<int>::iterator v;
		std::list<int> neighbours = adj_list[id]; // List of edges (neighbours of the node id)
		for (v = neighbours.begin(); v != neighbours.end(); v++) { //For every neighbour
			std::cout << "visiting " << *v << "\n"; // debugging
			*v_node = nodes_array[*v];				//Why do you we need *v_node = nodes_array[*v]; and not
													// v_node = nodes_array[v]; 				
			if ((*v_node).get_d() == INFINITY || (*v_node).inStack()) {
				if ((*v_node).get_d() == INFINITY)
					tarjan_visit((*v_node), visited, stack);
				(*v_node).set_low(min((*v_node).get_low(), node.get_low()));
			}
		}
		if (node.get_low() == node.get_d()) {
			while (&node != v_node) {
				(*v_node) = stack->popNode();
			}
		}
	}


};


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
	 g.listNodes();
	 g.tarjanSCC();

   std::cout << "number of nodes: " << nodes << "\n";
   std::cout << "number of edges: " << edges << "\n";

	 return 0;
}
