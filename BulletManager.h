#pragma once
#include "singletonBase.h"

class BulletManager : public singletonBase<BulletManager>
{
private:

public:
	image* bombImage;
	animation* bomb;
	BulletManager();
	~BulletManager();

	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	// 총알 이미지 이름, 총알을 담는 벡터, 플레이어&총알 x, y, 각도, 총알 속도, 총알 사정거리, 인터벌 돌리는 변수, 인터벌을 얼마나 줄것인지에 대한 변수
	void ShootBullet(string imageName, vector<BulletInfo>& bulletVector, float x, float y, float angle, float speed, float range, int count, int interval);
	// 폭탄 이미지 이름, 폭탄을 담는 벡터, 플레이어&폭탄 x, y, 데미지, 사정거리, 지연시간에 대한 변수
	void ShootBomb(string imageName, vector<BombInfo>& bombVector, int x, int y, int damage, int range, int delayTime);

	// 총알을 담은 벡터와 이터레이터를 넣어준다.
	void MovePlayerBullet(vector<BulletInfo>& bulletVector, vector<BulletInfo>::iterator& bulletIter);
	void MoveEnemyBullet(vector<BulletInfo>& bulletVector, vector<BulletInfo>::iterator& bulletIter);
	void RemoveBomb(vector<BombInfo>& bombVector, vector<BombInfo>::iterator& bombIter);

	void RenderBullet(HDC hdc, vector<BulletInfo>& bulletVector, vector<BulletInfo>::iterator& bulletIter);

	void RenderBomb(HDC hdc, vector<BombInfo>& bombVector, vector<BombInfo>::iterator& bombIter);
};

