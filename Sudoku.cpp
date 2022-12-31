// Sudoku.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;

#define empty 0
#define N 9

bool isGridSafe(int grid[N][N], int row, int col, int num);
bool isEmptyLoc(int grid[N][N], int& row, int& col);
int grid[N][N];


/* check if the grid have assigned already  */
bool isEmptyLoc(int grid[N][N], int& row, int& col)
{
	for (row = 0; row < N; row++)
	{
		for (col = 0; col < N; col++)
			if (grid[row][col] == empty)
				return true;
	}
	//this location is filled
	return false;
}

/* Check if entries num has been used in Row*/
bool UsedinRow(int grid[N][N], int checkrow, int number)
{
	for (int col = 0; col < N; col++)
	{
		if (grid[checkrow][col] == number)
			return true;
	}
	//Same Num Same Row
	return false;
}
/* Check if entries num has been used in Column*/
bool UsedinCol(int grid[N][N], int checkcol, int number)
{
	for (int row = 0; row < N; row++)
	{
		if (grid[row][checkcol] == number)
			return true;
	}
	//Same Num Same Col
	return false;
}

/* Check if entries num has been used in  3x3 Box*/
bool UsedinBox(int grid[N][N], int boxBeginRow, int BoxBeginCol, int number)
{
	bool breturn = false;

	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++)
		{
			if (grid[boxBeginRow + row][BoxBeginCol + col] == number)
				breturn = true;
		}
	//return false if have the same number inside the box
	return breturn;
}

/* check if the grid safe to assign number*/
bool isGridSafe(int grid[N][N], int row, int col, int num)
{
	return !UsedinRow(grid, row, num) && !UsedinCol(grid, col, num) &&
		!UsedinBox(grid, row - row % 3, col - col % 3, num);
}

//Generating

class Sudoku {
public:
	int** mat;
	
	//SQRT of N
	int SRN;

	//Missing Digits
	int K;
	
	//Constructor
	Sudoku(int K)
	{
		srand(time(0));
		this->K = K;

		//Cal SQRT of N
		double dSRN = sqrt(N);
		SRN = (int)dSRN;
		mat = new int* [N];

		//Create a Row for Pointer
		for (int i = 0; i < N; i++)
		{
			mat[i] = new int[N];
			
			memset(mat[i], 0, N * sizeof(int));
		}
	}

	bool unUsedInBox(int rowStart, int colStart, int num)
	{
		for (int i = 0; i < SRN; i++) {
			for (int j = 0; j < SRN; j++) {
				if (mat[rowStart + i][colStart + j] == num) 
				{
					return false;
				}
			}
		}
		return true;
	}

	// Returns false if given 3 x 3 block contains num.
	bool CheckIfSafe(int i, int j, int num)
	{
		return (
			unUsedInRow(i, num) && unUsedInCol(j, num)
			&& unUsedInBox(i - i % SRN, j - j % SRN, num));
	}
	// check in the row for existence
	bool unUsedInRow(int i, int num)
	{
		for (int j = 0; j < N; j++) {
			if (mat[i][j] == num) {
				return false;
			}
		}
		return true;
	}
	// check in the row for existence
	bool unUsedInCol(int j, int num)
	{
		for (int i = 0; i < N; i++) {
			if (mat[i][j] == num) {
				return false;
			}
		}
		return true;
	}

	//Generator
	void fillVal()
	{
		fillDiag();
		fillRemaining(0,SRN);

		removeKDigits();
	}

	//Fill Diagonally
	void fillDiag()
	{
		for (int i = 0; i < N; i = i + SRN)
		{
			fillBox(i, i);
		}
	}
	
	void fillBox(int row, int col)
	{
		int num;
		for (int i = 0; i < SRN; i++) {
			for (int j = 0; j < SRN; j++) {
				do {
					num = randomGenerator(N);
				} while (!unUsedInBox(row, col, num));
				mat[row + i][col + j] = num;
			}
		}
	}

	int randomGenerator(int num)
	{
		
		return (int)floor(
			(float)(rand() / double(RAND_MAX) * num + 1));
		
	}

