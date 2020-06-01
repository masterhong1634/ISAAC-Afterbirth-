#include "stdafx.h"
#include "MinionPacer.h"

MinionPacer::MinionPacer()
{
}

MinionPacer::~MinionPacer()
{
}

HRESULT MinionPacer::Init(POINT position, int EnemyNumber)
{
	//구조체 정보 기입
	EnemyInfo MinionPacer;
	MinionPacer.enemyNumber = EnemyNumber;
	MinionPacer.enemyRect = RectMakeCenter(position.x, position.y, 30, 22);
	MinionPacer.enemyHp = 15;
	MinionPacer.enemySpeed = 2.5f;
	// 애니메이션 Idle
	MinionPacer.enemyShadowImage = IMAGEMANAGER->addImage("PacerShadow", "images/monster/pacer/pacerShadow.bmp", 120 / 3, 49 / 3, true, RGB(255, 0, 255));
	MinionPacer.enemyImage = IMAGEMANAGER->addFrameImage("PacerMove", "images/monster/pacer/pacerMove.bmp", 1806 / 2, 690 / 2, 14, 5, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addAnimation("pacer", "PacerMove", 0, 21, 15, false, true);
	minionAni = ANIMATIONMANAGER->findAnimation("pacer");
	vMinionPacer.push_back(MinionPacer);

	// 이펙트
	EFFECTMANAGER->addEffect("effectPoof", "images/bullet/effectPoof.bmp", 256 * 2, 256 * 2, 64 * 2, 64 * 2, 30, 1.0f, 10);
	EFFECTMANAGER->play("effectPoof", MinionPacer.enemyRect.left, MinionPacer.enemyRect.top + 20);

	return S_OK;
}

void MinionPacer::Release()
{
}

void MinionPacer::Update()
{
	EnemyAi();
	COLLISIONMANAGER->SameVectorMinionCollision(vMinionPacer);
}

void MinionPacer::Render(HDC hdc)
{
	for (i = 0; i < vMinionPacer.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_F1))
		{
			//Rectangle(hdc, vMinionPacer[i].enemyFireRange.left, vMinionPacer[i].enemyFireRange.top, vMinionPacer[i].enemyFireRange.right, vMinionPacer[i].enemyFireRange.bottom);
			Rectangle(hdc, vMinionPacer[i].enemyRect.left, vMinionPacer[i].enemyRect.top, vMinionPacer[i].enemyRect.right, vMinionPacer[i].enemyRect.bottom);

			HBRUSH brush = CreateSolidBrush(RGB(102, 255, 204));
			FillRect(hdc, &vMinionPacer[i].enemyRect, brush);
			DeleteObject(brush);
		}

		vMinionPacer[i].enemyShadowImage->alphaRender(hdc, vMinionPacer[i].enemyRect.left - 5, vMinionPacer[i].enemyRect.top + 13, 70);
		vMinionPacer[i].enemyImage->aniRender(hdc, vMinionPacer[i].enemyRect.left - 18, vMinionPacer[i].enemyRect.top - 25, minionAni);
	}

	BULLETMANAGER->RenderBullet(hdc, vEnemyBullet, viEnemyBullet);
}

