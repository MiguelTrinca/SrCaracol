#include <vector>
#include <iostream>
#include <string>
#include <list>

class Node {
private:
	int _id, _d, _low;
public:
	Node(int id, int d, int low){
		_id = id;
		_d = d;
		_low = low;
		_in_stack = false; // tracks whether or not th Node is in the stack
	}
};


class Graph {
private:
	int _nodes_num;
    int _edges_num;
	std::list<int> *adj_list;
	Node *nodes; // ver se array é constant time
public:
	Graph(int nodes_num, int edges_num) {
		_nodes_num = nodes_num;
		_edges_num = edges_num;

		adj_list = new std::list<int>[nodes_num+1]; // zero is not used in this implementation

		// create the vertex list
		nodes = new Node[nodes_num+1]; // node start at 1
	}

	void addEdge(int beg, int end) {		
		adj_list[beg].push_front(end);
		_edges_num++;

		if (_edges_num == ed)
	}

	Node getNode(int num) { // gets the vertex corresponding to its id
		return nodes[num];
	}

private:
	void createNodes() {
		
	}


};


int main(int argc, char** argv) {

   int nodes;
   int edges;

   scanf("%d", &nodes);
   scanf("%d", &edges);
   
   Graph g(nodes);

   int beg, end;
   for (int i=0; i < edges; i++) {
   	scanf("%d %d", &beg, &end);
   	g.addEdge(beg, end);
   }

   std::cout << "number of nodes: " << nodes << "\n"; 
   std::cout << "number of edges: " << edges << "\n"; 

   /*
   Graph g(people);
 
    int p1;
    int p2;
    for (int i = 0; i < connections; i++) {
        std::cin >> p1 >> p2;
        
        g.addEdge(p1-1, p2-1);
  
    }
    
    g.findCritical();
    */
    return 0;
}


