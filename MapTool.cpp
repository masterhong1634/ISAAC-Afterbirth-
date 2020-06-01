#include "stdafx.h"
#include "MapTool.h"


MapTool::MapTool()
{
}


MapTool::~MapTool()
{
}

HRESULT MapTool::init()
{
	IMAGEMANAGER->addFrameImage("tilemap", "images/tilemap.bmp", 640, 256, SAMPLETILEX, SAMPLETILEY, true, RGB(255, 0, 255));
	maptoolSetup();

	_currentTile.x = 2;
	_currentTile.y = 0;


	return S_OK;
}

void MapTool::release()
{
}

void MapTool::update()
{
	// 게임 노드에 있던 마우스 관련 부분 업데이트에서 만들어서 테스트.
	//if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	//{
	//	SetMap();
	//}
	//if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	//{
	//	SetMap();
	//}
}

void MapTool::render()
{
	IMAGEMANAGER->render("tilemap", getMemDC(), WINSIZEX - IMAGEMANAGER->findImage("tilemap")->getWidth(), 0);

	for (int i = 0; i < TILEX * TILEY; i++)
	{
		IMAGEMANAGER->frameRender("tilemap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].terrainFrameX, _tiles[i].terrainFrameY);

		if (_tiles[i].obj == OBJ_NONE)continue;

		IMAGEMANAGER->frameRender("tilemap", getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].objFrameX, _tiles[i].objFrameY);
	}
}

void MapTool::maptoolSetup()
{
	_btnSave = CreateWindow("button", "Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1000, 600, 100, 30, m_hWnd, HMENU(0), m_hInstance, NULL);
	_btnLoad = CreateWindow("button", "Load", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 1150, 600, 100, 30, m_hWnd, HMENU(1), m_hInstance, NULL);
	_btnTerrainDraw = CreateWindow("button", "Terrain", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 850, 350, 100, 30, m_hWnd, HMENU(2), m_hInstance, NULL);
	_btnObjectDraw = CreateWindow("button", "OBJ", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 850, 400, 100, 30, m_hWnd, HMENU(3), m_hInstance, NULL);
	_btnEraser = CreateWindow("button", "Eraser", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 850, 450, 100, 30, m_hWnd, HMENU(4), m_hInstance, NULL);

	_ctrSelect = CTRL_TERRAINDRAW;

	//오른쪽 샘플 맵 셋팅
	for (int i = 0; i < SAMPLETILEY; i++)
	{
		for (int j = 0; j < SAMPLETILEX; j++)
		{
			_sampleTiles[i* SAMPLETILEX + j].terrainFrameX = j;
			_sampleTiles[i* SAMPLETILEX + j].terrainFrameY = i;

			//좌표값 설정
			SetRect(&_sampleTiles[i*SAMPLETILEX + j].rcTile,
				(WINSIZEX - IMAGEMANAGER->findImage("tilemap")->getWidth()) + j * TILESIZE,
				i* TILESIZE, (WINSIZEX - IMAGEMANAGER->findImage("tilemap")->getWidth()) + j * TILESIZE + TILESIZE,
				i*TILESIZE + TILESIZE);
		}
	}

	//왼쪽 맵 셋팅
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			SetRect(&_tiles[i*TILEX + j].rc,
				j*TILESIZE,
				i*TILESIZE,
				j*TILESIZE + TILESIZE,
				i*TILESIZE + TILESIZE);
		}
	}
	//처음 깔려있는 타일
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		_tiles[i].terrainFrameX = 3;
		_tiles[i].terrainFrameY = 0;

		_tiles[i].objFrameX = 0;
		_tiles[i].objFrameY = 0;

		_tiles[i].terrain = terrainSelect(_tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
		_tiles[i].obj = OBJ_NONE;
	}
}

void MapTool::SetMap()
{	
	//오른쪽 샘플
	for (int i = 0; i < SAMPLETILEX* SAMPLETILEY; i++)
	{
		if (PtInRect(&_sampleTiles[i].rcTile, m_ptMouse))
		{
			_currentTile.x = _sampleTiles[i].terrainFrameX;
			_currentTile.y = _sampleTiles[i].terrainFrameY;
			break;
		}
	}
	//왼쪽 타일
	for (int i = 0; i < TILEX* TILEY; i++)
	{
		if (PtInRect(&_tiles[i].rc, m_ptMouse))
		{
			if (_ctrSelect == CTRL_TERRAINDRAW)
			{
				_tiles[i].terrainFrameX = _currentTile.x;
				_tiles[i].terrainFrameY = _currentTile.y;

				_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);
			}
			else if (_ctrSelect == CTRL_OBJDRAW)
			{
				_tiles[i].objFrameX = _currentTile.x;
				_tiles[i].objFrameY = _currentTile.y;

				_tiles[i].obj = objSelect(_currentTile.x, _currentTile.y);
			}
			else if (_ctrSelect == CTRL_ERASER)
			{
				_tiles[i].objFrameX = 0;
				_tiles[i].objFrameY = 0;
				_tiles[i].obj = OBJ_NONE;
			}
			InvalidateRect(m_hWnd, NULL, false);
			break;
		}
	}
}

void MapTool::save()
{
}

void MapTool::load()
{
}

TERRAIN MapTool::terrainSelect(int frameX, int frameY)
{
	//시멘트
	if (frameX == 1 && frameY == 0)
	{
		return TR_CEMENT;
	}
	//흙바닥
	else if (frameX == 2 && frameY == 0)
	{
		return TR_DESERT;
	}
	//잔디
	else if (frameX == 3 && frameY == 0)
	{
		return TR_GRASS;
	}
	//물
	else if (frameX == 4 && frameY == 0)
	{
		return TR_WATER;
	}
	return TR_GRASS;
}

OBJECT MapTool::objSelect(int frameX, int frameY)
{
	if (frameX == 0 && frameY == 0)
	{
		return OBJ_TANK1;
	}
	else if (frameX == 0 && frameY == 8)
	{
		return OBJ_TANK2;
	}
	else if (frameX == 0 && frameY == 1)
	{
		return OBJ_BLOCK1;
	}
	else if (frameX == 0 && frameY == 2)
	{
		return OBJ_BLOCK3;
	}
	else if (frameX == 0 && frameY == 3)
	{
		return OBJ_BLOCKS;
	}
	return OBJ_BLOCK1;
}
