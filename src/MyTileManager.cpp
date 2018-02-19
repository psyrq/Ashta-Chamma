#include "MyTileManager.h"
#include "header.h"
#include "templates.h"
#include "BaseEngine.h"
#include "TileManager.h"
#include "JPGImage.h"

MyTileManager::MyTileManager() :TileManager(50, 50)
{
}


MyTileManager::~MyTileManager()
{
}

void MyTileManager::DrawTileAt(BaseEngine* pEngine,
	SDL_Surface* pSurface,
	int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY, int TileColour, int flag) const
{
	// Base class implementation just draws some grey tiles
	/*ImageData im;
	im.LoadImage("bomb.jpg");
	im.FlexibleRenderImageWithMask(pEngine->GetSDLSurface(),
	0, 0,
	iStartPositionScreenX, iStartPositionScreenY,
	im.GetWidth(), im.GetHeight());*/

	if (flag == 0) {
		//printf("draw tiles\n");
		pEngine->DrawRectangle(
			iStartPositionScreenX,
			iStartPositionScreenY,
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			TileColour,
			pSurface);
		pEngine->DrawLine(iStartPositionScreenX, iStartPositionScreenY, iStartPositionScreenX, iStartPositionScreenY + GetTileHeight() - 1, 0x000000, pSurface);
		pEngine->DrawLine(iStartPositionScreenX, iStartPositionScreenY, iStartPositionScreenX + GetTileWidth() - 1, iStartPositionScreenY, 0x000000, pSurface);
		pEngine->DrawLine(iStartPositionScreenX, iStartPositionScreenY + GetTileHeight() - 1, iStartPositionScreenX + GetTileWidth() - 1, iStartPositionScreenY + GetTileHeight() - 1, 0x000000, pSurface);
		pEngine->DrawLine(iStartPositionScreenX + GetTileWidth() - 1, iStartPositionScreenY, iStartPositionScreenX + GetTileWidth() - 1, iStartPositionScreenY + GetTileHeight() - 1, 0x000000, pSurface);
	}

	//SetSize(25, 25);

	else if (flag == 1) {
		//printf("draw chess\n");
		pEngine->DrawOval(
			iStartPositionScreenX,
			iStartPositionScreenY,
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			TileColour,
			pSurface);
	}
	
}

int MyTileManager::getTileWidth()
{
	return GetTileWidth();
}


int MyTileManager::getTileHeight()
{
	return GetTileHeight();
}
