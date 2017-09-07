#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"createGraph.h"
#include"statistics_errors.h"
#include<unistd.h>



int** parseInputFile(char *file, int *startx1Ptr, int *starty1Ptr, int *startx2Ptr, int *starty2Ptr, int *endx1Ptr,int *endy1Ptr, int *endx2Ptr, int *endy2Ptr, int *xSize, int *ySize ){
    
    FILE *fp = fopen(file, "r");
    
    if (fp == NULL){
        exit(INPUT_FILE_FAILED_TO_OPEN);
    }
    
    char c;
    int x = -1;
    int xmax = 0;
    int y = 0; 
    int startx1 = -1, starty1 = -1, startx2 = -1, starty2 = -1, endx1 = -1, endy1 = -1, endx2 = -1, endy2 = -1;
    
    
    
    while(fscanf(fp, "%c", &c) != EOF){
        x++;
        if(x > xmax){
            xmax = x;
        }
        
        if(c == '\n'){
            x = 0;
            y++;
        }
    }               //Initial scan to find size of input file layout
        
    *xSize = (xmax+1); //size of x array size of y array
    *ySize = (y+1);
    
    int **floorplan = malloc( sizeof(int*) * (xmax + 1) ); 
    
    int i = 0;
    
    for(i = 0; i <= xmax; i++){
        floorplan[i] = malloc(sizeof(int*) * (y+1));
    } //malloc space for floorplan
    
    x = -1;
    y = 0;
    rewind(fp);
    
    while(fscanf(fp, "%c", &c) != EOF){ //now parse input maze into an array representing it
        x++;
        
        if(c == '\n'){
            x = -1;
            y++;
        }
        
        if(c == '#'){
            floorplan[x][y] = 0;
        
        } else if (c == ' '){
            floorplan[x][y] = 1;
        
        } else if (c == 'S'){
            floorplan[x][y] = 1;
            startx1 = x;
            *startx1Ptr = startx1;
            starty1 = y;
            *starty1Ptr = starty1;
            printf("\nStart robot 1 (%d, %d)", x, y);  //Store robot locations start/finish
        
        } else if (c == 'F'){
            floorplan[x][y] = 1;
            startx2 = x;
            *startx2Ptr = startx2;
            starty2 = y;
            *starty2Ptr = starty2;
            printf("\nStart robot 2 (%d, %d)", x, y);
        
        } else if (c == 'E'){
            floorplan[x][y] = 1;
            endx1 = x;
            *endx1Ptr = endx1;
            endy1 = y;
            *endy1Ptr = endy1;
            printf("\nEnd Robot 1 (%d, %d)", x, y);
        } else if (c == 'L'){
            floorplan[x][y] = 1;
            endx2 = x;
            *endx2Ptr = endx2;
            endy2 = y;
            *endy2Ptr = endy2;
            printf("\nEnd Robot 2 (%d, %d",x, y);
        }
        
        
    }
    
    if ( fclose(fp) != 0){  
        exit(OUTPUT_FILE_FAILED_TO_CLOSE);
    }
    
    if (startx1 == -1||starty1 == -1||startx2 == -1||starty2 == -1||endx1 == -1||endy1 == -1||endx2 == -1||endy2 == -1){
        exit(DID_NOT_FIND_ROBOTS_OR_ENDSPACES);
    }
    
    return floorplan;
    
}

int isAllowed(int** floorplan, index vertex ,int distance){
    
    int i,j,k,l;
    
    i = vertex.x1;
    j = vertex.y1;
    k = vertex.x2;
    l = vertex.y2;
    
    //printf("\nFloorplan[%d][%d] = %d, flooplan[%d][%d] = %d", i, j, floorplan[i][j], k, l, floorplan[k][l]);
    
    int currDistance = 0;
                    
    if(floorplan[i][j] != 1){
        return 0;
    } else if (floorplan[k][l] != 1){  //logic to check if the sent index is allowed with current floorplan
        return 0;
    } else if(i == k && j == l){
        return 0;
    } else {    
    currDistance = 0;
    currDistance+=abs(i-k);
    currDistance+=abs(j-l);
    //printf("\nDistance calculated: %d, distance received:(r)%d", currDistance, distance);
                        
        if(currDistance <= distance && distance > 0){
            return 0;
        } else if (distance == 0 || currDistance > distance){
            
            return 1;
        
        }   
    }
}

