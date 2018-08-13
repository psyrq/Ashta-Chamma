#include "header.h"
#include "templates.h"
#include "BaseEngine.h"
#include "TileManager.h"
#include "MyEngine.h"
#include "PlayRules.h"
#include "windows.h"
#include "time.h"

#include <algorithm>

using namespace std;

MyEngine::MyEngine() :BaseEngine(), m_state(stateInit)
{
	ResetSettings();
}


MyEngine::~MyEngine()
{
}

//set the basic interface
void MyEngine::SetupBackgroundBuffer()
{
	switch (m_state)
	{
	case MyEngine::stateInit:

		for (int x = 0; x < GetScreenWidth(); x++) {
			for (int y = 0; y < GetScreenHeight(); y++) {
				SetBackgroundPixel(x, y, 0xffffff);
			}
		}
		break;

	case MyEngine::stateSelect:

		for (int x = 0; x < GetScreenWidth(); x++) {
			for (int y = 0; y < GetScreenHeight(); y++) {
				SetBackgroundPixel(x, y, 0xffffff);
			}
		}
		break;

	case MyEngine::stateMain:

		for (int x = 0; x < GetScreenWidth(); x++) {
			for (int y = 0; y < GetScreenHeight(); y++) {
				SetBackgroundPixel(x, y, 0xffffff);
			}
		}

		m_oTiles.SetSize(50, 50);
		InitBoard();

		break;

	case MyEngine::statePaused:
		break;
	case MyEngine::stateEnd:
		InitBoard();
		break;
	}
}


void MyEngine::DrawStrings()
{
	switch (m_state)
	{
	case stateInit:
		DrawScreenString(300, 200, "Ashta Chamma", 0x000000, NULL);
		DrawScreenString(285, 250, "The Board Game", 0x000000, NULL);
		DrawScreenString(250, 400, "Press Space to start", 0x000000, NULL);
		break;	//draw strings at init stage

	case stateSelect:
		DrawScreenString(100, 150, "Select a mode from the following then start game", 0x000000, NULL);
		DrawScreenString(200, 250, "player vs player", 0x000000, NULL);
		DrawScreenString(200, 300, "player vs simple AI", 0x000000, NULL);
		DrawScreenString(200, 350, "player vs harder AI", 0x000000, NULL);
		DrawScreenString(200, 450, "Use Space to select", 0x000000, NULL);

		BaseEngine::DrawTriangle(selectorStartPositionX,
			selectorStartPositionY,
			selectorStartPositionX,
			selectorStartPositionY + 30,
			selectorStartPositionX + 15,
			selectorStartPositionY + 15,
			0x000000);

		break;	//draw strings at select stage

	case stateMain:
		char score[128];
		char player[128];
		if (roll == 0) {
			BaseEngine::DrawRectangle(600, 50, 650, 80, 0x7fffaa);
			DrawScreenString(600, 50, "roll", 0x000000, NULL);
		}
		DrawScreenString(600, 100, "result : ", 0x000000, NULL);

		if (totalTurn % 2 == 0) {
			DrawScreenString(300, 75, "Player1's turn", 0x000000, NULL);
		}
		else if (totalTurn % 2 == 1) {
			DrawScreenString(300, 75, "Player2's turn", 0x000000, NULL);
		}

		if (totalTurn >= 0) {
			sprintf(score, "%d", result);
			DrawScreenString(720, 100, score, 0x000000, NULL);
		}

		if (whoseTurn == PLAYER1) {

			if (roll == 1 && result == 6 && tokenPlayer1.size() < 3) {
				if (tokenPlayer1.size() == 0)
					out[whoseTurn] = 1;
				BaseEngine::DrawRectangle(450, 500, 670, 530, 0x7fffaa);
				DrawScreenString(450, 500, "place a new piece", 0x000000, NULL);
			}
		}
		else if (whoseTurn == PLAYER2) {

			if (roll == 1 && result == 6 && tokenPlayer2.size() < 3) {
				if (tokenPlayer2.size() == 0)
					out[whoseTurn] = 1;
				BaseEngine::DrawRectangle(450, 500, 670, 530, 0x7fffaa);
				DrawScreenString(450, 500, "place a new piece", 0x000000, NULL);
			}
		}
		break;	//draw strings at main stage

	case statePaused:
		DrawScreenString(200, 100, "Paused. Press Space to restart", 0x000000, NULL);
		break;	//draw strings at paused stage

	case stateEnd:

		char winnerStatement[128];
		sprintf(winnerStatement, "winner is player %d", winner + 1);
		DrawScreenString(300, 75, winnerStatement, 0x000000, NULL);
		DrawScreenString(225, 500, "Press R to start a new game", 0x000000, NULL);
		break;
	}	//draw strings at end stage
}


