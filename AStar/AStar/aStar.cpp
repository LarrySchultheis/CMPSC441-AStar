//Larry Schultheis
//CMPSC 441
//Project 1 - AStar Searching Algorithm

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
		//create the obstacles
		cin >> tempRow;
		cin >> tempCol;

		obstacle ob;
		ob.row = tempRow;
		ob.col = tempCol;
		obsPos.push_back(ob);
	}

	//run the AStar algorithm and store the end result
	position end = aStar(startRow, startCol, goalRow, goalCol, obsPos);

	//signal the end of the search
	cout << -1 << " " << -1 << endl;

	if (end.parentRow == NULL)
	{
		//if the end is not found
		cerr << "No path exists to goal" << endl;
	}
	else
	{
		//if the end is found
		position temp = end;
		for (int i = closedList.size()-1; i >= 0; i--)
		{
			//reverse iterate across the closed list
			if (temp.row == closedList.at(i).row && temp.col == closedList.at(i).col)
			{
				//if the parent is found add it to the final path list 
				finalPath.push_back(closedList.at(i));

				//save the new parent
				temp.row = closedList.at(i).parentRow;
				temp.col = closedList.at(i).parentCol;
			}
		}

		for (int i = finalPath.size() - 1; i >= 0; i--)
		{
			//foreach node in the final path, output to the pathfinding environment 
			cout << finalPath.at(i).row << " " << finalPath.at(i).col << endl;
		}
	}

	//signal the end of the output
	cout << -1 << " " << -1 << endl;

	cout.flush();
	system("pause");
	return 0;
}

// Runs the AStar search and returns the goal node if found
// Paramaters: start row of the search, start column of the search, goal row, goal column, vector containing obstacles
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

	//add the start node to the open list
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

		//choose the node with the best f-value and remove from the open list
		position chosenNode = openList.back();
		openList.pop_back();

		//add the chosen node to the closed list
		closedList.push_back(chosenNode);

		//if the goal is reached, return that node
		if (evaluateSuccess(chosenNode, goalNode))
		{
			return chosenNode;
		}

		//for each possible successor 
		for (int i = 0; i < 4; i++)
		{
			position temp;

			//decide which successor to evaluate
			switch (i)
			{
			case 0:
				//evaluate successor above
				temp.row = chosenNode.row + 1;
				temp.col = chosenNode.col;
				break;
			case 1:
				//evaluate successor to the right
				temp.row = chosenNode.row;
				temp.col = chosenNode.col + 1;
				break;
			case 2:
				//evaluate successor below
				temp.row = chosenNode.row - 1;
				temp.col = chosenNode.col;
				break;
			case 3:
				//evaluate successor to the left
				temp.row = chosenNode.row;
				temp.col = chosenNode.col - 1;
				break;
			}

			//if the successor is in the bounds
			if (checkBounds(temp))
			{
				//save the attributes into the temp node
				temp.parentRow = chosenNode.row;
				temp.parentCol = chosenNode.col;
				temp.hVal = getDistance(temp, goalNode);
				temp.gVal = chosenNode.gVal + 1;
				temp.fval = temp.hVal + temp.gVal;

				bool skip = false;

				//decide if the successor should be skipped 
				if (skipSuccessor(openList, temp))
					skip = true;
				else if (skipSuccessor(closedList, temp))
					skip = true;
				if (foundObstacle(temp, obsPos))
					skip = true;
				//if the successor isn't skipped 
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
	//get the manhattan distance
	int rows = abs(goalNode.row - currentNode.row);
	int cols = abs(goalNode.col - currentNode.col);

	return rows + cols;
}

// Returns true or false based on if the goal is found
// Parameters: the current node position and the goal node position
bool evaluateSuccess(position currentNode, position goalNode)
{
	return (currentNode.row == goalNode.row && currentNode.col == goalNode.col);
}

// Returns true or false based on whether the successor is in bounds
// Paramaters: the position of the node to test
bool checkBounds(position node)
{
	return (node.row >= 0 && node.row < rows && node.col >= 0 && node.col < cols);
}

// Returns whether the successor should be skipped or not
// Parameters: List of nodes and the position of the successor 
bool skipSuccessor(vector<position> tempList, position successor)
{
	//foreach node in the list
	for (int i = 0; i < tempList.size(); i++)
	{
		position tempnode = tempList.at(i);
		//If the current node has already been visited in the list
		if (tempnode.row == successor.row && tempnode.col == successor.col)
		{
			//If the f value of the node is less than or equal to the one in the list
			if (tempnode.fval <= successor.fval)
			{
				//skip successor
				return true;
			}
		}
	}
	return false;
}

// Utility function to help sort a vector
// Paramaters: Two positions to compare 
bool compareValue(position p1, position p2)
{
	return (p1.fval > p2.fval);
}

// Returns whether an obstacles has been encountered'
// Paramaters: The position of the node and a vector of obstacles
bool foundObstacle(position node, vector<obstacle> obstacles)
{
	//foreach obstacle
	for (int i = 0; i < obstacles.size(); i++)
	{
		//If the node to evaluate is on an obstacle return true
		if (node.row == obstacles[i].row && node.col == obstacles[i].col)
		{
			return true;
		}
	}
	return false;
}
