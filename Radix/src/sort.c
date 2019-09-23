#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <omp.h>
#include <sort.h>
#include "sort.h"
#include "edgelist.h"

void countSortEdgesBySource(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges) {

    int i;
    int key;
    int pos;

    // auxiliary arrays, allocated at the start up of the program
    int *vertex_cnt = (int*)malloc(numVertices*sizeof(int)); // needed for bucketing Sort

    for(i = 0; i < numVertices; ++i) {
        vertex_cnt[i] = 0;
    }

    // bucket occurrence of key: id of a source vertex
    for(i = 0; i < numEdges; ++i) {
        key = edges[i].src;
        vertex_cnt[key]++;
    }

    // transform to cumulative sum
    for(i = 1; i < numVertices; ++i) {
        vertex_cnt[i] += vertex_cnt[i - 1];
    }

    // fill-in the sorted array of edges
    for(i = numEdges - 1; i >= 0; --i) {
        key = edges[i].src;
        pos = vertex_cnt[key] - 1;
        edges_sorted[pos] = edges[i];
        vertex_cnt[key]--;
    }


    free(vertex_cnt);

}

void radixSortEdgesBySource_Serial(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges) {
    int bit_length=0,num,i,k,p;
    while(numVertices>=10)
    {
        bit_length+=1;
        numVertices/=10;
    }
   // int bucket[256];
    for(i=0;i<bit_length;i++)
    {
        int bucket[256]={0};
        for(k=0;k<numEdges;k++)
        {
            num=(edges[k].src>>(8*i))&0xFF;
            bucket[num]++;
        }
        for(k=1;k<256;k++)
        {
            bucket[k]+=bucket[k-1];
        }
        for(k=numEdges-1;k>=0;k--)
        {
            num=(edges[k].src>>(8*i))&0xFF;
            p=bucket[num]-1;
            edges_sorted[p]=edges[k];
            bucket[num]--;
        }
        for(k=0;k<numEdges;k++)
        {
            edges[k]=edges_sorted[k];
        }
       // printEdgeArray(edges,numEdges);
    }

}


void radixSortEdgesBySource_OpenMP(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges) {
    int bit_length=0,num,i,k,p;
    //int *bucket=(int*)malloc(10*sizeof(int));;
    int bucket[256];
    while(numVertices>=10)
    {
        bit_length++;
        numVertices/=10;
    }

    //Define number of threads  
    omp_set_num_threads(N_threads);
    for(i=0;i<bit_length;i++)
    {
        #pragma omp parallel default(none) private(num,p) shared(i,numEdges,edges,edges_sorted,bucket)
        {
            int k;
        for(k=0;k<256;k++)
        {
            bucket[k]=0;
        }

        //#pragma omp parallel for private(num) reduction(+:bucket[:256]) schedule(static)
        for(k=0;k<numEdges;k++)
         {
            //printf("bit=%d\n",i );
            //printf("k=%d\n",k );
            
            num=(edges[k].src>>(8*i))&0xFF;
            bucket[num]++;
        }

        //#pragma omp parallel for 
        for(k=1;k<256;k++)
        {
            bucket[k]+=bucket[k-1];
        }

       //#pragma omp parallel for shared(bucket)
       // #pragma omp parallel for shared (bucket) schedule(static)
        #pragma omp barrier
            
            
        #pragma omp single
        for(k=numEdges-1;k>=0;k--)
        {
            num=(edges[k].src>>(8*i))&0xFF;
            p=bucket[num]-1;
            edges_sorted[p]=edges[k];       
            bucket[num]--;   
        }

        //#pragma omp parallel for 
        for(k=0;k<numEdges;k++)
        {
            edges[k]=edges_sorted[k];
           
        }   
        }
    }
       // printEdgeArray(edges,numEdges);
}

