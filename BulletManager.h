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

	// �Ѿ� �̹��� �̸�, �Ѿ��� ��� ����, �÷��̾�&�Ѿ� x, y, ����, �Ѿ� �ӵ�, �Ѿ� �����Ÿ�, ���͹� ������ ����, ���͹��� �󸶳� �ٰ������� ���� ����
	void ShootBullet(string imageName, vector<BulletInfo>& bulletVector, float x, float y, float angle, float speed, float range, int count, int interval);
	// ��ź �̹��� �̸�, ��ź�� ��� ����, �÷��̾�&��ź x, y, ������, �����Ÿ�, �����ð��� ���� ����
	void ShootBomb(string imageName, vector<BombInfo>& bombVector, int x, int y, int damage, int range, int delayTime);

	// �Ѿ��� ���� ���Ϳ� ���ͷ����͸� �־��ش�.
	void MovePlayerBullet(vector<BulletInfo>& bulletVector, vector<BulletInfo>::iterator& bulletIter);
	void MoveEnemyBullet(vector<BulletInfo>& bulletVector, vector<BulletInfo>::iterator& bulletIter);
	void RemoveBomb(vector<BombInfo>& bombVector, vector<BombInfo>::iterator& bombIter);

	void RenderBullet(HDC hdc, vector<BulletInfo>& bulletVector, vector<BulletInfo>::iterator& bulletIter);

	void RenderBomb(HDC hdc, vector<BombInfo>& bombVector, vector<BombInfo>::iterator& bombIter);
};

