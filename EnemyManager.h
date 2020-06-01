#pragma once
#include "singletonBase.h"
#include "Monstro.h"
#include "MinionAttackFly.h"
#include "MinionBlackFly.h"
#include "MinionMaw.h"
#include "MinionTumor.h"
#include "MinionGaper.h"
#include "MinionHorf.h"
#include "MinionMulligan.h"
#include "MinionPacer.h"
#include "MinionClot.h"
#include "MinionClotty.h"

#define ROOM_MAX_X 5
#define ROOM_MAX_Y 5

class EnemyManager : public singletonBase<EnemyManager>
{
private:
	// Ȳ�� ����
	animation*	goldBoxAnimation;
	image*		goldBoxImage;
	RECT		goldBoxRect;
	RECT		temp;

	//����
	Monstro*			m_Monstro;

	// ���߿��ʹ�
	MinionAttackFly*	m_MinionAttackFly;
	MinionBlackFly*		m_MinionBlackFly;
	MinionMaw*			m_MinionMaw;
	MinionTumor*		m_MinionTumor;

	// ���󿡳ʹ�
	MinionClot*			m_MinionClot;
	MinionClotty*		m_MinionClotty;
	MinionGaper*		m_MinionGaper;
	MinionHorf*			m_MinionHorf;
	MinionMulligan*		m_MinionMulligan;
	MinionPacer*		m_MinionPacer;

	int loadData;
	int endCount;

	image* blackBg;
	image* clearImage;
	int badEndCount;

	bool isCheckClear;
	bool isSummonEnemy;
	bool isBoss;
	bool openDoor;
	bool gameEnd;

	// ���� �׽�Ʈ�� ����
	char str[128];

public:
	EnemyManager();
	~EnemyManager();

	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	// �Ѿ� �浹�� �Լ�
	Monstro*			GetMonstro() { return m_Monstro; }

	MinionAttackFly*	GetMinionAttackFly() { return m_MinionAttackFly; }
	MinionBlackFly*		GetMinionBlackFly() { return m_MinionBlackFly; }
	MinionMaw*			GetMinionMaw() { return m_MinionMaw; }
	MinionTumor*		GetMinionTumor() { return m_MinionTumor; }

	MinionClot*			GetMinionClot() { return  m_MinionClot; }
	MinionClotty*		GetMinionClotty() { return m_MinionClotty; }
	MinionGaper*		GetMinionGaper() { return m_MinionGaper; }
	MinionHorf*			GetMinionHorf() { return m_MinionHorf; }
	MinionMulligan*		GetMinionMulligan() { return  m_MinionMulligan; }
	MinionPacer*		GetMinionPacer() { return  m_MinionPacer; }

	//bool isCheckClear[ROOM_MAX_X][ROOM_MAX_Y];
	//bool isSummonEnemy[ROOM_MAX_X][ROOM_MAX_Y];
	//bool isBoss[ROOM_MAX_X][ROOM_MAX_Y];;
	//bool isShop[ROOM_MAX_X][ROOM_MAX_Y];;
	//bool isGoldRoom[ROOM_MAX_X][ROOM_MAX_Y];;

	void SetLoadData(int loadData);
	void SetCheckClear(bool value);
	void SetSummonEnemy(bool value);
	void SetBoss(bool value);

	bool GetOpenDoor() { return openDoor; }
	bool GetGameEnd() { return gameEnd; }
};
