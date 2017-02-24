#include "maze.h"
using namespace std;

//No-parameter constructor
SquareMaze::SquareMaze() {
	maze = NULL;
}
//Deconstructor
SquareMaze::~SquareMaze() {
	clear();
}

//Makes a new SquareMaze of the given height and width
void SquareMaze::makeMaze(int width, int height) {
	clear();

	auto rowMajor = [&width](size_t x, size_t y) -> size_t {return width*x + y;};
	w = width;
	h = height;
	maze = new bitset<2>*[w];
	for (size_t x = 0; x < w; x++) {
		maze[x] = new bitset<2>[h];
		for (size_t y = 0; y < h; y++) {
			maze[x][y] = bitset<2>();
			maze[x][y][0] = 1;
			maze[x][y][1] = 1;
		}
	}

	size_t numIndependentTrees = w*h;
	DisjointSets mazeSet = DisjointSets();
	mazeSet.addelements(numIndependentTrees);

	srand(time(NULL));
	while (numIndependentTrees > 1) {
		size_t x1   = rand()%w;
		size_t y1   = rand()%h;
		size_t dir  = rand()%2;

		size_t x2   = x1 + (dir+1)%2; //if dir is 0 add 1 else add 0
		size_t y2   = y1 + (dir  )%2; //if dir is 1 add 1 else add 0
		if (valid(x2, y2)) {
			size_t idx1 = rowMajor(x1, y1);
			size_t idx2 = rowMajor(x2, y2);
			int root1 = mazeSet.find(idx1);
			int root2 = mazeSet.find(idx2);
			if (root1 != root2) {
				mazeSet.setunion(root1, root2);
				setWall(x1, y1, dir, false);
				numIndependentTrees--;
			}
		}
	}
}
//This uses your representation of the maze to determine whether it is possible
//to travel in the given direction from the square at coordinates (x,y)
bool SquareMaze::canTravel(int x, int y, int dir) const {
	int x2, y2;
	tie(x2, y2) = indicesForDirection(x, y, dir);
	if (valid(x2,y2)) {
		switch (dir) {
		case 0:
			return !maze[x][y][0];
		case 1:
			return !maze[x][y][1];
		case 2:
			return !maze[x2][y2][0];
		case 3:
			return !maze[x2][y2][1];
		}
	}
	return false;
}

bool SquareMaze::canTravelPrecomputed(int x, int y, int x2, int y2, int dir) const {
	if (valid(x2,y2)) {
		switch (dir) {
		case 0:
			return !maze[x][y][0];
		case 1:
			return !maze[x][y][1];
		case 2:
			return !maze[x2][y2][0];
		case 3:
			return !maze[x2][y2][1];
		}
	}
	return false;
}


