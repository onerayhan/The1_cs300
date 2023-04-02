#pragma once

#include <iostream>
#include <fstream>

using namespace std;

#ifndef MAZE
#define MAZE



struct MazeNode {
	//0: left, 1: right, 2: up, 3: down
	MazeNode(int x, int y) : X(x), Y(y) {}
	bool availableRoutes[4] = {0, 0, 0, 0};
	bool walls[4] = {1, 1, 1, 1 };
	MazeNode* mazeConnections[4] = {NULL, NULL, NULL, NULL };
	bool isVisited = 0;
	unsigned int X;
	unsigned int Y;
	/*
	MazeNode* left;
	MazeNode* right;
	MazeNode* up;
	MazeNode* down;
	
	*/
	/*
	bool isLeftBlocked;
	bool isRightBlocked;
	bool isUpBlocked;
	bool isDownBlocked;
	*/	
	
};



template <class T>
class Stack {
public:
	Stack() : topOfStack(nullptr) {};
	Stack(const Stack<T> & copy);
	~Stack();
	bool isEmpty() const;
	void makeEmpty();
	void pop();
	T topAndPop();
	T& top() ;
	void push(T& val);

	const Stack& operator=(const Stack& rhs);
private:
	struct StackNode
	{

		T& element;
		StackNode* next;

		StackNode( T& theElement, StackNode* n = NULL)
			: element(theElement), next(n) { }
	};

	StackNode * topOfStack;
};


class MazeGenerator {

public:
	MazeGenerator(int rows, int columns, MazeNode* head);
	MazeGenerator(const MazeGenerator& copy);
	MazeGenerator& operator=(const MazeGenerator& rhs);
	~MazeGenerator();
	void initialize();
	void generate();
	void checkAvailability();
	void formNullCells();
	bool hasAvailableRoutes(MazeNode& cell);
	int chooseRandomRoute(const MazeNode& cell) const;
	MazeNode* getHead() const;
	int getColumns() const;
	int getRows() const;
	Stack<MazeNode>* getStack() const;
private:
	Stack<MazeNode>* stack;
	int rows;
	int columns;
	MazeNode* head;

	friend class Maze;
};


class Maze {
public:
	Maze(int rows, int columns);
	Maze(const Maze& copy);
	Maze(const string & fName);
	Maze& operator=(const Maze& copy);
	~Maze();
	bool readFromFile() const;
	void formBlueprint();
	void mazeInit();
	void generateMaze();
	void outputToFile(const string& filename) const;
	void findRoute(int entryX, int entryY, int exitX, int exitY, int mazeID);
	bool hasAvailablePath(MazeNode& cell);
	void setNonVisited();
	int chooseRandomRoute(const MazeNode& cell) const;
private:
	MazeNode* head;
	int rows;
	int columns;

	friend class MazeGenerator;
};

//#include "Maze.cpp"
#endif