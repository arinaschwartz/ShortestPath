#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "heap.c"

/* Assignment 3 by Andrew Christoforakis and Arin Schwartz
 *
 * We tried to compile this as "gcc Dijkstra.c heap.c -o Dijkstra"
 * but we counldn't get it to work that way.
 *
 */

#define MAX_CHAR 10000 /* arbitrary maximum line length */

/* function prototypes */
void printArray(double* array, int size);
void setValues(double* array, char* line, int size);
void readFile(FILE* file, Heap h);

int main(int argc, char* argv[]){

        if (argc != 4){
                printf("usage: %s <filename> <start point> <end point>\n",argv[0]);
                exit(1);
        }

        FILE* file = fopen(argv[1], "r");
        if (file == NULL){
                printf("Can't open input file.\n");
                exit(1);
        }

        int i; /* general indexing variable */
        int size, start = atoi(argv[2]), end = atoi(argv[3]);

	/* initialize heap */
        fscanf(file,"%d\n",&size);
	Heap h = mkHeap(size);
	readFile(file,h);
	decreaseKeyTo(h,h->array[start],0);

	while (h->unsolved > 0){
		Node n = removeMin(h);
		if (n->ID == end)
			break;
		relaxEdges(h,n);
	}
	
	Node endPoint = nodeByID(h,end);
	printf("Weight: %.3f\n",endPoint->totalCost);
	printf("Path: ");
	printBestPath(endPoint);
	printf("\n");

	freeHeap(h);
        return 0;
}

/* prints array of doubles for testing purposes */
void printArray(double* array, int size){
        int i;
        for (i=0; i < size; i++)
                printf("%f ",array[i]);
        printf("\n");
}                             

/* transforms string into array of doubles */
void setValues(double* array, char* line, int size){
        int i;
        double value;
        for (i=0; i<size; i++){
                char* endp;
                array[i] = strtod(line,&endp);
                line = endp;
        }
}

/* reads file and creates nodes in heap */
void readFile(FILE* file, Heap h){
	int i;
	char line[MAX_CHAR];
	double values[h->size];

	/* initialize arrays to known default value */
	for (i=0; i < h->size; i++){
		values[i] = 0.0;
		line[i] = '\0';
	}

	for (i=0; i < h->size; i++){
		fgets(line,MAX_CHAR,file);
		setValues(values,line,h->size);
		addNode(values,i,h);
	}
	fclose(file);
} 