void DFS_Graph(vertex**** graph, int startx1, int starty1, int startx2, int starty2, index end, int xmax, int ymax){
    
    
   //printf("\nDFS call for (%d, %d, %d, %d)", startx1, starty1, startx2, starty2);
    
    
       if(graph[startx1][starty1][startx2][starty2].visited == 0){
       
           graph[startx1][starty1][startx2][starty2].visited = 1;  //Normal DFS here. The adj vertices are moves up down left right
                                                                    //for each robot    
           
        if( startx1 == end.x1 && startx2 == end.x2 && starty1 == end.y1 && starty2 == end.y2){
            
            
            return;
        
        } else {
       
           
           if(starty1 - 1 >= 0){
               
                if( graph[startx1][starty1 - 1][startx2][starty2].allowed == 1){
                    
                    graph[startx1][starty1 - 1][startx2][starty2].previous.x1 = startx1;
                    graph[startx1][starty1 - 1][startx2][starty2].previous.y1 = starty1;
                    graph[startx1][starty1 - 1][startx2][starty2].previous.x2 = startx2;
                    graph[startx1][starty1 - 1][startx2][starty2].previous.y2 =  starty2;
                    DFS_Graph(graph, startx1, starty1 - 1, startx2, starty2, end, xmax, ymax);
           
                    } 
           }
           if(starty1 + 1 <= ymax){


               if( graph[startx1][starty1+1][startx2][starty2].allowed == 1 ){
                    
                   graph[startx1][starty1 + 1][startx2][starty2].previous.x1 = startx1;
                   graph[startx1][starty1 + 1][startx2][starty2].previous.y1 = starty1;
                   graph[startx1][starty1 + 1][startx2][starty2].previous.x2 = startx2;
                   graph[startx1][starty1 + 1][startx2][starty2].previous.y2 =  starty2;
                   
                   DFS_Graph(graph, startx1, starty1 + 1, startx2, starty1, end, xmax, ymax);
           
               }
           }
           if(startx1 - 1 >= 0){
                
               if(graph[startx1-1][starty1][startx2][starty2].allowed == 1){
                    
                   graph[startx1 - 1][starty1][startx2][starty2].previous.x1 = startx1;
                   graph[startx1 - 1][starty1][startx2][starty2].previous.y1 = starty1;
                   graph[startx1 - 1][starty1][startx2][starty2].previous.x2 = startx2;
                   graph[startx1 - 1][starty1][startx2][starty2].previous.y2 =  starty2;
                   
                   DFS_Graph(graph, startx1-1, starty1, startx2, starty2, end, xmax, ymax);
                }
              
           }   
           
           if(startx1 + 1 <= xmax){
               
                if(graph[startx1+1][starty1][startx2][starty2].allowed == 1){
                    
                    graph[startx1 + 1][starty1][startx2][starty2].previous.x1 = startx1 ;
                    graph[startx1 + 1][starty1][startx2][starty2].previous.y1 = starty1;
                    graph[startx1 + 1][starty1][startx2][starty2].previous.x2 = startx2;
                    graph[startx1 + 1][starty1][startx2][starty2].previous.y2 =  starty2;
                    
                    
                    DFS_Graph(graph, startx1 + 1, starty1, startx2, starty2, end, xmax, ymax);
                }
           
           }

           if(starty2 - 1 >= 0){
         
               if( graph[startx1][starty1][startx2][starty2 - 1].allowed == 1){
                    
                   graph[startx1][starty1][startx2][starty2 - 1].previous.x1 = startx1;
                   graph[startx1][starty1][startx2][starty2 - 1].previous.y1 = starty1;
                   graph[startx1][starty1][startx2][starty2 - 1].previous.x2 = startx2;
                   graph[startx1][starty1][startx2][starty2 - 1].previous.y2 =  starty2;
                   
                   DFS_Graph(graph,startx1, starty1, startx2, starty2 - 1, end, xmax, ymax);
           
               } 
           }
           if(starty2 + 1 <= ymax){
  
               if(graph[startx1][starty1][startx2][starty2 + 1].allowed == 1){
                   
                   graph[startx1][starty1][startx2][starty2 + 1].previous.x1 = startx1;
                   graph[startx1][starty1][startx2][starty2 + 1].previous.y1 = starty1;
                   graph[startx1][starty1][startx2][starty2 + 1].previous.x2 = startx2;
                   graph[startx1][starty1][startx2][starty2 + 1].previous.y2 =  starty2;
                   
                   DFS_Graph(graph, startx1, starty1, startx2, starty2 + 1, end, xmax, ymax);
                }
           
           
           }
           if(startx2 - 1 >= 0){

               if(graph[startx1][starty1][startx2-1][starty2].allowed == 1){
                    
                   graph[startx1][starty1][startx2 - 1][starty2].previous.x1 = startx1;
                   graph[startx1][starty1][startx2 - 1][starty2].previous.y1 = starty1;
                   graph[startx1][starty1][startx2 - 1][starty2].previous.x2 = startx2 ;
                   graph[startx1][starty1][startx2 - 1][starty2].previous.y2 =  starty2;
                   
                   DFS_Graph(graph, startx1, starty1, startx2 - 1, starty2, end, xmax, ymax);
                }
           }
           if(startx2 + 1 <= xmax){
                
               if(graph[startx1][starty1][startx2 + 1][starty2].allowed == 1){
                    
                   graph[startx1][starty1][startx2 + 1][starty2].previous.x1 = startx1;
                   graph[startx1][starty1][startx2 + 1][starty2].previous.y1 = starty1;
                   graph[startx1][starty1][startx2 + 1][starty2].previous.x2 = startx2;
                   graph[startx1][starty1][startx2 + 1][starty2].previous.y2 =  starty2;
                   
                   DFS_Graph(graph, startx1, starty1, startx2 + 1, starty2, end, xmax, ymax);
                }  
               
           }
        }
    }
}