void MyEngine::KeyDown(int iKeyCode)
{
	switch (iKeyCode)
	{
	case SDLK_ESCAPE:
		ResetSettings();
		SetExitWithCode(0);
		break;

	case SDLK_UP:
		switch (m_state)
		{
		case stateSelect:
			if (selectorStartPositionY > 250) {
				selectorStartPositionY -= 50;
				Redraw(true);
			}
			break;
		default:
			break;
		}
		break;

	case SDLK_DOWN:
		switch (m_state)
		{
		case stateSelect:
			if (selectorStartPositionY < 350) {
				selectorStartPositionY += 50;
				Redraw(true);
			}
			break;
		default:
			break;
		}
		break;

	case SDLK_SPACE:
		switch (m_state)
		{
		case stateInit:
			m_state = stateSelect;
			SetupBackgroundBuffer();
			Redraw(true);
			break;

		case stateSelect:
			if (selectorStartPositionY == 250) {
				modeSelection = PVP_MODE;
				printf("enter pvp mode\n");
			}
			else if (selectorStartPositionY == 300) {
				modeSelection = PVE_SIMPLE_MODE;
				printf("enter pve simple mode\n");
			}
			else {
				modeSelection = PVE_HARDER_MODE;
				printf("enter pve harder mode\n");
			}

			m_state = stateMain;
			SetupBackgroundBuffer();
			Redraw(true);

			break;

		case stateMain:
			m_state = statePaused;
			//m_iPauseStarted = GetTime();
			SetupBackgroundBuffer();
			Redraw(true);
			break;

		case statePaused:
			m_state = stateMain;
			//IncreaseTimeOffset(m_iPauseStarted - GetTime());
			SetupBackgroundBuffer();
			DisplayPieces();
			Redraw(true);
			break;

		case stateEnd:
			break;

		default:
			break;
		}
		break;

	case 'r':
		switch (m_state)
		{
		case stateInit:
			break;
		case statePaused:
			ResetSettings();
			SetupBackgroundBuffer();
			Redraw(true);
		case stateEnd:
			ResetSettings();
			SetupBackgroundBuffer();
			Redraw(true);
		case stateMain:
			ResetSettings();
			SetupBackgroundBuffer();
			Redraw(true);
		default:
			break;
		}
		break;
	}
}


void MyEngine::MouseDown(int iButton, int iX, int iY)
{
	switch (m_state)
	{
	case stateInit:
		m_state = stateSelect;
		break;
	case stateMain:
		whoseTurn = ((totalTurn % 2) == 0) ? PLAYER1 : PLAYER2;

		if (modeSelection == PVP_MODE) {

			if (roll == 0 && iX < 650 && iX > 600 && iY < 80 && iY > 50) {

				result = m_playRules.RandomAshtaChamma();
				//whoseTurn = ((totalTurn % 2) == 0) ? PLAYER1 : PLAYER2;
				printf("result = %d\n", result);

				if (whoseTurn == PLAYER1) {
					if (tokenPlayer1.size() == 0 && result != 6) {
						totalTurn++;
					}
					if (tokenPlayer1.size() != 0 || result == 6) {
						roll = 1;
					}
				}
				else if (whoseTurn == PLAYER2) {
					if (tokenPlayer2.size() == 0 && result != 6) {
						totalTurn++;
					}
					if (tokenPlayer2.size() != 0 || result == 6) {
						roll = 1;
					}
				}

				DisplayPieces();
			}
			else if (roll == 1 && result == 6 && iX < 670 && iX > 450 && iY < 530 && iY > 500) {
				if (whoseTurn == PLAYER1) {
					if (tokenPlayer1.size() < 3) {
						PutANewTokenInBoard(whoseTurn);
					}
				}
				else if (whoseTurn == PLAYER2) {
					if (tokenPlayer2.size() < 3) {
						PutANewTokenInBoard(whoseTurn);
					}
				}
				//PutANewTokenInBoard(whoseTurn);
				totalTurn++;
				roll = 0;
			}
			else {
				movingPiece = SelectPiece(whoseTurn, iX, iY);
				if (movingPiece >= 0) {
					MovePiece(whoseTurn, result, movingPiece);
					printf("the moving piece is: %d\n", movingPiece);
					totalTurn++;
					roll = 0;
				}
			}
		}

		else {
			if (whoseTurn == PLAYER1) {

				if (roll == 0 && iX < 650 && iX > 600 && iY < 80 && iY > 50) {

					result = m_playRules.RandomAshtaChamma();
					printf("result = %d\n", result);
					if (tokenPlayer1.size() == 0 && result != 6) {
						totalTurn++;
					}
					if (tokenPlayer1.size() != 0 || result == 6) {
						roll = 1;
					}
					DisplayPieces();
				}
				else if (roll == 1 && result == 6 && iX < 670 && iX > 450 && iY < 530 && iY > 500) {
					if (tokenPlayer1.size() < 3) {
						PutANewTokenInBoard(whoseTurn);
						totalTurn++;
						roll = 0;
					}
				}
				else {
					movingPiece = SelectPiece(whoseTurn, iX, iY);
					if (movingPiece >= 0) {
						MovePiece(whoseTurn, result, movingPiece);
						printf("the moving piece is: %d\n", movingPiece);
						totalTurn++;
						roll = 0;
					}
				}
			}

			else if (whoseTurn == PLAYER2) {
				if (modeSelection == PVE_SIMPLE_MODE)
					SimpleAI();
				else if (modeSelection == PVE_HARDER_MODE)
					HigherLevelAI();
			}
		}
		break;
	case statePaused:
		break;
	case stateEnd:
		break;
	}
}

