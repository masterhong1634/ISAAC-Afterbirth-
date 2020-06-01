#include "stdafx.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

HRESULT TitleScene::init()
{
	// »ç¿îµå
	SOUNDMANAGER->play("title", 1.0f);

	IMAGEMANAGER->addImage("TitleBg", "images/UI/menu/titleBackground.bmp", WINSIZEX / 2, WINSIZEY / 2, WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TitleName", "images/UI/menu/TitleName.bmp", 262 * 2, 79 * 2, true, RGB(0, 0, 0));
	pressImage = IMAGEMANAGER->addFrameImage("pressImage", "images/UI/menu/TitlePress.bmp", 303 * 2, 134 * 2, 2, 1, true, RGB(255, 0, 255));

	nameRect = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 262, 79);

	ani = new animation;
	ani->init(pressImage->getWidth(), pressImage->getHeight(), 2, 1);
	ani->setDefPlayFrame(false, true);
	ani->setFPS(1);

	ANIMATIONMANAGER->addDefAnimation("ani", "pressImage", 2, false, true);
	ani = ANIMATIONMANAGER->findAnimation("ani");
	ANIMATIONMANAGER->resume("ani");

	return S_OK;
}

void TitleScene::release()
{
	SAFE_DELETE(ani);
}

void TitleScene::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN) || KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		SCENEMANAGER->changeScene("MainMenu");
	}
}

void TitleScene::render()
{
	IMAGEMANAGER->render("TitleBg", getMemDC());
	pressImage->aniRender(getMemDC(), WINSIZEX / 2 - 155, WINSIZEY / 2 - 90, ani);
	IMAGEMANAGER->render("TitleName", getMemDC(), WINSIZEX / 2 - 260, WINSIZEY / 2 - 230);
}
