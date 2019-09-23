#ifndef SORT_H
#define SORT_H
#define N_threads 4 //Define number of threads
#include "edgelist.h"
// Order edges by id of a source vertex, 
// using the Counting Sort
// Complexity: O(E + V)
void countSortEdgesBySource(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges);

//Radix Sort defined here
//Both Serial and OpenMP methods are implemented in "sort.c"
void radixSortEdgesBySource_Serial(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges);
void radixSortEdgesBySource_OpenMP(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges);
#endif