int MyEngine::SelectPiece(int player, int iX, int iY)
{
	int result = 0;
	int selectedPiece = -1;

	if (player == PLAYER1) {

		for (int i = 0; i < 26; i++) {
			if (iX > moveOrderPlayer1[i][0] && iX < moveOrderPlayer1[i][0] + m_oTiles.GetTileWidth() && iY > moveOrderPlayer1[i][1] && iY < moveOrderPlayer1[i][1] + m_oTiles.getTileHeight()) {
				result = i+1;
				break;
			}
		}

		for (int i = 0; i < tokenPlayer1.size(); i++) {
			if (result == tokenPlayer1[i].step) {
				selectedPiece = i;
				break;
			}
		}
	}

	else if (player == PLAYER2) {

		for (int i = 0; i < 26; i++) {
			if (iX > moveOrderPlayer2[i][0] && iX < moveOrderPlayer2[i][0] + m_oTiles.GetTileWidth() && iY > moveOrderPlayer2[i][1] && iY < moveOrderPlayer2[i][1] + m_oTiles.getTileHeight()) {
				result = i+1;
				break;
			}
		}

		for (int i = 0; i < tokenPlayer2.size(); i++) {
			if (result == tokenPlayer2[i].step) {
				selectedPiece = i;
				break;
			}
		}
	}

	return selectedPiece;
}

void MyEngine::MovePiece(int player, int result, int movingPiece)
{
	if (result != 6 && out[player] == 0)
		return;
	else if (result == 6 && out[player] == 0) {
		return;
	}
	else {

		if (player == PLAYER1) {
			tokenPlayer1[movingPiece].forward = 1;
			for (int i = 0; i < result; i++) {

				if (tokenPlayer1[movingPiece].step == 17 && tokenKill[player] == 0) {
					tokenPlayer1[movingPiece].step = 1;
				}

				if (tokenPlayer1[movingPiece].forward == 1) {
					tokenPlayer1[movingPiece].step += 1;
				}
				else if (tokenPlayer1[movingPiece].forward == 0) {
					tokenPlayer1[movingPiece].step -= 1;
				}

				if (tokenPlayer1[movingPiece].step == 26) tokenPlayer1[movingPiece].forward = 0;
				DisplayPieces();
			}
			if (out[PLAYER1] == 1 && out[PLAYER2] == 1)
				DeterminKilling(PLAYER1, movingPiece);
			if (tokenPlayer1[movingPiece].step == 26) {
				tokenPlayer1[movingPiece].finished = 1;
				finishedPiecesPlayer1 += 1;
				printf("Player1 has finished one piece\n");
			}
			if (finishedPiecesPlayer1 == 3) {
				winner = PLAYER1;
				m_state = stateEnd;
				SetupBackgroundBuffer();
			}
		}
		else if (player == PLAYER2) {
			tokenPlayer2[movingPiece].forward = 1;
			for (int i = 0; i < result; i++) {

				if (tokenPlayer2[movingPiece].step == 17 && tokenKill[player] == 0) {
					tokenPlayer2[movingPiece].step = 1;
				}

				if (tokenPlayer2[movingPiece].forward == 1) {
					tokenPlayer2[movingPiece].step += 1;
				}
				else if (tokenPlayer2[movingPiece].forward == 0) {
					tokenPlayer2[movingPiece].step -= 1;
				}

				if (tokenPlayer2[movingPiece].step == 26) tokenPlayer2[movingPiece].forward = 0;
				DisplayPieces();
			}

			if (out[PLAYER1] == 1 && out[PLAYER2] == 1)
				DeterminKilling(PLAYER2, movingPiece);
			if (tokenPlayer2[movingPiece].step == 26) {
				tokenPlayer2[movingPiece].finished = 1;
				finishedPiecesPlayer2 += 1;
				printf("Player2 has finished one piece\n");
			}
			if (finishedPiecesPlayer2 == 3) {
				winner = PLAYER2;
				m_state = stateEnd;
				SetupBackgroundBuffer();
			}
		}
	}
}

