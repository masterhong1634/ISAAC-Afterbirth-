#pragma once
//ũ��
#define CELL_WIDTH 52
#define CELL_HEIGHT 52

//�ʱ���ǥ
#define INIT_X -1768
#define INIT_Y -1144
//Ÿ�� ����
#define TILE_COUNT_X 85
#define TILE_COUNT_Y 55

//Ÿ�� �̹���
#define TILE_SIZE_X 8
#define TILE_SIZE_Y 10


#define TILE_MAX 3

enum TILEKIND
{
	TILEKIND_TERRAIN,
	TILEKIND_OBJECT,
	TILEKIND_INVISIBLE_BLOCK,
	TILEKIND_ITEMPOINT,
	TILEKIND_OPEN_DOOR,
	TILEKIND_CLOSE_DOOR,
	TILEKIND_NONE,

	TILEKIND_ITEMHEART,
	TILEKIND_ITEMGOLD,
	TILEKIND_ITEMBOMB,
	TILEKIND_ITEMATTACKBOMB,
	TILEKIND_ITEMKEY,
	TILEKIND_ITEMPILL,
	TILEKIND_ITEMCARD,
	TILEKIND_BLACKITEMBOXOPEN,
	TILEKIND_BLACKITEMBOXCLOSE,
	TILEKIND_GOLDITEMBOXOPEN,
	TILEKIND_GOLDITEMBOXCLOSE,
	TILEKIND_REDITEMBOXOPEN,
	TILEKIND_REDITEMBOXCLOSE,
	TILEKIND_CRACKEDITEMBOXOPEN,
	TILEKIND_CRACKEDITEMBOXCLOSE,
	TILEKIND_LOCKED_DOOR,
	TILEKIND_USEDKEY_DOOR,

	TILEKIND_POOP100,
	TILEKIND_POOP50,
	TILEKIND_POOP10,
	TILEKIND_FIREPLACE,
	TILEKIND_SHOP_HOST

};

struct TagTile
{
	int left, top, right, bottom; // x, y ��(rect�� left, top)

	RECT rect;

	int index; // ���ΰ�. tileNum, tileKind, tilePos ���� ���.

	int tileNum[TILE_MAX]; // ����Ÿ�� �ε���(���õ� â) index�� �����ͼ� ����.
	TILEKIND tileKind[TILE_MAX]; // ������Ʈ, Ÿ�� ����.
	POINT tilePos[TILE_MAX]; // ����Ÿ�Ͽ��� ������༮(frameX, frameY)


	int height; // ���ϼ��� Ŀ���� ����
};