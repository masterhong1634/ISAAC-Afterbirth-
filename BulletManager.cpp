#include "stdafx.h"
#include "BulletManager.h"

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
}

HRESULT BulletManager::Init()
{
	// 총알 이미지 추가
	IMAGEMANAGER->addImage("playerBullet", "images/bullet/playerBullet.bmp", 18 * 2, 18 * 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("enemyBullet", "images/bullet/enemyBullet.bmp", 18 * 2, 18 * 2, true, RGB(255, 0, 255));

	// 폭탄 이미지 추가
	bombImage = IMAGEMANAGER->addFrameImage("playerBomb", "images/bullet/bombPulse.bmp", 1730, 1002, 10, 6, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addDefAnimation("playerBomb", "playerBomb", 30, false, false);
	bomb = ANIMATIONMANAGER->findAnimation("playerBomb");
	ANIMATIONMANAGER->start("playerBomb");

	//IMAGEMANAGER->addFrameImage("boom", "images/bullet/boom.bmp", 3610 / 4, 2332 / 4, 5, 4, true, RGB(255, 0, 255));
	//ANIMATIONMANAGER->addDefAnimation("boomAni", "boom", 30, false, false);

	EFFECTMANAGER->addEffect("boom", "images/bullet/boom.bmp", 3610 / 2, 2332 / 2, 722 / 2, 583 / 2, 30, 1.0f, 10);
	EFFECTMANAGER->addEffect("tearpoof", "images/bullet/tearpoof.bmp", 256 * 2, 256 * 2, 64 * 2, 64 * 2, 30, 1.0f, 10);
	EFFECTMANAGER->addEffect("enemyBloodpoof", "images/bullet/enemyBloodpoof2.bmp", 256 * 2, 256 * 2, 64 * 2, 64 * 2, 30, 1.0f, 10);

	return S_OK;
}

void BulletManager::Release()
{
}

void BulletManager::Update()
{

}

void BulletManager::Render(HDC hdc)
{
}

void BulletManager::ShootBullet(string imageName, vector<BulletInfo>& bulletVector, float x, float y, float angle, float speed, float range, int count, int interval)
{
	// 카운트의 인터벌 모드 0이 될때마다 값을 조정해준다.
	if (count % interval == 0)
	{
		BulletInfo bullet;
		bullet = OBJECTPOOL->GetBullet();
		bullet.bulletImage = IMAGEMANAGER->findImage(imageName);
		bullet.angle = angle;
		bullet.speed = speed;
		bullet.bulletX = bullet.unitX = x;
		bullet.bulletY = bullet.unitY = y;
		bullet.range = range;
		bullet.rect = RectMakeCenter(bullet.bulletX, bullet.bulletY, bullet.bulletImage->getWidth(), bullet.bulletImage->getHeight());
		bulletVector.push_back(bullet);
	}
}

void BulletManager::ShootBomb(string imageName, vector<BombInfo>& bombVector, int x, int y, int damage, int range, int delayTime)
{
	if (PLAYERMANAGER->GetPlayerBomb() > 0)
	{
		/*
			image* bombImage;
			int bombX, bombY;
			int damage;
			int range;
			int delayTime;
			RECT rect;
		*/
		BombInfo bomb;
		bomb = OBJECTPOOL->GetBomb();
		bomb.bombImage = IMAGEMANAGER->findImage(imageName);
		bomb.bombX = x;
		bomb.bombY = y;
		bomb.damage = damage;
		bomb.range = range;
		bomb.delayTime = delayTime;
		bomb.rect = RectMakeCenter(bomb.bombX, bomb.bombY, bomb.bombImage->getWidth(), bomb.bombImage->getHeight());
		bombVector.push_back(bomb);
		PLAYERMANAGER->SetPlayerBomb(-1);
	}
}

void BulletManager::MovePlayerBullet(vector<BulletInfo>& bulletVector, vector<BulletInfo>::iterator & bulletIter)
{
	// 넣어둔 벡터의 이터레이터를 돌면서 값을 증가시켜 총알을 움직여준다.
	for (bulletIter = bulletVector.begin(); bulletIter != bulletVector.end();)
	{
		bulletIter->bulletX += cosf(bulletIter->angle) * bulletIter->speed;
		bulletIter->bulletY += -sinf(bulletIter->angle) * bulletIter->speed;

		bulletIter->rect = RectMakeCenter(bulletIter->bulletX, bulletIter->bulletY, bulletIter->bulletImage->getWidth() - 13, bulletIter->bulletImage->getHeight() - 13);
		//if (400 < getDistance(bulletIter->x, bulletIter->y, bulletIter->fireX, bulletIter->fireY))
		//{
		//   bulletIter->y += 1;
		//}
		RECT temp;
		if (bulletIter->range < getDistance(bulletIter->bulletX, bulletIter->bulletY, bulletIter->unitX, bulletIter->unitY))
		{
			OBJECTPOOL->SetBulletVector(bulletVector.front());
			EFFECTMANAGER->play("tearpoof", bulletIter->rect.left, bulletIter->rect.top + 20);
			bulletIter = bulletVector.erase(bulletIter);
		}
		else
		{
			++bulletIter;
		}
	}
}

void BulletManager::MoveEnemyBullet(vector<BulletInfo>& bulletVector, vector<BulletInfo>::iterator & bulletIter)
{
	// 넣어둔 벡터의 이터레이터를 돌면서 값을 증가시켜 총알을 움직여준다.
	for (bulletIter = bulletVector.begin(); bulletIter != bulletVector.end();)
	{
		bulletIter->bulletX += cosf(bulletIter->angle) * bulletIter->speed;
		bulletIter->bulletY += -sinf(bulletIter->angle) * bulletIter->speed;

		bulletIter->rect = RectMakeCenter(bulletIter->bulletX, bulletIter->bulletY, bulletIter->bulletImage->getWidth() - 13, bulletIter->bulletImage->getHeight() - 13);
		//if (400 < getDistance(bulletIter->x, bulletIter->y, bulletIter->fireX, bulletIter->fireY))
		//{
		//   bulletIter->y += 1;
		//}
		RECT temp;
		if (bulletIter->range < getDistance(bulletIter->bulletX, bulletIter->bulletY, bulletIter->unitX, bulletIter->unitY))
		{
			OBJECTPOOL->SetBulletVector(bulletVector.front());
			EFFECTMANAGER->play("enemyBloodpoof", bulletIter->rect.left, bulletIter->rect.top + 20);
			bulletIter = bulletVector.erase(bulletIter);
		}
		else
		{
			++bulletIter;
		}
	}
}

void BulletManager::RemoveBomb(vector<BombInfo>& bombVector, vector<BombInfo>::iterator & bombIter)
{
	for (bombIter = bombVector.begin(); bombIter != bombVector.end();)
	{
		bombIter->delayTime++;
		if (bombIter->delayTime > 120)
		{
			OBJECTPOOL->SetBombVector(bombVector.front());
			EFFECTMANAGER->play("boom", bombIter->rect.left + 80, bombIter->rect.top + 120);
			bombIter = bombVector.erase(bombIter);
		}
		else
		{
			++bombIter;
		}
	}
}

void BulletManager::RenderBullet(HDC hdc, vector<BulletInfo>& bulletVector, vector<BulletInfo>::iterator & bulletIter)
{
	//이터레이터를 돌면서 총알을 그려준다.
	bulletIter = bulletVector.begin();

	for (bulletIter; bulletIter != bulletVector.end(); ++bulletIter)
	{
		if (KEYMANAGER->isToggleKey(VK_F2))
		{
			Rectangle(hdc, bulletIter->rect.left, bulletIter->rect.top, bulletIter->rect.right, bulletIter->rect.bottom);
		}

		bulletIter->bulletImage->render(hdc, bulletIter->rect.left - 6, bulletIter->rect.top - 6);
	}
}

void BulletManager::RenderBomb(HDC hdc, vector<BombInfo>& bombVector, vector<BombInfo>::iterator & bombIter)
{
	//이터레이터를 돌면서 폭탄을 그려준다.
	bombIter = bombVector.begin();

	for (bombIter; bombIter != bombVector.end(); ++bombIter)
	{
		bombIter->bombImage->aniRender(hdc, bombIter->rect.left, bombIter->rect.top, bomb);
		//bombIter->bombImage->render(hdc, bombIter->rect.left, bombIter->rect.top);
	}
}