void MyEngine::DisplayPieces()
{
	SetupBackgroundBuffer();
	if (out[PLAYER1] == 1) {
		for (int i = 0; i < tokenPlayer1.size(); i++) {
			if (tokenPlayer1[i].finished == 0)
				m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, moveOrderPlayer1[tokenPlayer1[i].step - 1][0], moveOrderPlayer1[tokenPlayer1[i].step - 1][1], 0x000000, 1);
		}
	}
	if (out[PLAYER2] == 1) {
		for (int i = 0; i < tokenPlayer2.size(); i++) {
			if (tokenPlayer2[i].finished == 0)
				m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, moveOrderPlayer2[tokenPlayer2[i].step - 1][0], moveOrderPlayer2[tokenPlayer2[i].step - 1][1], 0xffffff, 1);
		}
	}
	Redraw(true);
}

void MyEngine::PutANewTokenInBoard(int player)
{
	if (player == PLAYER1) {
		tokenPlayer1.push_back({ 1, 1, 0, 1 });
		printf("Player 1 has put a new piece into board.\n");
	}
	else if (player == PLAYER2) {
		//tokenPlayer2[tokenInBoardPlayer2] = { 1, 1, 0, 1 };
		tokenPlayer2.push_back({ 1, 1, 0, 1 });
		//tokenInBoardPlayer2++;
		printf("Player 2 has put a new piece into board.\n");
	}
	DisplayPieces();
}

void MyEngine::DeterminKilling(int curPlayer, int movingPiece)
{
	if (curPlayer == PLAYER1) {

		int curStep = tokenPlayer1[movingPiece].step;

		if (curStep != 4 && curStep != 8 && curStep != 12 && curStep != 16) {

			if (curStep >= 2 && curStep <= 9) {
				for (int i = 0; i < tokenPlayer2.size(); i++) {
					if (tokenPlayer2[i].step == (curStep + 8)) {
						ResetSettings(PLAYER2, i);
						break;
					}
				}
			}

			else if (curStep >= 10 && curStep <= 17) {
				for (int i = 0; i < tokenPlayer2.size(); i++) {
					if (tokenPlayer2[i].step == (curStep - 8)) {
						ResetSettings(PLAYER2, i);
						break;
					}
				}
			}

			else if (curStep >= 18 && curStep <= 21) {
				for (int i = 0; i < tokenPlayer2.size(); i++) {
					if (tokenPlayer2[i].step == (curStep + 4)) {
						ResetSettings(PLAYER2, i);
						break;
					}
				}
			}

			else if (curStep >= 22 && curStep <= 25) {
				for (int i = 0; i < tokenPlayer2.size(); i++) {
					if (tokenPlayer2[i].step == (curStep - 4)) {
						ResetSettings(PLAYER2, i);
						break;
					}
				}
			}
		}
	}

	else if (curPlayer == PLAYER2) {

		int curStep = tokenPlayer2[movingPiece].step;

		if (curStep != 4 && curStep != 8 && curStep != 12 && curStep != 16) {

			if (curStep >= 2 && curStep <= 9) {
				for (int i = 0; i < tokenPlayer1.size(); i++) {
					if (tokenPlayer1[i].step == (curStep + 8)) {
						ResetSettings(PLAYER1, i);
						break;
					}
				}
			}

			else if (curStep >= 10 && curStep <= 17) {
				for (int i = 0; i < tokenPlayer1.size(); i++) {
					if (tokenPlayer1[i].step == (curStep - 8)) {
						ResetSettings(PLAYER1, i);
						break;
					}
				}
			}

			else if (curStep >= 18 && curStep <= 21) {
				for (int i = 0; i < tokenPlayer1.size(); i++) {
					if (tokenPlayer1[i].step == (curStep + 4)) {
						ResetSettings(PLAYER1, i);
						break;
					}
				}
			}

			else if (curStep >= 22 && curStep <= 25) {
				for (int i = 0; i < tokenPlayer1.size(); i++) {
					if (tokenPlayer1[i].step == (curStep - 4)) {
						ResetSettings(PLAYER1, i);
						break;
					}
				}
			}
		}
	}
	DisplayPieces();
}

