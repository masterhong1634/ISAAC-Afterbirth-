#pragma once
#include "singletonBase.h"
#define BULLETMAX 10000
#define BUMBMAX 100

// �Ѿ� ����ü ����
struct BulletInfo
{
	image* bulletImage;
	float angle;
	float speed;
	float bulletX, bulletY;
	float unitX, unitY;
	float range;
	RECT rect;
};
// ��ź ����ü ����
struct BombInfo
{
	image* bombImage;
	int bombX, bombY;
	int damage;
	int range;
	int delayTime;
	RECT rect;
};
class ObjectPool : public singletonBase<ObjectPool>
{
private:

	// �Ѿ� ����ü ����, ���ͷ�����
	vector<BulletInfo> vBulletRepository;
	vector<BulletInfo>::iterator viBulletRepository;

	// ��ź ����ü ����, ���ͷ�����
	vector<BombInfo> vBombRepository;
	vector<BombInfo>::iterator viBombRepository;
	
	// �׽�Ʈ�� 
	char str[128];

public:
	ObjectPool();
	~ObjectPool();

	void Init();
	void Render(HDC hdc);

	// �Ѿ� ���� ���� �Լ�
	void SetBulletVector(BulletInfo vSpentBullet);
	// ��ź ���� ���� �Լ�
	void SetBombVector(BombInfo vSpentBomb);

	// �� �� �Ѿ��� �������� �Լ�
	BulletInfo GetBullet();
	vector<BulletInfo>::iterator GetBulletVectorIterator() { return viBulletRepository; }
	// �� �� ��ź �������� �Լ�
	BombInfo GetBomb();
	vector<BombInfo>::iterator GetBombVectorIterator() { return viBombRepository; }
};

