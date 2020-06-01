#include "stdafx.h"
#include "MinionAttackFly.h"

MinionAttackFly::MinionAttackFly()
{
}

MinionAttackFly::~MinionAttackFly()
{
}

HRESULT MinionAttackFly::Init(POINT position, int EnemyNumber)
{
	//����ü ���� ����
	EnemyInfo MinionAttackFly;
	MinionAttackFly.enemyNumber = EnemyNumber;
	MinionAttackFly.enemyRect = RectMakeCenter(position.x, position.y, 20, 20);
	MinionAttackFly.enemyHp = 10;
	MinionAttackFly.enemySpeed = 2.0f;
	// �ִϸ��̼� Idle
	MinionAttackFly.enemyShadowImage = IMAGEMANAGER->addImage("AttackFlyShadow", "images/monster/attackfly/attackflyShadow.bmp", 120 / 4, 49 / 4, true, RGB(255, 0, 255));
	MinionAttackFly.enemyImage = IMAGEMANAGER->addFrameImage("AttackFlyMove", "images/monster/attackfly/attackflyMove.bmp", 712 / 2, 183 / 2, 4, 1, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addAnimation("AttackFlyLeft", "AttackFlyMove", 0, 1, 15, false, true);
	minionAni = ANIMATIONMANAGER->findAnimation("AttackFlyLeft");
	vMinionAttackFly.push_back(MinionAttackFly);

	// IDLE
	firstEnemyAiPattern = 1;
	secondEnemyAiPattern = 1;
	thirdEnemyAiPattern = 1;

	// ����Ʈ
	EFFECTMANAGER->addEffect("effectPoof", "images/bullet/effectPoof.bmp", 256 * 2, 256 * 2, 64 * 2, 64 * 2, 30, 1.0f, 10);
	EFFECTMANAGER->play("effectPoof", MinionAttackFly.enemyRect.left, MinionAttackFly.enemyRect.top + 20);

	enemyAreaCheck = false;

	return S_OK;
}

void MinionAttackFly::Release()
{
}

void MinionAttackFly::Update()
{
	EnemyAi();
	COLLISIONMANAGER->SameVectorMinionCollision(vMinionAttackFly);
}

void MinionAttackFly::Render(HDC hdc)
{
	for (i = 0; i < vMinionAttackFly.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_F1))
		{
			//Rectangle(hdc, vMinionAttackFly[i].enemyFireRange.left, vMinionAttackFly[i].enemyFireRange.top, vMinionAttackFly[i].enemyFireRange.right, vMinionAttackFly[i].enemyFireRange.bottom);
			Rectangle(hdc, vMinionAttackFly[i].enemyRect.left, vMinionAttackFly[i].enemyRect.top, vMinionAttackFly[i].enemyRect.right, vMinionAttackFly[i].enemyRect.bottom);

			HBRUSH brush = CreateSolidBrush(RGB(163, 255, 0));
			FillRect(hdc, &vMinionAttackFly[i].enemyRect, brush);
			DeleteObject(brush);
		}

		vMinionAttackFly[i].enemyShadowImage->alphaRender(hdc, vMinionAttackFly[i].enemyRect.left - 6, vMinionAttackFly[i].enemyRect.top + 42, 70);
		vMinionAttackFly[i].enemyImage->aniRender(hdc, vMinionAttackFly[i].enemyRect.left - 35, vMinionAttackFly[i].enemyRect.top - 40, minionAni);
	}

	BULLETMANAGER->RenderBullet(hdc, vEnemyBullet, viEnemyBullet);
}

