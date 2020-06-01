#pragma once
#include "EnemyBase.h"

class MinionBlackFly : public EnemyBase
{
private:
	image* blackFly;
	animation* ani;

private:
	vector<EnemyInfo> vMinionBlackFly;

	bool enemyMove;			// �ڵ� AI
	bool enemyLeftBoost;	// ������ x���� ���� �����̶�� ���� �ν�Ʈ
	bool enemyRightBoost;	// ������ x���� ���� �����̶�� ������ �ν�Ʈ
	bool enemyUpBoost;		// ������ y���� ���� �����̶�� ���� �ν�Ʈ
	bool enemyDownBoost;	// ������ y���� ���� �����̶�� �Ʒ��� �ν�Ʈ

public:
	MinionBlackFly();
	~MinionBlackFly();

	HRESULT Init(POINT position, int EnemyNumber);
	void Release();
	void Update();
	void Render(HDC hdc);

	void EnemyAiTime();		// AI�� �����ϴ� �ð�
	void EnemyAi();			// AI

	// ���ʹ� ���͸� ��ȯ�ϴ� �Լ�
	vector<EnemyInfo> GetMinionVector() { return vMinionBlackFly; }
	
	// ���ʹ� ���͸� ����� �Լ�
	void DeleteEnemy(int num);

	// �浹�� ��ġ ���� �����ִ� �Լ�
	void SetEnemyRectX(int enemyNum, int move);
	void SetEnemyRectY(int enemyNum, int move);

	// ���ʹ� Hp�� �������ִ� �Լ�
	int GetEnemyHp(int enemyNum) { return vMinionBlackFly[enemyNum].enemyHp; }
	void SetEnemyHp(int enemyNum, int hp) { vMinionBlackFly[enemyNum].enemyHp = hp; }
};