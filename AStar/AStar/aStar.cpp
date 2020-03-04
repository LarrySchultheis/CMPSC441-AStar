#include <iostream>
#include <vector>
using namespace std;

struct position
{
	int row;
	int col;
};

struct obstacle
{
	int row;
	int col;
};

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

int main()
{

	// add your code here
	int rows, cols, startRow, startCol, goalRow, goalCol, obstacles, tempRow, tempCol, curRow, curCol;
	bool foundGoal = false;
	bool done = false;
	bool obstacleFound = false;

	cin >> rows;
	cin >> cols;
	cin >> startRow;
	cin >> startCol;
	cin >> goalRow;
	cin >> goalCol;
	cin >> obstacles;


	vector<int> obstacleRows;
	vector<int> obstacleCols;
	vector<position> positionHistory;
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


	while (!done) {
		//up
		if ((curRow > goalRow) && !findObstacle(curRow, curCol, obsPos, obstacles, 0)) {
			curRow--;
		}
		//down
		else if ((curRow < goalRow) && !findObstacle(curRow, curCol, obsPos, obstacles, 2)) {
			curRow++;
		}

		else if ((curCol < goalCol) && !findObstacle(curRow, curCol, obsPos, obstacles, 1)) {
			curCol++;
		}
		else if ((curCol > goalCol) && !findObstacle(curRow, curCol, obsPos, obstacles, 3)) {
			curCol--;
		}
		else {
			//stuck
			if (curRow == goalRow && curCol == goalCol) {
				//found goal
				foundGoal = true;
				done = true;
			}
			else {
				//stuck and did not find goal
				foundGoal = false;
				done = true;
			}

		}

		position pos;
		pos.row = curRow;
		pos.col = curCol;

		positionHistory.push_back(pos);

	}

	cout << -1 << " " << -1 << endl;


	for (int i = 0; i < positionHistory.size(); i++) {
		cout << positionHistory.at(i).row << " " << positionHistory.at(i).col << endl;
	}


	cout << -1 << " " << -1 << endl;



	cout.flush();
	system("pause");
	return 0;
}