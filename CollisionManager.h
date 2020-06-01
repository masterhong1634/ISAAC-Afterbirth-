#pragma once
#include"singletonBase.h"
#define KNOCKBACK 3
#define PLAYERHITANIMATION 300
#define PLAYERINVINCIBILITYTIME 1000

class CollisionManager : public singletonBase<CollisionManager>
{
private:
	int direction;
	int hitCount;

	bool playerHit;
	bool playerHitAni;

public:
	CollisionManager();
	~CollisionManager();

	HRESULT Init();

	// 충돌 업데이트
	void UpdateCollision();

	// 플레이어 총알 충돌
	void PlayerBulletCollision(vector<BulletInfo>& playerBulletVector, vector<BulletInfo>::iterator& playerBulletIter);
	
	// 에너미 총알 충돌
	void EnemyBulletCollision(vector<BulletInfo>& enemyBulletVector, vector<BulletInfo>::iterator& enemyBulletIter);
	
	// 플레이어 -> 에너미 렉트 충돌
	void PlayerToMinionCollision();

	// 같은 벡터의 에너미 충돌
	void SameVectorMinionCollision(vector<EnemyInfo>& enemyVector);

	// 다른 벡터의 에너미 충돌
	void MinionToMinionCollision();

	// 플레이어 -> 장애물 렉트 충돌
	void PlayerToObstacleCollision(RECT obstacleRect);
	
	// 에너미 -> 장애물 렉트 충돌
	void EnemyToObstacleCollision(RECT obstacleRect);

	// 플레이어 -> 문 렉트 충돌
	int PlayerCollisionNextDoor(RECT nextDoor);
	
	bool GetplayerHitAni() { return playerHitAni; }

	bool PlayerBulletToObjectCollision(RECT obstacleRect);
};

