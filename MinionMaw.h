#pragma once
#include "EnemyBase.h"

class MinionMaw : public EnemyBase
{
private:
	vector<EnemyInfo> vMinionMaw;

	vector<BulletInfo> vEnemyBullet;
	vector<BulletInfo>::iterator viEnemyBullet;

public:
	MinionMaw();
	~MinionMaw();

	HRESULT Init(POINT position, int EnemyNumber);
	void Release();
	void Update();
	void Render(HDC hdc);

	void EnemyAiTime();		// AI�� �����ϴ� �ð�
	void EnemyAi();			// AI
	void EnemyShot();		// ����

	// ���ʹ� ���͸� ��ȯ�ϴ� �Լ�
	vector<EnemyInfo> GetMinionVector() { return vMinionMaw; }

	// ���ʹ� ���͸� ����� �Լ�
	void DeleteEnemy(int num);

	// �浹�� ��ġ ���� �����ִ� �Լ�
	void SetEnemyRectX(int enemyNum, int move);
	void SetEnemyRectY(int enemyNum, int move);

	// ���ʹ� Hp�� �������ִ� �Լ�
	int GetEnemyHp(int enemyNum) { return vMinionMaw[enemyNum].enemyHp; }
	void SetEnemyHp(int enemyNum, int hp) { vMinionMaw[enemyNum].enemyHp = hp; }
};