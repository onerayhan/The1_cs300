
#include "MazeInterFace.h"
#include <exception>
#include <string>
using namespace std;

//Stack Methods
//implemented as in Slides



template<class T>
bool Stack<T>::isEmpty() const{
	return topOfStack == nullptr;
}

template <class T>
T& Stack<T>::top()   {
	if (isEmpty()) {
		cout << "its empty";
		throw underflow_error("The number's a bit small, captain!");
	}
	else {
		return topOfStack->element;
	}
	
}

template<class T>
void Stack<T>::pop() {
	if (isEmpty()) {
		cout << "it's empty";
		
	}
	else {
		StackNode* oldTop = topOfStack;
		topOfStack = topOfStack->next;
		delete oldTop;
	}
	
}

template<class T>
void Stack<T>::push(T& newElement) {
	topOfStack = new StackNode(newElement, topOfStack);
}



template<class T>
T Stack<T>::topAndPop() {
	T topElement = top();
	pop();
	return topElement;
}

template <class T>
void Stack<T>::makeEmpty()
{
	while (!isEmpty())
		pop();
}

//deep Copy
template<class T>
const Stack<T>& Stack<T>::operator=(const Stack<T>& rhs) {
	if (this != rhs) {
		makeEmpty();
		if (rhs.isEmpty())
			return *this;
		StackNode* rptr = rhs.topOfStack;
		StackNode* ptr = new StackNode(rptr->element);
		topOfStack = ptr;
		for (rptr = rptr->next; rptr != nullptr; rptr = rptr->next) {
			ptr = ptr->next = new StackNode(rptr->element);
		}
		return *this;
	}
}

//copy constructor
template<class T>
Stack<T>::Stack(const Stack<T>& copy) {
	topOfStack = NULL;
	*this = copy; //deep copy
}

template <class T>
Stack<T>::~Stack()
{
	makeEmpty();
}





MazeGenerator::MazeGenerator(int rows, int columns, MazeNode* head) : head(head), rows(rows), columns(columns), stack(new Stack<MazeNode>()){
	initialize();
	generate();
}
void MazeGenerator::initialize() {
	cout<<("initialization started") << endl;
	
	stack->push(*head);
}

MazeNode* MazeGenerator::getHead() const {
	return head;
}

int MazeGenerator::getRows() const {
	return rows;
}

int MazeGenerator::getColumns() const {
	return columns;
}
Stack<MazeNode> * MazeGenerator::getStack() const {
	return stack;
}

void MazeGenerator::generate() {
	srand(time(0));
	cout << ("generation started") << endl;
	while (!stack->isEmpty()) {
		
		if (hasAvailableRoutes(stack->top())) {
			int route = chooseRandomRoute(stack->top());
			stack->top().walls[route] = 0;
			switch (route) {
			case 0:
				stack->top().mazeConnections[route]->walls[1] = 0;
				break;
			case 1:
				stack->top().mazeConnections[route]->walls[0] = 0;
				break;
			case 2:
				stack->top().mazeConnections[route]->walls[3] = 0;
				break;
			case 3:
				stack->top().mazeConnections[route]->walls[2] = 0;
				break;
			}
			//break walls
			//update availablity
			stack->top().isVisited = true;
			stack->push(*(stack->top().mazeConnections[route]));
		}
		else {
			stack->top().isVisited = true;
			stack->pop();
		}
		
	}
}

bool MazeGenerator::hasAvailableRoutes(MazeNode& cell)  {
	
	int counter = 0;
	//cout << "routes: ";
	for (int i = 0; i < 4; i++) {
		if (cell.mazeConnections[i] != nullptr && !cell.mazeConnections[i]->isVisited) {
			cell.availableRoutes[i] = 1;
			counter++;
		}
		else {
			cell.availableRoutes[i] = 0;
		}
		//cout << cell.availableRoutes[i];
	}
	//cout << endl;
	if (counter > 0)
		return true;
	else 
		return false;
	
}

int MazeGenerator::chooseRandomRoute(const MazeNode& cell) const {
	int a = rand() % 4;
	while (cell.availableRoutes[a] != 1) {
		a = rand() % 4;
		//cout << a ;
	}
	//cout << a << endl;
	return a;
}

//shallowCopying since no deep copy is necessary
MazeGenerator::MazeGenerator(const MazeGenerator& copy): head(copy.getHead()), stack(copy.getStack()), rows(copy.getRows()), columns(copy.getColumns()){
	
}

MazeGenerator::~MazeGenerator() {
	delete stack;

}

MazeGenerator& MazeGenerator::operator=(const MazeGenerator& rhs) {
	if (this != &rhs)
	{
		
		head = rhs.getHead();
		rows = rhs.getRows();
		columns = rhs.getColumns();
		stack = rhs.getStack();
	}
	return *this;

}


Maze::Maze(int rows, int columns) : rows(rows), columns(columns), head(nullptr) {
}

