// TicTacToe.cpp : Create a TicTacToe game that the user plays against the computer. User = X; computer = 0. User goes first.
//					Computer needs some strategy (which does not include loading a table full of all possible moves)
//					Game board must be built using a dynamic array (with cols = A - C and rows = 1 - 3) so that user can select with Letter# combo
//					Program should update and redraw board after each move and also recognize when a TicTacToe has been formed & notify user
//	Created by:		Karyn Boyd		boydp1@csp.edu		and	 Dan Sager		(email)
//	Due date:		First Draft - 10/15/17; Second draft - 10/20/17
//	Revisions:		Dan		(10/14/17) -	player/cpu logic changed  
//					Karyn	(10/14/17) -	replaced class CMyTicTacToe with struct SMyTicTacToe
//							(10/15/17) -	working on translating code so we can figure out how to change to dynamic array;
//											reworded output
//	Source: We're repurposing the code found at (http://www.codercaste.com/2011/01/26/how-to-code-a-tic-tac-toe-game-in-c-including-artificial-intelligence-opponent/) 
//			for our specs to save some time in figuring out how to make the computer strategize 
//			BUT we've gone through and changed the outputs for the players, removed the need to see if the computer or human will go first, 
//			and we're working on changing the board from the 1 - 9 array to a dynamic array.
//			We've also added notes to ourselves translating what's going on in the code so it'll be easier to tweak once we figure out the dynamic array thing
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "windows.h"

using namespace std;

enum marks { E = 0, X = 2, O = 1 };

struct SMyTicTacToe
{
	bool PlayAgain(); // ask user if they want to play again
					  // TicTacToeLoop() is the game itself - you do humanMove(), check IsATicTacToeFormed(), draw the board, 
					  // then do everything over for the computer starting with computerMove()
	void TicTacToeLoop();
	void DrawTicTacToe(); // lays out the T3 board each time a move is made
	bool CheckForPitFalls();  // looks for any wonkiness - like 2 TicTacToes
	bool IsATicTacToeFormed(); // checks for winners
	int GetMarkValue(int Xs, int Os); // human marker = X which is 2; cpu marker = O which is 1
	int EvaluatePosition(); // this is the AI's strategy; it uses a MinMax algorithm https://en.wikipedia.org/wiki/Minimax_theorem 
							// to test each move
							/* Never go up against a computer when WINNING is on the line!!
							3*O2 + O1 - ( 3*X2 + X1 )
							O2 = number of rows,columns or diagonals with 2 Xs and zero Os
							O1 = number of rows,columns or diagonals with 1 X and zero Os
							X2 = number of rows,columns or diagonals with 2 Os and zero Xs
							X1 = number of rows,columns or diagonals with 1 Os and zero Xs
							*/
	int NumberOfMovesPlayed; // this makes certain we don't go over 9 moves (the total # of spots on the board)
	void humanMove(); // this will take user input and place the human's X on the board
	void computerMove(); // this will use the EvaluatePosition() and the minmax algorithm to figure out where to put the computer's Os
	bool IsFormed;
	// bool DoIPlayFirst;
	int TicTacToeMarks[9];   // creates an array with 9 spots for the 3 X 3 board

};


// Human move.
// Draw board.
// Check for any TicTacToes

void SMyTicTacToe::humanMove()
{
	// Last move (with the exception of the starting move) was the computer so check to see if there's a T3.
	if (IsATicTacToeFormed())
	{
		IsFormed = true;
		// if there is, talk some smack
		cout << "\n Muahahaha! I win! You lose!\n";
	}
	else
	{
		// set the move var to 0 so we have something to work with
		int move = 0;
		/* move > than 9 or < 1?? We're looking for a # between 1 & 9 for the move?*/
		while (move>9 || move<1)
		{
			cout << "\n Please enter a number from 1-9 for your move: ";
			cin >> move;
		}

		// 2 in this isn't the #2; it means X (1 for Os; 2 for Xs)
		/****    THIS WILL NEED TO CHANGE ONCE WE GET THE DYNAMIC [] WORKING ***/
		// so take the move the human made and subtract 1 from it because
		// we're looking for the ELEMENT in the array where we can place our X
		// example: human enters 8; TicTacToeMarks[8-1] = 
		// ELEMENT 7 so the program puts an X in the 7 slot
		TicTacToeMarks[move - 1] = X;
		NumberOfMovesPlayed++;
	}
	//DrawTicTacToe();
}

