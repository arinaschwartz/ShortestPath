#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<assert.h>
#include "heap.h"

Edge mkEdge(Node start, Node finish, double cost){
	Edge new_edge = malloc(sizeof(struct edge_s));
	assert(new_edge != NULL);
	new_edge->start = start;
	new_edge->finish = finish;
	new_edge->cost = cost;
	new_edge->next = NULL;
	return new_edge;
}

void printEdge(Edge e){
	printf("Edge goes from %d to %d with cost %f\n",e->start->ID,e->finish->ID,e->cost);
}

void freeEdge(Edge e){
	free(e);
}

Node mkNode(int ID, int location){
	Node new_node = malloc(sizeof(struct node_s));
	assert(new_node != NULL);
	new_node->ID = ID;
	new_node->location = location;
	new_node->parent = NULL;
	new_node->head = NULL;
	new_node->totalCost = DBL_MAX;
	return new_node;
}

void printNode(Node n){
	printf("Node number %d is located in spot %d in heap.\n",n->ID,n->location);
	if (n->totalCost != DBL_MAX)
		printf("Total cost is %.3f.\n",n->totalCost);
	else
		printf("Total cost is DBL_MAX.\n");
}

void freeNode(Node n){
	while (n->head != NULL){
		Edge next = n->head->next;
		free(n->head);
		n->head = next;
	}
	free(n);
}

Heap mkHeap(int size){
	Heap new_heap = malloc(sizeof(struct heap_s));
	assert(new_heap != NULL);
	new_heap->size = size;
	new_heap->unsolved = size;
	new_heap->array = malloc(size*sizeof(Node));
	assert(new_heap->array != NULL);
	int i;
	for (i=0; i < size; i++)
		new_heap->array[i] = mkNode(i,i);
	return new_heap;
}

void printHeap(Heap h){
	int i;
	for (i=0; i < h->size; i++){
		printNode(h->array[i]);
		printf("\n");
	}
}

void freeHeap(Heap h){
	int i;
	for (i=0 ; i < h->size; i++)
		freeNode(h->array[i]);
	free(h->array);
	free(h);
}

void addEdge(Node n, Edge e){
	e->next = n->head;
	n->head = e;
}

void printEdges(Node n){
	Edge thisEdge = n->head;
	while (thisEdge != NULL) {
		printEdge(thisEdge);
		thisEdge = thisEdge->next;
	}
}

void addNode(double array[], int ID, Heap h){
	int i;
	Node thisNode = h->array[ID];
	for (i=0; i < h->size; i++){
		if (array[i] > 0.0){
			Edge e = mkEdge(thisNode, h->array[i], array[i]);
			addEdge(thisNode,e);
		}
	}
}

void swap(Heap h, Node n1, Node n2){
	h->array[n1->location] = n2;
	h->array[n2->location] = n1;

	int temp = n1->location;
	n1->location = n2->location;
	n2->location = temp;
}

Node parent(Heap h, Node n){
	if (n->location == 0)
		return NULL;

	int index = (n->location-1)/2;
	return h->array[index];
}

Node minChild(Heap h, Node n){
	int thisLocation = n->location;
	Node child1 = h->array[2*thisLocation+1];
	Node child2 = h->array[2*thisLocation+2];
	if (2*thisLocation + 1 >= h->size)
		return NULL;
	else if (!valid(h,child1) && !valid(h,child2))
		return NULL;
	else if (valid(h,child1) && !valid(h,child2))
		return child1;
	else if (valid(h,child2) && !valid(h,child1))
		return child2;
	else if (child1->totalCost < child2->totalCost)
		return child1;
	else
		return child2;
}

bool isSolved(Heap h, Node n){
	return (n->location >= h->unsolved);
}

bool valid(Heap h, Node n){
	return (n != NULL) && (!isSolved(h,n));
}

void siftUp(Heap h, Node n){
	while ((n->location > 0) && (n->totalCost < parent(h,n)->totalCost))
		swap(h,n,parent(h,n));
}

void decreaseKeyTo(Heap h, Node n, double newCost){
	n->totalCost = newCost;
	siftUp(h,n);
}

void siftDown(Heap h, Node n){
	Node min = minChild(h,n);
	while ((n->location < h->unsolved-1) && (min != NULL) && (n->totalCost > min->totalCost)){
		swap(h,n,min);
		min = minChild(h,n);
	}
}

/* Returns node with lowest cost and rearranges heap accordingly */
Node removeMin(Heap h){
	swap(h,h->array[0],h->array[h->unsolved-1]);
	h->unsolved--;
	//printf("Before sift down:\n");
	//printHeap(h);
	siftDown(h,h->array[0]);
	//printf("After sift down:\n");
	//printHeap(h);
	return h->array[h->unsolved];
}

/* updates node's total cost if appropriate */
void relax(Heap h, Node n, Edge e){
	double newDistance = n->totalCost + e->cost;
	if (newDistance < e->finish->totalCost){
		decreaseKeyTo(h,e->finish,newDistance);
		e->finish->parent = n;
	}
}

/* iterates over and relaxs all of node's edges */
void relaxEdges(Heap h, Node n){
	Edge e = n->head;
	while (e != NULL){
		relax(h,n,e);
		e = e->next;
	}
}

Node nodeByID(Heap h, int ID){
	int i;
	for (i=0; i < h->size; i++){
		if (h->array[i]->ID == ID)
			return h->array[i];
	}
	printf("ERROR: NO NODE WITH THAT ID.\n");
	return NULL;
}

void printBestPath(Node n){
	if (n->parent == NULL)
		printf("%d ",n->ID);
	else{
		printBestPath(n->parent);
		printf("%d ",n->ID);
	}
}
