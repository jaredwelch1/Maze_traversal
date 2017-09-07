# Maze Traversal

This is a Netbeans project written in C which creates a graph representing a maze for 2 robots to traverse through. Using a depth first search approach, it is checked if a path exists for those two robots to travel through the valid maze spaces while maintaining a distance of R between the two of them. If a path exists, the path is then shown to standard output. 

In order to execute this project, simply enter in the input file and paramter R into the command line at execution.
The program will then display the paths the two robots will take in standard output, in the form of the input file.

* In order to decrease to number of prints, as printing every single move was taking several minutes, the program will only print
if the robot is moving to a space that it has not occupied before. Backtraces do not need to be printed, as the robot was there at
one point in time, so it is obvious that it can go back to that spot again to go to a new node that has not been visited.
* R parameter is checked, and the graph will only output moves if the path is possible using that distance parameter.
However, since the output of moves only shows the robots moving to new spots, it sometimes may appear that they are within that
parameter even though the real path using every single node would represent a path that the two robots never do come within each
other. I could not figure out how to print the output in a reasonable time and also print the output exactly correct for every move.


