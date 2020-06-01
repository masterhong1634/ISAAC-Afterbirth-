#include "stdafx.h"
#include "MinionGaper.h"

MinionGaper::MinionGaper()
{
}

MinionGaper::~MinionGaper()
{
}

HRESULT MinionGaper::Init(POINT position, int EnemyNumber)
{
	gaperHead = IMAGEMANAGER->addImage("GaperHead", "images/monster/gaper/gaper.bmp", 32 * 2, 32 * 2, true, RGB(255, 0, 255));

	//구조체 정보 기입
	EnemyInfo MinionGaper;
	MinionGaper.enemyNumber = EnemyNumber;
	MinionGaper.enemyRect = RectMakeCenter(position.x, position.y, 30, 50);
	MinionGaper.enemyHp = 15;
	MinionGaper.enemySpeed = 2.0f;
	// 애니메이션 Idle
	MinionGaper.enemyShadowImage = IMAGEMANAGER->addImage("GaperShadow", "images/monster/gaper/gaperShadow.bmp", 120 / 3, 49 / 3, true, RGB(255, 0, 255));
	MinionGaper.enemyImage = IMAGEMANAGER->addFrameImage("GaperBody", "images/monster/gaper/gaperBody.bmp", 320 * 2, 124 * 2, 10, 4, true, RGB(255, 0, 255));
	int arrBodyIdle[] = { 22 };
	ANIMATIONMANAGER->addAnimation("gaper", "GaperBody", arrBodyIdle, 1, 1, true);
	minionAni = ANIMATIONMANAGER->findAnimation("gaper");
	vMinionGaper.push_back(MinionGaper);

	// IDLE
	firstEnemyAiPattern = 1;
	secondEnemyAiPattern = 1;
	thirdEnemyAiPattern = 1;

	// 이펙트
	EFFECTMANAGER->addEffect("effectPoof", "images/bullet/effectPoof.bmp", 256 * 2, 256 * 2, 64 * 2, 64 * 2, 30, 1.0f, 10);
	EFFECTMANAGER->play("effectPoof", MinionGaper.enemyRect.left, MinionGaper.enemyRect.top + 20);

	enemyAreaCheck = false;

	return S_OK;
}

void MinionGaper::Release()
{
}

void MinionGaper::Update()
{
	EnemyAi();
	COLLISIONMANAGER->SameVectorMinionCollision(vMinionGaper);
}

void MinionGaper::Render(HDC hdc)
{
	for (i = 0; i < vMinionGaper.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_F1))
		{
			//Rectangle(hdc, vMinionGaper[i].enemyFireRange.left, vMinionGaper[i].enemyFireRange.top, vMinionGaper[i].enemyFireRange.right, vMinionGaper[i].enemyFireRange.bottom);
			Rectangle(hdc, vMinionGaper[i].enemyRect.left, vMinionGaper[i].enemyRect.top, vMinionGaper[i].enemyRect.right, vMinionGaper[i].enemyRect.bottom);

			HBRUSH brush = CreateSolidBrush(RGB(102, 0, 204));
			FillRect(hdc, &vMinionGaper[i].enemyRect, brush);
			DeleteObject(brush);
		}

		vMinionGaper[i].enemyShadowImage->alphaRender(hdc, vMinionGaper[i].enemyRect.left - 5, vMinionGaper[i].enemyRect.top + 42, 70);
		vMinionGaper[i].enemyImage->aniRender(hdc, vMinionGaper[i].enemyRect.left - 17, vMinionGaper[i].enemyRect.top + 10, minionAni);
		gaperHead->render(hdc, vMinionGaper[i].enemyRect.left - 17, vMinionGaper[i].enemyRect.top - 19);
	}

	BULLETMANAGER->RenderBullet(hdc, vEnemyBullet, viEnemyBullet);
}

