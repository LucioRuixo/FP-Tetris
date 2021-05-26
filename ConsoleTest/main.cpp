#include "Console.h"
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

// --------------------------------------------------------------
// Funcion Main
int main()
{
	srand(time(0));

	GameState state = GameState::NotPlaying;

	//Inicializar
	Initialize();

	while (true)
	{
		MainMenu(state);

		Board board;
		InitializeBoard(board);

		Piece piece;
		int randomNumber = rand() % 7;
		int randomColor = rand() % 3;
		InitializePiece(board, piece, randomNumber, randomColor);

		//Variables de timing
		int tickCount = 0;
		int speed = 20;
		float dt;
		double oldTimer = clock();
		double timer;

		int clearedLines;
		int currentClearedLines = 0;
		int totalClearedLines = 0;
		int level = 0;
		int score = 0;

		bool holdRotate = false;
		bool lineDeletionActive = false;
		bool resetPiece = false;
		bool getRandom = true;

		while (state == GameState::Playing)
		{
			Clear();

			//Timing
			do
			{
				timer = clock();
				dt = (float)((timer - oldTimer));
			} while (dt < TICK_DURATION);

			oldTimer = timer;

			tickCount++;
			if (tickCount == speed)
			{
				tickCount = 0;

				MovePieceDown(board, piece, tickCount, lineDeletionActive, resetPiece);
			}

			//Input
			if (tickCount % 2 == 0)
				Input(board, piece, tickCount, holdRotate, lineDeletionActive, resetPiece);

			//Lógica & cálculos
			if (getRandom)
			{
				randomNumber = rand() % 7;
				randomColor = rand() % 3;
				getRandom = false;
			}

			if (resetPiece && tickCount == speed / 2)
			{
				InitializePiece(board, piece, randomNumber, randomColor);
				if (!DoesPieceFit(board, piece))
				{
					Clear();

					std::string str = "GAME OVER";
					WriteText((SCREEN_WIDTH - 9) / 2, (SCREEN_HEIGHT - 1) / 2, str, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
					SwapBuffers();

					Sleep(2000);

					state = GameState::NotPlaying;
				}

				getRandom = true;
				resetPiece = false;

				if (lineDeletionActive)
				{
					switch (clearedLines = DeleteLine(board))
					{
					case 1:
						score += 40 * (level + 1);
						break;
					case 2:
						score += 100 * (level + 1);
						break;
					case 3:
						score += 300 * (level + 1);
						break;
					case 4:
						score += 1200 * (level + 1);
						break;
					}

					currentClearedLines += clearedLines;
					if (currentClearedLines == 10)
						LevelUp(speed, level, currentClearedLines);

					totalClearedLines += clearedLines;

					lineDeletionActive = false;
				}
			}

			//Output
			AssignBoardInScreen(board);
			AssignPieceInScreen(piece);

			HUD(board, score, level, totalClearedLines);

			SwapBuffers();
		}

		Clear();
	}

	return 0;
}