#include "stdafx.h"
#include "MinionClot.h"

MinionClot::MinionClot()
{
}

MinionClot::~MinionClot()
{
}

HRESULT MinionClot::Init(POINT position, int EnemyNumber)
{
	//구조체 정보 기입
	EnemyInfo MinionClot;
	MinionClot.enemyNumber = EnemyNumber;
	MinionClot.enemyRect = RectMakeCenter(position.x, position.y, 40, 25);
	MinionClot.enemyHp = 20;
	MinionClot.enemyShotSpeed = 5.0f;
	MinionClot.enemyShotRange = 200.0f;
	MinionClot.enemyShotDelay = 150;
	MinionClot.enemySpeed = 2.0f;
	// 애니메이션 Idle
	MinionClot.enemyShadowImage = IMAGEMANAGER->addImage("ClotShadow", "images/monster/clot/clotShadow.bmp", 120 / 2, 49 / 3, true, RGB(255, 0, 255));
	MinionClot.enemyImage = IMAGEMANAGER->addFrameImage("ClotIdle", "images/monster/clot/clotAppear.bmp", 908 / 2, 1000 / 2, 4, 5, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addDefAnimation("clot", "ClotIdle", 15, false, true);
	minionAni = ANIMATIONMANAGER->findAnimation("clot");
	vMinionClot.push_back(MinionClot);

	// IDLE
	firstEnemyAiPattern = 1;
	secondEnemyAiPattern = 1;
	thirdEnemyAiPattern = 1;

	// 이펙트
	EFFECTMANAGER->addEffect("effectPoof", "images/bullet/effectPoof.bmp", 256 * 2, 256 * 2, 64 * 2, 64 * 2, 30, 1.0f, 10);
	EFFECTMANAGER->play("effectPoof", MinionClot.enemyRect.left, MinionClot.enemyRect.top + 20);

	return S_OK;
}

void MinionClot::Release()
{
}

void MinionClot::Update()
{
	EnemyAi();
	COLLISIONMANAGER->SameVectorMinionCollision(vMinionClot);
}

void MinionClot::Render(HDC hdc)
{
	for (i = 0; i < vMinionClot.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_F1))
		{
			//Rectangle(hdc, vMinionClot[i].enemyFireRange.left, vMinionClot[i].enemyFireRange.top, vMinionClot[i].enemyFireRange.right, vMinionClot[i].enemyFireRange.bottom);
			Rectangle(hdc, vMinionClot[i].enemyRect.left, vMinionClot[i].enemyRect.top, vMinionClot[i].enemyRect.right, vMinionClot[i].enemyRect.bottom);

			HBRUSH brush = CreateSolidBrush(RGB(204, 0, 102));
			FillRect(hdc, &vMinionClot[i].enemyRect, brush);
			DeleteObject(brush);
		}

		vMinionClot[i].enemyShadowImage->alphaRender(hdc, vMinionClot[i].enemyRect.left - 8, vMinionClot[i].enemyRect.top + 25, 70);
		vMinionClot[i].enemyImage->aniRender(hdc, vMinionClot[i].enemyRect.left - 36, vMinionClot[i].enemyRect.top - 65, minionAni);
	}

	BULLETMANAGER->RenderBullet(hdc, vEnemyBullet, viEnemyBullet);
}

