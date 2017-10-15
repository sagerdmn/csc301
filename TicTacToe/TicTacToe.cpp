// t3Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "windows.h"

using namespace std;

struct SMyTicTacToe
{
	bool PlayAgain();
	void TicTacToeLoop();
	void DrawTicTacToe();
	bool CheckForPitFalls();
	bool IsATicTacToeFormed();
	int GetMarkValue(int Xs, int Os);
	int EvaluatePosition();
	int NumberOfMovesPlayed;
	void humanMove();
	void computerMove();
	void VerifyFirstPlayer();
	bool IsFormed;
	bool DoIPlayFirst;
	int TicTacToeMarks[9];   // 1 for human,2 for cpu
	char WhoPlaysFirst;
};



////////////////////////////////////////////////////
// Initialise some data and check who plays first //
////////////////////////////////////////////////////

void SMyTicTacToe::VerifyFirstPlayer()
{
	ZeroMemory(TicTacToeMarks, sizeof(TicTacToeMarks));
	IsFormed = false;
	NumberOfMovesPlayed = 0;

	cout << "Player goes first.\n";
	cout << "\n";

}


// CPU has to make a move.Perform MinMax Algorithm.  
// Evaluate using EvaluatePosition() as a heuristic  
// function.Get the best position and output it.     
// Then draw the TicTacToe on screen.                 


void SMyTicTacToe::computerMove()
{
	if (IsATicTacToeFormed())
	{
		IsFormed = true;
		cout << "\nA TicTacToe was formed !!! You have somehow beaten the machine !!!\n Did you cheat :) ?\n";
	}
	else
	{
		int TicTacToeMarksBackUp[9];
		memcpy(TicTacToeMarksBackUp, TicTacToeMarks, sizeof(TicTacToeMarks));  // Create Local BackUp With Current Positions
		int best_value = -30;
		int best_position = 0;

		for (int i = 0; i <= 8; i++)
		{
			if (TicTacToeMarks[i] != 1 && TicTacToeMarks[i] != 2)  // If there's not a mark there..
			{
				TicTacToeMarks[i] = 1;
				int returned_value = EvaluatePosition();
				if (returned_value >= best_value)
				{
					best_value = returned_value;
					best_position = i;
				}
			}
			memcpy(TicTacToeMarks, TicTacToeMarksBackUp, sizeof(TicTacToeMarks));  // Recover Previous Values
		}

		cout << "\n\nDuring my move I placed a mark in square number: ";
		cout << best_position + 1;
		cout << "\n";
		TicTacToeMarks[best_position] = 1;
		NumberOfMovesPlayed++;
	}
	DrawTicTacToe();
}

//
// Human has to make a move too.    
// Then draw the TicTacToe on screen. 
//

void SMyTicTacToe::humanMove()
{
	if (IsATicTacToeFormed())
	{
		IsFormed = true;
		cout << "\nA Tic-Tac-Toe was formed !!! As usual,i won again.. :)\n";
	}
	else
	{
		int move = 0;
		while (move>9 || move<1)
		{
			cout << "\nPlease give a number from 1-9 for your move: ";
			cin >> move;
		}

		TicTacToeMarks[move - 1] = 2;
		NumberOfMovesPlayed++;
	}
	DrawTicTacToe();
}


// 3*O2 + O1 - ( 3*X2 + X1 )                                       
// O2 = number of rows,columns or diagonals with 2 Xs and zero Os  
// O1 = number of rows,columns or diagonals with 1 X and zero Os   
// X2 = number of rows,columns or diagonals with 2 Os and zero Xs  
// X1 = number of rows,columns or diagonals with 1 Os and zero Xs  
//                                                                 
// O represents cpu = 1                                            
// X represents human = 2                                      


int SMyTicTacToe::EvaluatePosition() // *** player/cpu logic changed -Dan ***
{
	if (GetMarkValue(3, 0))
		return 30;
	else if (GetMarkValue(0, 3))
		return 30;
	else if (CheckForPitFalls())
		return -30;
	else
	{
		int O2 = GetMarkValue(2, 0);  // determine x2 number
		int O1 = GetMarkValue(1, 0);  // determine x1 number
		int X2 = GetMarkValue(0, 2);  // determine O2 number
		int X1 = GetMarkValue(0, 1);  // determine O1 number
		return 3 * X2 + X1 - (3 * O2 + O1);
	}
}


