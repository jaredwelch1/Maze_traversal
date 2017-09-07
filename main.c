/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Jared
 *
 * Created on November 17, 2015, 10:49 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "statistics_errors.h"
#include "createGraph.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    if(argc != 3){
        exit(INCORRECT_COMMAND_LINE_ARGUMENTS);
    }
    
    int startx1, starty1, startx2, starty2, endx1, endy1, endx2, endy2;
    int *xSize = malloc(sizeof(int));
    int *ySize = malloc(sizeof(int));
    
    int **floorplan = parseInputFile(argv[1], &startx1, &starty1, &startx2, &starty2, &endx1, &endy1, &endx2, &endy2, xSize, ySize );    
    
    
    vertex ****graph = malloc(*xSize * sizeof(vertex***));      //logic to make 4D array 
    
    int i, j, k, count = 0;
    
    for(i = 0; i < *xSize; i++){
        graph[i] = malloc(*ySize * sizeof(vertex**));
    
        for(j = 0; j < *ySize; j++){
            graph[i][j] = malloc(*xSize * sizeof(vertex*));
            
            for(k = 0; k < *xSize; k++){
                graph[i][j][k] = malloc(*ySize * sizeof(vertex));
            }
        }
    }
    
    
    int r = atoi(argv[2]);
    int a, b, c, d;
    
    
    
    for(a = 0; a < *xSize; a++){
        for(b = 0; b < *ySize; b++){
            for(c = 0; c < *xSize; c++){
                for(d = 0; d < *ySize; d++){
                    graph[a][b][c][d].visited = 0;
                    index temp;
                    temp.x1 = a;
                    temp.y1 = b;
                    temp.x2 = c;
                    temp.y2 = d;
                    graph[a][b][c][d].allowed = isAllowed(floorplan,temp ,r);
                }    
            }
        }
    }
    
    index start;
    start.x1 = startx1;
    start.y1 = starty1;
    start.x2 = startx2;
    start.y2 = starty2;
    
    index end;
    end.x1 = endx1;
    end.y1 = endy1;
    end.x2 = endx2;
    end.y2 = endy2;
    
    int xmax = *xSize - 1;
    int ymax = *ySize - 1;
    
    //DFS_Graph(graph, start.x1, start.y1, start.x2, start.y2, end, xmax, ymax);
    int startDistance = 0;
    startDistance+= abs(start.x1 - start.x2);
    startDistance+= abs(start.y1 - start.y2);
    int found;
    if(startDistance >= r){
    
        found = DFS_Iterative(graph, start, end, xmax, ymax);
    } else {
        printf("\nThe robots started closer than the allowed distance.");
    }
    
    if(found == 1){
        
        index* path = NULL;
        int *path_size = malloc(sizeof(int));
        path = get_path(graph, end, start, path_size);
        printf("\nPath size: %d", *path_size);
        edit_floorplan(floorplan, path, *path_size, start,end, *xSize, *ySize);
        
        
    }

    for(i = 0; i < *xSize; i++){
        for(j = 0; j < *ySize; j++){
            for(k = 0; k < *xSize; k++){
                free(graph[i][j][k]);
            }
        }
    }
    
    for(i = 0; i < *xSize; i++){
        for(j = 0; i < *ySize; j++){
            free(graph[i][j]);
        }
    }
    
    for(i = 0; i < *xSize; i++){
        free(graph[i]);
    }
    
    free(graph);
    
    
    
    for(i = 0; i < *xSize; i++){
     free(floorplan[i]);   
    }
    free(floorplan);
    
    
    return (EXIT_SUCCESS);
}

