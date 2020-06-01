#include "stdafx.h"
#include "Monstro.h"

Monstro::Monstro()
{
}

Monstro::~Monstro()
{
}

HRESULT Monstro::Init(POINT position)
{
	// Hit Effect
	EFFECTMANAGER->addEffect("blood", "images/monster/boss/bloodLarge.bmp", 2532 / 1.5, 202 / 1.5, 211 / 1.5, 202 / 1.5, 1, 0.3f, 30);

	// HP ProgressBar
	PROGRESSMANAGER->init("images/UI/ingameUI/bossHP.bmp", "images/UI/ingameUI/bossHpBar.bmp", WINSIZEX / 2 - 129, 30, 129 * 2, 18 * 2);
	PROGRESSMANAGER->setGauge(currentHP, maxHP);
	currentHP = 280;
	maxHP = 280;
	damage = 0;

	// 구조체 정보 기입
	EnemyInfo Monstro;
	Monstro.enemyRect = RectMakeCenter(position.x, position.y, 120, 60);
	Monstro.enemyHp = 250;
	Monstro.enemyShotSpeed = 5.0f;
	Monstro.enemyShotRange = 500.0f;
	Monstro.enemyShotDelay = 30;
	Monstro.enemySpeed = 2.0f;
	// 애니메이션 Idle
	Monstro.enemyImage = IMAGEMANAGER->addFrameImage("monstroIdle", "images/monster/boss/monstroIdle.bmp", 2043, 729, 9, 3, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addAnimation("bossIdle", "monstroIdle", 0, 25, 15, false, true);
	bossAni = ANIMATIONMANAGER->findAnimation("bossIdle");
	vMonstro.push_back(Monstro);

	rndX = RND->getFromIntTo(-100, 100);
	rndY = RND->getFromIntTo(-100, 100);

	shotCount = 0;
	jumpCount = 0;
	teleportCount = 0;
	deathCount = 0;
	firstEnemyAiPattern = 0;

	jumpCollision = false;
	teleportCollision = false;

	return S_OK;
}

void Monstro::Release()
{
}

void Monstro::Update()
{
	if (!enemyDeath)
	{
		EnemyAi();
	}

	EnemyDeath();
}

void Monstro::Render(HDC hdc)
{
	for (i = 0; i < vMonstro.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_F1))
		{
			//Rectangle(hdc, vMonstro[i].enemyFireRange.left, vMonstro[i].enemyFireRange.top, vMonstro[i].enemyFireRange.right, vMonstro[i].enemyFireRange.bottom);
			Rectangle(hdc, vMonstro[i].enemyRect.left, vMonstro[i].enemyRect.top, vMonstro[i].enemyRect.right, vMonstro[i].enemyRect.bottom);
			HBRUSH brush = CreateSolidBrush(RGB(51, 153, 0));
			FillRect(hdc, &vMonstro[i].enemyRect, brush);
			DeleteObject(brush);
		}

		if (!enemyDeath)
		{
			if (jump)
			{
				vMonstro[i].enemyImage->aniRender(hdc, vMonstro[i].enemyRect.left - 75, vMonstro[i].enemyRect.top - 417, bossAni);
			}
			else if (snowPattern)
			{
				vMonstro[i].enemyImage->aniRender(hdc, vMonstro[i].enemyRect.left - 55, vMonstro[i].enemyRect.top - 170, bossAni);
			}
			else if (teleport)
			{
				if (!teleportImage)
				{
					vMonstro[i].enemyImage->aniRender(hdc, vMonstro[i].enemyRect.left - 52, vMonstro[i].enemyRect.top - 218, bossAni);
				}
			}
			else
			{
				vMonstro[i].enemyImage->aniRender(hdc, vMonstro[i].enemyRect.left - 55, vMonstro[i].enemyRect.top - 170, bossAni);
			}

			PROGRESSMANAGER->render(hdc);
			BULLETMANAGER->RenderBullet(hdc, vEnemyBullet, viEnemyBullet);
		}
		else
		{
			if (deathAni)
			{
				vMonstro[i].enemyImage->aniRender(hdc, vMonstro[i].enemyRect.left - 75, vMonstro[i].enemyRect.top - 420, bossAni);
			}
			else
			{
				vMonstro[i].enemyImage->aniRender(hdc, vMonstro[i].enemyRect.left - 230, vMonstro[i].enemyRect.top - 280, bossAni);
			}
		}
	}
}

