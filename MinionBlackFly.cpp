#include "stdafx.h"
#include "MinionBlackFly.h"

MinionBlackFly::MinionBlackFly()
{
}

MinionBlackFly::~MinionBlackFly()
{
}

HRESULT MinionBlackFly::Init(POINT position, int EnemyNumber)
{
	//����ü ���� ����
	EnemyInfo MinionBlackFly;
	MinionBlackFly.enemyNumber = EnemyNumber;
	MinionBlackFly.enemyRect = RectMakeCenter(position.x, position.y, 15, 15);
	MinionBlackFly.enemyHp = 5;
	MinionBlackFly.enemySpeed = 1.5f;
	// �ִϸ��̼� Idle
	MinionBlackFly.enemyShadowImage = IMAGEMANAGER->addImage("BlackFlyShadow", "images/monster/blackfly/blackflyShadow.bmp", 120 / 4, 49 / 4, true, RGB(255, 0, 255));
	MinionBlackFly.enemyImage = IMAGEMANAGER->addFrameImage("BlackFlyMove", "images/monster/blackfly/blackflyMove.bmp", 712 / 2, 183 / 2, 4, 1, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addAnimation("BlackFlyLeft", "BlackFlyMove", 0, 1, 15, false, true);
	minionAni = ANIMATIONMANAGER->findAnimation("BlackFlyLeft");
	vMinionBlackFly.push_back(MinionBlackFly);

	// IDLE
	firstEnemyAiPattern = 1;
	secondEnemyAiPattern = 1;
	thirdEnemyAiPattern = 1;

	// ����Ʈ
	EFFECTMANAGER->addEffect("effectPoof", "images/bullet/effectPoof.bmp", 256 * 2, 256 * 2, 64 * 2, 64 * 2, 30, 1.0f, 10);
	EFFECTMANAGER->play("effectPoof", MinionBlackFly.enemyRect.left, MinionBlackFly.enemyRect.top + 20);

	enemyMove = true;
	enemyAreaCheck = false;

	return S_OK;
}

void MinionBlackFly::Release()
{
}

void MinionBlackFly::Update()
{
	EnemyAi();
	COLLISIONMANAGER->SameVectorMinionCollision(vMinionBlackFly);
}

void MinionBlackFly::Render(HDC hdc)
{
	for (i = 0; i < vMinionBlackFly.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_F1))
		{
			//Rectangle(hdc, vMinionBlackFly[i].enemyFireRange.left, vMinionBlackFly[i].enemyFireRange.top, vMinionBlackFly[i].enemyFireRange.right, vMinionBlackFly[i].enemyFireRange.bottom);
			Rectangle(hdc, vMinionBlackFly[i].enemyRect.left, vMinionBlackFly[i].enemyRect.top, vMinionBlackFly[i].enemyRect.right, vMinionBlackFly[i].enemyRect.bottom);

			HBRUSH brush = CreateSolidBrush(RGB(255, 153, 0));
			FillRect(hdc, &vMinionBlackFly[i].enemyRect, brush);
			DeleteObject(brush);
		}

		vMinionBlackFly[i].enemyShadowImage->alphaRender(hdc, vMinionBlackFly[i].enemyRect.left - 8, vMinionBlackFly[i].enemyRect.top + 35, 70);
		vMinionBlackFly[i].enemyImage->aniRender(hdc, vMinionBlackFly[i].enemyRect.left - 37, vMinionBlackFly[i].enemyRect.top - 40, minionAni);
	}
}

