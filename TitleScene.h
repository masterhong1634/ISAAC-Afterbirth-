#pragma once
#include "gameNode.h"

class TitleScene : public gameNode
{
private:

	image titleBgImage;	// 타이틀 배경이미지
	image* titleNameImage;	// 타이틀 이미지
	image* pressImage;	// 타이틀 스타트 스프라이트

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

