#include "stdafx.h"
#include "CharacterSelect.h"

CharacterSelect::CharacterSelect()
{
}

CharacterSelect::~CharacterSelect()
{
}


HRESULT CharacterSelect::init()
{
	// character
	eveImage = IMAGEMANAGER->addImage("eveImage", "images/UI/menu/eve.bmp", 35 * 2, 42 * 2, true, RGB(255, 0, 255));
	isaacImage = IMAGEMANAGER->addImage("isaacImage", "images/UI/menu/isaac.bmp", 35 * 1.9, 42 * 1.9, true, RGB(255, 0, 255));
	guessImage = IMAGEMANAGER->addImage("guessImage", "images/UI/menu/guess.bmp", 35 * 1.9, 42 * 1.9, true, RGB(255, 0, 255));
	nameStat = IMAGEMANAGER->addFrameImage("nameStat", "images/UI/menu/nameStat.bmp", 396 * 1.9, 46 * 1.9, 3, 1, true, RGB(255, 0, 255));

	// dificulty 
	IMAGEMANAGER->addImage("difList", "images/UI/menu/difList.bmp", 71 * 1.9, 53 * 1.9, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("dnomal", "images/UI/menu/nomal.bmp", 72 * 1.9, 24 * 1.9, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("dhard", "images/UI/menu/hard.bmp", 72 * 1.9, 24 * 1.9, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("dgreed", "images/UI/menu/greed.bmp", 72 * 1.9, 24 * 1.9, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("difArrow", "images/UI/menu/difArrow.bmp", 7 * 1.9, 10 * 1.9, true, RGB(255, 0, 255));

	// background
	IMAGEMANAGER->addImage("nomal", "images/UI/menu/selectNomal.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hard", "images/UI/menu/selectHard.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("greed", "images/UI/menu/selectGreed.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	eveRect = RectMakeCenter(WINSIZEX / 2 - 120, WINSIZEY / 2 - 60, eveImage->getWidth(), eveImage->getHeight());
	isaacRect = RectMakeCenter(WINSIZEX / 2 - 10, WINSIZEY / 2 - 60, isaacImage->getWidth(), isaacImage->getHeight());
	guessRect = RectMakeCenter(WINSIZEX / 2 + 100, WINSIZEY / 2 - 60, guessImage->getWidth(), guessImage->getHeight());

	nomalRect = RectMakeCenter(WINSIZEX / 2 + 220, WINSIZEY / 2 + 5, 20, 25);
	hardRect = RectMakeCenter(WINSIZEX / 2 + 223, WINSIZEY / 2 + 36, 20, 25);
	greedRect = RectMakeCenter(WINSIZEX / 2 + 220, WINSIZEY / 2 + 63, 20, 25);

	difArrowRect = RectMakeCenter(662, 289, 5, 5);

	characterChoiceRect = RectMakeCenter(WINSIZEX / 2 - 10, WINSIZEY / 2 - 50, 50, 50);

	difficulty = NOMAL;
	character = ISAAC;

	return S_OK;
}

void CharacterSelect::release()
{

}

void CharacterSelect::update()
{
	UpDownMove();
	LeftRightMove();
	if (IntersectRect(&tempRect, &nomalRect, &difArrowRect))
	{
		if (IntersectRect(&tempRect, &isaacRect, &characterChoiceRect))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_RETURN) || KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				SCENEMANAGER->changeScene("GameScene");
			}
		}
	}
	//circle();	
}

void CharacterSelect::render()
{
	switch (difficulty)
	{
	case NOMAL:
		IMAGEMANAGER->render("nomal", getMemDC());
		IMAGEMANAGER->render("difList", getMemDC(), WINSIZEX / 2 + 230, WINSIZEY / 2 - 5);
		IMAGEMANAGER->render("dnomal", getMemDC(), WINSIZEX / 2 + 232, WINSIZEY / 2 - 9);
		break;

	case HARD:
		IMAGEMANAGER->render("hard", getMemDC());
		IMAGEMANAGER->render("difList", getMemDC(), WINSIZEX / 2 + 230, WINSIZEY / 2 - 5);
		IMAGEMANAGER->render("dhard", getMemDC(), WINSIZEX / 2 + 229, WINSIZEY / 2 + 25);

		break;

	case GREED:
		IMAGEMANAGER->render("greed", getMemDC());
		IMAGEMANAGER->render("difList", getMemDC(), WINSIZEX / 2 + 230, WINSIZEY / 2 - 5);
		IMAGEMANAGER->render("dgreed", getMemDC(), WINSIZEX / 2 + 229, WINSIZEY / 2 + 50);

		break;
	}

	switch (character)
	{
	case ISAAC:
		isaacImage->alphaRender(getMemDC(), isaacRect.left, isaacRect.top, 255);
		guessImage->alphaRender(getMemDC(), guessRect.left, guessRect.top, 180);
		eveImage->alphaRender(getMemDC(), eveRect.left, eveRect.top, 180);
		IMAGEMANAGER->frameRender("nameStat", getMemDC(), WINSIZEX / 2 - 110, WINSIZEY / 2 - 3, 0, 0);
		break;

	case GUESS:
		isaacImage->alphaRender(getMemDC(), isaacRect.left, isaacRect.top, 180);
		guessImage->alphaRender(getMemDC(), guessRect.left, guessRect.top, 255);
		eveImage->alphaRender(getMemDC(), eveRect.left, eveRect.top, 180);
		IMAGEMANAGER->frameRender("nameStat", getMemDC(), WINSIZEX / 2 - 110, WINSIZEY / 2, 1, 0);
		break;

	case EVE:
		isaacImage->alphaRender(getMemDC(), isaacRect.left, isaacRect.top, 180);
		guessImage->alphaRender(getMemDC(), guessRect.left, guessRect.top, 180);
		eveImage->alphaRender(getMemDC(), eveRect.left, eveRect.top, 255);
		IMAGEMANAGER->frameRender("nameStat", getMemDC(), WINSIZEX / 2 - 120, WINSIZEY / 2 - 3, 2, 0);
		break;
	}

	IMAGEMANAGER->render("difArrow", getMemDC(), difArrowRect.left, difArrowRect.top);

	if (KEYMANAGER->isToggleKey(VK_TAB))
	{
		Rectangle(getMemDC(), nomalRect.left, nomalRect.top, nomalRect.right, nomalRect.bottom);
		Rectangle(getMemDC(), hardRect.left, hardRect.top, hardRect.right, hardRect.bottom);
		Rectangle(getMemDC(), greedRect.left, greedRect.top, greedRect.right, greedRect.bottom);
		Rectangle(getMemDC(), difArrowRect.left, difArrowRect.top, difArrowRect.right, difArrowRect.bottom);
		Rectangle(getMemDC(), eveRect.left, eveRect.top, eveRect.right, eveRect.bottom);
		Rectangle(getMemDC(), isaacRect.left, isaacRect.top, isaacRect.right, isaacRect.bottom);
		Rectangle(getMemDC(), guessRect.left, guessRect.top, guessRect.right, guessRect.bottom);
		Rectangle(getMemDC(), characterChoiceRect.left, characterChoiceRect.top, characterChoiceRect.right, characterChoiceRect.bottom);
	}
	if (KEYMANAGER->isToggleKey(VK_F3))
	{
		//wsprintf(str, TEXT("x : %d, Y : %d"), m_ptMouse.x, m_ptMouse.y);
		//TextOut(getMemDC(), 10, 60, str, lstrlen(str));
	}
}

void CharacterSelect::UpDownMove()
{
	// 난이도 노말
	if (IntersectRect(&tempRect, &nomalRect, &difArrowRect))
	{
		difficulty = NOMAL;

		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			difArrowRect = RectMakeCenter(658, 353, 5, 5); // greed
		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			difArrowRect = RectMakeCenter(663, 315, 5, 5); //hard
		}
	}
	// 난이도 하드
	if (IntersectRect(&tempRect, &hardRect, &difArrowRect))
	{
		difficulty = HARD;

		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			difArrowRect = RectMakeCenter(662, 289, 5, 5); //nomal
		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			difArrowRect = RectMakeCenter(658, 353, 5, 5); // greed
		}
	}
	// 난이도 그리드
	if (IntersectRect(&tempRect, &greedRect, &difArrowRect))
	{
		difficulty = GREED;
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			difArrowRect = RectMakeCenter(663, 315, 5, 5); //hard
		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			difArrowRect = RectMakeCenter(662, 289, 5, 5); //nomal
		}
	}
}