void MinionBlackFly::EnemyAiTime()
{
	switch (vMinionBlackFly[i].enemyNumber)
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

void MinionBlackFly::EnemyAi()
{
	for (i = 0; i < vMinionBlackFly.size(); i++)
	{
		RECT temp;

		// �� x��, y�� ��ǥ
		vMinionBlackFly[i].enemyX = vMinionBlackFly[i].enemyRect.left + (vMinionBlackFly[i].enemyRect.right - vMinionBlackFly[i].enemyRect.left) / 2;
		vMinionBlackFly[i].enemyY = vMinionBlackFly[i].enemyRect.top + (vMinionBlackFly[i].enemyRect.bottom - vMinionBlackFly[i].enemyRect.top) / 2;

		// �÷��̾�� ���� ������ �浹��
		if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &vMinionBlackFly[i].enemyFireRange) &&
			PLAYERMANAGER->GetPlayerDeath() == false)
		{
			enemyAreaCheck = true;
		}
		else
		{
			enemyAreaCheck = false;
		}

		// ���� ������
		if (vMinionBlackFly[i].enemyX > PLAYERMANAGER->GetPlayerHitRectX() &&
			(vMinionBlackFly[i].enemyRect.top) < PLAYERMANAGER->GetPlayerHitRectY() &&
			(vMinionBlackFly[i].enemyRect.bottom) > PLAYERMANAGER->GetPlayerHitRectY() &&
			enemyAreaCheck)
		{
			//�ִϸ��̼� ������
			minionAni = ANIMATIONMANAGER->findAnimation("BlackFlyLeft");
			ANIMATIONMANAGER->resume("BlackFlyLeft");
			
			enemyMove = false;
			enemyLeftBoost = true;
			enemyRightBoost = false;
			enemyUpBoost = false;
			enemyDownBoost = false;
		}
		// ������ ������
		else if (vMinionBlackFly[i].enemyX < PLAYERMANAGER->GetPlayerHitRectX() &&
			(vMinionBlackFly[i].enemyRect.top) < PLAYERMANAGER->GetPlayerHitRectY() &&
			(vMinionBlackFly[i].enemyRect.bottom) > PLAYERMANAGER->GetPlayerHitRectY() &&
			enemyAreaCheck)
		{
			//�ִϸ��̼� ������
			ANIMATIONMANAGER->addAnimation("BlackFlyRight", "BlackFlyMove", 2, 3, 15, false, true);
			minionAni = ANIMATIONMANAGER->findAnimation("BlackFlyRight");
			ANIMATIONMANAGER->resume("BlackFlyRight");

			enemyMove = false;
			enemyRightBoost = true;
			enemyLeftBoost = false;
			enemyUpBoost = false;
			enemyDownBoost = false;
		}
		// �� ������
		else if (vMinionBlackFly[i].enemyY > PLAYERMANAGER->GetPlayerHitRectY())
		{
			if ((vMinionBlackFly[i].enemyRect.left) < PLAYERMANAGER->GetPlayerHitRectX() &&
				(vMinionBlackFly[i].enemyRect.right) > PLAYERMANAGER->GetPlayerHitRectX() &&
				enemyAreaCheck)
			{
				enemyMove = false;
				enemyUpBoost = true;
				enemyDownBoost = false;
				enemyLeftBoost = false;
				enemyRightBoost = false;
			}
		}
		// �Ʒ� ������
		else if (vMinionBlackFly[i].enemyY < PLAYERMANAGER->GetPlayerHitRectY())
		{
			if ((vMinionBlackFly[i].enemyRect.left) < PLAYERMANAGER->GetPlayerHitRectX() &&
				(vMinionBlackFly[i].enemyRect.right) > PLAYERMANAGER->GetPlayerHitRectX() &&
				enemyAreaCheck)
			{
				enemyMove = false;
				enemyDownBoost = true;
				enemyUpBoost = false;
				enemyLeftBoost = false;
				enemyRightBoost = false;
			}
		}

		// �����̸� ������ ��������
		if (enemyLeftBoost)
		{
			vMinionBlackFly[i].enemyRect.left -= vMinionBlackFly[i].enemySpeed * 2.5;
			vMinionBlackFly[i].enemyRect.right -= vMinionBlackFly[i].enemySpeed * 2.5;

			if (vMinionBlackFly[i].enemyRect.left <= 140)
			{
				enemyMove = true;
				enemyLeftBoost = false;
				enemyAreaCheck = false;
			}
		}
		// �������̸� ������ ��������
		else if (enemyRightBoost)
		{
			vMinionBlackFly[i].enemyRect.left += vMinionBlackFly[i].enemySpeed * 2.5;
			vMinionBlackFly[i].enemyRect.right += vMinionBlackFly[i].enemySpeed * 2.5;

			if (vMinionBlackFly[i].enemyRect.right >= 740)
			{
				enemyMove = true;
				enemyRightBoost = false;
				enemyAreaCheck = false;
			}
		}
		// ���� ������ ��������
		else if (enemyUpBoost)
		{
			vMinionBlackFly[i].enemyRect.top -= vMinionBlackFly[i].enemySpeed * 2.5;
			vMinionBlackFly[i].enemyRect.bottom -= vMinionBlackFly[i].enemySpeed * 2.5;

			if (vMinionBlackFly[i].enemyRect.top <= 140)
			{
				enemyMove = true;
				enemyUpBoost = false;
				enemyAreaCheck = false;
			}
		}
		// �Ʒ��� ������ ��������
		else if (enemyDownBoost)
		{
			vMinionBlackFly[i].enemyRect.top += vMinionBlackFly[i].enemySpeed * 2.5;
			vMinionBlackFly[i].enemyRect.bottom += vMinionBlackFly[i].enemySpeed * 2.5;

			if (vMinionBlackFly[i].enemyRect.bottom >= 430)
			{
				enemyMove = true;
				enemyDownBoost = false;
				enemyAreaCheck = false;
			}
		}

		// �����ൿ(AI)
		if (enemyMove)
		{
			EnemyAiTime();

			switch (vMinionBlackFly[i].enemyNumber)
			{
			case 0:
				switch (firstEnemyAiPattern)
				{
				case 1:		// IDLE
					//�ִϸ��̼� ������
					ANIMATIONMANAGER->resume("BlackFlyLeft");
					break;
				case 2:		// LEFT
					//�ִϸ��̼� ������
					minionAni = ANIMATIONMANAGER->findAnimation("BlackFlyLeft");
					ANIMATIONMANAGER->resume("BlackFlyLeft");

					if (vMinionBlackFly[i].enemyRect.left > 105) // ���� �̵� ���� ����
					{
						vMinionBlackFly[i].enemyRect.left -= vMinionBlackFly[i].enemySpeed;
						vMinionBlackFly[i].enemyRect.right -= vMinionBlackFly[i].enemySpeed;
					}
					if (vMinionBlackFly[i].enemyRect.left <= 120)
					{
						firstEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					//�ִϸ��̼� ������
					ANIMATIONMANAGER->addAnimation("BlackFlyRight", "BlackFlyMove", 2, 3, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("BlackFlyRight");
					ANIMATIONMANAGER->resume("BlackFlyRight");

					if (vMinionBlackFly[i].enemyRect.right < 780) // ���� �̵� ���� ����
					{
						vMinionBlackFly[i].enemyRect.left += vMinionBlackFly[i].enemySpeed;
						vMinionBlackFly[i].enemyRect.right += vMinionBlackFly[i].enemySpeed;
					}
					if (vMinionBlackFly[i].enemyRect.right >= 760)
					{
						firstEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					if (vMinionBlackFly[i].enemyRect.top > 105) // ���� �̵� ���� ����
					{
						vMinionBlackFly[i].enemyRect.top -= vMinionBlackFly[i].enemySpeed;
						vMinionBlackFly[i].enemyRect.bottom -= vMinionBlackFly[i].enemySpeed;
					}
					if (vMinionBlackFly[i].enemyRect.top <= 120)
					{
						firstEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					if (vMinionBlackFly[i].enemyRect.bottom < 465) // ���� �̵� ���� ����
					{
						vMinionBlackFly[i].enemyRect.top += vMinionBlackFly[i].enemySpeed;
						vMinionBlackFly[i].enemyRect.bottom += vMinionBlackFly[i].enemySpeed;
					}
					if (vMinionBlackFly[i].enemyRect.bottom >= 450)
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
					ANIMATIONMANAGER->resume("BlackFlyLeft");
					break;
				case 2:		// LEFT
					//�ִϸ��̼� ������
					minionAni = ANIMATIONMANAGER->findAnimation("BlackFlyLeft");
					ANIMATIONMANAGER->resume("BlackFlyLeft");

					if (vMinionBlackFly[i].enemyRect.left > 105) // ���� �̵� ���� ����
					{
						vMinionBlackFly[i].enemyRect.left -= vMinionBlackFly[i].enemySpeed;
						vMinionBlackFly[i].enemyRect.right -= vMinionBlackFly[i].enemySpeed;
					}
					if (vMinionBlackFly[i].enemyRect.left <= 120)
					{
						secondEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					//�ִϸ��̼� ������
					ANIMATIONMANAGER->addAnimation("BlackFlyRight", "BlackFlyMove", 2, 3, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("BlackFlyRight");
					ANIMATIONMANAGER->resume("BlackFlyRight");

					if (vMinionBlackFly[i].enemyRect.right < 780) // ���� �̵� ���� ����
					{
						vMinionBlackFly[i].enemyRect.left += vMinionBlackFly[i].enemySpeed;
						vMinionBlackFly[i].enemyRect.right += vMinionBlackFly[i].enemySpeed;
					}
					if (vMinionBlackFly[i].enemyRect.right >= 760)
					{
						secondEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					if (vMinionBlackFly[i].enemyRect.top > 105) // ���� �̵� ���� ����
					{
						vMinionBlackFly[i].enemyRect.top -= vMinionBlackFly[i].enemySpeed;
						vMinionBlackFly[i].enemyRect.bottom -= vMinionBlackFly[i].enemySpeed;
					}
					if (vMinionBlackFly[i].enemyRect.top <= 120)
					{
						secondEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					if (vMinionBlackFly[i].enemyRect.bottom < 465) // ���� �̵� ���� ����
					{
						vMinionBlackFly[i].enemyRect.top += vMinionBlackFly[i].enemySpeed;
						vMinionBlackFly[i].enemyRect.bottom += vMinionBlackFly[i].enemySpeed;
					}
					if (vMinionBlackFly[i].enemyRect.bottom >= 450)
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
					ANIMATIONMANAGER->resume("BlackFlyLeft");
					break;
				case 2:		// LEFT
					//�ִϸ��̼� ������
					minionAni = ANIMATIONMANAGER->findAnimation("BlackFlyLeft");
					ANIMATIONMANAGER->resume("BlackFlyLeft");

					if (vMinionBlackFly[i].enemyRect.left > 105) // ���� �̵� ���� ����
					{
						vMinionBlackFly[i].enemyRect.left -= vMinionBlackFly[i].enemySpeed;
						vMinionBlackFly[i].enemyRect.right -= vMinionBlackFly[i].enemySpeed;
					}
					if (vMinionBlackFly[i].enemyRect.left <= 120)
					{
						thirdEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					//�ִϸ��̼� ������
					ANIMATIONMANAGER->addAnimation("BlackFlyRight", "BlackFlyMove", 2, 3, 15, false, true);
					minionAni = ANIMATIONMANAGER->findAnimation("BlackFlyRight");
					ANIMATIONMANAGER->resume("BlackFlyRight");

					if (vMinionBlackFly[i].enemyRect.right < 780) // ���� �̵� ���� ����
					{
						vMinionBlackFly[i].enemyRect.left += vMinionBlackFly[i].enemySpeed;
						vMinionBlackFly[i].enemyRect.right += vMinionBlackFly[i].enemySpeed;
					}
					if (vMinionBlackFly[i].enemyRect.right >= 760)
					{
						thirdEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					if (vMinionBlackFly[i].enemyRect.top > 105) // ���� �̵� ���� ����
					{
						vMinionBlackFly[i].enemyRect.top -= vMinionBlackFly[i].enemySpeed;
						vMinionBlackFly[i].enemyRect.bottom -= vMinionBlackFly[i].enemySpeed;
					}
					if (vMinionBlackFly[i].enemyRect.top <= 120)
					{
						thirdEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					if (vMinionBlackFly[i].enemyRect.bottom < 465) // ���� �̵� ���� ����
					{
						vMinionBlackFly[i].enemyRect.top += vMinionBlackFly[i].enemySpeed;
						vMinionBlackFly[i].enemyRect.bottom += vMinionBlackFly[i].enemySpeed;
					}
					if (vMinionBlackFly[i].enemyRect.bottom >= 450)
					{
						thirdEnemyAiPattern = 4;
					}
					break;
				}
				break;
			}
		}

		// ���� ������ �׻� ���� ��ǥ�� �Ѿƴٴ�
		vMinionBlackFly[i].enemyFireRange = RectMakeCenter(vMinionBlackFly[i].enemyX, vMinionBlackFly[i].enemyY, 300, 300);
	}
}

void MinionBlackFly::DeleteEnemy(int num)
{
	vMinionBlackFly.erase(vMinionBlackFly.begin() + num);
}

void MinionBlackFly::SetEnemyRectX(int enemyNum, int move)
{
	vMinionBlackFly[enemyNum].enemyRect.left += move;
	vMinionBlackFly[enemyNum].enemyRect.right += move;
}

void MinionBlackFly::SetEnemyRectY(int enemyNum, int move)
{
	vMinionBlackFly[enemyNum].enemyRect.top += move;
	vMinionBlackFly[enemyNum].enemyRect.bottom += move;
}
