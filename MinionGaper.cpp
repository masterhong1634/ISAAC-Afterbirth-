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

	//����ü ���� ����
	EnemyInfo MinionGaper;
	MinionGaper.enemyNumber = EnemyNumber;
	MinionGaper.enemyRect = RectMakeCenter(position.x, position.y, 30, 50);
	MinionGaper.enemyHp = 15;
	MinionGaper.enemySpeed = 2.0f;
	// �ִϸ��̼� Idle
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

	// ����Ʈ
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
		// AI ���� �ð�
		firstEnemyAiTime++;
		if (firstEnemyAiTime / 60 == 2)
		{
			firstEnemyAiPattern = RND->getFromIntTo(2, 5);
			firstEnemyAiTime = 0;
		}
		break;
	case 1:
		// AI ���� �ð�
		secondEnemyAiTime++;
		if (secondEnemyAiTime / 60 == 2)
		{
			secondEnemyAiPattern = RND->getFromIntTo(2, 5);
			secondEnemyAiTime = 0;
		}
		break;
	case 2:
		// AI ���� �ð�
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

		// �� x��, y�� ��ǥ
		vMinionGaper[i].enemyX = vMinionGaper[i].enemyRect.left + (vMinionGaper[i].enemyRect.right - vMinionGaper[i].enemyRect.left) / 2;
		vMinionGaper[i].enemyY = vMinionGaper[i].enemyRect.top + (vMinionGaper[i].enemyRect.bottom - vMinionGaper[i].enemyRect.top) / 2;

		// �÷��̾�� ���� ������ �浹��
		if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &vMinionGaper[i].enemyFireRange) &&
			PLAYERMANAGER->GetPlayerDeath() == false)
		{
			// �÷��̾ �Ѿư���.
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

		// �÷��̾ ����ϸ� ���� AI�� ���ư���.
		if (PLAYERMANAGER->GetPlayerDeath() == true)
		{
			enemyAreaCheck = false;
		}

		// ���࿡ �÷��̾ ���� ���� �����ȿ� ���Դٸ� �÷��̾ �Ѿư���.
		if (enemyAreaCheck)
		{
			// getdistance(���� ��ġ x, y, �÷��̾��� ��ġ x, y)
			distance = getDistance(vMinionGaper[i].enemyX, vMinionGaper[i].enemyY, PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY());

			if (distance)
			{
				// vector = ((�÷��̾� ��ġ x / y) - (�� ��ġ x / y) / �Ÿ� * �� �ӵ�;
				vx = ((PLAYERMANAGER->GetPlayerHitRectX()) - vMinionGaper[i].enemyX) / distance * vMinionGaper[i].enemySpeed;
				vy = ((PLAYERMANAGER->GetPlayerHitRectY()) - vMinionGaper[i].enemyY) / distance * vMinionGaper[i].enemySpeed;
			}
			else
			{
				vx = 0;
				vy = vMinionGaper[i].enemySpeed;
			}

			// + - �ٲ㺸�� �̰� ���� ����� ��� �Ǵ���
			if (vMinionGaper[i].enemyRect.left >= 105 && vMinionGaper[i].enemyRect.right <= 780 &&
				vMinionGaper[i].enemyRect.top >= 105 && vMinionGaper[i].enemyRect.bottom <= 465)
			{
				vMinionGaper[i].enemyX += vx;
				vMinionGaper[i].enemyY += vy;
			}
			vMinionGaper[i].enemyRect = RectMakeCenter(vMinionGaper[i].enemyX, vMinionGaper[i].enemyY, 30, 50);
		}
		// �����ȿ� �÷��̾ ���ٸ� �����ൿ(AI)
		else
		{
			EnemyAiTime();

			switch (vMinionGaper[i].enemyNumber)
			{
			case 0:
				switch (firstEnemyAiPattern)
				{
				case 1:		// IDLE
					// �ִϸ��̼� Idle
					ANIMATIONMANAGER->resume("gaper");
					break;
				case 2:		// LEFT
					// BodyLeft
					ANIMATIONMANAGER->addAnimation("gaperLeft", "GaperBody", 10, 19, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperLeft");
					ANIMATIONMANAGER->resume("gaperLeft");

					if (vMinionGaper[i].enemyRect.left > 105) // ���� �̵� ���� ����
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

					if (vMinionGaper[i].enemyRect.right < 780) // ���� �̵� ���� ����
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

					if (vMinionGaper[i].enemyRect.top > 105) // ���� �̵� ���� ����
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

					if (vMinionGaper[i].enemyRect.bottom < 465) // ���� �̵� ���� ����
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
					// �ִϸ��̼� Idle
					ANIMATIONMANAGER->resume("gaper");
					break;
				case 2:		// LEFT
					// BodyLeft
					ANIMATIONMANAGER->addAnimation("gaperLeft", "GaperBody", 10, 19, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperLeft");
					ANIMATIONMANAGER->resume("gaperLeft");

					if (vMinionGaper[i].enemyRect.left > 105) // ���� �̵� ���� ����
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

					if (vMinionGaper[i].enemyRect.right < 780) // ���� �̵� ���� ����
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

					if (vMinionGaper[i].enemyRect.top > 105) // ���� �̵� ���� ����
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

					if (vMinionGaper[i].enemyRect.bottom < 465) // ���� �̵� ���� ����
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
					// �ִϸ��̼� Idle
					ANIMATIONMANAGER->resume("gaper");
					break;
				case 2:		// LEFT
					// BodyLeft
					ANIMATIONMANAGER->addAnimation("gaperLeft", "GaperBody", 10, 19, 12, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("gaperLeft");
					ANIMATIONMANAGER->resume("gaperLeft");

					if (vMinionGaper[i].enemyRect.left > 105) // ���� �̵� ���� ����
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

					if (vMinionGaper[i].enemyRect.right < 780) // ���� �̵� ���� ����
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

					if (vMinionGaper[i].enemyRect.top > 105) // ���� �̵� ���� ����
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

					if (vMinionGaper[i].enemyRect.bottom < 465) // ���� �̵� ���� ����
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

		// ���� ������ �׻� ���� ��ǥ�� �Ѿƴٴ�
		vMinionGaper[i].enemyFireRange = RectMakeCenter(vMinionGaper[i].enemyX, vMinionGaper[i].enemyY, 300, 300);
	}

	// ���� ��� �ҷ��� ������
	BULLETMANAGER->MoveEnemyBullet(vEnemyBullet, viEnemyBullet);

	// ���� ��� �ҷ� �浹
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
