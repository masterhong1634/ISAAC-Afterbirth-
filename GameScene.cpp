#include "stdafx.h"
#include "GameScene.h"
#include"MapToolTile.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

HRESULT GameScene::init()
{
	// 사운드
	SOUNDMANAGER->stop("main");
	SOUNDMANAGER->play("BGM", 1.0f);

	mainMap = new MainMap;
	mainMap->init();

	// 게임씬에서 필요한 매니저들 초기화.
	BULLETMANAGER->Init();
	ITEMMANAGER->Init();
	COLLISIONMANAGER->Init();

	// 인클루드한 것들 초기화
	m_shop = new Shop;
	m_shop->Init();

	// 플레이어, 에네미 메니저 초기화.
	ENEMYMANAGER->Init();
	PLAYERMANAGER->Init();

	return S_OK;
}

void GameScene::release()
{
	SAFE_DELETE(m_shop);
	SAFE_DELETE(mainMap);
}

void GameScene::update()
{
	mainMap->update();
	PLAYERMANAGER->Update();
	ENEMYMANAGER->Update();
	m_shop->Update();

	COLLISIONMANAGER->UpdateCollision();
	ANIMATIONMANAGER->update();
	EFFECTMANAGER->update();

	if (KEYMANAGER->isOnceKeyDown('P'))
	{
		SCENEMANAGER->changeScene("MainMenu");
	}
}

void GameScene::render()
{
	mainMap->render();
	m_shop->Render(getMemDC());
	ENEMYMANAGER->Render(getMemDC());
	PLAYERMANAGER->Render(getMemDC());
	EFFECTMANAGER->render();
	OBJECTPOOL->Render(getMemDC());
}