void Monstro::EnemyDeath()
{
	for (i = 0; i < vMonstro.size(); i++)
	{
		if (vMonstro[i].enemyHp <= 0)
		{
			deathCount++;
			enemyDeath = true;
			jumpCollision = true;
			teleportCollision = true;

			if (deathCount < 180)
			{
				deathAni = true;

				// 애니메이션 Death
				vMonstro[i].enemyImage = IMAGEMANAGER->addFrameImage("monstroDeath", "images/monster/boss/monstroDeath.bmp", 3406, 3036, 13, 6, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addDefAnimation("bossDeath", "monstroDeath", 25, false, true);
				bossAni = ANIMATIONMANAGER->findAnimation("bossDeath");
				ANIMATIONMANAGER->resume("bossDeath");

				// 피격 이펙트
				if (deathCount % 10 == 0)
				{
					EFFECTMANAGER->play("blood", RND->getFromIntTo(vMonstro[i].enemyRect.left - 50, vMonstro[i].enemyRect.right + 50),
						RND->getFromIntTo(vMonstro[i].enemyRect.top - 50, vMonstro[i].enemyRect.bottom));
				}
			}
			else if (deathCount >= 180 && deathCount < 224)
			{
				deathAni = false;

				// 애니메이션 Blood
				vMonstro[i].enemyImage = IMAGEMANAGER->addFrameImage("DeathBlood", "images/monster/boss/monstroDeathBlood.bmp", 1797, 1648, 3, 4, true, RGB(255, 0, 255));
				ANIMATIONMANAGER->addDefAnimation("Blood", "DeathBlood", 15, false, false);
				bossAni = ANIMATIONMANAGER->findAnimation("Blood");
				ANIMATIONMANAGER->resume("Blood");
			}
		}
	}
}

void Monstro::EnemyAiTime()
{
	// AI 패턴 시간
	firstEnemyAiTime++;
	if (firstEnemyAiTime / 60 == 3)
	{
		firstEnemyAiPattern = RND->getFromIntTo(1, 3);
		firstEnemyAiTime = 0;
	}
}

void Monstro::EnemyAi()
{
	for (i = 0; i < vMonstro.size(); i++)
	{
		RECT temp;

		// 적 x축, y축 좌표
		vMonstro[i].enemyX = vMonstro[i].enemyRect.left + (vMonstro[i].enemyRect.right - vMonstro[i].enemyRect.left) / 2;
		vMonstro[i].enemyY = vMonstro[i].enemyRect.top + (vMonstro[i].enemyRect.bottom - vMonstro[i].enemyRect.top) / 2;

		switch (firstEnemyAiPattern)
		{
		case 0:		 // 기본 IDLE 상태
			// 애니메이션 Idle
			vMonstro[i].enemyImage = IMAGEMANAGER->addFrameImage("monstroIdle", "images/monster/boss/monstroIdle.bmp", 2043, 729, 9, 3, true, RGB(255, 0, 255));
			bossAni = ANIMATIONMANAGER->findAnimation("bossIdle");
			ANIMATIONMANAGER->resume("bossIdle");
			// bool
			teleport = false;
			jump = false;
			snowPattern = false;

			if (PLAYERMANAGER->GetPlayerDeath() == false)
			{
				// 플레이어 추적
				distance = getDistance(vMonstro[i].enemyX, vMonstro[i].enemyY, PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY());
				if (distance)
				{
					// vector = ((플레이어 위치 x / y) - (적 위치 x / y) / 거리 * 적 속도;
					vx = ((PLAYERMANAGER->GetPlayerHitRectX()) - vMonstro[i].enemyX) / distance * vMonstro[i].enemySpeed;
					vy = ((PLAYERMANAGER->GetPlayerHitRectY()) - vMonstro[i].enemyY) / distance * vMonstro[i].enemySpeed;
				}
				else
				{
					vx = 0;
					vy = vMonstro[i].enemySpeed;
				}

				// + - 바꿔보기 이게 접근 방식이 어떻게 되는지
				vMonstro[i].enemyX += vx;
				vMonstro[i].enemyY += vy;
				vMonstro[i].enemyRect = RectMakeCenter(vMonstro[i].enemyX, vMonstro[i].enemyY, 120, 60);
			}

			// AI패턴
			EnemyAiTime();
			break;
		case 1:      // 점프 후 전방위 발사
			// 애니메이션 Idle
			ANIMATIONMANAGER->stop("bossIdle");
			// bool
			jump = true;
			snowPattern = false;
			teleport = false;
			jumpPower = 40.0f;
			break;
		case 2:      // 90도 눈꽃 공격
			// 애니메이션 Idle
			ANIMATIONMANAGER->stop("bossIdle");
			// bool
			jump = false;
			snowPattern = true;
			teleport = false;
			break;
		case 3:      // 플레이어의 근처로 순간이동
			// 애니메이션 Idle
			ANIMATIONMANAGER->stop("bossIdle");
			// bool
			jump = false;
			snowPattern = false;
			teleport = true;
			break;
		}

		EnemyPattern();

		PROGRESSMANAGER->update();
		PROGRESSMANAGER->setGauge(currentHP, maxHP);
	}

	// 적이 쏘는 불렛의 움직임
	BULLETMANAGER->MoveEnemyBullet(vEnemyBullet, viEnemyBullet);

	// 적이 쏘는 불렛 충돌
	COLLISIONMANAGER->EnemyBulletCollision(vEnemyBullet, viEnemyBullet);
}

void Monstro::EnemyPattern()
{
	// 패턴1 : 점프
	if (jump)
	{
		jumpCollision = true;
		jumpCount++;

		if (jumpCount < 21)
		{
			gravity = 5.0f;
			vMonstro[i].enemyRect.top -= jumpPower;
			vMonstro[i].enemyRect.bottom -= jumpPower;
			jumpPower -= gravity;
		}
		else if (jumpCount > 20 && jumpCount <= 165)
		{
			// 애니메이션 낙하
			vMonstro[i].enemyImage = IMAGEMANAGER->addFrameImage("monstroDown", "images/monster/boss/monstroJumpDown.bmp", 3406, 2530, 13, 5, true, RGB(255, 0, 255));
			ANIMATIONMANAGER->addAnimation("bossDown", "monstroDown", 0, 63, 25, true);
			bossAni = ANIMATIONMANAGER->findAnimation("bossDown");
			ANIMATIONMANAGER->resume("bossDown");

			// getdistance(적의 위치 x, y, 플레이어의 위치 x, y)
			distance = getDistance(vMonstro[i].enemyX, vMonstro[i].enemyY, PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY());
			if (distance)
			{
				// vector = ((플레이어 위치 x / y) - (적 위치 x / y) / 거리 * 적 속도;
				vx = ((WINSIZEX / 2) - vMonstro[i].enemyX) / distance * vMonstro[i].enemySpeed * 5;
				vy = ((WINSIZEY / 2) - vMonstro[i].enemyY) / distance * vMonstro[i].enemySpeed * 5;
			}
			else
			{
				vx = 0;
				vy = vMonstro[i].enemySpeed;
			}

			// 낙하
			vMonstro[i].enemyX += vx * 5;
			vMonstro[i].enemyY += vy * 5;
			vMonstro[i].enemyRect = RectMakeCenter(vMonstro[i].enemyX, vMonstro[i].enemyY, 120, 60);
		}
		else if (jumpCount > 165 && jumpCount <= 175)
		{
			EnemyAllShot();
		}
		else
		{
			firstEnemyAiPattern = 0;
			ANIMATIONMANAGER->stop("bossDown");
		}
	}
	else
	{
		jumpCollision = false;
		jumpCount = 0;
	}

	// 패턴2 : 눈꽃
	if (snowPattern)
	{
		// 애니메이션 Attack
		vMonstro[i].enemyImage = IMAGEMANAGER->addFrameImage("monstroAttack", "images/monster/boss/monstroAttack.bmp", 3859, 1944, 17, 8, true, RGB(255, 0, 255));

		// 카운트가 0일 때 방향을 결정한다.
		if (shotCount == 0)
		{
			if (vMonstro[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX()) isLeft = true;
			else isLeft = false;
		}

		shotCount++;

		if (isLeft)
		{
			if (shotCount > 0 && shotCount < 31)
			{
				EnemySnowShot();
			}

			if (shotCount < 150)
			{
				ANIMATIONMANAGER->addAnimation("bossLeftAttack", "monstroAttack", 0, 65, 25, true);
				bossAni = ANIMATIONMANAGER->findAnimation("bossLeftAttack");
				ANIMATIONMANAGER->resume("bossLeftAttack");
			}
			else
			{
				firstEnemyAiPattern = 0;
				ANIMATIONMANAGER->stop("bossLeftAttack");
			}
		}
		else
		{
			if (shotCount > 0 && shotCount < 31)
			{
				EnemySnowShot();
			}

			if (shotCount < 150)
			{
				ANIMATIONMANAGER->addAnimation("bossRightAttack", "monstroAttack", 66, 131, 25, true);
				bossAni = ANIMATIONMANAGER->findAnimation("bossRightAttack");
				ANIMATIONMANAGER->resume("bossRightAttack");
			}
			else
			{
				firstEnemyAiPattern = 0;
				ANIMATIONMANAGER->stop("bossRightAttack");
			}
		}
	}
	else
	{
		shotCount = 0;
	}

	// 패턴3 : 순간이동
	if (teleport)
	{
		// 애니메이션 텔레포트
		vMonstro[i].enemyImage = IMAGEMANAGER->addFrameImage("monstroTeleport", "images/monster/boss/monstroMove.bmp", 2043, 1455, 9, 5, true, RGB(255, 0, 255));
		teleportCollision = true;
		teleportCount++;

		if (teleportCount < 90)
		{
			ANIMATIONMANAGER->addAnimation("monstroTeleport", "monstroTeleport", 0, 44, 20, true);
			bossAni = ANIMATIONMANAGER->findAnimation("monstroTeleport");
			ANIMATIONMANAGER->resume("monstroTeleport");
		}
		else if (teleportCount > 89 && teleportCount < 300)
		{
			teleportImage = true;
			ANIMATIONMANAGER->pause("monstroTeleport");

			// getdistance(적의 위치 x, y, 플레이어의 위치 x, y)
			distance = getDistance(vMonstro[i].enemyX, vMonstro[i].enemyY, PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY());
			if (distance)
			{
				// vector = ((플레이어 위치 x / y) - (적 위치 x / y) / 거리 * 적 속도;
				vx = ((PLAYERMANAGER->GetPlayerHitRectX() + rndX) - vMonstro[i].enemyX) / distance * vMonstro[i].enemySpeed;
				vy = ((PLAYERMANAGER->GetPlayerHitRectY() + rndY) - vMonstro[i].enemyY) / distance * vMonstro[i].enemySpeed;
			}
			else
			{
				vx = 0;
				vy = vMonstro[i].enemySpeed;
			}

			vMonstro[i].enemyX += vx;
			vMonstro[i].enemyY += vy;
			vMonstro[i].enemyRect = RectMakeCenter(vMonstro[i].enemyX, vMonstro[i].enemyY, 120, 60);
		}
		else if (teleportCount > 299 & teleportCount < 350)
		{
			teleportImage = false;
			ANIMATIONMANAGER->resume("monstroTeleport");
		}
		else
		{
			firstEnemyAiPattern = 2;
			ANIMATIONMANAGER->stop("monstroTeleport");
		}
	}
	else
	{
		teleportCollision = false;
		teleportCount = 0;
	}
}

void Monstro::EnemySnowShot()
{
	vMonstro[i].enemyShotDelay = 30;

	// 캐릭터의 x축을 기준으로 눈꽃처럼 핏방울을 분출한다.
	if (isLeft)
	{
		for (int j = 1; j <= 15; j++)
		{
			BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMonstro[i].enemyX - 100, vMonstro[i].enemyY,
				ANGLE_0 - j, vMonstro[i].enemyShotSpeed, vMonstro[i].enemyShotRange, firstEnemyBulletCount + j, vMonstro[i].enemyShotDelay);
		}
	}
	else
	{
		for (int j = 1; j <= 15; j++)
		{
			BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMonstro[i].enemyX + 100, vMonstro[i].enemyY,
				ANGLE_0 - j, vMonstro[i].enemyShotSpeed, vMonstro[i].enemyShotRange, firstEnemyBulletCount + j, vMonstro[i].enemyShotDelay);
		}
	}

	// 적의 불렛 카운트를 한 번에 플러스
	SetFirstEnemyBulletCount();
}

void Monstro::EnemyAllShot()
{
	vMonstro[i].enemyShotDelay = 10;

	// 주위에 핏방울을 흩뿌린다.
	for (int j = 1; j <= 15; j++)
	{
		int rnd = RND->getInt(100);

		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMonstro[i].enemyX, vMonstro[i].enemyY,
			ANGLE_0 + rnd, vMonstro[i].enemyShotSpeed, vMonstro[i].enemyShotRange, secondEnemyBulletCount, vMonstro[i].enemyShotDelay);
	}

	// 적의 불렛 카운트를 한 번에 플러스
	SetSecondEnemyBulletCount();
}

void Monstro::DeleteEnemy(int num)
{
	vMonstro.erase(vMonstro.begin() + num);
}

void Monstro::SetEnemyRectX(int enemyNum, int move)
{
	vMonstro[enemyNum].enemyRect.left += move;
	vMonstro[enemyNum].enemyRect.right += move;
}

void Monstro::SetEnemyRectY(int enemyNum, int move)
{
	vMonstro[enemyNum].enemyRect.top += move;
	vMonstro[enemyNum].enemyRect.bottom += move;
}

void Monstro::hitDamage(int _damage)
{
	currentHP -= _damage;
}
