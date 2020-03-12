#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
using namespace std;

struct position
{
	int row;
	int col;
	int parentRow;
	int parentCol;
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
bool skipSuccessor(vector<position> tempList, position successor);
bool compareValue(position p1, position p2);
bool foundObstacle(position node, vector<obstacle> obstacles);
position aStar(int startRow, int startCol, int goalRow, int goalCol, vector<obstacle> obsPos);


int rows, cols;
vector<position> closedList;
vector<position> openList;

int main()
{

	// add your code here
	int startRow, startCol, goalRow, goalCol, obstacles, tempRow, tempCol;
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



	vector<int> obstacleRows;
	vector<int> obstacleCols;

	vector<position> finalPath;
	vector<obstacle> obsPos;

	for (int i = 0; i < obstacles; i++) {
		cin >> tempRow;
		cin >> tempCol;

		obstacle ob;
		ob.row = tempRow;
		ob.col = tempCol;
		obsPos.push_back(ob);
	}

	position end = aStar(startRow, startCol, goalRow, goalCol, obsPos);

	cout << -1 << " " << -1 << endl;

	if (end.parentRow == NULL)
	{
		cerr << "No path exists to goal" << endl;
	}
	else
	{
		position temp = end;
		for (int i = closedList.size()-1; i >= 0; i--)
		{
			if (temp.row == closedList.at(i).row && temp.col == closedList.at(i).col)
			{
				finalPath.push_back(closedList.at(i));
				temp.row = closedList.at(i).parentRow;
				temp.col = closedList.at(i).parentCol;
			}
		}

		for (int i = finalPath.size() - 1; i >= 0; i--)
		{
			cout << finalPath.at(i).row << " " << finalPath.at(i).col << endl;
		}
	}

	cout << -1 << " " << -1 << endl;

	cout.flush();
	system("pause");
	return 0;
}

position aStar(int startRow, int startCol, int goalRow, int goalCol, vector<obstacle> obsPos)
{
	bool failure;

	position goalNode, startNode;
	goalNode.row = goalRow;
	goalNode.col = goalCol;

	startNode.row = startRow;
	startNode.col = startCol;
	startNode.hVal = getDistance(startNode, goalNode);
	startNode.gVal = 0;
	startNode.parentRow = NULL;
	startNode.fval = startNode.hVal + startNode.gVal;

	openList.push_back(startNode);
	cout << startNode.row << " " << startNode.col << endl;

	while (true) {

		if (openList.size() == 0)
		{
			//failure
			position failPos;
			failPos.parentRow = NULL;
			return failPos;
		}

		position chosenNode = openList.back();
		openList.pop_back();

		closedList.push_back(chosenNode);

		if (evaluateSuccess(chosenNode, goalNode))
		{
			return chosenNode;
		}


		for (int i = 0; i < 4; i++)
		{
			position temp;
			vector<position> tempOpenList(openList);
			vector<position> tempClosedList(closedList);

			switch (i)
			{
			case 0:
				temp.row = chosenNode.row + 1;
				temp.col = chosenNode.col;
				break;
			case 1:
				temp.row = chosenNode.row;
				temp.col = chosenNode.col + 1;
				break;
			case 2:
				temp.row = chosenNode.row - 1;
				temp.col = chosenNode.col;
				break;
			case 3:
				temp.row = chosenNode.row;
				temp.col = chosenNode.col - 1;
				break;
			}

			if (checkBounds(temp))
			{
				temp.parentRow = chosenNode.row;
				temp.parentCol = chosenNode.col;
				temp.hVal = getDistance(temp, goalNode);
				temp.gVal = chosenNode.gVal + 1;
				temp.fval = temp.hVal + temp.gVal;

				bool skip = false;

				if (skipSuccessor(tempOpenList, temp))
					skip = true;
				else if (skipSuccessor(tempClosedList, temp))
					skip = true;
				if (foundObstacle(temp, obsPos))
					skip = true;
				if (!skip)
				{
					//add successor
					openList.push_back(temp);
					cout << temp.row << " " << temp.col << endl;

					//sort list
					sort(openList.begin(), openList.end(), compareValue);
				}
			}
		}
	}
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

bool skipSuccessor(vector<position> tempList, position successor)
{
	for (int i = 0; i < tempList.size(); i++)
	{
		position tempnode = tempList.at(i);
		if (tempnode.row == successor.row && tempnode.col == successor.col)
		{
			if (tempnode.fval <= successor.fval)
			{
				//skip successor
				return true;
			}
		}
	}
	return false;
}

bool compareValue(position p1, position p2)
{
	return (p1.fval > p2.fval);
}


bool foundObstacle(position node, vector<obstacle> obstacles)
{
	for (int i = 0; i < obstacles.size(); i++)
	{
		if (node.row == obstacles[i].row && node.col == obstacles[i].col)
		{
			return true;
		}
	}
	return false;
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