void MinionGaper::EnemyAiTime()
{
	switch (vMinionGaper[i].enemyNumber)
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

void MinionGaper::EnemyAi()
{
	for (i = 0; i < vMinionGaper.size(); i++)
	{
		RECT temp;

		// 적 x축, y축 좌표
		vMinionGaper[i].enemyX = vMinionGaper[i].enemyRect.left + (vMinionGaper[i].enemyRect.right - vMinionGaper[i].enemyRect.left) / 2;
		vMinionGaper[i].enemyY = vMinionGaper[i].enemyRect.top + (vMinionGaper[i].enemyRect.bottom - vMinionGaper[i].enemyRect.top) / 2;

		// 플레이어와 판정 범위가 충돌시
		if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &vMinionGaper[i].enemyFireRange) &&
			PLAYERMANAGER->GetPlayerDeath() == false)
		{
			// 플레이어를 쫓아가라.
			enemyAreaCheck = true;

			if (vMinionGaper[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
			{
				// BodyLeft
				ANIMATIONMANAGER->addAnimation("gaperLeft", "GaperBody", 10, 19, 12, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("gaperLeft");
				ANIMATIONMANAGER->resume("gaperLeft");
			}
			else
			{
				// BodyRight
				ANIMATIONMANAGER->addAnimation("gaperRight", "GaperBody", 30, 39, 12, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("gaperRight");
				ANIMATIONMANAGER->resume("gaperRight");
			}
		}

		// 플레이어가 사망하면 자율 AI로 돌아가라.
		if (PLAYERMANAGER->GetPlayerDeath() == true)
		{
			enemyAreaCheck = false;
		}

		// 만약에 플레이어가 적의 판정 범위안에 들어왔다면 플레이어를 쫓아간다.
		if (enemyAreaCheck)
		{
			// getdistance(적의 위치 x, y, 플레이어의 위치 x, y)
			distance = getDistance(vMinionGaper[i].enemyX, vMinionGaper[i].enemyY, PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY());

			if (distance)
			{
				// vector = ((플레이어 위치 x / y) - (적 위치 x / y) / 거리 * 적 속도;
				vx = ((PLAYERMANAGER->GetPlayerHitRectX()) - vMinionGaper[i].enemyX) / distance * vMinionGaper[i].enemySpeed;
				vy = ((PLAYERMANAGER->GetPlayerHitRectY()) - vMinionGaper[i].enemyY) / distance * vMinionGaper[i].enemySpeed;
			}
			else
			{
				vx = 0;
				vy = vMinionGaper[i].enemySpeed;
			}

			// + - 바꿔보기 이게 접근 방식이 어떻게 되는지
			if (vMinionGaper[i].enemyRect.left >= 105 && vMinionGaper[i].enemyRect.right <= 780 &&
				vMinionGaper[i].enemyRect.top >= 105 && vMinionGaper[i].enemyRect.bottom <= 465)
			{
				vMinionGaper[i].enemyX += vx;
				vMinionGaper[i].enemyY += vy;
			}
			vMinionGaper[i].enemyRect = RectMakeCenter(vMinionGaper[i].enemyX, vMinionGaper[i].enemyY, 30, 50);
		}
		// 범위안에 플레이어가 없다면 자율행동(AI)
		else
		{
			EnemyAiTime();

			switch (vMinionGaper[i].enemyNumber)
			{
			case 0:
				switch (firstEnemyAiPattern)
				{
				case 1:		// IDLE
					// 애니메이션 Idle
					ANIMATIONMANAGER->resume("gaper");
					break;
				case 2:		// LEFT
					// BodyLeft
					ANIMATIONMANAGER->addAnimation("gaperLeft", "GaperBody", 10, 19, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperLeft");
					ANIMATIONMANAGER->resume("gaperLeft");

					if (vMinionGaper[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
					{
						vMinionGaper[i].enemyRect.left -= vMinionGaper[i].enemySpeed;
						vMinionGaper[i].enemyRect.right -= vMinionGaper[i].enemySpeed;
					}
					if (vMinionGaper[i].enemyRect.left <= 120)
					{
						firstEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					// BodyRight
					ANIMATIONMANAGER->addAnimation("gaperRight", "GaperBody", 30, 39, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperRight");
					ANIMATIONMANAGER->resume("gaperRight");

					if (vMinionGaper[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
					{
						vMinionGaper[i].enemyRect.left += vMinionGaper[i].enemySpeed;
						vMinionGaper[i].enemyRect.right += vMinionGaper[i].enemySpeed;
					}
					if (vMinionGaper[i].enemyRect.right >= 760)
					{
						firstEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					// BodyUp
					ANIMATIONMANAGER->addAnimation("gaperUp", "GaperBody", 20, 29, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperUp");
					ANIMATIONMANAGER->resume("gaperUp");

					if (vMinionGaper[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
					{
						vMinionGaper[i].enemyRect.top -= vMinionGaper[i].enemySpeed;
						vMinionGaper[i].enemyRect.bottom -= vMinionGaper[i].enemySpeed;
					}
					if (vMinionGaper[i].enemyRect.top <= 120)
					{
						firstEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					// BodyDown
					ANIMATIONMANAGER->addAnimation("gaperDown", "GaperBody", 20, 29, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperDown");
					ANIMATIONMANAGER->resume("gaperDown");

					if (vMinionGaper[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
					{
						vMinionGaper[i].enemyRect.top += vMinionGaper[i].enemySpeed;
						vMinionGaper[i].enemyRect.bottom += vMinionGaper[i].enemySpeed;
					}
					if (vMinionGaper[i].enemyRect.bottom >= 450)
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
					ANIMATIONMANAGER->resume("gaper");
					break;
				case 2:		// LEFT
					// BodyLeft
					ANIMATIONMANAGER->addAnimation("gaperLeft", "GaperBody", 10, 19, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperLeft");
					ANIMATIONMANAGER->resume("gaperLeft");

					if (vMinionGaper[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
					{
						vMinionGaper[i].enemyRect.left -= vMinionGaper[i].enemySpeed;
						vMinionGaper[i].enemyRect.right -= vMinionGaper[i].enemySpeed;
					}
					if (vMinionGaper[i].enemyRect.left <= 120)
					{
						firstEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					// BodyRight
					ANIMATIONMANAGER->addAnimation("gaperRight", "GaperBody", 30, 39, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperRight");
					ANIMATIONMANAGER->resume("gaperRight");

					if (vMinionGaper[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
					{
						vMinionGaper[i].enemyRect.left += vMinionGaper[i].enemySpeed;
						vMinionGaper[i].enemyRect.right += vMinionGaper[i].enemySpeed;
					}
					if (vMinionGaper[i].enemyRect.right >= 760)
					{
						firstEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					// BodyUp
					ANIMATIONMANAGER->addAnimation("gaperUp", "GaperBody", 20, 29, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperUp");
					ANIMATIONMANAGER->resume("gaperUp");

					if (vMinionGaper[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
					{
						vMinionGaper[i].enemyRect.top -= vMinionGaper[i].enemySpeed;
						vMinionGaper[i].enemyRect.bottom -= vMinionGaper[i].enemySpeed;
					}
					if (vMinionGaper[i].enemyRect.top <= 120)
					{
						firstEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					// BodyDown
					ANIMATIONMANAGER->addAnimation("gaperDown", "GaperBody", 20, 29, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperDown");
					ANIMATIONMANAGER->resume("gaperDown");

					if (vMinionGaper[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
					{
						vMinionGaper[i].enemyRect.top += vMinionGaper[i].enemySpeed;
						vMinionGaper[i].enemyRect.bottom += vMinionGaper[i].enemySpeed;
					}
					if (vMinionGaper[i].enemyRect.bottom >= 450)
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
					ANIMATIONMANAGER->resume("gaper");
					break;
				case 2:		// LEFT
					// BodyLeft
					ANIMATIONMANAGER->addAnimation("gaperLeft", "GaperBody", 10, 19, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperLeft");
					ANIMATIONMANAGER->resume("gaperLeft");

					if (vMinionGaper[i].enemyRect.left > 105) // 몬스터 이동 범위 제한
					{
						vMinionGaper[i].enemyRect.left -= vMinionGaper[i].enemySpeed;
						vMinionGaper[i].enemyRect.right -= vMinionGaper[i].enemySpeed;
					}
					if (vMinionGaper[i].enemyRect.left <= 120)
					{
						firstEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					// BodyRight
					ANIMATIONMANAGER->addAnimation("gaperRight", "GaperBody", 30, 39, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperRight");
					ANIMATIONMANAGER->resume("gaperRight");

					if (vMinionGaper[i].enemyRect.right < 780) // 몬스터 이동 범위 제한
					{
						vMinionGaper[i].enemyRect.left += vMinionGaper[i].enemySpeed;
						vMinionGaper[i].enemyRect.right += vMinionGaper[i].enemySpeed;
					}
					if (vMinionGaper[i].enemyRect.right >= 760)
					{
						firstEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					// BodyUp
					ANIMATIONMANAGER->addAnimation("gaperUp", "GaperBody", 20, 29, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperUp");
					ANIMATIONMANAGER->resume("gaperUp");

					if (vMinionGaper[i].enemyRect.top > 105) // 몬스터 이동 범위 제한
					{
						vMinionGaper[i].enemyRect.top -= vMinionGaper[i].enemySpeed;
						vMinionGaper[i].enemyRect.bottom -= vMinionGaper[i].enemySpeed;
					}
					if (vMinionGaper[i].enemyRect.top <= 120)
					{
						firstEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					// BodyDown
					ANIMATIONMANAGER->addAnimation("gaperDown", "GaperBody", 20, 29, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperDown");
					ANIMATIONMANAGER->resume("gaperDown");

					if (vMinionGaper[i].enemyRect.bottom < 465) // 몬스터 이동 범위 제한
					{
						vMinionGaper[i].enemyRect.top += vMinionGaper[i].enemySpeed;
						vMinionGaper[i].enemyRect.bottom += vMinionGaper[i].enemySpeed;
					}
					if (vMinionGaper[i].enemyRect.bottom >= 450)
					{
						firstEnemyAiPattern = 4;
					}
					break;
				}
				break;
			}
		}

		// 판정 범위가 항상 적의 좌표를 쫓아다님
		vMinionGaper[i].enemyFireRange = RectMakeCenter(vMinionGaper[i].enemyX, vMinionGaper[i].enemyY, 300, 300);
	}

	// 적이 쏘는 불렛의 움직임
	BULLETMANAGER->MoveEnemyBullet(vEnemyBullet, viEnemyBullet);

	// 적이 쏘는 불렛 충돌
	COLLISIONMANAGER->EnemyBulletCollision(vEnemyBullet, viEnemyBullet);
}

void MinionGaper::DeleteEnemy(int num)
{
	vMinionGaper.erase(vMinionGaper.begin() + num);
}

void MinionGaper::SetEnemyRectX(int enemyNum, int move)
{
	vMinionGaper[enemyNum].enemyRect.left += move;
	vMinionGaper[enemyNum].enemyRect.right += move;
}

void MinionGaper::SetEnemyRectY(int enemyNum, int move)
{
	vMinionGaper[enemyNum].enemyRect.top += move;
	vMinionGaper[enemyNum].enemyRect.bottom += move;
}
