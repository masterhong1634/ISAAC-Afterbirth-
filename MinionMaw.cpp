#include "stdafx.h"
#include "MinionMaw.h"

MinionMaw::MinionMaw()
{
}

MinionMaw::~MinionMaw()
{
}

HRESULT MinionMaw::Init(POINT position, int EnemyNumber)
{
	//구조체 정보 기입
	EnemyInfo MinionMaw;
	MinionMaw.enemyNumber = EnemyNumber;
	MinionMaw.enemyRect = RectMakeCenter(position.x, position.y, 35, 35);
	MinionMaw.enemyHp = 25;
	MinionMaw.enemyShotSpeed = 5.0f;
	MinionMaw.enemyShotRange = 300.0f;
	MinionMaw.enemyShotDelay = 80;
	MinionMaw.enemySpeed = 1.5f;
	// 애니메이션 Idle
	MinionMaw.enemyShadowImage = IMAGEMANAGER->addImage("MawShadow", "images/monster/maw/mawShadow.bmp", 120 / 3, 49 / 3, true, RGB(255, 0, 255));
	MinionMaw.enemyImage = IMAGEMANAGER->addFrameImage("MawIdle", "images/monster/maw/MawIdle.bmp", 342 / 2, 181 / 2, 2, 1, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addDefAnimation("maw", "MawIdle", 15, false, true);
	minionAni = ANIMATIONMANAGER->findAnimation("maw");
	vMinionMaw.push_back(MinionMaw);

	// IDLE
	firstEnemyAiPattern = 1;
	secondEnemyAiPattern = 1;
	thirdEnemyAiPattern = 1;

	// 이펙트
	EFFECTMANAGER->addEffect("effectPoof", "images/bullet/effectPoof.bmp", 256 * 2, 256 * 2, 64 * 2, 64 * 2, 30, 1.0f, 10);
	EFFECTMANAGER->play("effectPoof", MinionMaw.enemyRect.left, MinionMaw.enemyRect.top + 20);

	enemyAreaCheck = false;

	return S_OK;
}

void MinionMaw::Release()
{
}

void MinionMaw::Update()
{
	EnemyAi();
	COLLISIONMANAGER->SameVectorMinionCollision(vMinionMaw);
}

void MinionMaw::Render(HDC hdc)
{
	for (i = 0; i < vMinionMaw.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_F1))
		{
			//Rectangle(hdc, vMinionMaw[i].enemyFireRange.left, vMinionMaw[i].enemyFireRange.top, vMinionMaw[i].enemyFireRange.right, vMinionMaw[i].enemyFireRange.bottom);
			Rectangle(hdc, vMinionMaw[i].enemyRect.left, vMinionMaw[i].enemyRect.top, vMinionMaw[i].enemyRect.right, vMinionMaw[i].enemyRect.bottom);

			HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
			FillRect(hdc, &vMinionMaw[i].enemyRect, brush);
			DeleteObject(brush);
		}

		if (enemyAtk)
		{
			vMinionMaw[i].enemyShadowImage->alphaRender(hdc, vMinionMaw[i].enemyRect.left - 2, vMinionMaw[i].enemyRect.top + 50, 70);
			vMinionMaw[i].enemyImage->aniRender(hdc, vMinionMaw[i].enemyRect.left - 25, vMinionMaw[i].enemyRect.top - 35, minionAni);
		}
		else
		{
			vMinionMaw[i].enemyShadowImage->alphaRender(hdc, vMinionMaw[i].enemyRect.left - 2, vMinionMaw[i].enemyRect.top + 50, 70);
			vMinionMaw[i].enemyImage->aniRender(hdc, vMinionMaw[i].enemyRect.left - 25, vMinionMaw[i].enemyRect.top - 35, minionAni);
		}
	}

	BULLETMANAGER->RenderBullet(hdc, vEnemyBullet, viEnemyBullet);
}

