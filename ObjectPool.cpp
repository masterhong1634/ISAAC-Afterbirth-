#include "stdafx.h"
#include "ObjectPool.h"


ObjectPool::ObjectPool()
{
}


ObjectPool::~ObjectPool()
{
}

void ObjectPool::Init()
{
	
	for (int i = 0; i < BULLETMAX; i++)
	{
		BulletInfo bullet;
		vBulletRepository.push_back(bullet);
	}
	for (int i = 0; i < BUMBMAX; i++)
	{
		BombInfo bomb;
		vBombRepository.push_back(bomb);
	}
}

void ObjectPool::Render(HDC hdc)
{
	//sprintf_s((str), "x : %d", m_ptMouse.x);
	//TextOut(hdc, 100, 100, str, strlen(str));

	//sprintf_s((str), "y : %d", m_ptMouse.y);
	//TextOut(hdc, 100, 120, str, strlen(str));
}

void ObjectPool::SetBulletVector(BulletInfo vSpentBullet)
{
	vBulletRepository.push_back(vSpentBullet);
}

void ObjectPool::SetBombVector(BombInfo vSpentBomb)
{
	vBombRepository.push_back(vSpentBomb);
}

BulletInfo ObjectPool::GetBullet()
{
	BulletInfo getBullet;
	getBullet = vBulletRepository.back();
	vBulletRepository.pop_back();
	return getBullet;
}

BombInfo ObjectPool::GetBomb()
{
	BombInfo getBomb;
	getBomb = vBombRepository.back();
	vBombRepository.pop_back();
	return getBomb;
}
