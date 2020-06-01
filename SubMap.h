#pragma once
#include"gameNode.h"
#include"MapToolTile.h"

class SubMap : public gameNode
{
private:

	bool isDebug;

	int currentTile;
	RECT rc[TILE_SIZE_X][TILE_SIZE_Y];

	int frameX, frameY;

public:
	SubMap();
	~SubMap();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);
};