void MinionMaw::EnemyAiTime()
{
	switch (vMinionMaw[i].enemyNumber)
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

void MinionMaw::EnemyAi()
{
	for (i = 0; i < vMinionMaw.size(); i++)
	{
		RECT temp;

		// 적 x축, y축 좌표
		vMinionMaw[i].enemyX = vMinionMaw[i].enemyRect.left + (vMinionMaw[i].enemyRect.right - vMinionMaw[i].enemyRect.left) / 2;
		vMinionMaw[i].enemyY = vMinionMaw[i].enemyRect.top + (vMinionMaw[i].enemyRect.bottom - vMinionMaw[i].enemyRect.top) / 2;

		// 플레이어와 판정 범위가 충돌시
		if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &vMinionMaw[i].enemyFireRange) &&
			PLAYERMANAGER->GetPlayerDeath() == false)
		{
			// 플레이어를 쫓아가라.
			enemyAreaCheck = true;
		}

		// 플레이어가 사망하면 자율 AI로 돌아가라.
		if (PLAYERMANAGER->GetPlayerDeath() == true)
		{
			enemyAreaCheck = false;
		}

		// 만약에 플레이어가 적의 판정 범위안에 들어왔다면 플레이어를 쫓아간다.
		if (enemyAreaCheck)
		{
			EnemyShot();

			// getdistance(적의 위치 x, y, 플레이어의 위치 x, y)
			distance = getDistance(vMinionMaw[i].enemyX, vMinionMaw[i].enemyY, PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY());

			if (distance)
			{
				// vector = ((플레이어 위치 x / y) - (적 위치 x / y) / 거리 * 적 속도;
				vx = ((PLAYERMANAGER->GetPlayerHitRectX()) - vMinionMaw[i].enemyX) / distance * vMinionMaw[i].enemySpeed;
				vy = ((PLAYERMANAGER->GetPlayerHitRectY()) - vMinionMaw[i].enemyY) / distance * vMinionMaw[i].enemySpeed;
			}
			else
			{
				vx = 0;
				vy = vMinionMaw[i].enemySpeed;
			}

			// + - 바꿔보기 이게 접근 방식이 어떻게 되는지
			if (vMinionMaw[i].enemyRect.left >= 105 && vMinionMaw[i].enemyRect.right <= 780 &&
				vMinionMaw[i].enemyRect.top >= 105 && vMinionMaw[i].enemyRect.bottom <= 465)
			{
				vMinionMaw[i].enemyX += vx;
				vMinionMaw[i].enemyY += vy;
			}
			vMinionMaw[i].enemyRect = RectMakeCenter(vMinionMaw[i].enemyX, vMinionMaw[i].enemyY, 35, 35);
		}
		// 범위안에 플레이어가 없다면 자율행동(AI)
		else
		{
			enemyAtk = false;
			EnemyAiTime();

			switch (vMinionMaw[i].enemyNumber)
			{
			case 0:
				switch (firstEnemyAiPattern)
				{
				case 1:		// IDLE
					//애니메이션 프레임
					ANIMATIONMANAGER->resume("maw");
					break;
				case 2:		// LEFT
					if (vMinionMaw[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
					{
						vMinionMaw[i].enemyRect.left -= vMinionMaw[i].enemySpeed;
						vMinionMaw[i].enemyRect.right -= vMinionMaw[i].enemySpeed;
					}
					if (vMinionMaw[i].enemyRect.left <= 120)
					{
						firstEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					if (vMinionMaw[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
					{
						vMinionMaw[i].enemyRect.left += vMinionMaw[i].enemySpeed;
						vMinionMaw[i].enemyRect.right += vMinionMaw[i].enemySpeed;
					}
					if (vMinionMaw[i].enemyRect.right >= 760)
					{
						firstEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					if (vMinionMaw[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
					{
						vMinionMaw[i].enemyRect.top -= vMinionMaw[i].enemySpeed;
						vMinionMaw[i].enemyRect.bottom -= vMinionMaw[i].enemySpeed;
					}
					if (vMinionMaw[i].enemyRect.top <= 120)
					{
						firstEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					if (vMinionMaw[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
					{
						vMinionMaw[i].enemyRect.top += vMinionMaw[i].enemySpeed;
						vMinionMaw[i].enemyRect.bottom += vMinionMaw[i].enemySpeed;
					}
					if (vMinionMaw[i].enemyRect.bottom >= 450)
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
					//애니메이션 프레임
					ANIMATIONMANAGER->resume("maw");
					break;
				case 2:		// LEFT
					if (vMinionMaw[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
					{
						vMinionMaw[i].enemyRect.left -= vMinionMaw[i].enemySpeed;
						vMinionMaw[i].enemyRect.right -= vMinionMaw[i].enemySpeed;
					}
					if (vMinionMaw[i].enemyRect.left <= 120)
					{
						secondEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					if (vMinionMaw[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
					{
						vMinionMaw[i].enemyRect.left += vMinionMaw[i].enemySpeed;
						vMinionMaw[i].enemyRect.right += vMinionMaw[i].enemySpeed;
					}
					if (vMinionMaw[i].enemyRect.right >= 760)
					{
						secondEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					if (vMinionMaw[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
					{
						vMinionMaw[i].enemyRect.top -= vMinionMaw[i].enemySpeed;
						vMinionMaw[i].enemyRect.bottom -= vMinionMaw[i].enemySpeed;
					}
					if (vMinionMaw[i].enemyRect.top <= 120)
					{
						secondEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					if (vMinionMaw[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
					{
						vMinionMaw[i].enemyRect.top += vMinionMaw[i].enemySpeed;
						vMinionMaw[i].enemyRect.bottom += vMinionMaw[i].enemySpeed;
					}
					if (vMinionMaw[i].enemyRect.bottom >= 450)
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
					//애니메이션 프레임
					ANIMATIONMANAGER->resume("maw");
					break;
				case 2:		// LEFT
					if (vMinionMaw[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
					{
						vMinionMaw[i].enemyRect.left -= vMinionMaw[i].enemySpeed;
						vMinionMaw[i].enemyRect.right -= vMinionMaw[i].enemySpeed;
					}
					if (vMinionMaw[i].enemyRect.left <= 120)
					{
						thirdEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					if (vMinionMaw[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
					{
						vMinionMaw[i].enemyRect.left += vMinionMaw[i].enemySpeed;
						vMinionMaw[i].enemyRect.right += vMinionMaw[i].enemySpeed;
					}
					if (vMinionMaw[i].enemyRect.right >= 760)
					{
						thirdEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					if (vMinionMaw[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
					{
						vMinionMaw[i].enemyRect.top -= vMinionMaw[i].enemySpeed;
						vMinionMaw[i].enemyRect.bottom -= vMinionMaw[i].enemySpeed;
					}
					if (vMinionMaw[i].enemyRect.top <= 120)
					{
						thirdEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					if (vMinionMaw[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
					{
						vMinionMaw[i].enemyRect.top += vMinionMaw[i].enemySpeed;
						vMinionMaw[i].enemyRect.bottom += vMinionMaw[i].enemySpeed;
					}
					if (vMinionMaw[i].enemyRect.bottom >= 450)
					{
						thirdEnemyAiPattern = 4;
					}
					break;
				}
				break;
			}
		}

		// 판정 범위가 항상 적의 좌표를 쫓아다님
		vMinionMaw[i].enemyFireRange = RectMakeCenter(vMinionMaw[i].enemyX, vMinionMaw[i].enemyY, 300, 300);
	}

	// 적이 쏘는 불렛의 움직임
	BULLETMANAGER->MoveEnemyBullet(vEnemyBullet, viEnemyBullet);

	// 적이 쏘는 불렛 충돌
	COLLISIONMANAGER->EnemyBulletCollision(vEnemyBullet, viEnemyBullet);
}

void MinionMaw::EnemyShot()
{
	enemyAtk = true;

	if (enemyAtk)
	{
		// 애니메이션
		vMinionMaw[i].enemyImage = IMAGEMANAGER->addFrameImage("mawAttack", "images/monster/maw/mawAttack.bmp", 1710 / 2, 724 / 2, 10, 4, true, RGB(255, 0, 255));
		if (vMinionMaw[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
		{
			ANIMATIONMANAGER->addAnimation("mawAtkLeft", "mawAttack", 0, 19, 25, false, true);
			minionAni = ANIMATIONMANAGER->findAnimation("mawAtkLeft");
			ANIMATIONMANAGER->resume("mawAtkLeft");
		}
		else
		{
			ANIMATIONMANAGER->addAnimation("mawAtkRight", "mawAttack", 20, 39, 25, false, true);
			minionAni = ANIMATIONMANAGER->findAnimation("mawAtkRight");
			ANIMATIONMANAGER->resume("mawAtkRight");
		}
	}

	switch (vMinionMaw[i].enemyNumber)
	{
	case 0:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionMaw[i].enemyX, vMinionMaw[i].enemyY,
			getAngle(vMinionMaw[i].enemyX, vMinionMaw[i].enemyY,
				PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY()),
			vMinionMaw[i].enemyShotSpeed, vMinionMaw[i].enemyShotRange, firstEnemyBulletCount, vMinionMaw[i].enemyShotDelay);

		// 적의 불렛 카운트를 한 번에 플러스
		SetFirstEnemyBulletCount();
		break;
	case 1:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionMaw[i].enemyX, vMinionMaw[i].enemyY,
			getAngle(vMinionMaw[i].enemyX, vMinionMaw[i].enemyY,
				PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY()),
			vMinionMaw[i].enemyShotSpeed, vMinionMaw[i].enemyShotRange, secondEnemyBulletCount, vMinionMaw[i].enemyShotDelay);

		// 적의 불렛 카운트를 한 번에 플러스
		SetSecondEnemyBulletCount();
		break;
	case 2:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionMaw[i].enemyX, vMinionMaw[i].enemyY,
			getAngle(vMinionMaw[i].enemyX, vMinionMaw[i].enemyY,
				PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY()),
			vMinionMaw[i].enemyShotSpeed, vMinionMaw[i].enemyShotRange, thirdEnemyBulletCount, vMinionMaw[i].enemyShotDelay);

		// 적의 불렛 카운트를 한 번에 플러스
		SetThirdEnemyBulletCount();
		break;
	}
}

void MinionMaw::DeleteEnemy(int num)
{
	vMinionMaw.erase(vMinionMaw.begin() + num);
}

void MinionMaw::SetEnemyRectX(int enemyNum, int move)
{
	vMinionMaw[enemyNum].enemyRect.left += move;
	vMinionMaw[enemyNum].enemyRect.right += move;
}

void MinionMaw::SetEnemyRectY(int enemyNum, int move)
{
	vMinionMaw[enemyNum].enemyRect.top += move;
	vMinionMaw[enemyNum].enemyRect.bottom += move;
}