// You pass the number of Xs and Os in this function.   
// Then it returns how many rows,columns and diagonals
// which have the same number of Xs and Os the TicTacToe 
// grid contains.                                       


int SMyTicTacToe::GetMarkValue(int Xs, int Os) // *** player/cpu logic changed -Dan ***
{
	int MarkValue = 0;
	int NumXs = 0, NumOs = 0;

	// Check Rows
	for (int i = 0; i <= 8; i++)
	{
		if (TicTacToeMarks[i] == 1) // If there's a CPU mark..
			NumOs++;
		else if (TicTacToeMarks[i] == 2) // If there's a HUMAN mark..
			NumXs++;

		if ((i + 1) % 3 == 0)  // If three row positions are over..
		{
			if (NumXs == Xs && NumOs == Os)
			{
				MarkValue++;
			}
			NumXs = 0;
			NumOs = 0;
		}
	}

	NumXs = 0, NumOs = 0;

	// Check Columns
	int i = 0;
	int next = 0;
	while (next <= 2)
	{
		for (int i = 0; i <= 8; i += 3)
		{
			if (TicTacToeMarks[i] == 1) // If there's CPU mark..
				NumOs++;
			else if (TicTacToeMarks[i] == 2) // If there's HUMAN mark..
				NumXs++;
		}
		if (NumXs == Xs && NumOs == Os)
		{
			NumXs = 0;
			NumOs = 0;
			MarkValue++;
		}
		next++;
		i = next;
		NumXs = 0, NumOs = 0;
	}

	NumXs = 0, NumOs = 0;

	//Check Diagonal 1
	for (int i = 0; i <= 8; i += 4)
	{
		if (TicTacToeMarks[i] == 1) // If there's CPU mark..
			NumOs++;
		else if (TicTacToeMarks[i] == 2) // If there's HUMAN mark..
			NumXs++;
	}
	if (NumXs == Xs && NumOs == Os)
	{
		NumXs = 0;
		NumOs = 0;
		MarkValue++;
	}

	NumXs = 0, NumOs = 0;

	//Check Diagonal 2
	for (i = 2; i <= 6; i += 2)
	{
		if (TicTacToeMarks[i] == 1) // If there's CPU mark..
			NumOs++;
		else if (TicTacToeMarks[i] == 2) // If there's HUMAN mark..
			NumXs++;
	}
	if (NumXs == Xs && NumOs == Os)
	{
		NumXs = 0;
		NumOs = 0;
		MarkValue++;
	}

	return MarkValue;
}

// Check if a TicTacToe is formed   
// Check for 3 Xs or Os for that  

bool SMyTicTacToe::IsATicTacToeFormed()
{
	if (GetMarkValue(3, 0))
		return true;
	else if (GetMarkValue(0, 3))
		return true;
	else
		return false;
}

// Check for some pitfalls of TicTacToe             
// Like Double TicTacToes.Find ways to bypass them  