int DFS_Iterative(vertex**** graph, index start, index end, int xmax, int ymax){
    
    stack *s = malloc(sizeof(stack));
    push(s, start, graph);
    index prev = start;
    index in;
    int end1 = 0, end2 = 0;
    //printf("\nXmax %d \nYmax %d", xmax, ymax);
    
    int i =0;
    while(s->size > 0){
        in = *pop(s);
        
        
        
       // printf("\n\tThis node is added to be searched(%d, %d, %d, %d) Visited: %d", in.x1, in.y1, in.x2, in.y2, graph[in.x1][in.y1][in.x2][in.y1].visited);
   
        
            if (graph[in.x1][in.y1][in.x2][in.y2].visited == 0){   //iterative DFS, checks if the vertex is the one 
                                                                    //that is wanted
                graph[in.x1][in.y1][in.x2][in.y2].visited = 1;
                
                if(in.x1 == end.x1 && in.y1 == end.y1 && in.x2 == end.x2 && in.y2 == end.y2){
            
                    printf("\nMatch found. (%d, %d, %d, %d)", in.x1, in.y1, in.x2, in.y2); //if we find end, return 1
                    graph[in.x1][in.y1][in.x2][in.y2].visited = 1;
                    
                    //free(s->stack);
                    //free(s);
                    return 1;
                } else if (in.x1 == end.x1 && in.x2 == end.x2){
                    end1 = 1;
                } else if(in.x2 == end.x2 && in.y2 == end.y2){
                    end2 = 1;
                }
                
                
            
                
                //logic to check adjacent edges for each vertex
                if(in.x2 - 1 >= 0){
                    if( graph[in.x1][in.y1][in.x2 - 1][in.y2].allowed == 1){
                        if(graph[in.x1][in.y1][in.x2 - 1][in.y2].visited == 0){
                        graph[in.x1][in.y1][in.x2 - 1][in.y2].previous = in;
                        }
                        in.x2--;
                        push(s, in, graph);
                        in.x2++;
                    }            
                }
                
                if(in.y2 + 1 <= ymax){
               
                    if( graph[in.x1][in.y1][in.x2][in.y2 + 1].allowed == 1){
                        
                        if(graph[in.x1][in.y1][in.x2][in.y2 + 1].visited == 0){
                        graph[in.x1][in.y1][in.x2][in.y2 + 1].previous = in;
                        }
                        in.y2++;
                        push(s, in, graph);
                        in.y2--;
                    }            
                }
                
               if(in.y2 - 1 >= 0){
                    if( graph[in.x1][in.y1][in.x2][in.y2 - 1].allowed == 1){

                        if(graph[in.x1][in.y1][in.x2][in.y2 - 1].visited == 0){
                        graph[in.x1][in.y1][in.x2][in.y2 - 1].previous = in;
                        }
                        in.y2--;
                        push(s, in, graph);
                        in.y2++;    
                    }            
                }

                if(in.x2 + 1 <= xmax){
               
                    if( graph[in.x1][in.y1][in.x2 + 1][in.y2].allowed == 1){
                        if(graph[in.x1][in.y1][in.x2 + 1][in.y2].visited == 0){
                        graph[in.x1][in.y1][in.x2 + 1][in.y2].previous = in;                       
                        }
                        in.x2++;
                        push(s, in, graph);
                        in.x2--;
                    }            
                }
                
                if(in.x1 + 1 <= xmax){
                    if(graph[in.x1 + 1][in.y1][in.x2][in.y2].allowed == 1){
                        
                        if(graph[in.x1 + 1][in.y1][in.x2][in.y2].visited == 0){
                        graph[in.x1 + 1][in.y1][in.x2][in.y2].previous = in;
                        }
                        in.x1++;
                        push(s, in, graph);
                        in.x1--;
                    }
                }
                if(in.x1 - 1 >= 0){
                
                    if(graph[in.x1 - 1][in.y1][in.x2][in.y2].allowed == 1){  

                        if(graph[in.x1 - 1][in.y1][in.x2][in.y2].visited == 0){
                        graph[in.x1 - 1][in.y1][in.x2][in.y2].previous = in;
                        }
                        in.x1--;
                        push(s, in, graph);
                        in.x1++;
                    }    
                }
                
                
                if(in.y1 - 1 >= 0){
               
                    if( graph[in.x1][in.y1 - 1][in.x2][in.y2].allowed == 1){
                        
                        if(graph[in.x1][in.y1 - 1][in.x2][in.y2].visited == 0){
                        graph[in.x1][in.y1 - 1][in.x2][in.y2].previous = in;
                        }
                        in.y1--;
                        push(s, in, graph);
                        in.y1++;
                    }            
                }
                
                if(in.y1 + 1 <= ymax){

                    if( graph[in.x1][in.y1 + 1][in.x2][in.y2].allowed == 1 ){
                        
                        if(graph[in.x1][in.y1 + 1][in.x2][in.y2].visited == 0){
                        graph[in.x1][in.y1 + 1][in.x2][in.y2].previous = in;
                        }
                        in.y1++;
                        push(s, in, graph);
                        in.y1--;
                    }
                }
            
                
                
                   
           
                  
            }
            
    }
    
    
    free(s); //if end not found return 0
    return 0;
    
}

