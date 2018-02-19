#pragma once
#include "BaseEngine.h"
#include "TileManager.h"
#include "MyTileManager.h"
#include "PlayRules.h"

#define PLAYER1 0
#define PLAYER2 1
#define PLAYER3 2
#define PLAYER4 3

class MyEngine : public BaseEngine
{
public:
	MyEngine();
	~MyEngine();
	void SetupBackgroundBuffer();

	void DrawStrings();
	void KeyDown(int iKeyCode);
	void MouseDown(int iButton, int iX, int iY);
	void InitBoard();
	void ResetSettings();
	void ResetSettings(int player);
	void MovePiece(int player, int result);
	void DisplayPieces();
	void DeterminKilling(int player1, int location1[2], int player2, int location2[2]);
	bool ComparePositions(int location1[2], int location2[2]);
	void TileOrderForPlayer(int startPositionX, int startPositionY);

public:
	enum State { stateInit, stateMain, statePaused, stateEnd };

private:
	int numTurn[4];
	int steps[4];
	int out[4];
	int ifForward[4];
	int tokenKill[4];

	int result = 0;
	int chessColour = 0;
	int totalTurn = 0;
	int whoseTurn = 0;

	int boxesPosition[4][2];
	int moveOrderPlayer1[26][2];
	int moveOrderPlayer2[26][2];
	int moveOrderPlayer3[26][2];
	int moveOrderPlayer4[26][2];

	float startPositionX;
	float startPositionY;
	float midPositionX;
	float midPositionY;

	MyTileManager m_oTiles;
	PlayRules m_playRules;

	State m_state;
};

