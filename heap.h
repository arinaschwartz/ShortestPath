/* Andrew Christoforakis and Arin Schwartz
 *
 * Header file for heap and graph structures
 */

#ifndef  HEAP
#define HEAP

typedef struct edge_s* Edge;
typedef struct node_s* Node;
typedef struct heap_s* Heap;

struct edge_s{
	Node start;
	Node finish;
	double cost;
	Edge next;
};

struct node_s{
	int ID;
	Node parent;
	int location;
	Edge head;
	double totalCost;
};

struct heap_s{
	int size;
	int unsolved;
	Node* array;
};

/* make, print, free nodes */
Edge mkEdge(Node start, Node finish, double cost);
void printEdge(Edge e);
void freeEdge(Edge e);

/* make, print, free vertices */
Node mkNode(int ID, int location);
void printNode(Node n);
void freeNode(Node n);

/* make, print, free heap */
Heap mkHeap(int size);
void printHeap(Heap h);
void freeHeap(Heap h);

/* add edge to node's linked list of edges */
void addEdge(Node n, Edge e);

/* prints all of nodes edges for testing purposes */
void printEdges(Node n);

/* initialize Node and add it to heap */
void addNode(double array[], int ID, Heap h);

/* establishes pointers for edges' end nodes */
void assignEndPointers(Heap h);

/* swap nodes in heap */
void swap(Heap h, Node n1, Node n2);

/* returns Node's 'parent' in heap */
Node parent(Heap h, Node n);

/* returns smallest of Node's two children in heap */
Node minChild(Heap h, Node n);

/* returns whether or not Node has already been solved */
bool isSolved(Heap h, Node n);

/* returns whether or not node is valid return values of minChild */
bool valid(Heap h, Node n);

/* moves node up in heap until it's greater than its parent */
void siftUp(Heap h, Node n);

/* decreases total cost of node and moves it up heap as necessary */
void decreaseKeyTo(Heap h, Node n, double newCost);

/* moves node down in heap until it's greater than its children */
void siftDown(Heap h, Node n);

/* removes node and decrements heap's unsolved count */
Node removeMin(Heap h);

/* checks if new path is better than current best path, updates accordingly */
void relax(Heap h, Node n, Edge e);

/* iterates over node's edges and updates costs */
void relaxEdges(Heap h, Node n);

/* returns node with given ID number, will be called only once at end */
Node nodeByID(Heap h, int ID);

/* prints optimal path given end node */
void printBestPath(Node n);

#endif