void MinionClot::EnemyAiTime()
{
	switch (vMinionClot[i].enemyNumber)
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

void MinionClot::EnemyAi()
{
	for (i = 0; i < vMinionClot.size(); i++)
	{
		// 적 x축, y축 좌표
		vMinionClot[i].enemyX = vMinionClot[i].enemyRect.left + (vMinionClot[i].enemyRect.right - vMinionClot[i].enemyRect.left) / 2;
		vMinionClot[i].enemyY = vMinionClot[i].enemyRect.top + (vMinionClot[i].enemyRect.bottom - vMinionClot[i].enemyRect.top) / 2;

		EnemyAiTime();

		switch (vMinionClot[i].enemyNumber)
		{
		case 0:
			switch (firstEnemyAiPattern)
			{
			case 1:		// IDLE
				// 애니메이션 Idle
				ANIMATIONMANAGER->resume("clot");
				break;
			case 2:		// LEFT
				// 애니메이션 Left
				vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotMove", "images/monster/clot/clotMove.bmp", 908 / 2, 2000 / 2, 4, 10, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("clotLeft", "clotMove", 0, 19, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("clotLeft");
				ANIMATIONMANAGER->resume("clotLeft");

				if (vMinionClot[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
				{
					vMinionClot[i].enemyRect.left -= vMinionClot[i].enemySpeed;
					vMinionClot[i].enemyRect.right -= vMinionClot[i].enemySpeed;
				}
				if (vMinionClot[i].enemyRect.left <= 120)
				{
					firstEnemyAiPattern = 3;
				}
				break;
			case 3:		// RIGHT
				// 애니메이션 Right
				vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotMove", "images/monster/clot/clotMove.bmp", 908 / 2, 2000 / 2, 4, 10, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("clotRight", "clotMove", 20, 39, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("clotRight");
				ANIMATIONMANAGER->resume("clotRight");

				if (vMinionClot[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
				{
					vMinionClot[i].enemyRect.left += vMinionClot[i].enemySpeed;
					vMinionClot[i].enemyRect.right += vMinionClot[i].enemySpeed;
				}
				if (vMinionClot[i].enemyRect.right >= 760)
				{
					firstEnemyAiPattern = 2;
				}
				break;
			case 4:		// UP
				if (vMinionClot[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
				{
					// 애니메이션 JUMP
					vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotJump", "images/monster/clot/clotJump.bmp", 1816 / 2, 1200 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clotJumpLeft", "clotJump", 23, 45, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clotJumpLeft");
					ANIMATIONMANAGER->resume("clotJumpLeft");
				}
				else
				{
					// 애니메이션 JUMP
					vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotJump", "images/monster/clot/clotJump.bmp", 1816 / 2, 1200 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clotJumpRight", "clotJump", 0, 22, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clotJumpRight");
					ANIMATIONMANAGER->resume("clotJumpRight");
				}

				if (vMinionClot[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
				{
					vMinionClot[i].enemyRect.top -= vMinionClot[i].enemySpeed;
					vMinionClot[i].enemyRect.bottom -= vMinionClot[i].enemySpeed;
				}
				if (vMinionClot[i].enemyRect.top <= 120)
				{
					firstEnemyAiPattern = 5;
				}
				break;
			case 5:		// DOWN
				if (vMinionClot[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
				{
					// 애니메이션 JUMP
					vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotJump", "images/monster/clot/clotJump.bmp", 1816 / 2, 1200 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clotJumpLeft", "clotJump", 23, 45, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clotJumpLeft");
					ANIMATIONMANAGER->resume("clotJumpLeft");
				}
				else
				{
					// 애니메이션 JUMP
					vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotJump", "images/monster/clot/clotJump.bmp", 1816 / 2, 1200 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clotJumpRight", "clotJump", 0, 22, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clotJumpRight");
					ANIMATIONMANAGER->resume("clotJumpRight");
				}

				if (vMinionClot[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
				{
					vMinionClot[i].enemyRect.top += vMinionClot[i].enemySpeed;
					vMinionClot[i].enemyRect.bottom += vMinionClot[i].enemySpeed;
				}
				if (vMinionClot[i].enemyRect.bottom >= 450)
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
				ANIMATIONMANAGER->resume("clot");
				break;
			case 2:		// LEFT
				// 애니메이션 Left
				vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotMove", "images/monster/clot/clotMove.bmp", 908 / 2, 2000 / 2, 4, 10, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("clotLeft", "clotMove", 0, 19, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("clotLeft");
				ANIMATIONMANAGER->resume("clotLeft");

				if (vMinionClot[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
				{
					vMinionClot[i].enemyRect.left -= vMinionClot[i].enemySpeed;
					vMinionClot[i].enemyRect.right -= vMinionClot[i].enemySpeed;
				}
				if (vMinionClot[i].enemyRect.left <= 120)
				{
					secondEnemyAiPattern = 3;
				}
				break;
			case 3:		// RIGHT
				// 애니메이션 Right
				vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotMove", "images/monster/clot/clotMove.bmp", 908 / 2, 2000 / 2, 4, 10, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("clotRight", "clotMove", 20, 39, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("clotRight");
				ANIMATIONMANAGER->resume("clotRight");

				if (vMinionClot[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
				{
					vMinionClot[i].enemyRect.left += vMinionClot[i].enemySpeed;
					vMinionClot[i].enemyRect.right += vMinionClot[i].enemySpeed;
				}
				if (vMinionClot[i].enemyRect.right >= 760)
				{
					secondEnemyAiPattern = 2;
				}
				break;
			case 4:		// UP
				if (vMinionClot[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
				{
					// 애니메이션 JUMP
					vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotJump", "images/monster/clot/clotJump.bmp", 1816 / 2, 1200 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clotJumpLeft", "clotJump", 23, 45, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clotJumpLeft");
					ANIMATIONMANAGER->resume("clotJumpLeft");
				}
				else
				{
					// 애니메이션 JUMP
					vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotJump", "images/monster/clot/clotJump.bmp", 1816 / 2, 1200 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clotJumpRight", "clotJump", 0, 22, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clotJumpRight");
					ANIMATIONMANAGER->resume("clotJumpRight");
				}

				if (vMinionClot[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
				{
					vMinionClot[i].enemyRect.top -= vMinionClot[i].enemySpeed;
					vMinionClot[i].enemyRect.bottom -= vMinionClot[i].enemySpeed;
				}
				if (vMinionClot[i].enemyRect.top <= 120)
				{
					secondEnemyAiPattern = 5;
				}
				break;
			case 5:		// DOWN
				if (vMinionClot[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
				{
					// 애니메이션 JUMP
					vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotJump", "images/monster/clot/clotJump.bmp", 1816 / 2, 1200 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clotJumpLeft", "clotJump", 23, 45, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clotJumpLeft");
					ANIMATIONMANAGER->resume("clotJumpLeft");
				}
				else
				{
					// 애니메이션 JUMP
					vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotJump", "images/monster/clot/clotJump.bmp", 1816 / 2, 1200 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clotJumpRight", "clotJump", 0, 22, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clotJumpRight");
					ANIMATIONMANAGER->resume("clotJumpRight");
				}

				if (vMinionClot[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
				{
					vMinionClot[i].enemyRect.top += vMinionClot[i].enemySpeed;
					vMinionClot[i].enemyRect.bottom += vMinionClot[i].enemySpeed;
				}
				if (vMinionClot[i].enemyRect.bottom >= 450)
				{
					secondEnemyAiPattern = 4;
				}
				break;
			}
			break;
		case 2:
			switch (thirdEnemyAiPattern)
			{
			case 1:		// IDLE
				// 애니메이션 Idle
				ANIMATIONMANAGER->resume("clot");
				break;
			case 2:		// LEFT
				// 애니메이션 Left
				vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotMove", "images/monster/clot/clotMove.bmp", 908 / 2, 2000 / 2, 4, 10, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("clotLeft", "clotMove", 0, 19, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("clotLeft");
				ANIMATIONMANAGER->resume("clotLeft");

				if (vMinionClot[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
				{
					vMinionClot[i].enemyRect.left -= vMinionClot[i].enemySpeed;
					vMinionClot[i].enemyRect.right -= vMinionClot[i].enemySpeed;
				}
				if (vMinionClot[i].enemyRect.left <= 120)
				{
					thirdEnemyAiPattern = 3;
				}
				break;
			case 3:		// RIGHT
				// 애니메이션 Right
				vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotMove", "images/monster/clot/clotMove.bmp", 908 / 2, 2000 / 2, 4, 10, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addAnimation("clotRight", "clotMove", 20, 39, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("clotRight");
				ANIMATIONMANAGER->resume("clotRight");

				if (vMinionClot[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
				{
					vMinionClot[i].enemyRect.left += vMinionClot[i].enemySpeed;
					vMinionClot[i].enemyRect.right += vMinionClot[i].enemySpeed;
				}
				if (vMinionClot[i].enemyRect.right >= 760)
				{
					thirdEnemyAiPattern = 2;
				}
				break;
			case 4:		// UP
				if (vMinionClot[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
				{
					// 애니메이션 JUMP
					vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotJump", "images/monster/clot/clotJump.bmp", 1816 / 2, 1200 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clotJumpLeft", "clotJump", 23, 45, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clotJumpLeft");
					ANIMATIONMANAGER->resume("clotJumpLeft");
				}
				else
				{
					// 애니메이션 JUMP
					vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotJump", "images/monster/clot/clotJump.bmp", 1816 / 2, 1200 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clotJumpRight", "clotJump", 0, 22, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clotJumpRight");
					ANIMATIONMANAGER->resume("clotJumpRight");
				}

				if (vMinionClot[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
				{
					vMinionClot[i].enemyRect.top -= vMinionClot[i].enemySpeed;
					vMinionClot[i].enemyRect.bottom -= vMinionClot[i].enemySpeed;
				}
				if (vMinionClot[i].enemyRect.top <= 120)
				{
					thirdEnemyAiPattern = 5;
				}
				break;
			case 5:		// DOWN
				if (vMinionClot[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
				{
					// 애니메이션 JUMP
					vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotJump", "images/monster/clot/clotJump.bmp", 1816 / 2, 1200 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clotJumpLeft", "clotJump", 23, 45, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clotJumpLeft");
					ANIMATIONMANAGER->resume("clotJumpLeft");
				}
				else
				{
					// 애니메이션 JUMP
					vMinionClot[i].enemyImage = IMAGEMANAGER->addFrameImage("clotJump", "images/monster/clot/clotJump.bmp", 1816 / 2, 1200 / 2, 8, 6, true, RGB(255, 0, 255));
					ANIMATIONMANAGER->addAnimation("clotJumpRight", "clotJump", 0, 22, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("clotJumpRight");
					ANIMATIONMANAGER->resume("clotJumpRight");
				}

				if (vMinionClot[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
				{
					vMinionClot[i].enemyRect.top += vMinionClot[i].enemySpeed;
					vMinionClot[i].enemyRect.bottom += vMinionClot[i].enemySpeed;
				}
				if (vMinionClot[i].enemyRect.bottom >= 450)
				{
					thirdEnemyAiPattern = 4;
				}
				break;
			}
			break;
		}

		EnemyShot();
	}

	// 적이 쏘는 불렛의 움직임
	BULLETMANAGER->MoveEnemyBullet(vEnemyBullet, viEnemyBullet);

	// 적이 쏘는 불렛 충돌
	COLLISIONMANAGER->EnemyBulletCollision(vEnemyBullet, viEnemyBullet);
}

void MinionClot::EnemyShot()
{
	switch (vMinionClot[i].enemyNumber)
	{
	case 0:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClot[i].enemyX, vMinionClot[i].enemyY,
			ANGLE_45, vMinionClot[i].enemyShotSpeed, vMinionClot[i].enemyShotRange, firstEnemyBulletCount, vMinionClot[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClot[i].enemyX, vMinionClot[i].enemyY,
			ANGLE_135, vMinionClot[i].enemyShotSpeed, vMinionClot[i].enemyShotRange, firstEnemyBulletCount, vMinionClot[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClot[i].enemyX, vMinionClot[i].enemyY,
			ANGLE_225, vMinionClot[i].enemyShotSpeed, vMinionClot[i].enemyShotRange, firstEnemyBulletCount, vMinionClot[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClot[i].enemyX, vMinionClot[i].enemyY,
			ANGLE_315, vMinionClot[i].enemyShotSpeed, vMinionClot[i].enemyShotRange, firstEnemyBulletCount, vMinionClot[i].enemyShotDelay);

		// 적의 불렛 카운트를 한 번에 플러스
		SetFirstEnemyBulletCount();
		break;
	case 1:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClot[i].enemyX, vMinionClot[i].enemyY,
			ANGLE_45, vMinionClot[i].enemyShotSpeed, vMinionClot[i].enemyShotRange, secondEnemyBulletCount, vMinionClot[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClot[i].enemyX, vMinionClot[i].enemyY,
			ANGLE_135, vMinionClot[i].enemyShotSpeed, vMinionClot[i].enemyShotRange, secondEnemyBulletCount, vMinionClot[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClot[i].enemyX, vMinionClot[i].enemyY,
			ANGLE_225, vMinionClot[i].enemyShotSpeed, vMinionClot[i].enemyShotRange, secondEnemyBulletCount, vMinionClot[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClot[i].enemyX, vMinionClot[i].enemyY,
			ANGLE_315, vMinionClot[i].enemyShotSpeed, vMinionClot[i].enemyShotRange, secondEnemyBulletCount, vMinionClot[i].enemyShotDelay);

		// 적의 불렛 카운트를 한 번에 플러스
		SetSecondEnemyBulletCount();
		break;
	case 2:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClot[i].enemyX, vMinionClot[i].enemyY,
			ANGLE_45, vMinionClot[i].enemyShotSpeed, vMinionClot[i].enemyShotRange, thirdEnemyBulletCount, vMinionClot[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClot[i].enemyX, vMinionClot[i].enemyY,
			ANGLE_135, vMinionClot[i].enemyShotSpeed, vMinionClot[i].enemyShotRange, thirdEnemyBulletCount, vMinionClot[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClot[i].enemyX, vMinionClot[i].enemyY,
			ANGLE_225, vMinionClot[i].enemyShotSpeed, vMinionClot[i].enemyShotRange, thirdEnemyBulletCount, vMinionClot[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionClot[i].enemyX, vMinionClot[i].enemyY,
			ANGLE_315, vMinionClot[i].enemyShotSpeed, vMinionClot[i].enemyShotRange, thirdEnemyBulletCount, vMinionClot[i].enemyShotDelay);

		// 적의 불렛 카운트를 한 번에 플러스
		SetThirdEnemyBulletCount();
		break;
	}
}

void MinionClot::DeleteEnemy(int num)
{
	vMinionClot.erase(vMinionClot.begin() + num);
}

void MinionClot::SetEnemyRectX(int enemyNum, int move)
{
	vMinionClot[enemyNum].enemyRect.left += move;
	vMinionClot[enemyNum].enemyRect.right += move;
}

void MinionClot::SetEnemyRectY(int enemyNum, int move)
{
	vMinionClot[enemyNum].enemyRect.top += move;
	vMinionClot[enemyNum].enemyRect.bottom += move;
}
