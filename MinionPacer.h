#pragma once
#include "EnemyBase.h"

class MinionPacer : public EnemyBase
{
private:
	vector<EnemyInfo> vMinionPacer;

	vector<BulletInfo> vEnemyBullet;
	vector<BulletInfo>::iterator viEnemyBullet;

public:
	MinionPacer();
	~MinionPacer();

	HRESULT Init(POINT position, int EnemyNumber);
	void Release();
	void Update();
	void Render(HDC hdc);

	void EnemyAiTime();
	void EnemyAi();

	// ���ʹ� ���͸� ��ȯ�ϴ� �Լ�
	vector<EnemyInfo> GetMinionVector() { return vMinionPacer; }

	// ���ʹ� ���͸� ����� �Լ�
	void DeleteEnemy(int num);

	// �浹�� ��ġ ���� �����ִ� �Լ�
	void SetEnemyRectX(int enemyNum, int move);
	void SetEnemyRectY(int enemyNum, int move);

	// ���ʹ� Hp�� �������ִ� �Լ�
	int GetEnemyHp(int enemyNum) { return vMinionPacer[enemyNum].enemyHp; }
	void SetEnemyHp(int enemyNum, int hp) { vMinionPacer[enemyNum].enemyHp = hp; }
};