void MyEngine::ResetSettings()
{
	srand((unsigned)time(0));

	result = 0;
	totalTurn = 0;
	whoseTurn = 0;
	finishedPiecesPlayer1 = 0;
	finishedPiecesPlayer2 = 0;
	tokenPlayer1.clear();
	tokenPlayer2.clear();

	memset(steps, 0, sizeof(steps));
	memset(out, 0, sizeof(out));
	memset(ifForward, 0, sizeof(ifForward));
	memset(tokenKill, 0, sizeof(tokenKill));
}

void MyEngine::ResetSettings(int player, int movingPiece)
{
	if (player == PLAYER1) {
		tokenPlayer1.erase(tokenPlayer1.begin() + movingPiece);
		printf("One of player1's piece has been removed from board\n");
		tokenKill[PLAYER2] = 1;
	}

	else if (player == PLAYER2) {
		tokenPlayer2.erase(tokenPlayer2.begin() + movingPiece);
		printf("One of player2's piece has been removed from board\n");
		tokenKill[PLAYER1] = 1;
	}

	totalTurn++;
}

bool MyEngine::ComparePositions(int position1[2], int position2[2])
{
	if (position1[0] == position2[0] && position1[1] == position2[1])
		return true;
	else
		return false;
}

void MyEngine::InitBoard() {

	startPositionX = (GetScreenWidth() - 7 * m_oTiles.GetTileWidth()) / 2;
	startPositionY = (GetScreenHeight() - 7 * m_oTiles.GetTileHeight()) / 2;

	TileOrderForPlayer(startPositionX, startPositionY);

	for (int i = 0; i < 7; i++) {

		for (int j = 0; j < 7; j++) {

			if (i >= 1 && i <= 5 && j >= 1 && j <= 5) {
				m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x7fffaa, 0);
			}

			else {
				m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0xffffff, 0);
			}

			if ((i == 0 && j == 3) || (i == 1 && j == 1) || (i == 1 && j == 5) || (i == 3 && j == 0) || (i == 3 && j == 6) || (i == 5 && j == 1) || (i == 5 && j == 5) || (i == 6 && j == 3)) {
				m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x3cb371, 0);
			}

			startPositionX += m_oTiles.getTileWidth();
		}

		startPositionX = (GetScreenWidth() - 7 * m_oTiles.GetTileWidth()) / 2;
		startPositionY += m_oTiles.getTileHeight();
	}
	DrawGuidence();
}

void MyEngine::SimpleAI()
{
	int numChoice = 0;
	int result = m_playRules.RandomAshtaChamma();

	if (result == 6 && tokenPlayer2.size() < 3)
		numChoice = tokenPlayer2.size() + 1;
	else
		numChoice = tokenPlayer2.size();

	int curChoice = m_playRules.RandomChoice(numChoice);

	if (curChoice < tokenPlayer2.size()) {
		MovePiece(PLAYER2, result, curChoice );
		printf("AI move piece: %d\n", curChoice);
	}
	else {
		if (tokenPlayer2.size() == 0)
			out[PLAYER2] = 1;
		PutANewTokenInBoard(PLAYER2);
		printf("AI put a new piece into board\n");
	}
	totalTurn++;
	DisplayPieces();
}

bool CompareMarks(AIMovingChoice choice1, AIMovingChoice choice2)
{
	return choice1.mark > choice2.mark;
}

bool MyEngine::KilledRisk(int movingPiece, int player)
{
	bool exist = false;

	if (player == PLAYER2) {
		int getStep = tokenPlayer2[movingPiece].step;
		int getLocation[2] = { moveOrderPlayer2[getStep][0], moveOrderPlayer2[getStep][1] };

		for (int i = getStep - 6; i < getStep; i++) {

			for (int j = 0; j < tokenPlayer1.size(); j++) {
				exist = ComparePositions(moveOrderPlayer1[tokenPlayer1[j].step], moveOrderPlayer2[i]);
				if (exist) {
					break;
				}
			}

			if (exist) {
				break;
			}
		}
	}

	if (exist) return true;
	else return false;
}

bool MyEngine::LandSafeBox(int result, int movingPiece, int player)
{
	int destination = 0;

	if (player == PLAYER2) {
		int getStep = tokenPlayer2[movingPiece].step;
		if (tokenKill[PLAYER2] == 1) {
			destination = getStep + result;
		}
		else {
			destination = getStep + result;
			if (destination > 17) {
				destination -= 16;
			}
		}
	}

	if (destination == 4 || destination == 8 || destination == 12 || destination == 16) {
		return true;
	}
	else {
		return false;
	}
}

