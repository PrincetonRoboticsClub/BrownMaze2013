
TestMaze: MazeNode.h Maze.h Maze.o MazeNode.o TestMaze.cpp Brain.o
	g++ -g MazeNode.cpp Maze.cpp Brain.cpp TestMaze.cpp -o TestMaze

Maze.o: Maze.cpp Maze.h
	g++ -c Maze.cpp

MazeNode.o: MazeNode.h MazeNode.cpp
	g++ -c MazeNode.cpp

Brain.o: Brain.h Brain.cpp
	g++ -c Brain.cpp