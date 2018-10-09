
// Dijkstra ADT interface for Ass2 (COMP2521)
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

//a very very very big number
#define INF 0x7FFFFFFF

//helper functions for inserting into pred
void set_pred(ShortestPaths *,Vertex, Vertex, Vertex);
void set_equal_pred(ShortestPaths *,Vertex, Vertex, Vertex);


/**
  * Implementation of dijkstra's algorithm. It is used to determine
  * the shortest distance between a start node and any other node in a
  * graph. It continiously calculates the shortest path from a starting point
  * and to exclude longer distance when making an update.
  *
  */
ShortestPaths dijkstra(Graph g, Vertex source) {

    //make sure the graph is valid
    assert (g != NULL);

    //make a ShortestPath and intialise everything within
    //the struct src = Vertex source and noNodes in shortest path
    //would be equal to number of vertex in a graph
    ShortestPaths *sp = malloc(sizeof(ShortestPaths));

    sp->src = source;
    sp->noNodes = numVerticies(g);

    //initialise the distance array and make sure it isn't NULL
    //dist array has a size of g->nV
    sp->dist = malloc(numVerticies(g) *sizeof(int));
    assert(sp->dist != NULL);

    //initialise the predecessor array and make sure it isn't NULL
    //pred list has a size of g->nV
	sp->pred = malloc(numVerticies(g) * sizeof(PredNode *));
	assert(sp->pred != NULL);

    //this seen array to keep track of vertex seen when
    //finding the shortest path
    int seen[numVerticies(g)];

    //make a new priority queue 
    PQ vertexSet = newPQ();

    //initialisation for dist array, pred list and seen array
    //for every vertex in the graph
    for (int i = 0; i < numVerticies(g); i++) {
        //initial distance from source to vertex i is infinity
        sp->dist[i] = INF;
        //predecessor node for all is undefined
        sp->pred[i] = NULL;
        //set every vertex to not seen
        seen[i] = -1;
    }

    //distance from source to source in 0
    sp->dist[source] = 0;

    //Make an item, initialise it and add it to the priority queue
    ItemPQ new;
    //key of the item is the vertex source
    new.key = source;
    //value of item is the distance
    new.value = sp->dist[source];

    addPQ(vertexSet, new);

    //main loop where Dijsktra's magic begin
    while(PQEmpty(vertexSet) != 1){

    	//remove the item with the smallest distance
        ItemPQ curr = dequeuePQ(vertexSet);

        //if the vertex has been seen then jump back up to the top of
        //the loop
        if(seen[curr.key] != -1) continue;

        //if vertex hasn't been seen set it to seen
        seen[curr.key] = 1;

        //get the list of edges that current vertex can reach
        AdjList edge = outIncident(g, curr.key);

        //Check all neighbours of vertex that was dequeued
        //this step is similar to BFS
        while(edge != NULL){

            //relax curr and current edge we are on
            //also need to make sure it is less than equal not just less
            //than because of centrality
            if(sp->dist[curr.key] + edge->weight <= sp->dist[edge->w]){
                //if current edge is less than curr
                if(sp->dist[curr.key] + edge->weight < sp->dist[edge->w]){
                    //update the distance array
                    sp->dist[edge->w] = sp->dist[curr.key] + edge->weight;
                    //update the predecessor list
                    set_pred(sp, edge->w, curr.key, source);
                //the case where we find another path that is equal to
                } else if(sp->dist[curr.key] + edge->weight == sp->dist[edge->w]){
                    //we also update the distance array
                    sp->dist[edge->w] = sp->dist[curr.key] + edge->weight;
                    //and update the predcessor list
                    set_equal_pred(sp, edge->w, curr.key, source);
                }
                //add the edge we are on to the queue
                new.key = edge->w;
                new.value = edge->weight;
                addPQ(vertexSet, new);
            }
            
            edge = edge->next;
        }
    }

    //if the path is stil infintiy then set it to zero
    //as path's weight has to be > 0
    for(int i = 0; i < numVerticies(g); i++){
        if(sp->dist[i] == INF){
            sp->dist[i] = 0;
        }
    }

    return *sp;

}

//sets pred node
void set_pred(ShortestPaths *path,Vertex w, Vertex v, Vertex src){

    //make a new predecessor node and initialise it
    path->pred[w] = malloc(sizeof(PredNode));
    path->pred[w]->v = v;
    path->pred[w]->next = NULL;
}

//function to see if a string of paths is better than another path which may be direct
void set_equal_pred(ShortestPaths *path,Vertex w, Vertex v, Vertex src){

    //make a new node and initialise it 
    PredNode * new = malloc(sizeof(PredNode));
    new->v = v;
    new->next = NULL;
    //keep track of the first thing in the list
    PredNode * curr = path->pred[w];
    //find appropraite location to insert the predecessor list
    while(path->pred[w]->next!=NULL){
        path->pred[w] = path->pred[w]->next;
    }
    path->pred[w]->next = new;
    path->pred[w] = curr;
}

/**
  * show the shortestpath 
  *
  */
void showShortestPaths(ShortestPaths sp) {

    int i = sp.src;
    //print the distance array
    while(i < sp.noNodes){
        printf("%d ", sp.dist[i]);
        i++;
    }
    printf("\n");

    //print the predecessor list
    PredNode *curr = sp.pred[sp.src];
    while(curr != NULL){
        printf("%d ", curr->v);
        curr = curr->next;
    }
    printf("\n");

}

/**
  * free every that was malloced
  *
  */
void  freeShortestPaths(ShortestPaths sp) {

    free(sp.pred);
    free(sp.dist);
}
