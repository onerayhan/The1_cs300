
#include <iostream>
#include "MazeInterFace.h"

int main()
{
    
    Maze* maze = new Maze(35,250);
    maze->generateMaze();
    maze->outputToFile("ilktest.txt");
    maze->findRoute(0, 0, 249, 4, 2);
}
