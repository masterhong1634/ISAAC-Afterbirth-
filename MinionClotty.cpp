#include "stdafx.h"
#include "MinionClotty.h"

MinionClotty::MinionClotty()
{
}

MinionClotty::~MinionClotty()
{
}

HRESULT MinionClotty::Init(POINT position, int EnemyNumber)
{
	//구조체 정보 기입
	EnemyInfo MinionClotty;
	MinionClotty.enemyNumber = EnemyNumber;
	MinionClotty.enemyRect = RectMakeCenter(position.x, position.y, 40, 25);
	MinionClotty.enemyHp = 20;
	MinionClotty.enemyShotSpeed = 5.0f;
	MinionClotty.enemyShotRange = 200.0f;
	MinionClotty.enemyShotDelay = 150;
	MinionClotty.enemySpeed = 2.5f;
	// 애니메이션 Idle
	MinionClotty.enemyShadowImage = IMAGEMANAGER->addImage("ClottyShadow", "images/monster/clotty/clottyShadow.bmp", 120 / 2, 49 / 3, true, RGB(255, 0, 255));
	MinionClotty.enemyImage = IMAGEMANAGER->addFrameImage("ClottyIdle", "images/monster/clotty/clottyAppear.bmp", 980 / 2, 1060 / 2, 4, 5, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addDefAnimation("clotty", "ClottyIdle", 15, false, true);
	minionAni = ANIMATIONMANAGER->findAnimation("clotty");
	vMinionClotty.push_back(MinionClotty);

	// IDLE
	firstEnemyAiPattern = 1;
	secondEnemyAiPattern = 1;
	thirdEnemyAiPattern = 1;

	// 이펙트
	EFFECTMANAGER->addEffect("effectPoof", "images/bullet/effectPoof.bmp", 256 * 2, 256 * 2, 64 * 2, 64 * 2, 30, 1.0f, 10);
	EFFECTMANAGER->play("effectPoof", MinionClotty.enemyRect.left, MinionClotty.enemyRect.top + 20);

	return S_OK;
}

void MinionClotty::Release()
{
}

void MinionClotty::Update()
{
	EnemyAi();
	COLLISIONMANAGER->SameVectorMinionCollision(vMinionClotty);
}

void MinionClotty::Render(HDC hdc)
{
	for (i = 0; i < vMinionClotty.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_F1))
		{
			//Rectangle(hdc, vMinionClotty[i].enemyFireRange.left, vMinionClotty[i].enemyFireRange.top, vMinionClotty[i].enemyFireRange.right, vMinionClotty[i].enemyFireRange.bottom);
			Rectangle(hdc, vMinionClotty[i].enemyRect.left, vMinionClotty[i].enemyRect.top, vMinionClotty[i].enemyRect.right, vMinionClotty[i].enemyRect.bottom);

			HBRUSH brush = CreateSolidBrush(RGB(102, 0, 153));
			FillRect(hdc, &vMinionClotty[i].enemyRect, brush);
			DeleteObject(brush);
		}

		vMinionClotty[i].enemyShadowImage->alphaRender(hdc, vMinionClotty[i].enemyRect.left - 11, vMinionClotty[i].enemyRect.top + 22, 70);
		vMinionClotty[i].enemyImage->aniRender(hdc, vMinionClotty[i].enemyRect.left - 40, vMinionClotty[i].enemyRect.top - 75, minionAni);
	}

	BULLETMANAGER->RenderBullet(hdc, vEnemyBullet, viEnemyBullet);
}

void MinionClotty::EnemyAiTime()
{
	switch (vMinionClotty[i].enemyNumber)
	{
	case 0:
		// AI 패턴 시간
		firstEnemyAiTime++;
		if (firstEnemyAiTime / 60 == 2)
		{
			firstEnemyAiPattern = RND->getFromIntTo(2, 5);
			firstEnemyAiTime = 0;
		}
		break;
	case 1:
		// AI 패턴 시간
		secondEnemyAiTime++;
		if (secondEnemyAiTime / 60 == 2)
		{
			secondEnemyAiPattern = RND->getFromIntTo(2, 5);
			secondEnemyAiTime = 0;
		}
		break;
	case 2:
		// AI 패턴 시간
		thirdEnemyAiTime++;
		if (thirdEnemyAiTime / 60 == 2)
		{
			thirdEnemyAiPattern = RND->getFromIntTo(2, 5);
			thirdEnemyAiTime = 0;
		}
		break;
	}
}

