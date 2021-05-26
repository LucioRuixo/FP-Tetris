#pragma once
#include <Windows.h>
#include <string>

// --------------------------------------------------------------
// Chequea por teclas

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) 

// --------------------------------------------------------------
// Propiedades de la consola de Windows

const int SCREEN_HEIGHT = 30;
const int SCREEN_WIDTH = 120;

// --------------------------------------------------------------
// Codigos de Teclas 

const int UP_ARROW = 72;
const int DOWN_ARROW = 80;
const int LEFT_ARROW = 75;
const int RIGHT_ARROW = 77;
const int SPACE_BAR = 0x20;

// --------------------------------------------------------------
// Consola de Windows

extern char screen[SCREEN_HEIGHT * SCREEN_WIDTH];
extern WORD screenColor[SCREEN_HEIGHT * SCREEN_WIDTH];
extern HANDLE console;

// --------------------------------------------------------------
// Constantes
const int BOARD_HEIGHT = 18;
const int BOARD_WIDTH = 12;
const int PIECE_HEIGHT = 4;
const int PIECE_WIDTH = 4;

const float TICK_DURATION = 37.5f;

// --------------------------------------------------------------
// Estructuras

enum GameState
{
	Playing,
	NotPlaying
};

enum PieceType
{
	Square,
	T,
	L,
	InvertedL,
	S,
	InvertedS,
	Stick
};

struct Position
{
	int x;
	int y;
};

struct SpaceMember
{
	Position pos;

	DWORD color;

	char ch;
};

struct Board
{
	SpaceMember space[BOARD_HEIGHT][BOARD_WIDTH];
};

struct Piece
{
	SpaceMember space[PIECE_HEIGHT][PIECE_WIDTH];

	PieceType type;
};

extern GameState state;

// --------------------------------------------------------------
// Funciones

void Initialize();
void Clear();
void SetChar(int x, int y, char ch);
void SetColor(int x, int y, WORD color);
void SwapBuffers();
void WriteText(int x, int y, std::string str, WORD color);

void MainMenu(GameState &state);
void InitializeBoard(Board &board);
void InitializePiece(Board board, Piece &piece, int randomNumber, int randomColor);
void DrawPieceOnBoard(Board &board, Piece piece, int &tickCount, bool &lineDeletionActive);
void DetectLine(Board &board, Piece piece, int &tickCount, bool &lineDeletionActive);
void Input(Board &board, Piece &piece, int &tickCount, bool &holdRotate, bool &lineDeletionActive, bool &resetPiece);
void MovePieceLeft(Board board, Piece &piece);
void MovePieceRight(Board board, Piece &piece);
void MovePieceDown(Board &board, Piece &piece, int &tickCount, bool &lineDeletionActive, bool &resetPiece);
void RotatePiece(Piece &piece);
void LevelUp(int &speed, int &level, int &currentClearedLines);
void AssignBoardInScreen(Board board);
void AssignPieceInScreen(Piece piece);
void HUD(Board board, int score, int level, int totalClearedLines);

int DeleteLine(Board &board);

bool DoesPieceFit(Board board, Piece piece);