void MinionPacer::EnemyAiTime()
{
	switch (vMinionPacer[i].enemyNumber)
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

void MinionPacer::EnemyAi()
{
	for (i = 0; i < vMinionPacer.size(); i++)
	{
		EnemyAiTime();

		switch (vMinionPacer[i].enemyNumber)
		{
		case 0:
			switch (firstEnemyAiPattern)
			{
			case 1:		// IDLE
				// 애니메이션 Idle
				ANIMATIONMANAGER->stop("pacer");
				break;
			case 2:		// LEFT
				// 애니메이션 Left
				ANIMATIONMANAGER->addAnimation("pacerLeft", "PacerMove", 44, 65, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("pacerLeft");
				ANIMATIONMANAGER->resume("pacerLeft");

				if (vMinionPacer[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
				{
					vMinionPacer[i].enemyRect.left -= vMinionPacer[i].enemySpeed;
					vMinionPacer[i].enemyRect.right -= vMinionPacer[i].enemySpeed;
				}
				if (vMinionPacer[i].enemyRect.left <= 120)
				{
					firstEnemyAiPattern = 3;
				}
				break;
			case 3:		// RIGHT
				// 애니메이션 RIGHT
				ANIMATIONMANAGER->addAnimation("pacerRight", "PacerMove", 22, 43, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("pacerRight");
				ANIMATIONMANAGER->resume("pacerRight");

				if (vMinionPacer[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
				{
					vMinionPacer[i].enemyRect.left += vMinionPacer[i].enemySpeed;
					vMinionPacer[i].enemyRect.right += vMinionPacer[i].enemySpeed;
				}
				if (vMinionPacer[i].enemyRect.right >= 760)
				{
					firstEnemyAiPattern = 2;
				}
				break;
			case 4:		// UP
				// 애니메이션 Up
				ANIMATIONMANAGER->addAnimation("pacerUp", "PacerMove", 0, 21, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("pacerUp");
				ANIMATIONMANAGER->resume("pacerUp");

				if (vMinionPacer[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
				{
					vMinionPacer[i].enemyRect.top -= vMinionPacer[i].enemySpeed;
					vMinionPacer[i].enemyRect.bottom -= vMinionPacer[i].enemySpeed;
				}
				if (vMinionPacer[i].enemyRect.top <= 120)
				{
					firstEnemyAiPattern = 5;
				}
				break;
			case 5:		// DOWN
				// 애니메이션 Down
				ANIMATIONMANAGER->addAnimation("pacerDown", "PacerMove", 0, 21, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("pacerDown");
				ANIMATIONMANAGER->resume("pacerDown");

				if (vMinionPacer[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
				{
					vMinionPacer[i].enemyRect.top += vMinionPacer[i].enemySpeed;
					vMinionPacer[i].enemyRect.bottom += vMinionPacer[i].enemySpeed;
				}
				if (vMinionPacer[i].enemyRect.bottom >= 450)
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
				ANIMATIONMANAGER->stop("pacer");
				break;
			case 2:		// LEFT
				// 애니메이션 Left
				ANIMATIONMANAGER->addAnimation("pacerLeft", "PacerMove", 44, 65, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("pacerLeft");
				ANIMATIONMANAGER->resume("pacerLeft");

				if (vMinionPacer[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
				{
					vMinionPacer[i].enemyRect.left -= vMinionPacer[i].enemySpeed;
					vMinionPacer[i].enemyRect.right -= vMinionPacer[i].enemySpeed;
				}
				if (vMinionPacer[i].enemyRect.left <= 120)
				{
					firstEnemyAiPattern = 3;
				}
				break;
			case 3:		// RIGHT
				// 애니메이션 RIGHT
				ANIMATIONMANAGER->addAnimation("pacerRight", "PacerMove", 22, 43, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("pacerRight");
				ANIMATIONMANAGER->resume("pacerRight");

				if (vMinionPacer[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
				{
					vMinionPacer[i].enemyRect.left += vMinionPacer[i].enemySpeed;
					vMinionPacer[i].enemyRect.right += vMinionPacer[i].enemySpeed;
				}
				if (vMinionPacer[i].enemyRect.right >= 760)
				{
					firstEnemyAiPattern = 2;
				}
				break;
			case 4:		// UP
				// 애니메이션 Up
				ANIMATIONMANAGER->addAnimation("pacerUp", "PacerMove", 0, 21, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("pacerUp");
				ANIMATIONMANAGER->resume("pacerUp");

				if (vMinionPacer[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
				{
					vMinionPacer[i].enemyRect.top -= vMinionPacer[i].enemySpeed;
					vMinionPacer[i].enemyRect.bottom -= vMinionPacer[i].enemySpeed;
				}
				if (vMinionPacer[i].enemyRect.top <= 120)
				{
					firstEnemyAiPattern = 5;
				}
				break;
			case 5:		// DOWN
				// 애니메이션 Down
				ANIMATIONMANAGER->addAnimation("pacerDown", "PacerMove", 0, 21, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("pacerDown");
				ANIMATIONMANAGER->resume("pacerDown");

				if (vMinionPacer[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
				{
					vMinionPacer[i].enemyRect.top += vMinionPacer[i].enemySpeed;
					vMinionPacer[i].enemyRect.bottom += vMinionPacer[i].enemySpeed;
				}
				if (vMinionPacer[i].enemyRect.bottom >= 450)
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
				ANIMATIONMANAGER->stop("pacer");
				break;
			case 2:		// LEFT
				// 애니메이션 Left
				ANIMATIONMANAGER->addAnimation("pacerLeft", "PacerMove", 44, 65, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("pacerLeft");
				ANIMATIONMANAGER->resume("pacerLeft");

				if (vMinionPacer[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
				{
					vMinionPacer[i].enemyRect.left -= vMinionPacer[i].enemySpeed;
					vMinionPacer[i].enemyRect.right -= vMinionPacer[i].enemySpeed;
				}
				if (vMinionPacer[i].enemyRect.left <= 120)
				{
					firstEnemyAiPattern = 3;
				}
				break;
			case 3:		// RIGHT
				// 애니메이션 RIGHT
				ANIMATIONMANAGER->addAnimation("pacerRight", "PacerMove", 22, 43, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("pacerRight");
				ANIMATIONMANAGER->resume("pacerRight");

				if (vMinionPacer[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
				{
					vMinionPacer[i].enemyRect.left += vMinionPacer[i].enemySpeed;
					vMinionPacer[i].enemyRect.right += vMinionPacer[i].enemySpeed;
				}
				if (vMinionPacer[i].enemyRect.right >= 760)
				{
					firstEnemyAiPattern = 2;
				}
				break;
			case 4:		// UP
				// 애니메이션 Up
				ANIMATIONMANAGER->addAnimation("pacerUp", "PacerMove", 0, 21, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("pacerUp");
				ANIMATIONMANAGER->resume("pacerUp");

				if (vMinionPacer[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
				{
					vMinionPacer[i].enemyRect.top -= vMinionPacer[i].enemySpeed;
					vMinionPacer[i].enemyRect.bottom -= vMinionPacer[i].enemySpeed;
				}
				if (vMinionPacer[i].enemyRect.top <= 120)
				{
					firstEnemyAiPattern = 5;
				}
				break;
			case 5:		// DOWN
				// 애니메이션 Down
				ANIMATIONMANAGER->addAnimation("pacerDown", "PacerMove", 0, 21, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("pacerDown");
				ANIMATIONMANAGER->resume("pacerDown");

				if (vMinionPacer[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
				{
					vMinionPacer[i].enemyRect.top += vMinionPacer[i].enemySpeed;
					vMinionPacer[i].enemyRect.bottom += vMinionPacer[i].enemySpeed;
				}
				if (vMinionPacer[i].enemyRect.bottom >= 450)
				{
					firstEnemyAiPattern = 4;
				}
				break;
			}
			break;
		}

		// 판정 범위가 항상 적의 좌표를 쫓아다님
		vMinionPacer[i].enemyFireRange = RectMakeCenter(vMinionPacer[i].enemyX, vMinionPacer[i].enemyY, 200, 200);
	}
}

void MinionPacer::DeleteEnemy(int num)
{
	vMinionPacer.erase(vMinionPacer.begin() + num);
}

void MinionPacer::SetEnemyRectX(int enemyNum, int move)
{
	vMinionPacer[enemyNum].enemyRect.left += move;
	vMinionPacer[enemyNum].enemyRect.right += move;
}

void MinionPacer::SetEnemyRectY(int enemyNum, int move)
{
	vMinionPacer[enemyNum].enemyRect.top += move;
	vMinionPacer[enemyNum].enemyRect.bottom += move;
}
