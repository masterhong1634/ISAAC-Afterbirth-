#include "stdafx.h"
#include "MinionHorf.h"

MinionHorf::MinionHorf()
{
}

MinionHorf::~MinionHorf()
{
}

HRESULT MinionHorf::Init(POINT position, int EnemyNumber)
{
	//구조체 정보 기입
	EnemyInfo MinionHorf;
	MinionHorf.enemyNumber = EnemyNumber;
	MinionHorf.enemyRect = RectMakeCenter(position.x, position.y, 30, 30);
	MinionHorf.enemyHp = 15;
	MinionHorf.enemyShotSpeed = 6.0f;
	MinionHorf.enemyShotRange = 600.0f;
	MinionHorf.enemyShotDelay = 65;
	// 애니메이션 Idle
	MinionHorf.enemyShadowImage = IMAGEMANAGER->addImage("HorfShadow", "images/monster/horf/horfShadow.bmp", 120 / 3, 49 / 3, true, RGB(255, 0, 255));
	MinionHorf.enemyImage = IMAGEMANAGER->addFrameImage("HorfIdle", "images/monster/horf/horfIdle.bmp", 534 / 2, 172 / 2, 3, 1, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addDefAnimation("horf", "HorfIdle", 25, false, true);
	minionAni = ANIMATIONMANAGER->findAnimation("horf");
	vMinionHorf.push_back(MinionHorf);

	// 이펙트
	EFFECTMANAGER->addEffect("effectPoof", "images/bullet/effectPoof.bmp", 256 * 2, 256 * 2, 64 * 2, 64 * 2, 30, 1.0f, 10);
	EFFECTMANAGER->play("effectPoof", MinionHorf.enemyRect.left, MinionHorf.enemyRect.top + 20);

	return S_OK;
}

void MinionHorf::Release()
{
}

void MinionHorf::Update()
{
	EnemyAi();
	COLLISIONMANAGER->SameVectorMinionCollision(vMinionHorf);
}

void MinionHorf::Render(HDC hdc)
{
	for (i = 0; i < vMinionHorf.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_F1))
		{
			Rectangle(hdc, vMinionHorf[i].enemyFireRange.left, vMinionHorf[i].enemyFireRange.top, vMinionHorf[i].enemyFireRange.right, vMinionHorf[i].enemyFireRange.bottom);
			Rectangle(hdc, vMinionHorf[i].enemyRect.left, vMinionHorf[i].enemyRect.top, vMinionHorf[i].enemyRect.right, vMinionHorf[i].enemyRect.bottom);

			HBRUSH brush = CreateSolidBrush(RGB(51, 102, 255));
			FillRect(hdc, &vMinionHorf[i].enemyRect, brush);
			DeleteObject(brush);
		}

		vMinionHorf[i].enemyShadowImage->alphaRender(hdc, vMinionHorf[i].enemyRect.left - 7, vMinionHorf[i].enemyRect.top + 34, 70);
		vMinionHorf[i].enemyImage->aniRender(hdc, vMinionHorf[i].enemyRect.left - 30, vMinionHorf[i].enemyRect.top - 40, minionAni);
	}

	BULLETMANAGER->RenderBullet(hdc, vEnemyBullet, viEnemyBullet);
}