void MyEngine::HigherLevelAI()
{
	int curHighestStepsPlayer1 = 0;
	int destinationStep = 0;
	bool killingMovement = false;
	bool landSafeBox = false;
	bool killedRisk = false;

	result = m_playRules.RandomAshtaChamma();
	printf("result of AI: %d\n", result);

	AIMovementMark.clear();

	if (result == 6 && tokenPlayer2.size() < 3) {
		printf("AI can put a new piece into board\n");
		if (tokenPlayer2.size() == 0) out[PLAYER2] = 1;
		AIMovementMark.push_back({ PUT_NEW_PIECE, 0, 0, true});
	}

	for (int i = 0; i < tokenPlayer2.size(); i++) {

		if (tokenKill[PLAYER2] == 1)
			destinationStep = tokenPlayer2[i].step + result;
		else {
			destinationStep = tokenPlayer2[i].step + result;
			if (destinationStep > 17) {
				destinationStep -= 16;
			}
		}

		for (int j = 0; j < tokenPlayer1.size(); j++) {

			bool ifKilling = ComparePositions(moveOrderPlayer1[tokenPlayer1[j].step], moveOrderPlayer2[destinationStep]);

			if (ifKilling) {
				AIMovementMark.push_back({ KILLING_MOVEMENT, i, result, false });
				killingMovement = true;
				break;
			}
		}

		if (!killingMovement) {

			int actualDestination = 0;
			int actualForward = 0;

			int getStep = tokenPlayer2[i].step;
			int exceptStep = getStep + result;

			if (exceptStep > 26) {
				actualDestination = 26 - (result - (26 - getStep));
			}
			else {
				actualDestination = exceptStep;
			}
			actualForward = actualDestination - getStep;

			landSafeBox = LandSafeBox(result, i, PLAYER2);
			if (landSafeBox) {
				killedRisk = KilledRisk(i, PLAYER2);
				if (killedRisk) {
					AIMovementMark.push_back({ LAND_SAFE_BOX, i, result, false });
				}
				else {
					AIMovementMark.push_back({ actualForward, i, result, false });
				}
			}

			AIMovementMark.push_back({ actualForward, i, result, false });
		}
	}

	if (AIMovementMark.size() == 0) {
		totalTurn++;
		DisplayPieces();
		return;
	}

	std::sort(AIMovementMark.begin(), AIMovementMark.end(), CompareMarks);
	AIMovingChoice finalChoice = AIMovementMark[0];

	/*for (int i = 0; i < AIMovementMark.size(); i++) {
		printf("choice %d\n", i);
		printf("current mark: %d\n", AIMovementMark[i].mark);
		printf("current moving piece: %d\n", AIMovementMark[i].movingPiece);
		printf("current rolling result: %d\n", AIMovementMark[i].result);
		printf("whether put a new piece into board: %d\n", AIMovementMark[i].newPiece);
		printf("\n\n");
	}*/

	if (finalChoice.newPiece) {
		PutANewTokenInBoard(PLAYER2);
	}
	else {
		MovePiece(PLAYER2, finalChoice.result, finalChoice.movingPiece);
	}

	totalTurn++;
	DisplayPieces();
}

