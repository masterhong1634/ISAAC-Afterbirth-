#pragma once
#include "gameNode.h"
#include "MapToolTile.h"
#include "SubMap.h"

#define SUBWINOPEN 1

class MapToolScene : public gameNode
{
private:
	SubMap* sub = new SubMap;

	TagTile _tileMap[TILE_COUNT_X][TILE_COUNT_Y];

	int _locationX, _locationY;
	int _center;
	char str[128];


	int currentX;
	int currentY;

	bool openDoor;

	POINT imageFrame;
	CTRL _currentCTRL;
	RECT cameraRect = { 0,0,WINSIZEX,WINSIZEY };
	RECT temp;

	bool _isDebug;

	//
	const char* fileName[10] = { "map1.map","map2.map","map3.map","map4.map","map5.map","map6.map","map7.map","map8.map","map9.map","map10.map" };
	HANDLE file;
	DWORD write;
	DWORD read;

public:
	MapToolScene();
	~MapToolScene();

	HRESULT init();
	void release();
	void update();
	void render();

	void SetSubWindow();

	//타일맵 그리자.
	void DrawTileMap();

	//라인 그려주기
	void Draw_Line_X(int left, int top);
	void Draw_Line_Y(int left, int top);

	void MapToolSetup();
	void setMap(int locationX, int locationY, bool isAdd);
	TILEKIND kindSelect(int frameX, int frameY);

	void TlieInit();
	void Load(int loadCount);
	void Save(int saveCount);
};