void push(stack *s, index in, vertex**** graph){
    
    if (s->size == 0){
        
        s->size = 1;  //stack push
        //printf("\nSize %d", s->size);
        
        s->stack = malloc(sizeof(vertex*));
        s->stack[s->size - 1] = &graph[in.x1][in.y1][in.x2][in.y2];
        index in1 =in ;
        
        
        s->stack[s->size - 1]->ownIndex = in1;
    
    } else {
        
        s->size++;
        s->stack = realloc(s->stack, sizeof(vertex*) * s->size);
        s->stack[s->size - 1] = &graph[in.x1][in.y1][in.x2][in.y2];
        index in1 = in;
        s->stack[s->size - 1]->ownIndex = in1;
        //s->stack[s->size - 1]->ownIndex = in;
    }
    
    
    
 }

index* pop(stack *s){
  vertex *v = s->stack[s->size - 1];  //normal stack pop
    
    if (s->size == 1){
        //printf("\nSize %d", s->size);
        s->size = 0;
        free(s->stack); 
        
    } else {
        
        s->size--;
        //printf("\nSize %d", s->size);
        int size = s->size;
        s->stack = realloc(s->stack, sizeof(v) * size);
        
    }
    
    return &(v->ownIndex);
    
}

index* get_path(vertex**** graph, index end, index start, int *pathsize){
    index temp;
    temp = end;
    index *path = malloc(sizeof(index));                    //loops through previous vertices and creates an array of indexes
    path[0] = temp;
    temp = graph[temp.x1][temp.y1][temp.x2][temp.y2].previous;
    int size = 1;
    printf("\nStart(%d, %d, %d, %d)", start.x1, start.y1, start.x2, start.y2);
    while( temp.x1 != start.x1 || temp.y1 != start.y1 || temp.x2 != start.x2 || temp.y2 != start.y2 ){
        path = realloc(path, sizeof(index) * (++size));
        path[size - 1] = temp;
        temp = graph[temp.x1][temp.y1][temp.x2][temp.y2].previous;
    }
        path = realloc(path, sizeof(index) * (++size));
        path[size - 1] = temp;
        temp = graph[temp.x1][temp.y1][temp.x2][temp.y2].previous;
    
    //printf("\nTemp(%d, %d, %d, %d) ", temp.x1, temp.y1, temp.x2, temp.y2);
    *pathsize = size;
    return path;
}

