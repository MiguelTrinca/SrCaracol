#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <stdio.h>

class Node {
private:
	int _id = 0;
	int _d, _low;
public:
	Node(int id, int d, int low){
		_id = id;
		_d = d;
		_low = low;
	//	_in_stack = false; // tracks whether or not th Node is in the stack
	}
	
	int	getID(){
		return _id;
	}

	void setID(int id){
		_id = id;
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
		nodes_array[_nodes_num + 1];
	}

	void addEdge(int beg, int end) {		
		adj_list[beg].push_front(end);  //Criar a edge.
		}
	
	void createNode(int id){
		if(nodes_array[id].getID() == 0){ //Quero construir quando tenho NULL
			Node n(id, 0 , 0); //Crio o node
			nodes_array[id] = n;          // Meto no array
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

   std::cout << "number of nodes: " << nodes << "\n"; 
   std::cout << "number of edges: " << edges << "\n"; 

    return 0;
}


