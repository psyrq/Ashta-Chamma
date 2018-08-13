#pragma once
#include "BaseEngine.h"
#include "TileManager.h"
#include "MyTileManager.h"
#include "PlayRules.h"

#include <vector>

#define PLAYER1 0
#define PLAYER2 1
#define PLAYER3 2
#define PLAYER4 3

#define LAND_SAFE_BOX 8
#define PUT_NEW_PIECE 10
#define KILLING_MOVEMENT 15

#define PVP_MODE 0
#define PVE_SIMPLE_MODE 1
#define PVE_HARDER_MODE 2

struct tokens
{
	int step;	//record current step of this token
	int forward;	//record whether the token should travel forward or backward
	int finished;	//record whether this token has been finished
	int out;	//indicate whether the token can be placed into the board
};

struct AIMovingChoice
{
	int mark;
	int movingPiece;
	int result;
	bool newPiece;
};

class MyEngine : public BaseEngine
{
public:
	MyEngine();
	~MyEngine();
	void SetupBackgroundBuffer();

	void DrawStrings();
	void InitBoard();
	void DrawGuidence();

	void KeyDown(int iKeyCode);
	void MouseDown(int iButton, int iX, int iY);

	void ResetSettings();
	void ResetSettings(int player, int movingPiece);

	void PutANewTokenInBoard(int player);
	void MovePiece(int player, int result, int movingPiece);
	void DisplayPieces();

	void DeterminKilling(int player1, int movingPiece);
	bool ComparePositions(int location1[2], int location2[2]);

	void TileOrderForPlayer(int startPositionX, int startPositionY);
	void SimpleAI();
	void HigherLevelAI();
	bool KilledRisk(int movingPiece, int player);
	bool LandSafeBox(int result, int movingPiece, int player);

	int SelectPiece(int player, int iX, int iY);

public:
	enum State { stateInit, stateSelect, stateMain, statePaused, stateEnd };

private:
	int steps[2];
	int out[2];
	int ifForward[2];
	int tokenKill[2];

	int result = 0;
	int chessColour = 0;
	int totalTurn = 0;
	int whoseTurn = 0;
	int roll = 0;

	int movingPiece = 0;
	int finishedPiecesPlayer1 = 0;
	int finishedPiecesPlayer2 = 0;
	int winner = -1;

	int modeSelection = -1;
	int selectorStartPositionX = 150;
	int selectorStartPositionY = 250;

	int moveOrderPlayer1[26][2];
	int moveOrderPlayer2[26][2];

	float startPositionX;
	float startPositionY;
	float midPositionX;
	float midPositionY;

	MyTileManager m_oTiles;
	PlayRules m_playRules;

	State m_state;

	//tokens tokenPlayer1[3], tokenPlayer2[3];
	vector<tokens> tokenPlayer1, tokenPlayer2;
	vector<AIMovingChoice> AIMovementMark;
};

bool CompareMarks(AIMovingChoice choice1, AIMovingChoice choice2);
