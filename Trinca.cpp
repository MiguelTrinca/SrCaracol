#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <stdio.h>
#include <stack>



class Node {
private:
	int _id = 0;
	int _d, _low;
	bool _in_stack;
public:
	Node(int id, int d, int low){
		_id = id;
		_d = d;
		_low = low;
		_in_stack = false; // tracks whether or not th Node is in the stack
	}
	bool inStack() { return _in_stack; }
	void setStack(bool in_stack) { _in_stack = in_stack;	}
	int getID() {return _id;}
};

class MyStack : public std::stack<int> {
private:
	Node *nodes_array; //Array de nodes
public:
	MyStack(Node *nod_array) { // /!\ don't know if it should be created internally or externaly
		nodes_array = nod_array;
	}

	void push(const int id) {
		std::stack<int>::push(id);
		nodes_array[id].setStack(true);
	}

	void pop(const int id) {
		std::stack<int>::pop(id);
		nodes_array[id].setStack(false);
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

		Node *empty_node = new Node(0,0,0);

		adj_list = new std::list<int>[_nodes_num+1]; //Criar o array com dimensao nodes + 1
		nodes_array[_nodes_num + 1];				//Criar o array com dimensao nodes + 1

		/*Criacao de todos os nodes quando e chamado o construtor do grafo
		/		estamos a supor que os id dos nodes sao sequenciais ou seja
		/		Se tivermos 3 nodes, sao 1,2 e 3.
		*/
		int id;
		for(id = 0; id<_nodes_num+1; id++)
			createNode(id);
	}

	void addEdge(int beg, int end) {
		adj_list[beg].push_front(end);  //Criar a edge.
	}

	/*Criacao de um node, caso o nosso postulado esteja errado*/
	void createNode(int id){
		//if(nodes_array[id].getID() == 0){ //Quero construir quando tenho NULL
			Node *n = new Node(id, 0 , 0); //Crio o node
			nodes_array[id] = *n;          // Meto no array
		//}
	}

	void listNodes() {
		std::cout << "listing nodes" << "\n";
		for(int id = 0; id<_nodes_num+1; id++)
			std::cout << nodes_array[id].getID() << "\n";
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

   std::cout << "number of nodes: " << nodes << "\n";
   std::cout << "number of edges: " << edges << "\n";

    return 0;
}
