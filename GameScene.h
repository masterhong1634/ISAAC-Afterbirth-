#pragma once
#include "gameNode.h"
#include "Shop.h"
#include "MainMap.h"

class GameScene : public gameNode
{
private:
	Shop* m_shop;
	image* testMapImage;
	MainMap * mainMap;

public:
	GameScene();
	~GameScene();

	HRESULT init();
	void release();
	void update();
	void render();
};