// CPU uses minmax algorithm to EvaluatePosition()
// 


void SMyTicTacToe::computerMove()
{
	if (IsATicTacToeFormed())
	{
		IsFormed = true;
		cout << "\n Inconceivable! You cheated! I demand a recount! \n";
	}
	else
	{
		/* THIS WILL NEED TO CHANGE WHEN WE GET THE DYNAMIC ARRAY WORKING */
		int TicTacToeMarksBackUp[9];
		// This puts all the current TicTacToeMarks (T3M) into T3MBackup
		memcpy(TicTacToeMarksBackUp, TicTacToeMarks, sizeof(TicTacToeMarks));
		int bestValue = -30;
		int bestPosition = 0;

		/* THIS WILL NEED TO CHANGE WHEN WE GET THE DYNAMIC ARRAY WORKING */
		for (int i = 0; i <= 8; i++)
		{
			if (TicTacToeMarks[i] != O && TicTacToeMarks[i] != X)  // If there's not already a mark there
			{
				TicTacToeMarks[i] = O;
				// run the returned value through the EvaluatePosition() function
				int returnedValue = EvaluatePosition();
				// the evaluate it against the variable we set up earlier as -30
				if (returnedValue >= bestValue)
				{
					// if rV is bigger or equal to bV make bV whatever the returnedValue is
					bestValue = returnedValue;
					// stick the O into that spot/element in the array
					bestPosition = i; // hold this element for the computer's mark
				}
			}
			// Now take everything from T3MBackup and put it back out into T3 so we can draw the board.
			memcpy(TicTacToeMarks, TicTacToeMarksBackUp, sizeof(TicTacToeMarks));
		}

		cout << "\n\n After some genius-level calculations. I put my mark in ";
		cout << bestPosition + 1; //this returns the actual letter# combo as output
		cout << "! Top that!\n";
		TicTacToeMarks[bestPosition] = O; // this sticks the O into the element's spot
		NumberOfMovesPlayed++; // and then ups the # of moves so we don't go over
	}
	//DrawTicTacToe(); // and draws the board again
}

/* Karyn note: I'm still not fully getting what's going on between GetMarkValue and EvaluatePosition */

// EvaluatePosition relies on GetMarkValue()
int SMyTicTacToe::EvaluatePosition() // *** player/cpu logic changed -Dan ***
{
	// if there's a row/col with 3 Xs and 0 Os (Human wins!!!)
	if (GetMarkValue(3, 0))
		return 30;
	// if there's a row/col with 0 Xs & 3 Os (Computer wins!!)
	else if (GetMarkValue(0, 3))
		return 30;
	// NOTE: Can we pull this? It wasn't working earlier.  We'd also need to pull the function which is later on
	else if (CheckForPitFalls())
		return -30;
	else
	{
		int O2 = GetMarkValue(2, 0);  // (rows/cols with 2 Xs, 0 Os)
		int O1 = GetMarkValue(1, 0);  // (rows/cols with 1 X, 0 Os)
		int X2 = GetMarkValue(0, 2);  // (rows/cols with 0 Xs, 2 Os)
		int X1 = GetMarkValue(0, 1);  // (rows/cols with 0 Xs, 1 O)
									  // so looking at the board you take:
									  // 3 ( # r/c with 0 Xs and 2 Os) + ( # r/c with 0Xs and 1O) - (( 3 * (# r/c with 2 Xs and 0 Os) + (#r/c with 1 X and 0 Os))
									  // let's say the top row has 2 Xs and 0 Os --> O2
									  // the 2nd row has 1 O and 0 Xs -->  X1
									  // and the third row has 1 O and 0 Xs --> X1
		return 3 * X2 + X1 - (3 * O2 + O1);
		//  return (3 * 0) + 2 - ((3 * 1) + 0) --> (0 + 2) - (3 + 0) or 2 - 3 = -1

	}
}