void edit_floorplan(int **floorplan, index *path, int pathsize, index start, index end, int xSize,int ySize){
    int t = pathsize-1;
    printf("\nPath size %d", pathsize);
    printf("\nPath starting:\n\n");
    printf("This is the first node in the path.(%d, %d, %d, %d)", path[t].x1,path[t].y1,path[t].x2,path[t].y2);
    index prev = start;
    index robots = start;
    int i, j;
    int end1 = 0, end2 = 0;
    int sleep = 0;
    for(t = pathsize - 1; t >= 0; t--){
                                                                //This function loops through all of the indexes in the path.
        
        index tmp = path[t];
        
        if(tmp.x1 != prev.x1 || tmp.y1 != prev.y1){                         /*print logic:
                                                                            Print if new vertex that hasn't been printed.*/
            if(end1 != 1){                                                   //If robot finds its end point, dont print that robot
                if(tmp.x1 == end.x1 && tmp.y1 == end.y1){                   //moving
                    floorplan[robots.x1][robots.y1] = 2;
                    floorplan[prev.x1][prev.y1] = 2;
                    robots.x1 = tmp.x1;
                    robots.y1 = tmp.y1;
                    end1 = 1;
                    
                    for(i = 0; i < ySize; i++){
                        printf("\n");
                        
                        for(j = 0; j < xSize; j++){
                            if(robots.x1 == j && robots.y1 == i){
                                    printf("S");
                                } else if(robots.x2 == j && robots.y2 == i){
                                    printf("F");
                                } else if(floorplan[j][i] == 1){
                                    printf(" "); 
                                } else if(floorplan[j][i] == 2) {
                                    printf(" ");
                                } else if (floorplan[j][i] == 3){    
                                    printf(" ");
                                } else if (floorplan[j][i] == 4){
                                    printf("+");
                                } else {
                                    printf("#");
                                }
                           
                        }
                    }
                
                    for(sleep = 0; sleep < 25000000; sleep++){ //slows the time between printing to output
                                
                    }
                
                } else {
                    if(floorplan[tmp.x1][tmp.y1] != 2 && floorplan[tmp.x1][tmp.y1] != 4){
                        floorplan[prev.x1][prev.y1]= 2;
                        floorplan[robots.x1][robots.y1]=2;
                        robots.x1 = tmp.x1;
                        robots.y1 = tmp.y1;
                    
                        for(i = 0; i < ySize; i++){
                        printf("\n");
                        
                            for(j = 0; j < xSize; j++){
                                if(robots.x1 == j && robots.y1 == i){
                                    printf("S");
                                } else if(robots.x2 == j && robots.y2 == i){
                                    printf("F");
                                } else if(floorplan[j][i] == 1){
                                    printf(" "); 
                                } else if(floorplan[j][i] == 2) {
                                    printf(" ");
                                } else if (floorplan[j][i] == 3){    
                                    printf(" ");
                                } else if (floorplan[j][i] == 4){
                                    printf("+");
                                } else {
                                    printf("#");
                                }
                           
                            }
                        }
                    
                        for(sleep = 0; sleep < 25000000; sleep++){
                                
                        }
                    }
                }
            }
        } else if(tmp.x2 != prev.x2 || tmp.y2 != tmp.y2){
            if(end2 != 1){
                if(tmp.x2 == end.x2 && tmp.y2 == end.y2){
                    floorplan[robots.x2][robots.y2]=3;
                    floorplan[prev.x2][prev.y2]=3;
                    robots.x2 = tmp.x2;
                    robots.y2 = tmp.y2;
                    end2 = 1;
                    
                    for(i = 0; i < ySize; i++){
                        printf("\n");
                        
                            for(j = 0; j < xSize; j++){
                                if(robots.x1 == j && robots.y1 == i){
                                    printf("S");
                                } else if(robots.x2 == j && robots.y2 == i){
                                    printf("F");
                                } else if(floorplan[j][i] == 1){
                                    printf(" "); 
                                } else if(floorplan[j][i] == 2) {
                                    printf(" ");
                                } else if (floorplan[j][i] == 3){    
                                    printf(" ");
                                } else if (floorplan[j][i] == 4){
                                    printf("+");
                                } else {
                                    printf("#");
                                }
                           
                            
                            
                            
                        }
                    }
                    
                    for(sleep = 0; sleep < 25000000; sleep++){
                                
                    }
                    
                } else {
                    if(floorplan[tmp.x2][tmp.y2] != 3 && floorplan[tmp.x2][tmp.y2] != 4){
                        floorplan[prev.x2][prev.y2]=3;
                        floorplan[robots.x2][robots.y2]=3;
                        robots.x2 = tmp.x2;
                        robots.y2 = tmp.y2;
                        
                        for(i = 0; i < ySize; i++){
                        printf("\n");
                        
                            for(j = 0; j < xSize; j++){
                                
                                if(robots.x1 == j && robots.y1 == i){
                                    printf("S");
                                } else if(robots.x2 == j && robots.y2 == i){
                                    printf("F");
                                } else if(floorplan[j][i] == 1){
                                    printf(" "); 
                                } else if(floorplan[j][i] == 2) {
                                    printf(" ");
                                } else if (floorplan[j][i] == 3){    
                                    printf(" ");
                                } else if (floorplan[j][i] == 4){
                                    printf("+");
                                } else {
                                    printf("#");
                                }
                           
                            }
                        }
                    
                        for(sleep = 0; sleep < 25000000; sleep++){
                                
                        }
                    
                    }
                }
            }
        }
        
    
        prev = tmp; 
    }

}