void MinionAttackFly::EnemyAiTime()
{
	switch (vMinionAttackFly[i].enemyNumber)
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

void MinionAttackFly::EnemyAi()
{
	for (i = 0; i < vMinionAttackFly.size(); i++)
	{
		RECT temp;

		// �� x��, y�� ��ǥ
		vMinionAttackFly[i].enemyX = vMinionAttackFly[i].enemyRect.left + (vMinionAttackFly[i].enemyRect.right - vMinionAttackFly[i].enemyRect.left) / 2;
		vMinionAttackFly[i].enemyY = vMinionAttackFly[i].enemyRect.top + (vMinionAttackFly[i].enemyRect.bottom - vMinionAttackFly[i].enemyRect.top) / 2;

		// �÷��̾�� ���� ������ �浹��
		if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &vMinionAttackFly[i].enemyFireRange) &&
			PLAYERMANAGER->GetPlayerDeath() == false)
		{
			// �÷��̾ �Ѿư���.
			enemyAreaCheck = true;

			if (vMinionAttackFly[i].enemyX >= PLAYERMANAGER->GetPlayerHitRectX())
			{
				//�ִϸ��̼� ������
				minionAni = ANIMATIONMANAGER->findAnimation("AttackFlyLeft");
				ANIMATIONMANAGER->start("AttackFlyLeft");
			}
			else
			{
				//�ִϸ��̼� ������
				ANIMATIONMANAGER->addAnimation("AttackFlyRight", "AttackFlyMove", 2, 3, 15, false, true);
				minionAni = ANIMATIONMANAGER->findAnimation("AttackFlyRight");
				ANIMATIONMANAGER->start("AttackFlyRight");
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
			distance = getDistance(vMinionAttackFly[i].enemyX, vMinionAttackFly[i].enemyY, PLAYERMANAGER->GetPlayerHitRectX(), PLAYERMANAGER->GetPlayerHitRectY());

			if (distance)
			{
				// vector = ((�÷��̾� ��ġ x / y) - (�� ��ġ x / y) / �Ÿ� * �� �ӵ�;
				vx = ((PLAYERMANAGER->GetPlayerHitRectX()) - vMinionAttackFly[i].enemyX) / distance * vMinionAttackFly[i].enemySpeed;
				vy = ((PLAYERMANAGER->GetPlayerHitRectY()) - vMinionAttackFly[i].enemyY) / distance * vMinionAttackFly[i].enemySpeed;
			}
			else
			{
				vx = 0;
				vy = vMinionAttackFly[i].enemySpeed;
			}

			// + - �ٲ㺸�� �̰� ���� ����� ��� �Ǵ���
			if (vMinionAttackFly[i].enemyRect.left >= 105 && vMinionAttackFly[i].enemyRect.right <= 780 &&
				vMinionAttackFly[i].enemyRect.top >= 105 && vMinionAttackFly[i].enemyRect.bottom <= 465)
			{
				vMinionAttackFly[i].enemyX += vx;
				vMinionAttackFly[i].enemyY += vy;
			}
			vMinionAttackFly[i].enemyRect = RectMakeCenter(vMinionAttackFly[i].enemyX, vMinionAttackFly[i].enemyY, 20, 20);
		}
		// �����ȿ� �÷��̾ ���ٸ� �����ൿ(AI)
		else
		{
			EnemyAiTime();

			switch (vMinionAttackFly[i].enemyNumber)
			{
			case 0:
				switch (firstEnemyAiPattern)
				{
				case 1:		// IDLE
					//�ִϸ��̼� ������
					ANIMATIONMANAGER->start("AttackFlyLeft");
					break;
				case 2:		// LEFT
					//�ִϸ��̼� ������
					minionAni = ANIMATIONMANAGER->findAnimation("AttackFlyLeft");
					ANIMATIONMANAGER->start("AttackFlyLeft");

					if (vMinionAttackFly[i].enemyRect.left > 105) // ���� �̵� ���� ����
					{
						vMinionAttackFly[i].enemyRect.left -= vMinionAttackFly[i].enemySpeed;
						vMinionAttackFly[i].enemyRect.right -= vMinionAttackFly[i].enemySpeed;
					}
					if (vMinionAttackFly[i].enemyRect.left <= 120)
					{
						firstEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					//�ִϸ��̼� ������
					ANIMATIONMANAGER->addAnimation("AttackFlyRight", "AttackFlyMove", 2, 3, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("AttackFlyRight");
					ANIMATIONMANAGER->start("AttackFlyRight");

					if (vMinionAttackFly[i].enemyRect.right < 780) // ���� �̵� ���� ����
					{
						vMinionAttackFly[i].enemyRect.left += vMinionAttackFly[i].enemySpeed;
						vMinionAttackFly[i].enemyRect.right += vMinionAttackFly[i].enemySpeed;
					}
					if (vMinionAttackFly[i].enemyRect.right >= 760)
					{
						firstEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					if (vMinionAttackFly[i].enemyRect.top > 105) // ���� �̵� ���� ����
					{
						vMinionAttackFly[i].enemyRect.top -= vMinionAttackFly[i].enemySpeed;
						vMinionAttackFly[i].enemyRect.bottom -= vMinionAttackFly[i].enemySpeed;
					}
					if (vMinionAttackFly[i].enemyRect.top <= 120)
					{
						firstEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					if (vMinionAttackFly[i].enemyRect.bottom < 465) // ���� �̵� ���� ����
					{
						vMinionAttackFly[i].enemyRect.top += vMinionAttackFly[i].enemySpeed;
						vMinionAttackFly[i].enemyRect.bottom += vMinionAttackFly[i].enemySpeed;
					}
					if (vMinionAttackFly[i].enemyRect.bottom >= 450)
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
					//�ִϸ��̼� ������
					ANIMATIONMANAGER->start("AttackFlyLeft");
					break;
				case 2:		// LEFT
					//�ִϸ��̼� ������
					minionAni = ANIMATIONMANAGER->findAnimation("AttackFlyLeft");
					ANIMATIONMANAGER->start("AttackFlyLeft");

					if (vMinionAttackFly[i].enemyRect.left > 105) // ���� �̵� ���� ����
					{
						vMinionAttackFly[i].enemyRect.left -= vMinionAttackFly[i].enemySpeed;
						vMinionAttackFly[i].enemyRect.right -= vMinionAttackFly[i].enemySpeed;
					}
					if (vMinionAttackFly[i].enemyRect.left <= 120)
					{
						secondEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					//�ִϸ��̼� ������
					ANIMATIONMANAGER->addAnimation("AttackFlyRight", "AttackFlyMove", 2, 3, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("AttackFlyRight");
					ANIMATIONMANAGER->start("AttackFlyRight");

					if (vMinionAttackFly[i].enemyRect.right < 780) // ���� �̵� ���� ����
					{
						vMinionAttackFly[i].enemyRect.left += vMinionAttackFly[i].enemySpeed;
						vMinionAttackFly[i].enemyRect.right += vMinionAttackFly[i].enemySpeed;
					}
					if (vMinionAttackFly[i].enemyRect.right >= 760)
					{
						secondEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					if (vMinionAttackFly[i].enemyRect.top > 105) // ���� �̵� ���� ����
					{
						vMinionAttackFly[i].enemyRect.top -= vMinionAttackFly[i].enemySpeed;
						vMinionAttackFly[i].enemyRect.bottom -= vMinionAttackFly[i].enemySpeed;
					}
					if (vMinionAttackFly[i].enemyRect.top <= 120)
					{
						secondEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					if (vMinionAttackFly[i].enemyRect.bottom < 465) // ���� �̵� ���� ����
					{
						vMinionAttackFly[i].enemyRect.top += vMinionAttackFly[i].enemySpeed;
						vMinionAttackFly[i].enemyRect.bottom += vMinionAttackFly[i].enemySpeed;
					}
					if (vMinionAttackFly[i].enemyRect.bottom >= 450)
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
					//�ִϸ��̼� ������
					ANIMATIONMANAGER->start("AttackFlyLeft");
					break;
				case 2:		// LEFT
					//�ִϸ��̼� ������
					minionAni = ANIMATIONMANAGER->findAnimation("AttackFlyLeft");
					ANIMATIONMANAGER->start("AttackFlyLeft");

					if (vMinionAttackFly[i].enemyRect.left > 105) // ���� �̵� ���� ����
					{
						vMinionAttackFly[i].enemyRect.left -= vMinionAttackFly[i].enemySpeed;
						vMinionAttackFly[i].enemyRect.right -= vMinionAttackFly[i].enemySpeed;
					}
					if (vMinionAttackFly[i].enemyRect.left <= 120)
					{
						thirdEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					//�ִϸ��̼� ������
					ANIMATIONMANAGER->addAnimation("AttackFlyRight", "AttackFlyMove", 2, 3, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("AttackFlyRight");
					ANIMATIONMANAGER->start("AttackFlyRight");

					if (vMinionAttackFly[i].enemyRect.right < 780) // ���� �̵� ���� ����
					{
						vMinionAttackFly[i].enemyRect.left += vMinionAttackFly[i].enemySpeed;
						vMinionAttackFly[i].enemyRect.right += vMinionAttackFly[i].enemySpeed;
					}
					if (vMinionAttackFly[i].enemyRect.right >= 760)
					{
						thirdEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					if (vMinionAttackFly[i].enemyRect.top > 105) // ���� �̵� ���� ����
					{
						vMinionAttackFly[i].enemyRect.top -= vMinionAttackFly[i].enemySpeed;
						vMinionAttackFly[i].enemyRect.bottom -= vMinionAttackFly[i].enemySpeed;
					}
					if (vMinionAttackFly[i].enemyRect.top <= 120)
					{
						thirdEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					if (vMinionAttackFly[i].enemyRect.bottom < 465) // ���� �̵� ���� ����
					{
						vMinionAttackFly[i].enemyRect.top += vMinionAttackFly[i].enemySpeed;
						vMinionAttackFly[i].enemyRect.bottom += vMinionAttackFly[i].enemySpeed;
					}
					if (vMinionAttackFly[i].enemyRect.bottom >= 450)
					{
						thirdEnemyAiPattern = 4;
					}
					break;
				}
				break;
			}
		}

		// ���� ������ �׻� ���� ��ǥ�� �Ѿƴٴ�
		vMinionAttackFly[i].enemyFireRange = RectMakeCenter(vMinionAttackFly[i].enemyX, vMinionAttackFly[i].enemyY, 300, 300);
	}

	// ���� ��� �ҷ��� ������
	BULLETMANAGER->MoveEnemyBullet(vEnemyBullet, viEnemyBullet);

	// ���� ��� �ҷ� �浹
	COLLISIONMANAGER->EnemyBulletCollision(vEnemyBullet, viEnemyBullet);
}

void MinionAttackFly::DeleteEnemy(int num)
{
	vMinionAttackFly.erase(vMinionAttackFly.begin() + num);
}

void MinionAttackFly::SetEnemyRectX(int enemyNum, int move)
{
	vMinionAttackFly[enemyNum].enemyRect.left += move;
	vMinionAttackFly[enemyNum].enemyRect.right += move;
}

void MinionAttackFly::SetEnemyRectY(int enemyNum, int move)
{
	vMinionAttackFly[enemyNum].enemyRect.top += move;
	vMinionAttackFly[enemyNum].enemyRect.bottom += move;
}