/* Karyn note: Still chewing on this one. Every time I think I've gotten it, I'm totally wrong*/
int SMyTicTacToe::GetMarkValue(int Xs, int Os) // *** player/cpu logic changed -Dan ***
{
	// we start off with everything set to 0
	int MarkValue = 0;
	int NumXs = 0, NumOs = 0;

	for (int i = 0; i <= 8; i++)
	{
		// this will loop through every element looking for a
		// CPU mark (1) or a Human mark (2)
		// so for [0] (which is box 1), there IS a human mark so
		if (TicTacToeMarks[i] == O)
			NumOs++;
		// we up the # of Xs by 1 --> NumXs = 1
		else if (TicTacToeMarks[i] == X)
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
			if (TicTacToeMarks[i] == O) // If there's CPU mark..
				NumOs++;
			else if (TicTacToeMarks[i] == X) // If there's HUMAN mark..
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
		if (TicTacToeMarks[i] == O) // If there's CPU mark..
			NumOs++;
		else if (TicTacToeMarks[i] == X) // If there's HUMAN mark..
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
		if (TicTacToeMarks[i] == O) // If there's CPU mark..
			NumOs++;
		else if (TicTacToeMarks[i] == X) // If there's HUMAN mark..
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

//  

bool SMyTicTacToe::CheckForPitFalls()
{
	if (TicTacToeMarks[4] == 1
		&& ((TicTacToeMarks[0] == 2 && TicTacToeMarks[8] == 2) || (TicTacToeMarks[2] == 2 && TicTacToeMarks[6] == 2))
		&& (TicTacToeMarks[1] == 0 && TicTacToeMarks[3] == 0 && TicTacToeMarks[5] == 0 && TicTacToeMarks[7] == 0))
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
	else if (TicTacToeMarks[4] == 1 && TicTacToeMarks[0] == 1 && TicTacToeMarks[6] == 1 && TicTacToeMarks[7] == 1
		&& TicTacToeMarks[1] == 2 && TicTacToeMarks[2] == 2 && TicTacToeMarks[3] == 2 && TicTacToeMarks[8] == 2)
		return true;
	else
		return false;
}

// This draws the grid after each move  

void SMyTicTacToe::DrawTicTacToe() // *** player/cpu logic changed -Dan ***
{

	cout << "\n";
	cout << "  1| 2| 3  \n";
	cout << " --------  \n";
	cout << "  4| 5| 6  \n";
	cout << " --------  \n";
	cout << "  7| 8| 9  \n\n\n";
	cout << " O is for the Computer Opponent.\n";
	cout << " X is for the Human Opponent.\n\n\n";
	cout << "\n Here's where we stand puny human : \n\n";

	for (int i = 0; i <= 8; i++)
	{
		if ((i + 1) % 3 != 0)
		{
			if (TicTacToeMarks[i] == O)
				cout << " O|";
			else if (TicTacToeMarks[i] == X)
				cout << " X|";
			else if (TicTacToeMarks[i] == E)
				cout << "  |";
		}
		else
		{
			if (TicTacToeMarks[i] == O)
				cout << " O\n";
			else if (TicTacToeMarks[i] == X)
				cout << " X\n";
			else if (TicTacToeMarks[i] == E)
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
		DrawTicTacToe();
		humanMove();  // Human plays first here..
		if (IsFormed)
			break;
		if (NumberOfMovesPlayed != 9)
			computerMove();
		if (IsFormed)
			break;
		system("cls");
	}
	if (!IsFormed)
		cout << "\n Nobody wins..How can i win if i don't play first ?\n";
}

bool SMyTicTacToe::PlayAgain()
{
	char PlayAgain = 0;
	while (PlayAgain != 'y' && PlayAgain != 'n')
	{
		cout << "\n Another try?! Pretty please!? (y|n)\n";
		cin >> PlayAgain;
	}
	cout << "\n";

	return PlayAgain == 'y';
}


SMyTicTacToe *TicTacToe = new SMyTicTacToe;

int main()
{
	cout << " Shall we play a game?\n\n";
	cout << " I'll even let you go first!\n";
	cout << " Select a location on the board by entering its number (1 - 9).\n";
	cout << "  1| 2| 3  \n";
	cout << " --------  \n";
	cout << "  4| 5| 6  \n";
	cout << " --------  \n";
	cout << "  7| 8| 9  \n\n\n";
	cout << " O is for the Computer Opponent.\n";
	cout << " X is for the Human Opponent.\n\n\n";
	cout << " Good luck! You're gonna need it!! :) \n";


	while (1)
	{
		TicTacToe = new SMyTicTacToe();
		//TicTacToe->VerifyFirstPlayer();
		TicTacToe->TicTacToeLoop();

		if (!TicTacToe->PlayAgain())
			break;
	}

	return 0;
}