Maze::Maze(const string& fName) {

}
void Maze::mazeInit() {
	head = new MazeNode(0, 0);
	MazeNode* belowRow = head;

	for (int i = 0; i < rows; i++) {
		MazeNode* currentRow = new MazeNode(0, i);
		belowRow->mazeConnections[2] = currentRow;
		currentRow->mazeConnections[3] = belowRow;
		MazeNode* currentCol = currentRow;
		MazeNode* belowCol = belowRow;
		for (int j = 1; j < columns; j++) {
			currentCol->mazeConnections[1] = new MazeNode(j, i);
			currentCol->mazeConnections[1]->mazeConnections[0] = currentCol;
			currentCol = currentCol->mazeConnections[1];
			if (i > 0) {
				belowCol = belowCol->mazeConnections[1];
				currentCol->mazeConnections[3] = belowCol;
				belowCol->mazeConnections[2] = currentCol;
			}
		}
		belowRow = belowRow->mazeConnections[2];
	}
	MazeNode* temp = head;
	head = head->mazeConnections[2];
	delete temp;
}




void Maze::generateMaze() {
	mazeInit();
	MazeGenerator generator = MazeGenerator(rows, columns, head);
	
}

void Maze::outputToFile(const string& filename) const {
	ofstream outFile(filename);
	if (!outFile.is_open()) {
		throw std::runtime_error("Failed to open output file");
	}
	outFile << rows << " " << columns << endl;
	MazeNode* rowNode = head;
	while (rowNode != nullptr) {
		MazeNode* colNode = rowNode;
		while (colNode != nullptr)
		{
			outFile << "x=" << colNode->X << " y=" << colNode->Y << " l=" << colNode->walls[0] << " r=" << colNode->walls[1] << " u=" << colNode->walls[2] << " d=" << colNode->walls[3] <<endl;
			colNode = colNode->mazeConnections[1];
		}
		rowNode = rowNode->mazeConnections[2];
	}
	outFile.close();
}


void Maze::findRoute(int entryX, int entryY, int exitX, int exitY, int mazeID) {
	MazeNode * temp = head;
	setNonVisited();
	for (int i = 0; i < exitY; i++) {
		temp = temp->mazeConnections[2];
	}
	for (int j = 0; j < exitX; j++) {
		temp = temp->mazeConnections[1];
	}

	Stack<MazeNode>* stack = new Stack<MazeNode>();
	stack->push(*temp);
	while (!stack->isEmpty()) {
		if (stack->top().X == entryX && stack->top().Y == entryY) {
			break;
		}
		else if (hasAvailablePath(stack->top())) {
			int route = chooseRandomRoute(stack->top());
			stack->top().isVisited = 1;
			stack->push(*(stack->top().mazeConnections[route]));
		}
		else{
			stack->top().isVisited = true;
			stack->pop();
		}
	}
	while (!stack->isEmpty()) {
		string fileName = "maze_" + to_string(mazeID) + "_path_" + to_string(entryX) + "_" + to_string(entryY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt";
		cout << fileName;
		ofstream outFile(fileName);
		if (!outFile.is_open()) {
			throw std::runtime_error("Failed to open output file");
		}
		while (!stack->isEmpty()) {
			outFile << stack->top().X << " " << stack->top().Y << endl;
			stack->pop();
		}
	}

}

void Maze::setNonVisited() {
	MazeNode* rowNode = head;
	while (rowNode != nullptr) {
		MazeNode* colNode = rowNode;
		while (colNode != nullptr)
		{
			colNode->isVisited = 0;
			colNode = colNode->mazeConnections[1];
		}
		rowNode = rowNode->mazeConnections[2];
	}
}

Maze& Maze::operator=(const Maze& rhs) {
	if (this != &rhs)
	{
		this->rows = rhs.rows;
		this->columns = rhs.columns;
		this->head = rhs.head;
		/*
		MazeNode* tempRow = head;
		MazeNode* rowNode = rhs.head;
		while (rowNode != nullptr) {
			MazeNode* tempCol = tempRow;
			MazeNode* colNode = rowNode;
			while (colNode != nullptr)
			{
				
				outFile << "x=" << colNode->X << " y=" << colNode->Y << " l=" << colNode->walls[0] << " r=" << colNode->walls[1] << " u=" << colNode->walls[2] << " d=" << colNode->walls[3] << endl;
				colNode = colNode->mazeConnections[1];
			}
			rowNode = rowNode->mazeConnections[2];
		}*/
	}
	return *this;
}

bool Maze::hasAvailablePath(MazeNode & cell) {
	int counter = 0;
	//cout << "routes: ";
	for (int i = 0; i < 4; i++) {
		if (cell.walls[i] == 0 && cell.mazeConnections[i] != nullptr && cell.mazeConnections[i]->isVisited != 1) {
			cell.availableRoutes[i] = 1;
			counter++;
		}
		else {
			cell.availableRoutes[i] = 0;
		}
		//cout << cell.availableRoutes[i];
	}
	//cout << endl;
	if (counter > 0)
		return true;
	else
		return false;
}

int Maze::chooseRandomRoute(const MazeNode& cell) const {
	int a = rand() % 4;
	while (cell.availableRoutes[a] != 1) {
		a = rand() % 4;
		//cout << a ;
	}
	//cout << a << endl;
	return a;
}
