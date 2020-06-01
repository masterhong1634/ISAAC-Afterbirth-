#pragma once
#include"gameNode.h"

enum DIFFICULTY
{
	NOMAL,
	HARD,
	GREED
};

enum CHARACTER
{
	ISAAC,
	GUESS,
	EVE
};

class CharacterSelect : public gameNode
{
private:
	DIFFICULTY difficulty;
	CHARACTER character;

	image* nomalBg;				// 노말 배경
	image* hardBg;				// 하드 배경
	image* greedBg;				// 그리드 배경
	image* eveImage;			// 캐릭터 이미지
	image* isaacImage;			// 캐릭터 이미지
	image* guessImage;			// 캐릭터 이미지
	image* nameStat;			// 캐릭터 이름, 스탯

	RECT eveRect;				// 캐릭터 이미지 렉트
	RECT isaacRect;				// 캐릭터 이미지 렉트
	RECT guessRect;				// 캐릭터 이미지 렉트
	RECT characterChoiceRect;	// 캐릭터 커서 렉트
	RECT difArrowRect;			// 난이도 커서 렉트
	RECT tempRect;				// 템프
	RECT nomalRect;				// 노말 배경
	RECT hardRect;				// 하드	 
	RECT greedRect;				// 그리드

	char str[126];

public:
	CharacterSelect();
	~CharacterSelect();

	HRESULT init();
	void release();
	void update();
	void render();

	void UpDownMove();
	void LeftRightMove();
};
