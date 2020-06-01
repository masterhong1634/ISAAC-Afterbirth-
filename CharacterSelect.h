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

	image* nomalBg;				// �븻 ���
	image* hardBg;				// �ϵ� ���
	image* greedBg;				// �׸��� ���
	image* eveImage;			// ĳ���� �̹���
	image* isaacImage;			// ĳ���� �̹���
	image* guessImage;			// ĳ���� �̹���
	image* nameStat;			// ĳ���� �̸�, ����

	RECT eveRect;				// ĳ���� �̹��� ��Ʈ
	RECT isaacRect;				// ĳ���� �̹��� ��Ʈ
	RECT guessRect;				// ĳ���� �̹��� ��Ʈ
	RECT characterChoiceRect;	// ĳ���� Ŀ�� ��Ʈ
	RECT difArrowRect;			// ���̵� Ŀ�� ��Ʈ
	RECT tempRect;				// ����
	RECT nomalRect;				// �븻 ���
	RECT hardRect;				// �ϵ�	 
	RECT greedRect;				// �׸���

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
