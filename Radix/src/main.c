#include <stdio.h>
#include <memory.h>
#include <omp.h>
#include "bfs.h"
#include "sort.h"
#include "edgelist.h" 
#include "vertex.h"
#include "timer.h"

void printMessageWithtime(const char * msg, double time){

    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", msg);
    printf(" -----------------------------------------------------\n");
    printf("| %-51f | \n", time);
    printf(" -----------------------------------------------------\n");

}


int main(void) {

    
    //const char * fname = "./datasets/test/test.txt";
    //const char * fname = "./datasets/wiki-vote/wiki-Vote.txt";
    const char * fname = "./datasets/facebook/facebook_combined.txt";
    
    int numOfVertices = 0;
    int numOfEdges = 0;
    struct Timer* timer1 = (struct Timer*) malloc(sizeof(struct Timer));
    struct Timer* timer2 = (struct Timer*) malloc(sizeof(struct Timer));
    struct Timer* timer3 = (struct Timer*) malloc(sizeof(struct Timer));
    // get |v| |e| count do we can allocate our edge array and vertex array
    loadEdgeArrayInfo(fname, &numOfVertices, &numOfEdges);
    printf("Edges : %d Vertices: %d\n", numOfEdges, numOfVertices);

    // allocate our edge array and vertex array
    struct Edge * edgeArray = newEdgeArray(numOfEdges);
    struct Edge * sortedEdgeArray = newEdgeArray(numOfEdges);
    struct Vertex * vertexArray =  newVertexArray(numOfVertices);

    // populate the edge array from file
    loadEdgeArray(fname, edgeArray);
    


    /*the function you need to optimize*/


    Start(timer1);
    countSortEdgesBySource(sortedEdgeArray, edgeArray, numOfVertices, numOfEdges);
    Stop(timer1);
    printf("Count Sort Finished!\n");

    struct Edge * edges_tmp1 = newEdgeArray(numOfEdges);
    for(int i=0;i<numOfEdges;i++)
    {
        edges_tmp1[i]=edgeArray[i];
    }

    Start(timer2);
    radixSortEdgesBySource_Serial(sortedEdgeArray, edges_tmp1, numOfVertices, numOfEdges);
    Stop(timer2);
    printf("Radix_Serial Sort Finished!\n");

    struct Edge * edges_tmp2 = newEdgeArray(numOfEdges);
    for(int i=0;i<numOfEdges;i++)
    {
        edges_tmp2[i]=edgeArray[i];
    }
      //  printEdgeArray(edges_tmp2, numOfEdges); 
    Start(timer3);
    radixSortEdgesBySource_OpenMP(sortedEdgeArray, edges_tmp2, numOfVertices, numOfEdges);
    Stop(timer3);
    printf("Radix_OpenMP Sort Finished! N_threads=%d\n",N_threads);
    // printMessageWithtime("Time Count Sorting (Seconds)",Seconds(timer1));
    /*the function you need to optimize*/
    printMessageWithtime("Time Count Sorting (Seconds)",Seconds(timer1));
    printMessageWithtime("Time Radix_Serial Sorting (Seconds)",Seconds(timer2));
    printMessageWithtime("Time Radix_OpenMP Sorting (Seconds)",Seconds(timer3));
    
    // if you want to check
    //printEdgeArray(edgeArray, numOfEdges); // print the edge list unsorted 
    //printEdgeArray(sortedEdgeArray, numOfEdges); 

    mapVertices(vertexArray, sortedEdgeArray, numOfVertices, numOfEdges);

    Start(timer1);
    bfs(6, vertexArray, sortedEdgeArray, numOfVertices, numOfEdges);
    Stop(timer1);
    printMessageWithtime("Time BFS (Seconds)",Seconds(timer1));

    free(sortedEdgeArray);
    free(vertexArray);
    free(edges_tmp1);
     free(edges_tmp2);
    free(edgeArray);
    
    return 0;
}