	//Using Recursicve to fill remaining
	bool fillRemaining(int i, int j)
	{
		// System.out.println(i+" "+j);
		if (j >= N && i < N - 1) {
			i = i + 1;
			j = 0;
		}
		if (i >= N && j >= N) {
			return true;
		}
		if (i < SRN) {
			if (j < SRN) {
				j = SRN;
			}
		}
		else if (i < N - SRN) {
			if (j == (int)(i / SRN) * SRN) {
				j = j + SRN;
			}
		}
		else {
			if (j == N - SRN) {
				i = i + 1;
				j = 0;
				if (i >= N) {
					return true;
				}
			}
		}
		for (int num = 1; num <= N; num++) {
			if (CheckIfSafe(i, j, num)) {
				mat[i][j] = num;
				if (fillRemaining(i, j + 1)) {
					return true;
				}
				mat[i][j] = 0;
			}
		}
		return false;
	}

	//Remove K number of Digits
	void removeKDigits()
	{
		int count = K;
		while (count != 0)
		{
			int cellId = randomGenerator(N * N) - 1;
			
			int i = (cellId / N);
			int j = cellId % 9;
			if (mat[i][j] != 0)
			{
				count--;
				mat[i][j] = 0;
			}
		}
	}
};

//Solving
/* assign 0 (unassigned) to all values for sudoku solution */

//using backtracking algorithm to solve Sudoku
bool SolveSudoku(int grid[N][N])
{
	int row, col;
	if (!isEmptyLoc(grid, row, col))
		return true;
	for (int num = 1; num <= 9; num++)
	{
		if (isGridSafe(grid, row, col, num))
		{
			grid[row][col] = num;
			if (SolveSudoku(grid))
				return true;
			grid[row][col] = empty;
		}
	}
	return false;
}

/* print result */
void printgrid(int grid[N][N])
{
	for (int row = 0; row < N; row++)
	{
		if (row % 3 == 0)
		{
			for (int i = 0; i < N * 4; i++)
				cout << "-";
			cout << endl;
		}

		for (int col = 0; col < N; col++)
		{
			if (col % 3 == 0)
			{
				cout << "|";
			}
			
			cout << grid[row][col];

			if(col != N-1)
			cout << "   ";
			else 
			cout << "|";
			
		}
			
		cout << endl;
	}
}

bool isGridNotClear(int grid[N][N])
{
	int row, col;
	bool breturn = true;

	for (row = 0; row < N; row++)
	{
		for (col = 0; col < N; col++)
		{
			if (grid[row, col] == 0)
				breturn = true;
		}
	}
	return breturn;
}

int main()
{
	int K = 20;
	string input;

	//make generate the grid
	Sudoku* sudoku = new Sudoku(K);
	sudoku->fillVal();
	/* Get Answer for Sudoku*/
	int answergrid[N][N];
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
				grid[i][j] = sudoku->mat[i][j];
				answergrid[i][j]  = grid[i][j];
		}
	};

	if (SolveSudoku(answergrid) == true)
	{
		cout << endl;
		//cout << "Solved" << endl;
		//printgrid(SolvedGrid);
	}
	else
	{
		cout << "No Solution Found" << endl;
		return 0;
	}

	printgrid(grid);
	

	/* Let Player Play the Game.*/
	while (isGridNotClear(grid))
	{
		cout << "Type Row Grid and Number to Assign in Order OR type 'Solve' to solve the problem " << endl;

		cin >> input;

		int arow = 0, acol = 0, anum = 0;

		//Show the answer. 
		if (input == "Solve" || input == "solve")
		{
			printgrid(answergrid);
			return 0;
		}

		//To Change String to Integer by Getting String and -48 to get values of integers.
		if (input[0] && input[1] && input[2])
		{
			arow = input[0] - 48;
			acol = input[1] - 48;
			anum = input[2] - 48;


			printf("try to assign %i at grid (%i,%i) \n", anum, acol, arow);

			if (0 < arow || arow < 9 && 0 < acol || acol < 9 && 0 < anum || anum < 9)
			{
				if (grid[arow][acol] == answergrid[arow][acol])
				{
					grid[arow][acol] = anum;
					printgrid(grid);
				}
				else
				{
					cout << "Wrong answer. please try again." << endl;
				}
			}
			else
			{
				cout << "Out of Range" << endl;
			}
		}
		else
		{
			cout << "ERROR. Please Try Again." << endl;
		}

		cout << endl;

	}
	
	cout << "Congrats! You solved the Problem" << endl;
	return 1;

}