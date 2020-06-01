#pragma once
#include "EnemyBase.h"

class MinionAttackFly : public EnemyBase
{
private:
	vector<EnemyInfo> vMinionAttackFly;

	vector<BulletInfo> vEnemyBullet;
	vector<BulletInfo>::iterator viEnemyBullet;

public:
	MinionAttackFly();
	~MinionAttackFly();

	HRESULT Init(POINT position, int EnemyNumber);
	void Release();
	void Update();
	void Render(HDC hdc);

	void EnemyAiTime();		// AI�� �����ϴ� �ð�
	void EnemyAi();			// AI

	// ���ʹ� ���͸� ��ȯ�ϴ� �Լ�
	vector<EnemyInfo> GetMinionVector() { return vMinionAttackFly; }

	// ���ʹ� ���͸� ����� �Լ�
	void DeleteEnemy(int num);

	// �浹�� ��ġ ���� �����ִ� �Լ�
	void SetEnemyRectX(int enemyNum, int move);
	void SetEnemyRectY(int enemyNum, int move);

	// ���ʹ� Hp�� �������ִ� �Լ�
	int GetEnemyHp(int enemyNum) { return vMinionAttackFly[enemyNum].enemyHp; }
	void SetEnemyHp(int enemyNum, int hp) { vMinionAttackFly[enemyNum].enemyHp = hp; }
};