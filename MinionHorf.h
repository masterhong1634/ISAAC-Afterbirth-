#pragma once
#include "EnemyBase.h"

class MinionHorf : public EnemyBase
{
private:
	vector<EnemyInfo> vMinionHorf;

	vector<BulletInfo> vEnemyBullet;
	vector<BulletInfo>::iterator viEnemyBullet;

public:
	MinionHorf();
	~MinionHorf();

	HRESULT Init(POINT position, int EnemyNumber);
	void Release();
	void Update();
	void Render(HDC hdc);

	void EnemyAi();
	void EnemyShot();

	// ���ʹ� ���͸� ��ȯ�ϴ� �Լ�
	vector<EnemyInfo> GetMinionVector() { return vMinionHorf; }

	// ���ʹ� ���͸� ����� �Լ�
	void DeleteEnemy(int num);

	// �浹�� ��ġ ���� �����ִ� �Լ�
	void SetEnemyRectX(int enemyNum, int move);
	void SetEnemyRectY(int enemyNum, int move);

	// ���ʹ� Hp�� �������ִ� �Լ�
	int GetEnemyHp(int enemyNum) { return vMinionHorf[enemyNum].enemyHp; }
	void SetEnemyHp(int enemyNum, int hp) { vMinionHorf[enemyNum].enemyHp = hp; }
};