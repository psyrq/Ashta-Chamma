#pragma once
#include "header.h"
#include "TileManager.h"
#include "JPGImage.h"
class MyTileManager:public TileManager
{
public:
	MyTileManager();
	~MyTileManager();

	void DrawTileAt(BaseEngine* pEngine,
		SDL_Surface* pSurface,
		int iMapX, int iMapY,
		int iStartPOsitionScreenX, int iStartPositionScreenY, int TileColour, int flag) const;
	int getTileWidth();
	int getTileHeight();
};