void MinionClotty::EnemyAi()
{
	for (i = 0; i < vMinionClotty.size(); i++)
	{
		// 적 x축, y축 좌표
		vMinionClotty[i].enemyX = vMinionClotty[i].enemyRect.left + (vMinionClotty[i].enemyRect.right - vMinionClotty[i].enemyRect.left) / 2;
		vMinionClotty[i].enemyY = vMinionClotty[i].enemyRect.top + (vMinionClotty[i].enemyRect.bottom - vMinionClotty[i].enemyRect.top) / 2;

		EnemyShot();
		EnemyAiTime();

		switch (vMinionClotty[i].enemyNumber)
		{
		case 0:
			switch (firstEnemyAiPattern)
			{
			case 1:		// IDLE
				// 애니메이션 Idle
				ANIMATIONMANAGER->resume("clotty");
				break;
			case 2:		// LEFT
				// 애니메이션 Left
				vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyMove", "images/monster/clotty/clottyMove.bmp", 1960 / 2, 1060 / 2, 8, 5, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("clottyLeft", "clottyMove", 20, 39, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("clottyLeft");
				ANIMATIONMANAGER->resume("clottyLeft");

				if (vMinionClotty[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
				{
					vMinionClotty[i].enemyRect.left -= vMinionClotty[i].enemySpeed;
					vMinionClotty[i].enemyRect.right -= vMinionClotty[i].enemySpeed;
				}
				if (vMinionClotty[i].enemyRect.left <= 120)
				{
					firstEnemyAiPattern = 3;
				}
				break;
			case 3:		// RIGHT
				// 애니메이션 Right
				vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyMove", "images/monster/clotty/clottyMove.bmp", 1960 / 2, 1060 / 2, 8, 5, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("clottyRight", "clottyMove", 0, 19, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("clottyRight");
				ANIMATIONMANAGER->resume("clottyRight");

				if (vMinionClotty[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
				{
					vMinionClotty[i].enemyRect.left += vMinionClotty[i].enemySpeed;
					vMinionClotty[i].enemyRect.right += vMinionClotty[i].enemySpeed;
				}
				if (vMinionClotty[i].enemyRect.right >= 760)
				{
					firstEnemyAiPattern = 2;
				}
				break;
			case 4:		// UP
				if (vMinionClotty[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
				{
					// 애니메이션 JUMP
					vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyJump", "images/monster/clotty/clottyAttack.bmp", 1960 / 2, 1272 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clottyJumpLeft", "clottyJump", 0, 22, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clottyJumpLeft");
					ANIMATIONMANAGER->resume("clottyJumpLeft");
				}
				else
				{
					// 애니메이션 JUMP
					vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyJump", "images/monster/clotty/clottyAttack.bmp", 1960 / 2, 1272 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clottyJumpRight", "clottyJump", 23, 45, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clottyJumpRight");
					ANIMATIONMANAGER->resume("clottyJumpRight");
				}

				if (vMinionClotty[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
				{
					vMinionClotty[i].enemyRect.top -= vMinionClotty[i].enemySpeed;
					vMinionClotty[i].enemyRect.bottom -= vMinionClotty[i].enemySpeed;
				}
				if (vMinionClotty[i].enemyRect.top <= 120)
				{
					firstEnemyAiPattern = 5;
				}
				break;
			case 5:		// DOWN
				if (vMinionClotty[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
				{
					// 애니메이션 JUMP
					vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyJump", "images/monster/clotty/clottyAttack.bmp", 1960 / 2, 1272 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clottyJumpLeft", "clottyJump", 0, 22, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clottyJumpLeft");
					ANIMATIONMANAGER->resume("clottyJumpLeft");
				}
				else
				{
					// 애니메이션 JUMP
					vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyJump", "images/monster/clotty/clottyAttack.bmp", 1960 / 2, 1272 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clottyJumpRight", "clottyJump", 23, 45, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clottyJumpRight");
					ANIMATIONMANAGER->resume("clottyJumpRight");
				}

				if (vMinionClotty[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
				{
					vMinionClotty[i].enemyRect.top += vMinionClotty[i].enemySpeed;
					vMinionClotty[i].enemyRect.bottom += vMinionClotty[i].enemySpeed;
				}
				if (vMinionClotty[i].enemyRect.bottom >= 450)
				{
					firstEnemyAiPattern = 4;
				}
				break;
			}
			break;
		case 1:
			switch (secondEnemyAiPattern)
			{
			case 1:		// IDLE
				// 애니메이션 Idle
				ANIMATIONMANAGER->resume("clotty");
				break;
			case 2:		// LEFT
				// 애니메이션 Left
				vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyMove", "images/monster/clotty/clottyMove.bmp", 1960 / 2, 1060 / 2, 8, 5, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("clottyLeft", "clottyMove", 20, 39, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("clottyLeft");
				ANIMATIONMANAGER->resume("clottyLeft");

				if (vMinionClotty[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
				{
					vMinionClotty[i].enemyRect.left -= vMinionClotty[i].enemySpeed;
					vMinionClotty[i].enemyRect.right -= vMinionClotty[i].enemySpeed;
				}
				if (vMinionClotty[i].enemyRect.left <= 120)
				{
					firstEnemyAiPattern = 3;
				}
				break;
			case 3:		// RIGHT
				// 애니메이션 Right
				vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyMove", "images/monster/clotty/clottyMove.bmp", 1960 / 2, 1060 / 2, 8, 5, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("clottyRight", "clottyMove", 0, 19, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("clottyRight");
				ANIMATIONMANAGER->resume("clottyRight");

				if (vMinionClotty[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
				{
					vMinionClotty[i].enemyRect.left += vMinionClotty[i].enemySpeed;
					vMinionClotty[i].enemyRect.right += vMinionClotty[i].enemySpeed;
				}
				if (vMinionClotty[i].enemyRect.right >= 760)
				{
					firstEnemyAiPattern = 2;
				}
				break;
			case 4:		// UP
				if (vMinionClotty[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
				{
					// 애니메이션 JUMP
					vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyJump", "images/monster/clotty/clottyAttack.bmp", 1960 / 2, 1272 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clottyJumpLeft", "clottyJump", 0, 22, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clottyJumpLeft");
					ANIMATIONMANAGER->resume("clottyJumpLeft");
				}
				else
				{
					// 애니메이션 JUMP
					vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyJump", "images/monster/clotty/clottyAttack.bmp", 1960 / 2, 1272 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clottyJumpRight", "clottyJump", 23, 45, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clottyJumpRight");
					ANIMATIONMANAGER->resume("clottyJumpRight");
				}

				if (vMinionClotty[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
				{
					vMinionClotty[i].enemyRect.top -= vMinionClotty[i].enemySpeed;
					vMinionClotty[i].enemyRect.bottom -= vMinionClotty[i].enemySpeed;
				}
				if (vMinionClotty[i].enemyRect.top <= 120)
				{
					firstEnemyAiPattern = 5;
				}
				break;
			case 5:		// DOWN
				if (vMinionClotty[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
				{
					// 애니메이션 JUMP
					vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyJump", "images/monster/clotty/clottyAttack.bmp", 1960 / 2, 1272 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clottyJumpLeft", "clottyJump", 0, 22, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clottyJumpLeft");
					ANIMATIONMANAGER->resume("clottyJumpLeft");
				}
				else
				{
					// 애니메이션 JUMP
					vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyJump", "images/monster/clotty/clottyAttack.bmp", 1960 / 2, 1272 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clottyJumpRight", "clottyJump", 23, 45, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clottyJumpRight");
					ANIMATIONMANAGER->resume("clottyJumpRight");
				}

				if (vMinionClotty[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
				{
					vMinionClotty[i].enemyRect.top += vMinionClotty[i].enemySpeed;
					vMinionClotty[i].enemyRect.bottom += vMinionClotty[i].enemySpeed;
				}
				if (vMinionClotty[i].enemyRect.bottom >= 450)
				{
					firstEnemyAiPattern = 4;
				}
				break;
			}
			break;
		case 2:
			switch (thirdEnemyAiPattern)
			{
			case 1:		// IDLE
				// 애니메이션 Idle
				ANIMATIONMANAGER->resume("clotty");
				break;
			case 2:		// LEFT
				// 애니메이션 Left
				vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyMove", "images/monster/clotty/clottyMove.bmp", 1960 / 2, 1060 / 2, 8, 5, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("clottyLeft", "clottyMove", 20, 39, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("clottyLeft");
				ANIMATIONMANAGER->resume("clottyLeft");

				if (vMinionClotty[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
				{
					vMinionClotty[i].enemyRect.left -= vMinionClotty[i].enemySpeed;
					vMinionClotty[i].enemyRect.right -= vMinionClotty[i].enemySpeed;
				}
				if (vMinionClotty[i].enemyRect.left <= 120)
				{
					firstEnemyAiPattern = 3;
				}
				break;
			case 3:		// RIGHT
				// 애니메이션 Right
				vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyMove", "images/monster/clotty/clottyMove.bmp", 1960 / 2, 1060 / 2, 8, 5, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("clottyRight", "clottyMove", 0, 19, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("clottyRight");
				ANIMATIONMANAGER->resume("clottyRight");

				if (vMinionClotty[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
				{
					vMinionClotty[i].enemyRect.left += vMinionClotty[i].enemySpeed;
					vMinionClotty[i].enemyRect.right += vMinionClotty[i].enemySpeed;
				}
				if (vMinionClotty[i].enemyRect.right >= 760)
				{
					firstEnemyAiPattern = 2;
				}
				break;
			case 4:		// UP
				if (vMinionClotty[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
				{
					// 애니메이션 JUMP
					vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyJump", "images/monster/clotty/clottyAttack.bmp", 1960 / 2, 1272 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clottyJumpLeft", "clottyJump", 0, 22, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clottyJumpLeft");
					ANIMATIONMANAGER->resume("clottyJumpLeft");
				}
				else
				{
					// 애니메이션 JUMP
					vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyJump", "images/monster/clotty/clottyAttack.bmp", 1960 / 2, 1272 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clottyJumpRight", "clottyJump", 23, 45, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clottyJumpRight");
					ANIMATIONMANAGER->resume("clottyJumpRight");
				}

				if (vMinionClotty[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
				{
					vMinionClotty[i].enemyRect.top -= vMinionClotty[i].enemySpeed;
					vMinionClotty[i].enemyRect.bottom -= vMinionClotty[i].enemySpeed;
				}
				if (vMinionClotty[i].enemyRect.top <= 120)
				{
					firstEnemyAiPattern = 5;
				}
				break;
			case 5:		// DOWN
				if (vMinionClotty[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
				{
					// 애니메이션 JUMP
					vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyJump", "images/monster/clotty/clottyAttack.bmp", 1960 / 2, 1272 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clottyJumpLeft", "clottyJump", 0, 22, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clottyJumpLeft");
					ANIMATIONMANAGER->resume("clottyJumpLeft");
				}
				else
				{
					// 애니메이션 JUMP
					vMinionClotty[i].enemyImage = IMAGEMANAGER->addFrameImage("clottyJump", "images/monster/clotty/clottyAttack.bmp", 1960 / 2, 1272 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clottyJumpRight", "clottyJump", 23, 45, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clottyJumpRight");
					ANIMATIONMANAGER->resume("clottyJumpRight");
				}

				if (vMinionClotty[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
				{
					vMinionClotty[i].enemyRect.top += vMinionClotty[i].enemySpeed;
					vMinionClotty[i].enemyRect.bottom += vMinionClotty[i].enemySpeed;
				}
				if (vMinionClotty[i].enemyRect.bottom >= 450)
				{
					firstEnemyAiPattern = 4;
				}
				break;
			}
			break;
		}
	}

	// 적이 쏘는 불렛의 움직임
	BULLETMANAGER->MoveEnemyBullet(vEnemyBullet, viEnemyBullet);

	// 적이 쏘는 불렛 충돌
	COLLISIONMANAGER->EnemyBulletCollision(vEnemyBullet, viEnemyBullet);
}

void MinionClotty::EnemyShot()
{
	switch (vMinionClotty[i].enemyNumber)
	{
	case 0:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClotty[i].enemyX, vMinionClotty[i].enemyY,
			ANGLE_0, vMinionClotty[i].enemyShotSpeed, vMinionClotty[i].enemyShotRange, firstEnemyBulletCount, vMinionClotty[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClotty[i].enemyX, vMinionClotty[i].enemyY,
			ANGLE_90, vMinionClotty[i].enemyShotSpeed, vMinionClotty[i].enemyShotRange, firstEnemyBulletCount, vMinionClotty[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClotty[i].enemyX, vMinionClotty[i].enemyY,
			ANGLE_180, vMinionClotty[i].enemyShotSpeed, vMinionClotty[i].enemyShotRange, firstEnemyBulletCount, vMinionClotty[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClotty[i].enemyX, vMinionClotty[i].enemyY,
			ANGLE_270, vMinionClotty[i].enemyShotSpeed, vMinionClotty[i].enemyShotRange, firstEnemyBulletCount, vMinionClotty[i].enemyShotDelay);

		// 적의 불렛 카운트를 한 번에 플러스
		SetFirstEnemyBulletCount();
		break;
	case 1:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClotty[i].enemyX, vMinionClotty[i].enemyY,
			ANGLE_0, vMinionClotty[i].enemyShotSpeed, vMinionClotty[i].enemyShotRange, secondEnemyBulletCount, vMinionClotty[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClotty[i].enemyX, vMinionClotty[i].enemyY,
			ANGLE_90, vMinionClotty[i].enemyShotSpeed, vMinionClotty[i].enemyShotRange, secondEnemyBulletCount, vMinionClotty[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClotty[i].enemyX, vMinionClotty[i].enemyY,
			ANGLE_180, vMinionClotty[i].enemyShotSpeed, vMinionClotty[i].enemyShotRange, secondEnemyBulletCount, vMinionClotty[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClotty[i].enemyX, vMinionClotty[i].enemyY,
			ANGLE_270, vMinionClotty[i].enemyShotSpeed, vMinionClotty[i].enemyShotRange, secondEnemyBulletCount, vMinionClotty[i].enemyShotDelay);

		// 적의 불렛 카운트를 한 번에 플러스
		SetSecondEnemyBulletCount();
		break;
	case 2:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClotty[i].enemyX, vMinionClotty[i].enemyY,
			ANGLE_0, vMinionClotty[i].enemyShotSpeed, vMinionClotty[i].enemyShotRange, thirdEnemyBulletCount, vMinionClotty[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClotty[i].enemyX, vMinionClotty[i].enemyY,
			ANGLE_90, vMinionClotty[i].enemyShotSpeed, vMinionClotty[i].enemyShotRange, thirdEnemyBulletCount, vMinionClotty[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClotty[i].enemyX, vMinionClotty[i].enemyY,
			ANGLE_180, vMinionClotty[i].enemyShotSpeed, vMinionClotty[i].enemyShotRange, thirdEnemyBulletCount, vMinionClotty[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClotty[i].enemyX, vMinionClotty[i].enemyY,
			ANGLE_270, vMinionClotty[i].enemyShotSpeed, vMinionClotty[i].enemyShotRange, thirdEnemyBulletCount, vMinionClotty[i].enemyShotDelay);

		// 적의 불렛 카운트를 한 번에 플러스
		SetThirdEnemyBulletCount();
		break;
	}
}

void MinionClotty::DeleteEnemy(int num)
{
	vMinionClotty.erase(vMinionClotty.begin() + num);
}

void MinionClotty::SetEnemyRectX(int enemyNum, int move)
{
	vMinionClotty[enemyNum].enemyRect.left += move;
	vMinionClotty[enemyNum].enemyRect.right += move;
}

void MinionClotty::SetEnemyRectY(int enemyNum, int move)
{
	vMinionClotty[enemyNum].enemyRect.top += move;
	vMinionClotty[enemyNum].enemyRect.bottom += move;
}
