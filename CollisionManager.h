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

	// �浹 ������Ʈ
	void UpdateCollision();

	// �÷��̾� �Ѿ� �浹
	void PlayerBulletCollision(vector<BulletInfo>& playerBulletVector, vector<BulletInfo>::iterator& playerBulletIter);
	
	// ���ʹ� �Ѿ� �浹
	void EnemyBulletCollision(vector<BulletInfo>& enemyBulletVector, vector<BulletInfo>::iterator& enemyBulletIter);
	
	// �÷��̾� -> ���ʹ� ��Ʈ �浹
	void PlayerToMinionCollision();

	// ���� ������ ���ʹ� �浹
	void SameVectorMinionCollision(vector<EnemyInfo>& enemyVector);

	// �ٸ� ������ ���ʹ� �浹
	void MinionToMinionCollision();

	// �÷��̾� -> ��ֹ� ��Ʈ �浹
	void PlayerToObstacleCollision(RECT obstacleRect);
	
	// ���ʹ� -> ��ֹ� ��Ʈ �浹
	void EnemyToObstacleCollision(RECT obstacleRect);

	// �÷��̾� -> �� ��Ʈ �浹
	int PlayerCollisionNextDoor(RECT nextDoor);
	
	bool GetplayerHitAni() { return playerHitAni; }

	bool PlayerBulletToObjectCollision(RECT obstacleRect);
};

