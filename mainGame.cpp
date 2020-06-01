#include "stdafx.h"
#include "mainGame.h"

mainGame::mainGame()
{
}

mainGame::~mainGame()
{
}

HRESULT mainGame::init()
{
	gameNode::init(true);

	// ����
	SOUNDMANAGER->addSound("title", "sound/Title Screen.mp3", true, true);
	SOUNDMANAGER->addSound("main", "sound/ReGenesis.mp3", true, true);
	SOUNDMANAGER->addSound("BGM", "sound/Diptera Sonata.mp3", true, true);
	SOUNDMANAGER->addSound("BossBGM", "sound/Boss Fight.mp3", true, true);

	// Ÿ��Ʋ ��
	SCENEMANAGER->addScene("TitleScene", new TitleScene);
	
	// �޴�
	SCENEMANAGER->addScene("MainMenu", new MainMenu);

	// ĳ���� ����â 
	SCENEMANAGER->addScene("CharacterSelect", new CharacterSelect);

	// ���� ���� Scene
	SCENEMANAGER->addScene("GameScene", new GameScene);
	
	// ������ �̹�����
	IMAGEMANAGER->addFrameImage("blocks", "images/maptool/blocks2.bmp",
		0, 0, 52 * 8, 52 * 9, 8, 9, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("mapTile", "images/maptool/SampleMap2.bmp",
		0, 0, 52 * 17 * 4, 52 * 11 * 8, 4, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("door", "images/maptool/doorSprite2.bmp",
		0, 0, 52 * 2.5 * 8, 52 * 2 * 10, 8, 10, true, RGB(255, 0, 255));

	MapToolScene* map = new MapToolScene;
	SCENEMANAGER->addScene("MapTool", map);

	SUBWIN->SetIsoMap(map);

	// ���Ӿ����� �ʿ��� �Ŵ����� �ʱ�ȭ.
	OBJECTPOOL->Init();

	SCENEMANAGER->changeScene("TitleScene");

	return S_OK;
}

void mainGame::release()
{
	gameNode::release();
}

void mainGame::update()
{
	gameNode::update();

	SCENEMANAGER->update();

//#ifdef SUBWINOPEN
//
//	SUBWIN->update();
//#endif // SUBWINOPEN
}

void mainGame::render(/*HDC hdc*/)
{
	//��� ��Ʈ��
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//===================================================

	SCENEMANAGER->render();
//#ifdef SUBWINOPEN
//	SUBWIN->render();
//#endif // SUBWINOPEN
	TIMEMANAGER->render(getMemDC());

	//===================================================
	//������� ������ HDC�� �׸���.(�ǵ帮������.)
	this->getBackBuffer()->render(getHDC(), 0, 0);
}