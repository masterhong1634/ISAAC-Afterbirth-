#pragma once
#include "EnemyBase.h"

class Monstro : public EnemyBase
{
private:
	vector<EnemyInfo> vMonstro;

	vector<BulletInfo> vEnemyBullet;
	vector<BulletInfo>::iterator viEnemyBullet;

	int currentHP;				// ������ ���� HP
	int maxHP;					// ������ ��ü HP
	int damage;					// ������ ���� ������
	int shotCount;				// ���� ī��Ʈ
	int jumpCount;				// ���� ī��Ʈ
	int teleportCount;			// �ڷ���Ʈ ī��Ʈ
	int deathCount;				// ��� ī��Ʈ
	float rndX;					// �ڷ���Ʈ �� ���� x��ǥ
	float rndY;					// �ڷ���Ʈ �� ���� y��ǥ
	float jumpPower;			// ���� �Ŀ�
	float gravity;				// �߷°�

	bool jumpCollision;			// ������ �浹 ����
	bool teleportCollision;		// �ڷ���Ʈ�� �浹 ����
	bool teleportImage;			// �ڷ���Ʈ ǥ���� �̹���
	bool teleport;				// �ڷ���Ʈ
	bool jump;					// �������� �� ���� üũ
	bool snowPattern;			// ���� ����
	bool isLeft;				// ���� ���Ͽ��� ������ ������ bool
	bool deathAni;				// ����� �ִϸ��̼� üũ

public:
	Monstro();
	~Monstro();

	HRESULT Init(POINT position);
	void Release();
	void Update();
	void Render(HDC hdc);

	void EnemyDeath();			// ����
	void EnemyAiTime();			// AI�� �����ϴ� �ð�
	void EnemyAi();				// AI
	void EnemyPattern();		// ����
	void EnemySnowShot();		// ���� ����
	void EnemyAllShot();		// ������ ����
	void hitDamage(int _damage);

	// ���ʹ� ���͸� ��ȯ�ϴ� �Լ�
	vector<EnemyInfo> GetMinionVector() { return vMonstro; }

	// ���ʹ� ���͸� ����� �Լ�
	void DeleteEnemy(int num);

	// �浹�� ��ġ ���� �����ִ� �Լ�
	void SetEnemyRectX(int enemyNum, int move);
	void SetEnemyRectY(int enemyNum, int move);

	// ���ʹ� Hp�� �������ִ� �Լ�
	int GetEnemyHp(int enemyNum) { return vMonstro[enemyNum].enemyHp; }
	void SetEnemyHp(int enemyNum, int hp) { vMonstro[enemyNum].enemyHp = hp; }

	// ������ �浹�� �����Ѵ�.
	bool GetJumpCollision() { return jumpCollision; }

	// �ڷ���Ʈ�� �浹�� �����Ѵ�.
	bool GetTeleportCollision() { return teleportCollision; }
};
