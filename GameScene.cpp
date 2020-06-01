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
	// ����
	SOUNDMANAGER->stop("main");
	SOUNDMANAGER->play("BGM", 1.0f);

	mainMap = new MainMap;
	mainMap->init();

	// ���Ӿ����� �ʿ��� �Ŵ����� �ʱ�ȭ.
	BULLETMANAGER->Init();
	ITEMMANAGER->Init();
	COLLISIONMANAGER->Init();

	// ��Ŭ����� �͵� �ʱ�ȭ
	m_shop = new Shop;
	m_shop->Init();

	// �÷��̾�, ���׹� �޴��� �ʱ�ȭ.
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