void MinionHorf::EnemyAi()
{
	for (i = 0; i < vMinionHorf.size(); i++)
	{
		RECT temp;

		// 적 x축, y축 좌표
		vMinionHorf[i].enemyX = vMinionHorf[i].enemyRect.left + (vMinionHorf[i].enemyRect.right - vMinionHorf[i].enemyRect.left) / 2;
		vMinionHorf[i].enemyY = vMinionHorf[i].enemyRect.top + (vMinionHorf[i].enemyRect.bottom - vMinionHorf[i].enemyRect.top) / 2;

		// 플레이어와 판정 범위가 충돌시
		if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &vMinionHorf[i].enemyFireRange) &&
			PLAYERMANAGER->GetPlayerDeath() == false)
		{
			EnemyShot();

			if (vMinionHorf[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
			{
				// 애니메이션 좌측 공격
				vMinionHorf[i].enemyImage = IMAGEMANAGER->addFrameImage("HorfAttack", "images/monster/horf/horfAttack.bmp", 890 / 2, 1376 / 2, 5, 8, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("horfAtkLeft", "HorfAttack", 20, 39, 16, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("horfAtkLeft");
				ANIMATIONMANAGER->resume("horfAtkLeft");
			}
			else
			{
				// 애니메이션 우측 공격
				vMinionHorf[i].enemyImage = IMAGEMANAGER->addFrameImage("HorfAttack", "images/monster/horf/horfAttack.bmp", 890 / 2, 1376 / 2, 5, 8, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("horfAtkRight", "HorfAttack", 0, 19, 16, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("horfAtkRight");
				ANIMATIONMANAGER->resume("horfAtkRight");
			}
		}
		else
		{
			vMinionHorf[i].enemyImage = IMAGEMANAGER->addFrameImage("HorfIdle", "images/monster/horf/horfIdle.bmp", 534 / 2, 172 / 2, 3, 1, true, RGB(255, 0, 255));
			ANIMATIONMANAGER->addDefAnimation("horf", "HorfIdle", 25, false, true);
			minionAni = ANIMATIONMANAGER->findAnimation("horf");
			ANIMATIONMANAGER->resume("horf");
		}

		// 판정 범위가 항상 적의 좌표를 쫓아다님
		vMinionHorf[i].enemyFireRange = RectMakeCenter(vMinionHorf[i].enemyX, vMinionHorf[i].enemyY, 600, WINSIZEY);
	}

	// 적이 쏘는 불렛의 움직임
	BULLETMANAGER->MoveEnemyBullet(vEnemyBullet, viEnemyBullet);

	// 적이 쏘는 불렛 충돌
	COLLISIONMANAGER->EnemyBulletCollision(vEnemyBullet, viEnemyBullet);
}

void MinionHorf::EnemyShot()
{
	switch (vMinionHorf[i].enemyNumber)
	{
	case 0:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionHorf[i].enemyX, vMinionHorf[i].enemyY,
			getAngle(vMinionHorf[i].enemyX, vMinionHorf[i].enemyY,
				PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY()),
			vMinionHorf[i].enemyShotSpeed, vMinionHorf[i].enemyShotRange, firstEnemyBulletCount, vMinionHorf[i].enemyShotDelay);

		// 적의 불렛 카운트를 한 번에 플러스
		SetFirstEnemyBulletCount();
		break;
	case 1:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionHorf[i].enemyX, vMinionHorf[i].enemyY,
			getAngle(vMinionHorf[i].enemyX, vMinionHorf[i].enemyY,
				PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY()),
			vMinionHorf[i].enemyShotSpeed, vMinionHorf[i].enemyShotRange, secondEnemyBulletCount, vMinionHorf[i].enemyShotDelay);

		// 적의 불렛 카운트를 한 번에 플러스
		SetSecondEnemyBulletCount();
		break;
	case 2:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionHorf[i].enemyX, vMinionHorf[i].enemyY,
			getAngle(vMinionHorf[i].enemyX, vMinionHorf[i].enemyY,
				PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY()),
			vMinionHorf[i].enemyShotSpeed, vMinionHorf[i].enemyShotRange, thirdEnemyBulletCount, vMinionHorf[i].enemyShotDelay);

		// 적의 불렛 카운트를 한 번에 플러스
		SetThirdEnemyBulletCount();
		break;
	}
}

void MinionHorf::DeleteEnemy(int num)
{
	vMinionHorf.erase(vMinionHorf.begin() + num);
}

void MinionHorf::SetEnemyRectX(int enemyNum, int move)
{
	vMinionHorf[enemyNum].enemyRect.left += move;
	vMinionHorf[enemyNum].enemyRect.right += move;
}

void MinionHorf::SetEnemyRectY(int enemyNum, int move)
{
	vMinionHorf[enemyNum].enemyRect.top += move;
	vMinionHorf[enemyNum].enemyRect.bottom += move;
}
