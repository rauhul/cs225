#ifndef MAZE_H
#define MAZE_H

// c-style includes
#include <stdio.h>
#include <stdlib.h>

// c++ style includes
#include <iostream>
#include <vector>
#include <queue>

#include "png.h"
#include "dsets.h"
using namespace std;

class SquareMaze {
    public:
		//No-parameter constructor
    	SquareMaze();
		//Deconstructor
    	~SquareMaze();
		//Makes a new SquareMaze of the given height and width
		void makeMaze(int width, int height);
		//This uses your representation of the maze to determine whether it is possible
		//to travel in the given direction from the square at coordinates (x,y)
		bool canTravel(int x, int y, int dir) const;
		bool canTravelPrecomputed(int x, int y, int x2, int y2, int dir) const;
		//Sets whether or not the specified wall exists
		void setWall(int x, int y, int dir, bool exists);
		//Solves this SquareMaze
		vector<int> solveMaze();
		//Draws the maze without the solution
		PNG* drawMaze() const;
		//This function calls drawMaze, then solveMaze; it modifies the PNG from drawMaze
		//to show the solution vector and the exit
		PNG* drawMazeWithSolution();
	private:
		//Helper method to clear the memory held by the maze array
		void clear();
		//Helper method to check index validity before use
		bool valid(int x, int y) const;

		tuple<int, int> indicesForDirection(size_t x, size_t y, size_t dir) const;
		tuple<int, int> reverseIndicesForDirection(size_t x, size_t y, size_t dir) const;

		DisjointSets mazeSet;
		bitset<2>** maze;
		size_t w;
		size_t h;
};
#endif // MAZE_H