void CharacterSelect::LeftRightMove()
{
	if (IntersectRect(&tempRect, &isaacRect, &characterChoiceRect))
	{
		character = ISAAC;
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			characterChoiceRect = RectMakeCenter(WINSIZEX / 2 - 120, WINSIZEY / 2 - 50, 50, 50);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			characterChoiceRect = RectMakeCenter(WINSIZEX / 2 + 100, WINSIZEY / 2 - 50, 50, 50);
		}
	}
	if (IntersectRect(&tempRect, &eveRect, &characterChoiceRect))
	{
		character = EVE;
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			characterChoiceRect = RectMakeCenter(WINSIZEX / 2 + 100, WINSIZEY / 2 - 50, 50, 50);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			characterChoiceRect = RectMakeCenter(WINSIZEX / 2 - 10, WINSIZEY / 2 - 50, 50, 50);
		}
	}
	if (IntersectRect(&tempRect, &guessRect, &characterChoiceRect))
	{
		character = GUESS;
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			characterChoiceRect = RectMakeCenter(WINSIZEX / 2 - 10, WINSIZEY / 2 - 50, 50, 50);
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			characterChoiceRect = RectMakeCenter(WINSIZEX / 2 - 120, WINSIZEY / 2 - 50, 50, 50);
		}
	}
}
