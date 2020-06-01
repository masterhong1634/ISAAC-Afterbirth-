#pragma once
#include "singletonBase.h"
#define BULLETMAX 10000
#define BUMBMAX 100

// 총알 구조체 정보
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
// 폭탄 구조체 정보
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

	// 총알 구조체 벡터, 이터레이터
	vector<BulletInfo> vBulletRepository;
	vector<BulletInfo>::iterator viBulletRepository;

	// 폭탄 구조체 벡터, 이터레이터
	vector<BombInfo> vBombRepository;
	vector<BombInfo>::iterator viBombRepository;
	
	// 테스트용 
	char str[128];

public:
	ObjectPool();
	~ObjectPool();

	void Init();
	void Render(HDC hdc);

	// 총알 정보 세팅 함수
	void SetBulletVector(BulletInfo vSpentBullet);
	// 폭탄 정보 세팅 함수
	void SetBombVector(BombInfo vSpentBomb);

	// 다 쓴 총알을 가져오는 함수
	BulletInfo GetBullet();
	vector<BulletInfo>::iterator GetBulletVectorIterator() { return viBulletRepository; }
	// 다 쓴 폭탄 가져오는 함수
	BombInfo GetBomb();
	vector<BombInfo>::iterator GetBombVectorIterator() { return viBombRepository; }
};

