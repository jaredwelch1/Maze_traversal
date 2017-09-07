/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   createGraph.h
 * Author: Jared
 *
 * Created on December 1, 2015, 2:33 PM
 */

#ifndef CREATEGRAPH_H
#define CREATEGRAPH_H

#ifdef __cplusplus
extern "C" {
#endif
    
        typedef struct index{
            int x1;
            int y1;
            int x2;
            int y2;
        }index;

        typedef enum color{
            WHITE = 0,
            GREY, 
            BLACK,
        }color;

        typedef struct vertex{
            int allowed;
            index previous;
            index ownIndex;
            int visited;
        }vertex;
        
        typedef struct stack{
            vertex **stack;
            int size;
        }stack;
    
        int** parseInputFile(char *file, int *startx1Ptr, int *starty1Prt, int *startx2Ptr, int *starty2Ptr, int *endx1Ptr,int *endy1Ptr, int *endyx2Ptr, int *endy2Ptr, int *xmax, int *ymax );
        int isAllowed(int**, index, int);
        void DFS_Graph(vertex**** graph, int startx1, int starty1, int startx2, int starty2, index end, int, int);
        void push(stack *s, index in, vertex**** graph);
        index* pop(stack *s);
        int DFS_Iterative(vertex**** graph, index start, index end, int xmax, int ymax);
        index* get_path(vertex**** graph, index end, index start, int*);
        void edit_floorplan(int **floorplan, index *path, int, index, index,int, int);

#ifdef __cplusplus
}
#endif

#endif /* CREATEGRAPH_H */