bool SMyTicTacToe::CheckForPitFalls()
{
	if (TicTacToeMarks[4] == 1 && ((TicTacToeMarks[0] == 2 && TicTacToeMarks[8] == 2) || (TicTacToeMarks[2] == 2 && TicTacToeMarks[6] == 2)) && (TicTacToeMarks[1] == 0 && TicTacToeMarks[3] == 0 && TicTacToeMarks[5] == 0 && TicTacToeMarks[7] == 0))
		return true;
	else if (TicTacToeMarks[4] == 1 && TicTacToeMarks[8] == 1 && TicTacToeMarks[2] == 2 && TicTacToeMarks[3] == 2)
		return true;
	else if (TicTacToeMarks[4] == 1 && TicTacToeMarks[6] == 1 && TicTacToeMarks[0] == 2 && TicTacToeMarks[5] == 2)
		return true;
	else if (TicTacToeMarks[4] == 1 && TicTacToeMarks[6] == 1 && TicTacToeMarks[1] == 2 && TicTacToeMarks[8] == 2)
		return true;
	else if (TicTacToeMarks[4] == 1 && TicTacToeMarks[8] == 1 && TicTacToeMarks[1] == 2 && TicTacToeMarks[6] == 2)
		return true;
	else if (TicTacToeMarks[4] == 1 && TicTacToeMarks[8] == 1 && TicTacToeMarks[1] == 2 && TicTacToeMarks[3] == 2)
		return true;
	else if (TicTacToeMarks[4] == 1 && TicTacToeMarks[0] == 1 && TicTacToeMarks[6] == 1 && TicTacToeMarks[7] == 1 && TicTacToeMarks[1] == 2 && TicTacToeMarks[2] == 2 && TicTacToeMarks[3] == 2 && TicTacToeMarks[8] == 2)
		return true;
	else
		return false;
}

// This function draws the TicTacToe grid after each move  

void SMyTicTacToe::DrawTicTacToe() // *** player/cpu logic changed -Dan ***
{

	cout << "\nThis is what the TicTacToe grid looks like after the last move : \n\n";

	for (int i = 0; i <= 8; i++)
	{
		if ((i + 1) % 3 != 0)
		{
			if (TicTacToeMarks[i] == 1)
				cout << " O|";
			else if (TicTacToeMarks[i] == 2)
				cout << " X|";
			else if (TicTacToeMarks[i] == 0)
				cout << "  |";
		}
		else
		{
			if (TicTacToeMarks[i] == 1)
				cout << " O\n";
			else if (TicTacToeMarks[i] == 2)
				cout << " X\n";
			else if (TicTacToeMarks[i] == 0)
				cout << "  \n";
			if ((i + 1) % 9 != 0)
				cout << "-------- \n";
		}
	}
}

void SMyTicTacToe::TicTacToeLoop() // removed option to allow the player to choose who goes first
{
	while (NumberOfMovesPlayed <= 8)
	{
		humanMove();  // Human plays first here..
		if (IsFormed)
			break;
		if (NumberOfMovesPlayed != 9)
			computerMove();
		if (IsFormed)
			break;
	}
	if (!IsFormed)
		cout << "\nNobody wins..How can i win if i don't play first ?\n";
}

bool SMyTicTacToe::PlayAgain()
{
	char PlayAgain = 0;
	while (PlayAgain != 'y' && PlayAgain != 'n')
	{
		cout << "\nDo you want to play again ?(y|n)\n";
		cin >> PlayAgain;
	}
	cout << "\n";
	switch (PlayAgain)
	{
	case 'n':
		return false;
	case 'y':
		return true;
	}
	return false;
}


SMyTicTacToe *TicTacToe = new SMyTicTacToe;

int main()
{
	cout << "This is a Tic-Tac-Toe game created by human_thought.In order to play\nthis game you first decide ";
	cout << "whether YOU or the COMPUTER should play first.\n\n";
	cout << "In order to choose a position on which you want to place your mark,think of the ";
	cout << "Tic-Tac-Toe grid as having numbered squares.A TicTacToe grid contains 9 squares,so\n";
	cout << "just pick a number from 1-9 to identify where you want your mark put.";
	cout << "Keep in\nmind that number 1 is the upper left corner,while 9 is the lower right corner..\n";
	cout << "You can verify the positions watching the diagram below..\n";
	cout << "Good Luck !!!\n\n";


	cout << "  1| 2| 3  \n";
	cout << " --------  \n";
	cout << "  4| 5| 6  \n";
	cout << " --------  \n";
	cout << "  7| 8| 9  \n\n\n";

	cout << "O is for the Computer Opponent.\n";
	cout << "X is for the Human Opponent.\n\n\n";

	while (1)
	{
		TicTacToe = new SMyTicTacToe();
		TicTacToe->VerifyFirstPlayer();
		TicTacToe->TicTacToeLoop();

		if (!TicTacToe->PlayAgain())
			break;
	}

	return 0;
}