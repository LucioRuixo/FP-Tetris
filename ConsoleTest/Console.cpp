#include "Console.h"
#include <time.h>

// --------------------------------------------------------------
// Consola de Windows
char screen[SCREEN_HEIGHT * SCREEN_WIDTH];
WORD screenColor[SCREEN_HEIGHT * SCREEN_WIDTH];
HANDLE console;

char auxScreen[SCREEN_HEIGHT * SCREEN_WIDTH];

GameState state = GameState::Playing;

// --------------------------------------------------------------
// Funciones Tetris

void MainMenu(GameState &state)
{
	bool spacebarPressed = false;

	while (!spacebarPressed)
	{
		std::string str = "TETRIS";
		WriteText((SCREEN_WIDTH - 6) / 2, SCREEN_HEIGHT / 2 - 5, str, FOREGROUND_RED | FOREGROUND_INTENSITY);

		str = "Press the spacebar to play!";
		WriteText((SCREEN_WIDTH - 25) / 2, SCREEN_HEIGHT / 2 + 1, str, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		SwapBuffers();

		if (KEY_DOWN(VK_SPACE))
		{
			spacebarPressed = true;

			state = GameState::Playing;
		}
	}
}

void InitializeBoard(Board &board)
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			//Definir posición
			board.space[y][x].pos.y = (SCREEN_HEIGHT - BOARD_HEIGHT) / 2 + y;
			board.space[y][x].pos.x = (SCREEN_WIDTH - BOARD_WIDTH) / 2 + x;

			//Definir color
			board.space[y][x].color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

			//Definir caracteres
			if (y == BOARD_HEIGHT - 1 || x == 0 || x == BOARD_WIDTH - 1)
			{
				board.space[y][x].ch = '#';
			}
			else
				board.space[y][x].ch = '\0';
		}
	}
}

void InitializePiece(Board board, Piece &piece, int randomNumber, int randomColor)
{
	//Definir tipo de pieza
	switch (randomNumber)
	{
		case 0:
			piece.type = Square;
			break;
		case 1:
			piece.type = T;
			break;
		case 2:
			piece.type = L;
			break;
		case 3:
			piece.type = InvertedL;
			break;
		case 4:
			piece.type = S;
			break;
		case 5:
			piece.type = InvertedS;
			break;
		case 6:
			piece.type = Stick;
			break;
	}

	for (int y = 0; y < PIECE_HEIGHT; y++)
	{
		for (int x = 0; x < PIECE_WIDTH; x++)
		{
			//Definir posición
			piece.space[y][x].pos.y = (SCREEN_HEIGHT - BOARD_HEIGHT) / 2 + y;
			piece.space[y][x].pos.x = (SCREEN_WIDTH - BOARD_WIDTH) / 2 + (BOARD_WIDTH - PIECE_WIDTH) / 2 + x;

			//Definir color
			switch (randomColor)
			{
			case 0:
				piece.space[y][x].color = FOREGROUND_RED | FOREGROUND_INTENSITY;
				break;
			case 1:
				piece.space[y][x].color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;
			case 2:
				piece.space[y][x].color = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				break;
			}

			//Definir caracteres
			switch (piece.type)
			{
			case PieceType::Square:
				if (y == 0 || y == 1)
					piece.space[y][x].ch = '\0';
				else
					if (x == 1 || x == 2)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';

				break;

			case PieceType::T:
				switch (y)
				{
				case 0:
				case 1:
					piece.space[y][x].ch = '\0';
					break;
				case 2:
					if (x == 0)
						piece.space[y][x].ch = '\0';
					else
						piece.space[y][x].ch = 'H';
					break;
				case 3:
					if (x == 2)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';
					break;
				}

				break;

			case PieceType::L:
				switch (y)
				{
				case 0:
					piece.space[y][x].ch = '\0';
					break;
				case 1:
				case 2:
					if (x == 1)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';
					break;
				case 3:
					if (x == 1 || x == 2)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';
					break;
				}

				break;

			case PieceType::InvertedL:
				switch (y)
				{
				case 0:
					piece.space[y][x].ch = '\0';
					break;
				case 1:
				case 2:
					if (x == 2)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';
					break;
				case 3:
					if (x == 1 || x == 2)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';
					break;
				}

				break;

			case PieceType::S:
				switch (y)
				{
				case 0:
					piece.space[y][x].ch = '\0';
					break;
				case 1:
					if (x == 1)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';
					break;
				case 2:
					if (x == 1 || x == 2)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';
					break;
				case 3:
					if (x == 2)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';
					break;
				}

				break;

			case PieceType::InvertedS:
				switch (y)
				{
				case 0:
					piece.space[y][x].ch = '\0';
					break;
				case 1:
					if (x == 2)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';
					break;
				case 2:
					if (x == 1 || x == 2)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';
					break;
				case 3:
					if (x == 1)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';
					break;
				}

				break;

			case PieceType::Stick:
				switch (y)
				{
				case 0:
				case 1:
				case 2:
				case 3:
					if (x == 1)
						piece.space[y][x].ch = 'H';
					else
						piece.space[y][x].ch = '\0';
					break;
				}

				break;
			}
		}
	}
}

