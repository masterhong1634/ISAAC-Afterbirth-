#pragma once
#include "gameNode.h"

class MainMenu : public gameNode
{
private:
	image* mainMenuImage;
	image* cursorImage;

	RECT temp;				// �浹�� ��Ʈ
	RECT newRunRect;		// �� ���� ��Ʈ
	RECT mapToolRect;		// ���� ��Ʈ
	RECT optionRect;		// �ɼ� ��Ʈ
	RECT cursorRect;		// Ŀ�� ��Ʈ

	char str[128];

public:
	MainMenu();
	~MainMenu();

	HRESULT init();
	void release();
	void update();
	void render();

	void CursorMove();
};