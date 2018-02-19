#include "header.h"
#include "templates.h"
#include "BaseEngine.h"
#include "TileManager.h"
#include "MyEngine.h"
#include "PlayRules.h"
#include "windows.h"
#include "time.h"

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
		break;
	default:
		break;
	}
}


void MyEngine::DrawStrings()
{
	switch (m_state)
	{
	case stateInit:
		DrawScreenString(200, 100, "Press Space to start", 0x000000, NULL);
		break;
	case stateMain:
		char score[128];
		char player[128];
		BaseEngine::DrawRectangle(600, 50, 680, 80, 0xffffff);
		DrawScreenString(600, 50, "start", 0x000000, NULL);
		DrawScreenString(600, 100, "result : ", 0x000000, NULL);

		if (totalTurn % 2 == 0) {
			DrawScreenString(300, 75, "Player1's turn", 0x000000, NULL);
		}
		else if (totalTurn % 2 == 1) {
			DrawScreenString(300, 75, "Player2's turn", 0x000000, NULL);
		}

		if (totalTurn != 0) {
			sprintf(score, "%d", result);
			DrawScreenString(720, 100, score, 0x000000, NULL);
		}

		if (steps[PLAYER1] == 26 || steps[PLAYER2] == 26) {
			if (steps[PLAYER1] == 26)
				DrawScreenString(600, 150, "Player1 win", 0x000000, NULL);
			else
				DrawScreenString(600, 150, "Player2 win", 0x000000, NULL);

			DrawScreenString(600, 200, "Press R", 0x000000, NULL);
			DrawScreenString(600, 250, "to start", 0x000000, NULL);
			DrawScreenString(600, 300, "a new game", 0x000000, NULL);
		}
		break;
	case statePaused:
		DrawScreenString(200, 100, "Paused. Press Space to restart", 0x000000, NULL);
		break;
	case stateEnd:
		DrawScreenString(300, 100, "Press Space to start a new game", 0x000000, NULL);
		break;
	}
}


void MyEngine::KeyDown(int iKeyCode)
{
	switch (iKeyCode)
	{
	case SDLK_ESCAPE:
		SetExitWithCode(0);
		break;
	case SDLK_SPACE:
		switch (m_state)
		{
		case stateInit:
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
			m_state = stateMain;
			Redraw(true);
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
			break;
		case stateEnd:
			break;
		case stateMain:
			ResetSettings();
			SetupBackgroundBuffer();
			Redraw(true);
		}
		break;
	}
}


void MyEngine::MouseDown(int iButton, int iX, int iY)
{
	switch (m_state)
	{
	case stateInit:
		break;
	case stateMain:

		if (iX < 680 && iX > 600 && iY < 80 && iY > 50) {

			result = m_playRules.RandomAshtaChamma();
			printf("result = %d\n", result);
			whoseTurn = ((totalTurn % 2) == 0) ? PLAYER1 : PLAYER2;
			MovePiece(whoseTurn, result);
			totalTurn++;

			Redraw(true);
		}
		break;
	case statePaused:
		break;
	case stateEnd:
		break;
	}
}

void MyEngine::MovePiece(int player, int result)
{
	if (result != 6 && out[player] == 0)
		return;
	else if (result == 6 && out[player] == 0) {
		out[player] = 1;
		steps[player]++;
		DisplayPieces();
	}
	else {
		if (player == PLAYER1) result = 5;
		if (player == PLAYER2) result = 2;
		//if (player == PLAYER1) result = 2;
		//if (player == PLAYER2) result = 4;
		ifForward[player] = 1;

		for (int i = 0; i < result; i++) {

			if (steps[player] == 17 && tokenKill[player] == 0) {
				steps[player] = 1;
			}

			if (ifForward[player] == 1) {
				steps[player]++;
			}
			else if (ifForward[player] == 0) {
				steps[player]--;
			}

			if (steps[player] == 26) ifForward[player] = 0;
			DisplayPieces();
			printf("steps = %d\n", steps[player]);
		}
		if (player == PLAYER1) {
			DeterminKilling(PLAYER1, moveOrderPlayer1[steps[PLAYER1] - 1], PLAYER2, moveOrderPlayer2[steps[PLAYER2] - 1]);
		}
		else if (player == PLAYER2) {
			DeterminKilling(PLAYER2, moveOrderPlayer2[steps[PLAYER2] - 1], PLAYER1, moveOrderPlayer1[steps[PLAYER1] - 1]);
		}
		//Sleep(500);
		DisplayPieces();
	}
}

void MyEngine::DisplayPieces()
{
	SetupBackgroundBuffer();
	if (out[PLAYER1] == 1)
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, moveOrderPlayer1[steps[PLAYER1]-1][0], moveOrderPlayer1[steps[PLAYER1]-1][1], 0x000000, 1);
	if (out[PLAYER2] == 1)
		m_oTiles.DrawTileAt(this, this->GetBackground(), 0, 0, moveOrderPlayer2[steps[PLAYER2]-1][0], moveOrderPlayer2[steps[PLAYER2]-1][1], 0xffffff, 1);
}

void MyEngine::DeterminKilling(int curPlayer, int location1[2], int theOtherPlayer, int location2[2])
{
	if (steps[curPlayer] != 4 && steps[curPlayer] != 8 && steps[curPlayer] != 12 && steps[curPlayer] != 16) {

		if (location1[0] == location2[0] && location1[1] == location2[1]) {
			tokenKill[curPlayer] = 1;
			totalTurn++;
			ResetSettings(theOtherPlayer);
			printf("the player%d's piece has been killed.\n", theOtherPlayer + 1);
		}
	}
}

void MyEngine::ResetSettings()
{
	srand((unsigned)time(0));

	result = 0;
	totalTurn = 0;
	whoseTurn = 0;

	memset(numTurn, 0, sizeof(numTurn));
	memset(steps, 0, sizeof(steps));
	memset(out, 0, sizeof(out));
	memset(ifForward, 0, sizeof(ifForward));
	memset(tokenKill, 0, sizeof(tokenKill));
}

void MyEngine::ResetSettings(int player)
{
	steps[player] = 0;
	out[player] = 0;
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