void DrawPieceOnBoard(Board &board, Piece piece, int &tickCount, bool &lineDeletionActive)
{
	int boardY;
	int boardX;

	for (int y = 0; y < PIECE_HEIGHT; y++)
	{
		for (int x = 0; x < PIECE_WIDTH; x++)
		{
			boardY = piece.space[y][x].pos.y - (SCREEN_HEIGHT - BOARD_HEIGHT) / 2;
			boardX = piece.space[y][x].pos.x - (SCREEN_WIDTH - BOARD_WIDTH) / 2;

			if (piece.space[y][x].ch == 'H')
			{
				board.space[boardY][boardX].ch = piece.space[y][x].ch;
				board.space[boardY][boardX].color = piece.space[y][x].color;
			}
		}
	}

	DetectLine(board, piece, tickCount, lineDeletionActive);
}

void DetectLine(Board &board, Piece piece, int &tickCount, bool &lineDeletionActive)
{
	int boardY;

	for (int y = PIECE_HEIGHT - 1; y >= 0 ; y--)
	{
		bool lineFound = false;

		boardY = piece.space[y][0].pos.y - (SCREEN_HEIGHT - BOARD_HEIGHT) / 2;

		for (int x = 1; x < BOARD_WIDTH - 1; x++)
		{
			if (board.space[boardY][x].ch == 'H')
			{
				if (x == BOARD_WIDTH - 2)
					lineFound = true;
			}
			else
				break;
		}

		if (lineFound)
		{
			for (int x = 1; x < BOARD_WIDTH - 1; x++)
			{
				board.space[boardY][x].ch = '=';
			}

			lineDeletionActive = true;
		}
	}
}

void Input(Board &board, Piece &piece, int &tickCount, bool &holdRotate, bool &lineDeletionActive, bool &resetPiece)
{
	if (KEY_DOWN(VK_LEFT))
		MovePieceLeft(board, piece);

	if (KEY_DOWN(VK_RIGHT))
		MovePieceRight(board, piece);

	if (KEY_DOWN(VK_DOWN))
		MovePieceDown(board, piece, tickCount, lineDeletionActive, resetPiece);

	if (KEY_DOWN(VK_SPACE))
	{
		if (!holdRotate)
		{
			RotatePiece(piece);
			if (!DoesPieceFit(board, piece))
			{
				RotatePiece(piece);
				RotatePiece(piece);
				RotatePiece(piece);
			}	
		}

		holdRotate = true;
	}
	else
		holdRotate = false;
}

void MovePieceLeft(Board board, Piece &piece)
{
	{
		for (int y = 0; y < PIECE_HEIGHT; y++)
		{
			for (int x = 0; x < PIECE_WIDTH; x++)
			{
				piece.space[y][x].pos.x--;
			}
		}

		if (!DoesPieceFit(board, piece))
		{
			for (int y = 0; y < PIECE_HEIGHT; y++)
			{
				for (int x = 0; x < PIECE_WIDTH; x++)
				{
					piece.space[y][x].pos.x++;
				}
			}
		}
	}
}

void MovePieceRight(Board board, Piece &piece)
{
	{
		for (int y = 0; y < PIECE_HEIGHT; y++)
		{
			for (int x = 0; x < PIECE_WIDTH; x++)
			{
				piece.space[y][x].pos.x++;
			}
		}

		if (!DoesPieceFit(board, piece))
		{
			for (int y = 0; y < PIECE_HEIGHT; y++)
			{
				for (int x = 0; x < PIECE_WIDTH; x++)
				{
					piece.space[y][x].pos.x--;
				}
			}
		}
	}
}

void MovePieceDown(Board &board, Piece &piece, int &tickCount, bool &lineDeletionActive, bool &resetPiece)
{
	for (int y = 0; y < PIECE_HEIGHT; y++)
	{
		for (int x = 0; x < PIECE_WIDTH; x++)
		{
			piece.space[y][x].pos.y++;
		}
	}

	if (!DoesPieceFit(board, piece))
	{
		for (int y = 0; y < PIECE_HEIGHT; y++)
		{
			for (int x = 0; x < PIECE_WIDTH; x++)
			{
				piece.space[y][x].pos.y--;
			}
		}

		DrawPieceOnBoard(board, piece, tickCount, lineDeletionActive);

		resetPiece = true;
		tickCount = 0;
	}
}

void RotatePiece(Piece &piece)
{
	int maxCycles = PIECE_WIDTH / 2;
	int maxCellNumber = PIECE_WIDTH - 1;

	int minN = 0;
	int maxN = 3;
	int n1 = 0;
	int n2 = maxCellNumber;

	char aux1;
	char aux2;

	for (int cycleCount = 1; cycleCount <= maxCycles; cycleCount++)
	{
		for (int cellNumber = 0; cellNumber < maxCellNumber; cellNumber++)
		{
			aux1 = piece.space[n1][maxN].ch;
			piece.space[n1][maxN].ch = piece.space[minN][n1].ch;

			aux2 = piece.space[maxN][n2].ch;
			piece.space[maxN][n2].ch = aux1;

			aux1 = piece.space[n2][minN].ch;
			piece.space[n2][minN].ch = aux2;

			piece.space[minN][n1].ch = aux1;

			n1++;
			n2--;
		}

		minN++;
		maxN--;

		maxCellNumber -= 2;

		n1 = cycleCount;
		n2 = maxCellNumber + 1;
	}
}