//Sets whether or not the specified wall exists
void SquareMaze::setWall(int x, int y, int dir, bool exists) {
	if (!valid(x,y))
		return;
	maze[x][y][dir] = exists;
}
//Solves this SquareMaze
vector<int> SquareMaze::solveMaze() {
	vector<int>** solutions = new vector<int>*[w];
	for (size_t x = 0; x < w; x++) {
		solutions[x] = new vector<int>[h];
		for (size_t y = 0; y < h; y++) {
			solutions[x][y] = vector<int>();
		}
	}

	auto q = queue<tuple<size_t, size_t>>();
	solutions[0][0].push_back(0);
	q.push(tuple<size_t, size_t>(0,0));

	while (!q.empty()) {
		size_t x, y;
		tie(x,y) = q.front();
		q.pop();
		for (size_t dir = 0; dir < 4; dir++) {
			int x2, y2;
			tie(x2, y2) = indicesForDirection(x, y, dir);
			if (canTravelPrecomputed(x, y, x2, y2, dir)) {
				if (solutions[x2][y2].size() == 0) {
					// solutions[x2][y2] = solutions[x][y];
					solutions[x2][y2].push_back(solutions[x][y][0] + 1);
					solutions[x2][y2].push_back(dir);
					// solutions[x2][y2].push_back(dir);
					q.push(tuple<size_t, size_t>(x2,y2));
				}
			}
		}
	}

	int longest = solutions[0][h-1][0];
	size_t x = 0;
	for (size_t i = 0; i < w; i++) {
		if (solutions[i][h-1][0] > longest) {
			longest = solutions[i][h-1][0];
			x = i;
		}
	}

	vector<int> solution = vector<int>();
	size_t currX = x, currY = h - 1;

	auto currDir = [&currX, &currY, &solutions]() -> int {
		return solutions[currX][currY][1];
	};

	while (solutions[currX][currY].size() == 2) {
		solution.insert(solution.begin(), currDir());
		tie(currX, currY) = reverseIndicesForDirection(currX, currY, currDir());
	}


	for (size_t x = 0; x < w; x++) {
		delete [] solutions[x];
	}
	delete [] solutions;
	return solution;
}
//Draws the maze without the solution
PNG* SquareMaze::drawMaze() const {
	size_t width  = w*10+1;
	size_t height = h*10+1;
	PNG* mazeImage = new PNG(width, height);

	RGBAPixel black(0, 0, 0);

	for (size_t i = 0; i < width; i++) {
		if (i < 1 || i > 9)
			*(*mazeImage)(i,0) = black;
	}
	for (size_t i = 0; i < height; i++) {
		*(*mazeImage)(0,i) = black;
	}
	for (size_t x = 0; x < w; x++) {
		for (size_t y = 0; y < h; y++) {
			if (maze[x][y][0]) {
				for (size_t k = 0; k <= 10; k++) {
					*(*mazeImage)((x+1)*10, y*10+k) = black;
				}
			}
			if (maze[x][y][1]) {
				for (size_t k = 0; k <= 10; k++) {
					*(*mazeImage)(x*10+k, (y+1)*10) = black;
				}
			}
		}
	}
	return mazeImage;
}
//This function calls drawMaze, then solveMaze; it modifies the PNG from drawMaze
//to show the solution vector and the exit
PNG* SquareMaze::drawMazeWithSolution() {
	PNG* mazeImage = drawMaze();

	RGBAPixel   red(255,   0,   0);
	RGBAPixel white(255, 255, 255);

	vector<int> solution = solveMaze();
	int x = 5;
	int y = 5;
	int i;
	for (size_t iter = 0; iter < solution.size(); iter++) {
		switch (solution[iter]) {
		case 0:
			for (i = 0; i <= 10; i++) {
				*(*mazeImage)(x+i,y) = red;
			}
			x += 10;
			break;
		case 1:
			for (i = 0; i <= 10; i++) {
				*(*mazeImage)(x,y+i) = red;
			}
			y += 10;
			break;
		case 2:
			for (i = 0; i <= 10; i++) {
				*(*mazeImage)(x-i,y) = red;
			}
			x -= 10;
			break;
		case 3:
			for (i = 0; i <= 10; i++) {
				*(*mazeImage)(x,y-i) = red;
			}
			y -= 10;
			break;
		}
	}
	x -= 5;
	y += 5;
	for (i = 1; i < 10; i++) {
		*(*mazeImage)((x+i),y) = white;
	}
	return mazeImage;
}
//Helper Method to clear the memory held by the maze array
void SquareMaze::clear() {
	if (maze != NULL) {
		for (size_t x = 0; x < w; x++) {
			if (maze[x] != NULL) {
				for (size_t y = 0; y < h; y++) {
					maze[x][y].reset();
				}
				delete [] maze[x];
			}
		}
		delete [] maze;
	}
}
//Helper method to check index validity before use
bool SquareMaze::valid(int x, int y) const {
	return 0 <= x && x < int(w) && 0 <= y && y < int(h);
}
//Helper method
tuple<int, int> SquareMaze::indicesForDirection(size_t x, size_t y, size_t dir) const {
	int x2 = x;
	int y2 = y;
	switch (dir) {
	case 0:
		x2++;
		break;
	case 1:
		y2++;
		break;
	case 2:
		x2--;
		break;
	case 3:
		y2--;
		break;
	}
	return tuple<int, int>(x2, y2);
}

//Helper method
tuple<int, int> SquareMaze::reverseIndicesForDirection(size_t x, size_t y, size_t dir) const {
	int x2 = x;
	int y2 = y;
	switch (dir) {
	case 0:
		x2--;
		break;
	case 1:
		y2--;
		break;
	case 2:
		x2++;
		break;
	case 3:
		y2++;
		break;
	}
	return tuple<int, int>(x2, y2);
}