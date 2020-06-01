#pragma once

// ���� �Ӽ� ����ü
struct EnemyInfo
{
	image*	enemyShadowImage;	// �� �׸��� �̹���
	image*	enemyImage;			// �� ��ü �̹���
	RECT	enemyHeadRect;		// �� �Ӹ� ����
	RECT	enemyBodyRect;		// �� �� ����
	RECT	enemyRect;			// �� ��ü ��Ʈ
	RECT	enemyFireRange;		// ���� ���� ���� ����
	int		enemyNumber;		// ���� �ѹ�
	int		enemyHp;			// �� ü��
	int		enemyShotDelay;		// ���� �ֱ�
	float	enemyShotSpeed;		// ���ݼӵ�
	float	enemyShotRange;		// ���� ��Ÿ�
	float	enemySpeed;			// �̵��ӵ�
	float	enemyX;				// �� x��ǥ
	float	enemyY;				// �� y��ǥ
};

class EnemyBase
{
protected:
	// �̴Ͼ� �ִϸ��̼�
	animation* minionAni;

	// ���� �ִϸ��̼�
	animation* bossAni;

	// ������ ����
	EnemyInfo enemy;

	int firstEnemyAiTime;		// ù��° �� AI �ൿ�ð�
	int firstEnemyAiPattern;	// ù��° �� AI �ൿ����
	int firstEnemyBulletCount;	// ù��° �� �ҷ� ���͹� ī��Ʈ
	int secondEnemyAiTime;		// �ι�° �� AI �ൿ�ð�
	int secondEnemyAiPattern;	// �ι�° �� AI �ൿ����
	int secondEnemyBulletCount;	// �ι�° �� �ҷ� ���͹� ī��Ʈ
	int thirdEnemyAiTime;		// ����° �� AI �ൿ�ð�
	int thirdEnemyAiPattern;	// ����° �� AI �ൿ����
	int thirdEnemyBulletCount;	// ����° �� �ҷ� ���͹� ī��Ʈ
	int i;						// for���� ����

	float distance;				// ���� �÷��̾��� �Ÿ�
	float vx, vy;				// ���� �ӵ� ����

	bool enemyAreaCheck;		// ���� ���� ������ �÷��̾��� �浹�� üũ�ϴ� ����
	bool enemyAtk;				// �̴Ͼ� ���� üũ
	bool enemyDeath;			// ���� ������ üũ���ִ� ����

public:
	EnemyBase();
	~EnemyBase();

	// ������ �� ���������� �ڸ��� ����ش�. position�� �� ������ PointMake(x, y)
	virtual HRESULT Init(POINT position);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	// ���� �ҷ� ī��Ʈ�� �� ���� �÷���
	int SetFirstEnemyBulletCount() { firstEnemyBulletCount++; return firstEnemyBulletCount; }
	int SetSecondEnemyBulletCount() { secondEnemyBulletCount++; return secondEnemyBulletCount; }
	int SetThirdEnemyBulletCount() { thirdEnemyBulletCount++; return thirdEnemyBulletCount; }
};
