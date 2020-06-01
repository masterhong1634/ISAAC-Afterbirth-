#pragma once
#include"gameNode.h"

#define TILESIZE 32 //오른쪽 화면 타일 사이즈
#define TILEX 20	//왼쪽화면 타일 사이즈 X
#define TILEY 20	//왼쪽화면 타일 사이즈 Y

#define TILESIZEX TILEX * TILESIZE
#define TILESIZEY TILEY * TILESIZE

//화면 오른쪽 이미지 타일 갯수
#define SAMPLETILEX 20
#define SAMPLETILEY 8

//지형(EX.밟을때 사운드를 나게 하거나 이동할 수 없는 지역등등)
enum TERRAIN
{
	TR_CEMENT, TR_DESERT, TR_GRASS, TR_WATER, TR_END
};

//오브젝트(EX :지형위쪽에 배치. 움직이거나 부서지거나 하는 녀석 등등 변화를 줄수 있는 물체)
enum OBJECT
{
	OBJ_BLOCK1,
	OBJ_BLOCK3,
	OBJ_BLOCKS,
	OBJ_FLAG1,
	OBJ_FLAG2,
	OBJ_TANK1,
	OBJ_TANK2,
	OBJ_NONE
};
//위치 좌표
enum POS
{
	POS_FLAG1,
	POS_FLAG2,
	POS_TANK1,
	POS_TANK2

};
//타일 구조체
struct tagTile
{
	TERRAIN terrain;
	OBJECT obj;
	RECT rc;

	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;
};
//이미지 타일 구조체
struct tagSampleTile
{
	RECT rcTile;
	int terrainFrameX;
	int terrainFrameY;

};
//현재 타일 구조체
struct tagCurrentTile
{
	int x;
	int y;
};
class MapTool : public gameNode
{
private:
	//세이브, 로드, 지형, 오브젝트, 지우개 등등등
	HWND _btnSave;
	HWND _btnLoad;
	HWND _btnTerrainDraw;
	HWND _btnObjectDraw;
	HWND _btnEraser;

	HANDLE file;
	DWORD write;
	int pos[2];

	tagCurrentTile _currentTile;
	tagTile _tiles[TILEX* TILEY];
	tagSampleTile _sampleTiles[SAMPLETILEX * SAMPLETILEY];

public:
	MapTool();
	~MapTool();

	HRESULT init();	//초기화
	void release();//해제
	void update();//연산하는곳
	void render();//그리는곳

	void maptoolSetup();
	void SetMap();

	void save();
	void load();

	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objSelect(int frameX, int frameY);
};

