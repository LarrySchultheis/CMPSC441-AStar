#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
using namespace std;

struct position
{
	int row;
	int col;
	position* parent;
	int gVal;
	int hVal;
	int fval;
};

struct obstacle
{
	int row;
	int col;
};

bool findObstacle(int row, int col, vector<obstacle> obsPos, int obstacles, int direction);
int getDistance(position currentNode, position goalNode);
bool evaluateSuccess(position currentNode, position goalNode);
bool checkBounds(position node);
bool skipSuccessor(list<position> tempList, position successor);
bool compareValue(position p1, position p2);

int rows, cols;

int main()
{

	// add your code here
	int startRow, startCol, goalRow, goalCol, obstacles, tempRow, tempCol, curRow, curCol;
	bool foundGoal = false;
	bool done = false;
	bool obstacleFound = false;
	bool failure = false;

	cin >> rows;
	cin >> cols;
	cin >> startRow;
	cin >> startCol;
	cin >> goalRow;
	cin >> goalCol;
	cin >> obstacles;

	position goalNode, startNode;
	goalNode.row = goalRow;
	goalNode.col = goalCol;

	vector<int> obstacleRows;
	vector<int> obstacleCols;
	list<position> closedList;
	list<position> openList;
	vector<obstacle> obsPos;

	for (int i = 0; i < obstacles; i++) {
		cin >> tempRow;
		cin >> tempCol;

		obstacle ob;
		ob.row = tempRow;
		ob.col = tempCol;
		obsPos.push_back(ob);
	}

	curRow = startRow;
	curCol = startCol;

	startNode.row = curRow;
	startNode.col = curCol;
	startNode.hVal = getDistance(startNode, goalNode);
	startNode.gVal = 0;
	startNode.parent = NULL;
	startNode.fval = startNode.hVal + startNode.gVal;

	openList.push_back(startNode);
	cout << startNode.row << " " << startNode.col << " " << endl;




	while (true) {

		////up
		//if ((curRow > goalRow) && !findObstacle(curRow, curCol, obsPos, obstacles, 0)) {
		//	curRow--;
		//}
		////down
		//else if ((curRow < goalRow) && !findObstacle(curRow, curCol, obsPos, obstacles, 2)) {
		//	curRow++;
		//}

		//else if ((curCol < goalCol) && !findObstacle(curRow, curCol, obsPos, obstacles, 1)) {
		//	curCol++;
		//}
		//else if ((curCol > goalCol) && !findObstacle(curRow, curCol, obsPos, obstacles, 3)) {
		//	curCol--;
		//}
		//else {
		//	//stuck
		//	if (curRow == goalRow && curCol == goalCol) {
		//		//found goal
		//		foundGoal = true;
		//		done = true;
		//	}
		//	else {
		//		//stuck and did not find goal
		//		foundGoal = false;
		//		done = true;
		//	}

		//}

		if (openList.size() == 0)
		{
			//failure
			failure = true;
			break;
		}

		position chosenNode = openList.front();
		openList.pop_front();

		if (evaluateSuccess(chosenNode, goalNode))
		{
			foundGoal = true;
			break;
		}

		closedList.push_back(chosenNode);

		for (int i = 0; i < 4; i++)
		{
			position temp;
			list<position> tempOpenList = openList;
			list<position> tempClosedList = closedList;

			switch (i)
			{
				case 0:
					temp.row = chosenNode.row + 1;
					temp.col = chosenNode.col + 1;
					break;
				case 1:
					temp.row = chosenNode.row + 1;
					temp.col = chosenNode.col - 1;
					break;
				case 2:
					temp.row = chosenNode.row - 1;
					temp.col = chosenNode.col - 1;
					break;
				case 3: 
					temp.row = chosenNode.row - 1;
					temp.col = chosenNode.col + 1;
					break;
			}

			if (checkBounds(temp))
			{
				temp.parent = &chosenNode;
				temp.hVal = getDistance(temp, goalNode);
				temp.gVal = chosenNode.gVal + 1;
				temp.fval = temp.hVal + temp.gVal;
			}

			if (skipSuccessor(tempOpenList, temp))
				break;
			else if (skipSuccessor(tempClosedList, temp))
				break;
			else
			{
				//add successor
				openList.push_back(temp);
				cout << temp.row << " " << temp.col << " " << endl;

				//sort list
				//sort(openList.begin(), openList.end(), compareValue);
			}
		}
	
	}

	cout << -1 << " " << -1 << endl;


	for (int i = 0; i < closedList.size(); i++) {
		cout << closedList.front().row << " " << closedList.front().col << endl;
		closedList.pop_front();
	}


	cout << -1 << " " << -1 << endl;



	cout.flush();
	system("pause");
	return 0;
}

// Returns the h or g value of the current node
// Parameters: the current node and the start/goal node for g/h values, respectively
int getDistance(position currentNode, position goalNode)
{
	int rows = abs(goalNode.row - currentNode.row);
	int cols = abs(goalNode.col - currentNode.col);

	return rows + cols;
}

bool evaluateSuccess(position currentNode, position goalNode)
{
	return (currentNode.row == goalNode.row && currentNode.col == goalNode.col);
}

bool checkBounds(position node)
{
	return (node.row >= 0 && node.row < rows && node.col >= 0 && node.col < cols);
}

bool skipSuccessor(list<position> tempList, position successor)
{
	for (int j = 0; j < tempList.size(); j++)
	{
		position tempnode = tempList.front();
		if (tempnode.row == successor.row && tempnode.col == successor.col)
		{
			if (tempnode.hVal <= successor.fval)
			{
				//skip successor
				return true;
			}
		}
		tempList.pop_front();
	}
	return false;
}

bool compareValue(position p1, position p2)
{
	return (p1.fval < p2.fval);
}

bool findObstacle(int row, int col, vector<obstacle> obsPos, int obstacles, int direction)
{
	if (direction == 0) {
		//up
		for (int i = 0; i < obstacles; i++) {
			if ((row - 1) == obsPos.at(i).row && col == obsPos.at(i).col) {
				//obstacle found
				return true;
			}
		}
	}
	else if (direction == 1) {
		//right
		for (int i = 0; i < obstacles; i++) {
			if (row == obsPos.at(i).row && (col + 1) == obsPos.at(i).col) {
				//obstacle found
				return true;
			}
		}
	}
	else if (direction == 2) {
		//down
		for (int i = 0; i < obstacles; i++) {
			if ((row + 1) == obsPos.at(i).row && col == obsPos.at(i).col) {
				//obstacle found
				return true;
			}
		}
	}
	else if (direction == 3) {
		//left
		for (int i = 0; i < obstacles; i++) {
			if (row == obsPos.at(i).row && (col - 1) == obsPos.at(i).col) {
				//obstacle found
				return true;
			}
		}
	}
	else {
		//invalid
		cerr << "Invalid direction" << endl;
	}


	return false;
}