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

	// 에너미 벡터를 반환하는 함수
	vector<EnemyInfo> GetMinionVector() { return vMinionPacer; }

	// 에너미 벡터를 지우는 함수
	void DeleteEnemy(int num);

	// 충돌시 위치 변경 시켜주는 함수
	void SetEnemyRectX(int enemyNum, int move);
	void SetEnemyRectY(int enemyNum, int move);

	// 에너미 Hp를 세팅해주는 함수
	int GetEnemyHp(int enemyNum) { return vMinionPacer[enemyNum].enemyHp; }
	void SetEnemyHp(int enemyNum, int hp) { vMinionPacer[enemyNum].enemyHp = hp; }
};