void LevelUp(int &speed, int &level, int &currentClearedLines)
{
	speed--;

	level++;

	currentClearedLines = 0;
}

void AssignBoardInScreen(Board board)
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			SetChar(board.space[y][x].pos.x, board.space[y][x].pos.y, board.space[y][x].ch);
			SetColor(board.space[y][x].pos.x, board.space[y][x].pos.y, board.space[y][x].color);
		}
	}
}

void AssignPieceInScreen(Piece piece)
{
	for (int y = 0; y < PIECE_HEIGHT; y++)
	{
		for (int x = 0; x < PIECE_WIDTH; x++)
		{
			if (piece.space[y][x].ch == 'H')
			{
				SetChar(piece.space[y][x].pos.x, piece.space[y][x].pos.y, piece.space[y][x].ch);
				SetColor(piece.space[y][x].pos.x, piece.space[y][x].pos.y, piece.space[y][x].color);
			}
		}
	}
}

void HUD(Board board, int score, int level, int totalClearedLines)
{
	std::string str = "Score - " + std::to_string(score);
	WriteText((SCREEN_WIDTH - BOARD_WIDTH) / 2 + 1, board.space[0][0].pos.y - 4, str, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	str = "Level - " + std::to_string(level);
	WriteText((SCREEN_WIDTH - BOARD_WIDTH) / 2 + 1, board.space[0][0].pos.y - 3, str, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	str = "Lines - " + std::to_string(totalClearedLines);
	WriteText((SCREEN_WIDTH - BOARD_WIDTH) / 2 + 1, board.space[0][0].pos.y - 2, str, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}


int DeleteLine(Board &board)
{
	int lineCount = 0;
	int totalLines = 0;

	bool lineFound = false;

	for (int y = BOARD_HEIGHT - 2; y >= 0; y--)
	{
		for (int x = 1; x < BOARD_WIDTH - 1; x++)
		{
			if (board.space[y][x].ch == '=')
			{
				lineFound = true;
				break;
			}
		}

		if (lineFound)
		{
			lineCount++;
			lineFound = false;
			totalLines++;
		}
		else
		{
			if (lineCount > 0)
			{
				for (int cellY = y; cellY >= 0; cellY--)
				{
					for (int x = 1; x < BOARD_WIDTH - 1; x++)
					{
						board.space[cellY + lineCount][x].ch = board.space[cellY][x].ch;
					}
				}

				for (int cellY = 0; cellY < lineCount; cellY++)
				{
					for (int x = 1; x < BOARD_WIDTH - 1; x++)
					{
						board.space[cellY][x].ch = '\0';
					}
				}

				lineCount = 0;
				y = BOARD_HEIGHT - 2;
			}
		}

	}

	return totalLines;
}


bool DoesPieceFit(Board board, Piece piece)
{
	int boardY;
	int boardX;

	for (int y = 0; y < PIECE_HEIGHT; y++)
	{
		for (int x = 0; x < PIECE_WIDTH; x++)
		{
			boardY = piece.space[y][x].pos.y - (SCREEN_HEIGHT - BOARD_HEIGHT) / 2;
			boardX = piece.space[y][x].pos.x - (SCREEN_WIDTH - BOARD_WIDTH) / 2;

			if (piece.space[y][x].ch == 'H' && board.space[boardY][boardX].ch != '\0')
				return false;
		}
	}

	return true;
}

// --------------------------------------------------------------
// Definicion de funciones
void SetChar(int x, int y, char ch)
{
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
		screen[y * SCREEN_WIDTH + x] = ch;
}

void SetColor(int x, int y, WORD color)
{
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
		screenColor[y * SCREEN_WIDTH + x] = color;
}

char GetChar(int x, int y)
{
	char ch = '\0';

	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
		ch = screen[y * SCREEN_WIDTH + x];

	return ch;
}

void SwapBuffers()
{
	DWORD bytesWritten = 0;
	WriteConsoleOutputCharacter(console, screen, SCREEN_WIDTH * SCREEN_HEIGHT, { 0,0 }, &bytesWritten);
	WriteConsoleOutputAttribute(console, screenColor, SCREEN_WIDTH * SCREEN_HEIGHT, { 0,0 }, &bytesWritten);
}

void Initialize()
{
	console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
	Clear();
}

void Clear()
{
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
	{
		auxScreen[i] = screen[i];
		screen[i] = '\0';
		screenColor[i] = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	}
}

void WriteText(int x, int y, std::string str, WORD color)
{
	for (int i = 0; i < str.size(); i++)
	{
		SetChar(x + i, y, str[i]);
		SetColor(x + i, y, color);
	}
}