void MyEngine::DrawGuidence()
{
	int width = m_oTiles.getTileWidth();
	int height = m_oTiles.getTileHeight();

	//for player 1
	startPositionX = (GetScreenWidth() - 7 * width) / 2 + 0.2 * width;
	startPositionY = (GetScreenHeight() - 7 * height) / 2 + 3.8 * height;

	m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x000000, 3);
	startPositionX += width;

	for (int i = 0; i < 2; i++) {
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x000000, 4);
		startPositionY += height;
	}

	for (int i = 0; i < 4; i++) {
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x000000, 3);
		startPositionX += width;
	}

	for (int i = 0; i < 4; i++) {
		startPositionY -= height;
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x000000, 4);
	}

	for (int i = 0; i < 4; i++) {
		startPositionX -= width;
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x000000, 3);
	}

	m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x000000, 4);
	startPositionY += height;

	for (int i = 0; i < 3; i++) {
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x000000, 3);
		startPositionX += width;
	}

	for (int i = 0; i < 2; i++) {
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x000000, 4);
		startPositionY += height;
	}

	for (int i = 0; i < 2; i++) {
		startPositionX -= width;
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x000000, 3);
	}

	startPositionY -= height;
	m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x000000, 4);
	m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0x000000, 3);

	//for player 2
	startPositionX = (GetScreenWidth() - 7 * width) / 2 + 6.8 * width;
	startPositionY = (GetScreenHeight() - 7 * height) / 2 + 3.2 * height;

	startPositionX -= width;
	m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0xffffff, 3);

	for (int i = 0; i < 2; i++) {
		startPositionY -= height;
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0xffffff, 4);
	}

	for (int i = 0; i < 4; i++) {
		startPositionX -= width;
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0xffffff, 3);
	}

	for (int i = 0; i < 4; i++) {
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0xffffff, 4);
		startPositionY += height;
	}

	for (int i = 0; i < 4; i++) {
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0xffffff, 3);
		startPositionX += width;
	}

	startPositionY -= height;
	m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0xffffff, 4);

	for (int i = 0; i < 3; i++) {
		startPositionX -= width;
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0xffffff, 3);
	}

	for (int i = 0; i < 2; i++) {
		startPositionY -= height;
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0xffffff, 4);
	}

	for (int i = 0; i < 2; i++) {
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0xffffff, 3);
		startPositionX += width;
	}

	m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0xffffff, 4);
	startPositionY += height;

	startPositionX -= width;
	m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, startPositionX, startPositionY, 0xffffff, 3);
}

