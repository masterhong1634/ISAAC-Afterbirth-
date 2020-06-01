#pragma once
#include "EnemyBase.h"

class MinionMulligan : public EnemyBase
{
private:
	vector<EnemyInfo> vMinionMulligan;

	vector<BulletInfo> vEnemyBullet;
	vector<BulletInfo>::iterator viEnemyBullet;

	bool moveCheck;
	bool collisionCheck;

public:
	MinionMulligan();
	~MinionMulligan();

	HRESULT Init(POINT position, int EnemyNumber);
	void Release();
	void Update();
	void Render(HDC hdc);

	void EnemyAiTime();
	void EnemyAi();
	void DeleteEnemy();
	void EnemyShot();

	// ���ʹ� ���͸� ��ȯ�ϴ� �Լ�
	vector<EnemyInfo> GetMinionVector() { return vMinionMulligan; }

	// ���ʹ� ���͸� ����� �Լ�
	void DeleteEnemy(int num);

	// �浹�� ��ġ ���� �����ִ� �Լ�
	void SetEnemyRectX(int enemyNum, int move);
	void SetEnemyRectY(int enemyNum, int move);

	// ���ʹ� Hp�� �������ִ� �Լ�
	int GetEnemyHp(int enemyNum) { return vMinionMulligan[enemyNum].enemyHp; }
	void SetEnemyHp(int enemyNum, int hp) { vMinionMulligan[enemyNum].enemyHp = hp; }
};

