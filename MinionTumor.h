#pragma once
#include "EnemyBase.h"

class MinionTumor : public EnemyBase
{
private:
	vector<EnemyInfo> vMinionTumor;

	vector<BulletInfo> vEnemyBullet;
	vector<BulletInfo>::iterator viEnemyBullet;

	int attackCount;		// ���� ī��Ʈ

	bool enemyMove;			// �ڵ� AI
	bool enemyLeftBoost;	// ������ x���� ���� �����̶�� ���� �ν�Ʈ
	bool enemyRightBoost;	// ������ x���� ���� �����̶�� ������ �ν�Ʈ
	bool enemyUpBoost;		// ������ y���� ���� �����̶�� ���� �ν�Ʈ
	bool enemyDownBoost;	// ������ y���� ���� �����̶�� �Ʒ��� �ν�Ʈ

public:
	MinionTumor();
	~MinionTumor();

	HRESULT Init(POINT position, int EnemyNumber);
	void Release();
	void Update();
	void Render(HDC hdc);

	void EnemyAiTime();		// AI�� �����ϴ� �ð�
	void EnemyAi();			// AI
	void EnemyShot();		// ����
	void EnemyAnimation();	// ���� �ִϸ��̼�

	// ���ʹ� ���͸� ��ȯ�ϴ� �Լ�
	vector<EnemyInfo> GetMinionVector() { return vMinionTumor; }

	// ���ʹ� ���͸� ����� �Լ�
	void DeleteEnemy(int num);

	// �浹�� ��ġ ���� �����ִ� �Լ�
	void SetEnemyRectX(int enemyNum, int move);
	void SetEnemyRectY(int enemyNum, int move);

	// ���ʹ� Hp�� �������ִ� �Լ�
	int GetEnemyHp(int enemyNum) { return vMinionTumor[enemyNum].enemyHp; }
	void SetEnemyHp(int enemyNum, int hp) { vMinionTumor[enemyNum].enemyHp = hp; }
};