void MyEngine::TileOrderForPlayer(int startX, int startY)
{
	int tileWidth = m_oTiles.getTileWidth();
	int tileHeight = m_oTiles.getTileHeight();

	//save player1's move order
	moveOrderPlayer1[0][0] = startX;
	moveOrderPlayer1[0][1] = startY + tileHeight * 3;

	moveOrderPlayer1[1][0] = startX + tileWidth;
	moveOrderPlayer1[1][1] = startY + tileHeight * 3;

	moveOrderPlayer1[2][0] = startX + tileWidth;
	moveOrderPlayer1[2][1] = startY + tileHeight * 4;

	moveOrderPlayer1[3][0] = startX + tileWidth;
	moveOrderPlayer1[3][1] = startY + tileHeight * 5;

	moveOrderPlayer1[4][0] = startX + tileWidth * 2;
	moveOrderPlayer1[4][1] = startY + tileHeight * 5;

	moveOrderPlayer1[5][0] = startX + tileWidth * 3;
	moveOrderPlayer1[5][1] = startY + tileHeight * 5;

	moveOrderPlayer1[6][0] = startX + tileWidth * 4;
	moveOrderPlayer1[6][1] = startY + tileHeight * 5;

	moveOrderPlayer1[7][0] = startX + tileWidth * 5;
	moveOrderPlayer1[7][1] = startY + tileHeight * 5;

	moveOrderPlayer1[8][0] = startX + tileWidth * 5;
	moveOrderPlayer1[8][1] = startY + tileHeight * 4;

	moveOrderPlayer1[9][0] = startX + tileWidth * 5;
	moveOrderPlayer1[9][1] = startY + tileHeight * 3;

	moveOrderPlayer1[10][0] = startX + tileWidth * 5;
	moveOrderPlayer1[10][1] = startY + tileHeight * 2;

	moveOrderPlayer1[11][0] = startX + tileWidth * 5;
	moveOrderPlayer1[11][1] = startY + tileHeight;

	moveOrderPlayer1[12][0] = startX + tileWidth * 4;
	moveOrderPlayer1[12][1] = startY + tileHeight;

	moveOrderPlayer1[13][0] = startX + tileWidth * 3;
	moveOrderPlayer1[13][1] = startY + tileHeight;

	moveOrderPlayer1[14][0] = startX + tileWidth * 2;
	moveOrderPlayer1[14][1] = startY + tileHeight;

	moveOrderPlayer1[15][0] = startX + tileWidth;
	moveOrderPlayer1[15][1] = startY + tileHeight;

	moveOrderPlayer1[16][0] = startX + tileWidth;
	moveOrderPlayer1[16][1] = startY + tileHeight * 2;

	moveOrderPlayer1[17][0] = startX + tileWidth * 2;
	moveOrderPlayer1[17][1] = startY + tileHeight * 2;

	moveOrderPlayer1[18][0] = startX + tileWidth * 3;
	moveOrderPlayer1[18][1] = startY + tileHeight * 2;

	moveOrderPlayer1[19][0] = startX + tileWidth * 4;
	moveOrderPlayer1[19][1] = startY + tileHeight * 2;

	moveOrderPlayer1[20][0] = startX + tileWidth * 4;
	moveOrderPlayer1[20][1] = startY + tileHeight * 3;

	moveOrderPlayer1[21][0] = startX + tileWidth * 4;
	moveOrderPlayer1[21][1] = startY + tileHeight * 4;

	moveOrderPlayer1[22][0] = startX + tileWidth * 3;
	moveOrderPlayer1[22][1] = startY + tileHeight * 4;

	moveOrderPlayer1[23][0] = startX + tileWidth * 2;
	moveOrderPlayer1[23][1] = startY + tileHeight * 4;

	moveOrderPlayer1[24][0] = startX + tileWidth * 2;
	moveOrderPlayer1[24][1] = startY + tileHeight * 3;

	moveOrderPlayer1[25][0] = startX + tileWidth * 3;
	moveOrderPlayer1[25][1] = startY + tileHeight * 3;


	//save player2's move order
	moveOrderPlayer2[0][0] = startX + tileWidth * 6;
	moveOrderPlayer2[0][1] = startY + tileHeight * 3;

	moveOrderPlayer2[1][0] = startX + tileWidth * 5;
	moveOrderPlayer2[1][1] = startY + tileHeight * 3;

	moveOrderPlayer2[2][0] = startX + tileWidth * 5;
	moveOrderPlayer2[2][1] = startY + tileHeight * 2;

	moveOrderPlayer2[3][0] = startX + tileWidth * 5;
	moveOrderPlayer2[3][1] = startY + tileHeight;

	moveOrderPlayer2[4][0] = startX + tileWidth * 4;
	moveOrderPlayer2[4][1] = startY + tileHeight;

	moveOrderPlayer2[5][0] = startX + tileWidth * 3;
	moveOrderPlayer2[5][1] = startY + tileHeight;

	moveOrderPlayer2[6][0] = startX + tileWidth * 2;
	moveOrderPlayer2[6][1] = startY + tileHeight;

	moveOrderPlayer2[7][0] = startX + tileWidth;
	moveOrderPlayer2[7][1] = startY + tileHeight;

	moveOrderPlayer2[8][0] = startX + tileWidth;
	moveOrderPlayer2[8][1] = startY + tileHeight * 2;

	moveOrderPlayer2[9][0] = startX + tileWidth;
	moveOrderPlayer2[9][1] = startY + tileHeight * 3;

	moveOrderPlayer2[10][0] = startX + tileWidth;
	moveOrderPlayer2[10][1] = startY + tileHeight * 4;

	moveOrderPlayer2[11][0] = startX + tileWidth;
	moveOrderPlayer2[11][1] = startY + tileHeight * 5;

	moveOrderPlayer2[12][0] = startX + tileWidth * 2;
	moveOrderPlayer2[12][1] = startY + tileHeight * 5;

	moveOrderPlayer2[13][0] = startX + tileWidth * 3;
	moveOrderPlayer2[13][1] = startY + tileHeight * 5;

	moveOrderPlayer2[14][0] = startX + tileWidth * 4;
	moveOrderPlayer2[14][1] = startY + tileHeight * 5;

	moveOrderPlayer2[15][0] = startX + tileWidth * 5;
	moveOrderPlayer2[15][1] = startY + tileWidth * 5;

	moveOrderPlayer2[16][0] = startX + tileWidth * 5;
	moveOrderPlayer2[16][1] = startY + tileHeight * 4;

	moveOrderPlayer2[17][0] = startX + tileWidth * 4;
	moveOrderPlayer2[17][1] = startY + tileHeight * 4;

	moveOrderPlayer2[18][0] = startX + tileWidth * 3;
	moveOrderPlayer2[18][1] = startY + tileHeight * 4;

	moveOrderPlayer2[19][0] = startX + tileWidth * 2;
	moveOrderPlayer2[19][1] = startY + tileHeight * 4;

	moveOrderPlayer2[20][0] = startX + tileWidth * 2;
	moveOrderPlayer2[20][1] = startY + tileHeight * 3;

	moveOrderPlayer2[21][0] = startX + tileWidth * 2;
	moveOrderPlayer2[21][1] = startY + tileHeight * 2;

	moveOrderPlayer2[22][0] = startX + tileWidth * 3;
	moveOrderPlayer2[22][1] = startY + tileHeight * 2;

	moveOrderPlayer2[23][0] = startX + tileWidth * 4;
	moveOrderPlayer2[23][1] = startY + tileHeight * 2;

	moveOrderPlayer2[24][0] = startX + tileWidth * 4;
	moveOrderPlayer2[24][1] = startY + tileHeight * 3;

	moveOrderPlayer2[25][0] = startX + tileWidth * 3;
	moveOrderPlayer2[25][1] = startY + tileHeight * 3;
}
