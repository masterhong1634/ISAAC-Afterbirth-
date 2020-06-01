#pragma once
#include "gameNode.h"

class TitleScene : public gameNode
{
private:

	image titleBgImage;	// Ÿ��Ʋ ����̹���
	image* titleNameImage;	// Ÿ��Ʋ �̹���
	image* pressImage;	// Ÿ��Ʋ ��ŸƮ ��������Ʈ

	animation* ani;

	RECT nameRect;
	RECT pressRect;

	char str[128];

public:
	TitleScene();
	~TitleScene();

	HRESULT init();
	void release();
	void update();
	void render();
};

