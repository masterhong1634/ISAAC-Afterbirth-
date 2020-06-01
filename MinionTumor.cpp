#include "stdafx.h"
#include "MinionTumor.h"

MinionTumor::MinionTumor()
{
}

MinionTumor::~MinionTumor()
{
}

HRESULT MinionTumor::Init(POINT position, int EnemyNumber)
{
	//����ü ���� ����
	EnemyInfo MinionTumor;
	MinionTumor.enemyNumber = EnemyNumber;
	MinionTumor.enemyRect = RectMakeCenter(position.x, position.y, 45, 45);
	MinionTumor.enemyHp = 20;
	MinionTumor.enemyShotSpeed = 4.0f;
	MinionTumor.enemyShotRange = 300.0f;
	MinionTumor.enemyShotDelay = 20;
	MinionTumor.enemySpeed = 1.5f;
	// �ִϸ��̼� Idle
	MinionTumor.enemyShadowImage = IMAGEMANAGER->addImage("TomorShadow", "images/monster/tumor/tumorShadow.bmp", 120 / 3, 49 / 3, true, RGB(255, 0, 255));
	MinionTumor.enemyImage = IMAGEMANAGER->addFrameImage("TumorIdle", "images/monster/tumor/tumorMove.bmp", 1600, 200, 16, 2, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addAnimation("tumor", "TumorIdle", 0, 15, 15, false, true);
	minionAni = ANIMATIONMANAGER->findAnimation("tumor");
	vMinionTumor.push_back(MinionTumor);

	// IDLE
	firstEnemyAiPattern = 1;
	secondEnemyAiPattern = 1;
	thirdEnemyAiPattern = 1;

	// ����Ʈ
	EFFECTMANAGER->addEffect("effectPoof", "images/bullet/effectPoof.bmp", 256 * 2, 256 * 2, 64 * 2, 64 * 2, 30, 1.0f, 10);
	EFFECTMANAGER->play("effectPoof", MinionTumor.enemyRect.left, MinionTumor.enemyRect.top + 20);

	enemyMove = true;
	enemyAreaCheck = false;

	return S_OK;
}

void MinionTumor::Release()
{
}

void MinionTumor::Update()
{
	EnemyAi();
	COLLISIONMANAGER->SameVectorMinionCollision(vMinionTumor);
}

void MinionTumor::Render(HDC hdc)
{
	for (i = 0; i < vMinionTumor.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_F1))
		{
			//Rectangle(hdc, vMinionTumor[i].enemyFireRange.left, vMinionTumor[i].enemyFireRange.top, vMinionTumor[i].enemyFireRange.right, vMinionTumor[i].enemyFireRange.bottom);
			Rectangle(hdc, vMinionTumor[i].enemyRect.left, vMinionTumor[i].enemyRect.top, vMinionTumor[i].enemyRect.right, vMinionTumor[i].enemyRect.bottom);

			HBRUSH brush = CreateSolidBrush(RGB(255, 0, 51));
			FillRect(hdc, &vMinionTumor[i].enemyRect, brush);
			DeleteObject(brush);
		}

		if (enemyAtk)
		{
			vMinionTumor[i].enemyShadowImage->alphaRender(hdc, vMinionTumor[i].enemyRect.left + 3, vMinionTumor[i].enemyRect.top + 70, 70);
			vMinionTumor[i].enemyImage->aniRender(hdc, vMinionTumor[i].enemyRect.left - 20, vMinionTumor[i].enemyRect.top - 30, minionAni);
		}
		else
		{
			vMinionTumor[i].enemyShadowImage->alphaRender(hdc, vMinionTumor[i].enemyRect.left + 3, vMinionTumor[i].enemyRect.top + 70, 70);
			vMinionTumor[i].enemyImage->aniRender(hdc, vMinionTumor[i].enemyRect.left - 25, vMinionTumor[i].enemyRect.top - 30, minionAni);
		}
	}

	BULLETMANAGER->RenderBullet(hdc, vEnemyBullet, viEnemyBullet);
}

void MinionTumor::EnemyAiTime()
{
	switch (vMinionTumor[i].enemyNumber)
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

void MinionTumor::EnemyAi()
{
	for (i = 0; i < vMinionTumor.size(); i++)
	{
		RECT temp;

		// �� x��, y�� ��ǥ
		vMinionTumor[i].enemyX = vMinionTumor[i].enemyRect.left + (vMinionTumor[i].enemyRect.right - vMinionTumor[i].enemyRect.left) / 2;
		vMinionTumor[i].enemyY = vMinionTumor[i].enemyRect.top + (vMinionTumor[i].enemyRect.bottom - vMinionTumor[i].enemyRect.top) / 2;
		
		// ���� ���� �ִϸ��̼�
		EnemyAnimation();

		// �÷��̾�� ���� ������ �浹��
		if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &vMinionTumor[i].enemyFireRange) &&
			PLAYERMANAGER->GetPlayerDeath() == false)
		{
			enemyAreaCheck = true;
		}
		else
		{
			enemyAreaCheck = false;
		}

		// ���� ������
		if (vMinionTumor[i].enemyX > PLAYERMANAGER->GetPlayerHitRectX() &&
			(vMinionTumor[i].enemyRect.top + 15) < PLAYERMANAGER->GetPlayerHitRectY() &&
			(vMinionTumor[i].enemyRect.bottom - 15) > PLAYERMANAGER->GetPlayerHitRectY() &&
			enemyAreaCheck)
		{
			enemyMove = false;
			enemyLeftBoost = true;
			enemyRightBoost = false;
			enemyUpBoost = false;
			enemyDownBoost = false;
		}
		// ������ ������
		else if (vMinionTumor[i].enemyX < PLAYERMANAGER->GetPlayerHitRectX() &&
			(vMinionTumor[i].enemyRect.top + 15) < PLAYERMANAGER->GetPlayerHitRectY() &&
			(vMinionTumor[i].enemyRect.bottom - 15) > PLAYERMANAGER->GetPlayerHitRectY() &&
			enemyAreaCheck)
		{
			enemyMove = false;
			enemyRightBoost = true;
			enemyLeftBoost = false;
			enemyUpBoost = false;
			enemyDownBoost = false;
		}
		// �� ������
		else if (vMinionTumor[i].enemyY > PLAYERMANAGER->GetPlayerHitRectY())
		{
			if ((vMinionTumor[i].enemyRect.left + 15) < PLAYERMANAGER->GetPlayerHitRectX() &&
				(vMinionTumor[i].enemyRect.right - 15) > PLAYERMANAGER->GetPlayerHitRectX() &&
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
		else if (vMinionTumor[i].enemyY < PLAYERMANAGER->GetPlayerHitRectY())
		{
			if ((vMinionTumor[i].enemyRect.left + 15) < PLAYERMANAGER->GetPlayerHitRectX() &&
				(vMinionTumor[i].enemyRect.right - 15) > PLAYERMANAGER->GetPlayerHitRectX() &&
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
			vMinionTumor[i].enemyRect.left -= vMinionTumor[i].enemySpeed * 2;
			vMinionTumor[i].enemyRect.right -= vMinionTumor[i].enemySpeed * 2;

			if (vMinionTumor[i].enemyRect.left <= 140)
			{
				enemyMove = true;
				enemyLeftBoost = false;
				enemyAreaCheck = false;
			}
		}
		// �������̸� ������ ��������
		else if (enemyRightBoost)
		{
			vMinionTumor[i].enemyRect.left += vMinionTumor[i].enemySpeed * 2;
			vMinionTumor[i].enemyRect.right += vMinionTumor[i].enemySpeed * 2;

			if (vMinionTumor[i].enemyRect.right >= 740)
			{
				enemyMove = true;
				enemyRightBoost = false;
				enemyAreaCheck = false;
			}
		}
		// ���� ������ ��������
		else if (enemyUpBoost)
		{
			vMinionTumor[i].enemyRect.top -= vMinionTumor[i].enemySpeed * 2;
			vMinionTumor[i].enemyRect.bottom -= vMinionTumor[i].enemySpeed * 2;

			if (vMinionTumor[i].enemyRect.top <= 140)
			{
				enemyMove = true;
				enemyUpBoost = false;
				enemyAreaCheck = false;
			}
		}
		// �Ʒ��� ������ ��������
		else if (enemyDownBoost)
		{
			vMinionTumor[i].enemyRect.top += vMinionTumor[i].enemySpeed * 2;
			vMinionTumor[i].enemyRect.bottom += vMinionTumor[i].enemySpeed * 2;

			if (vMinionTumor[i].enemyRect.bottom >= 430)
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

			switch (vMinionTumor[i].enemyNumber)
			{
			case 0:
				switch (firstEnemyAiPattern)
				{
				case 1:		// IDLE
					break;
				case 2:		// LEFT
					if (vMinionTumor[i].enemyRect.left > 105) // ���� �̵� ���� ����
					{
						vMinionTumor[i].enemyRect.left -= vMinionTumor[i].enemySpeed;
						vMinionTumor[i].enemyRect.right -= vMinionTumor[i].enemySpeed;
					}
					if (vMinionTumor[i].enemyRect.left <= 120)
					{
						firstEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					if (vMinionTumor[i].enemyRect.right < 780) // ���� �̵� ���� ����
					{
						vMinionTumor[i].enemyRect.left += vMinionTumor[i].enemySpeed;
						vMinionTumor[i].enemyRect.right += vMinionTumor[i].enemySpeed;
					}
					if (vMinionTumor[i].enemyRect.right >= 760)
					{
						firstEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					if (vMinionTumor[i].enemyRect.top > 105) // ���� �̵� ���� ����
					{
						vMinionTumor[i].enemyRect.top -= vMinionTumor[i].enemySpeed;
						vMinionTumor[i].enemyRect.bottom -= vMinionTumor[i].enemySpeed;
					}
					if (vMinionTumor[i].enemyRect.top <= 120)
					{
						firstEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					if (vMinionTumor[i].enemyRect.bottom < 465) // ���� �̵� ���� ����
					{
						vMinionTumor[i].enemyRect.top += vMinionTumor[i].enemySpeed;
						vMinionTumor[i].enemyRect.bottom += vMinionTumor[i].enemySpeed;
					}
					if (vMinionTumor[i].enemyRect.bottom >= 450)
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
					break;
				case 2:		// LEFT
					if (vMinionTumor[i].enemyRect.left > 105) // ���� �̵� ���� ����
					{
						vMinionTumor[i].enemyRect.left -= vMinionTumor[i].enemySpeed;
						vMinionTumor[i].enemyRect.right -= vMinionTumor[i].enemySpeed;
					}
					if (vMinionTumor[i].enemyRect.left <= 120)
					{
						secondEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					if (vMinionTumor[i].enemyRect.right < 780) // ���� �̵� ���� ����
					{
						vMinionTumor[i].enemyRect.left += vMinionTumor[i].enemySpeed;
						vMinionTumor[i].enemyRect.right += vMinionTumor[i].enemySpeed;
					}
					if (vMinionTumor[i].enemyRect.right >= 760)
					{
						secondEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					if (vMinionTumor[i].enemyRect.top > 105) // ���� �̵� ���� ����
					{
						vMinionTumor[i].enemyRect.top -= vMinionTumor[i].enemySpeed;
						vMinionTumor[i].enemyRect.bottom -= vMinionTumor[i].enemySpeed;
					}
					if (vMinionTumor[i].enemyRect.top <= 120)
					{
						secondEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					if (vMinionTumor[i].enemyRect.bottom < 465) // ���� �̵� ���� ����
					{
						vMinionTumor[i].enemyRect.top += vMinionTumor[i].enemySpeed;
						vMinionTumor[i].enemyRect.bottom += vMinionTumor[i].enemySpeed;
					}
					if (vMinionTumor[i].enemyRect.bottom >= 450)
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
					break;
				case 2:		// LEFT
					if (vMinionTumor[i].enemyRect.left > 105) // ���� �̵� ���� ����
					{
						vMinionTumor[i].enemyRect.left -= vMinionTumor[i].enemySpeed;
						vMinionTumor[i].enemyRect.right -= vMinionTumor[i].enemySpeed;
					}
					if (vMinionTumor[i].enemyRect.left <= 120)
					{
						thirdEnemyAiPattern = 3;
					}
					break;
				case 3:		// RIGHT
					if (vMinionTumor[i].enemyRect.right < 780) // ���� �̵� ���� ����
					{
						vMinionTumor[i].enemyRect.left += vMinionTumor[i].enemySpeed;
						vMinionTumor[i].enemyRect.right += vMinionTumor[i].enemySpeed;
					}
					if (vMinionTumor[i].enemyRect.right >= 760)
					{
						thirdEnemyAiPattern = 2;
					}
					break;
				case 4:		// UP
					if (vMinionTumor[i].enemyRect.top > 105) // ���� �̵� ���� ����
					{
						vMinionTumor[i].enemyRect.top -= vMinionTumor[i].enemySpeed;
						vMinionTumor[i].enemyRect.bottom -= vMinionTumor[i].enemySpeed;
					}
					if (vMinionTumor[i].enemyRect.top <= 120)
					{
						thirdEnemyAiPattern = 5;
					}
					break;
				case 5:		// DOWN
					if (vMinionTumor[i].enemyRect.bottom < 465) // ���� �̵� ���� ����
					{
						vMinionTumor[i].enemyRect.top += vMinionTumor[i].enemySpeed;
						vMinionTumor[i].enemyRect.bottom += vMinionTumor[i].enemySpeed;
					}
					if (vMinionTumor[i].enemyRect.bottom >= 450)
					{
						thirdEnemyAiPattern = 4;
					}
					break;
				}
				break;
			}
		}

		// ���� ������ �׻� ���� ��ǥ�� �Ѿƴٴ�
		vMinionTumor[i].enemyFireRange = RectMakeCenter(vMinionTumor[i].enemyX, vMinionTumor[i].enemyY, 300, 300);
	}

	// ���� ��� �ҷ��� ������
	BULLETMANAGER->MoveEnemyBullet(vEnemyBullet, viEnemyBullet);

	// ���� ��� �ҷ� �浹
	COLLISIONMANAGER->EnemyBulletCollision(vEnemyBullet, viEnemyBullet);
}

void MinionTumor::EnemyShot()
{
	switch (vMinionTumor[i].enemyNumber)
	{
	case 0:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_0, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, firstEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_45, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, firstEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_135, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, firstEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_180, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, firstEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_225, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, firstEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_315, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, firstEnemyBulletCount, vMinionTumor[i].enemyShotDelay);

		// ���� �ҷ� ī��Ʈ�� �� ���� �÷���
		SetFirstEnemyBulletCount();
		break;
	case 1:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_0, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, secondEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_45, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, secondEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_135, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, secondEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_180, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, secondEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_225, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, secondEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_315, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, secondEnemyBulletCount, vMinionTumor[i].enemyShotDelay);

		// ���� �ҷ� ī��Ʈ�� �� ���� �÷���
		SetSecondEnemyBulletCount();
		break;
	case 2:
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_0, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, thirdEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_45, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, thirdEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_135, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, thirdEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_180, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, thirdEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_225, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, thirdEnemyBulletCount, vMinionTumor[i].enemyShotDelay);
		BULLETMANAGER->ShootBullet("enemyBullet", vEnemyBullet, vMinionTumor[i].enemyX, vMinionTumor[i].enemyY,
			ANGLE_315, vMinionTumor[i].enemyShotSpeed, vMinionTumor[i].enemyShotRange, thirdEnemyBulletCount, vMinionTumor[i].enemyShotDelay);

		// ���� �ҷ� ī��Ʈ�� �� ���� �÷���
		SetThirdEnemyBulletCount();
		break;
	}
}

void MinionTumor::EnemyAnimation()
{
	attackCount++;

	// ī��Ʈ�� 20 ~ 80 ���̶�� ���� �ִϸ��̼��� �����Ѵ�.
	if (attackCount > 20 && attackCount < 80)
	{
		enemyAtk = true;
	}
	// �� �ܿ��� ���� Idle �ִϸ��̼�
	else
	{
		enemyAtk = false;
	}

	// ī��Ʈ�� 50 ~ 70 ���̶�� ���� ����.
	if (attackCount > 50 && attackCount < 70)
	{
		EnemyShot();
	}
	// ���� ī��Ʈ�� 200�� �Ѿ�� �ʱ�ȭ
	else if (attackCount > 200)
	{
		attackCount = 0;
		ANIMATIONMANAGER->stop("tumorAtk");
	}

	if (enemyAtk)
	{
		// �ִϸ��̼� ������
		vMinionTumor[i].enemyImage = IMAGEMANAGER->addFrameImage("tumorAttack", "images/monster/tumor/tumorAttack.bmp", 500, 600, 5, 6, true, RGB(255, 0, 255));
		ANIMATIONMANAGER->addAnimation("tumorAtk", "tumorAttack", 0, 13, 15, false, true);
		minionAni = ANIMATIONMANAGER->findAnimation("tumorAtk");
		ANIMATIONMANAGER->resume("tumorAtk");
	}
	else
	{
		//�ִϸ��̼� ������
		vMinionTumor[i].enemyImage = IMAGEMANAGER->addFrameImage("TumorIdle", "images/monster/tumor/tumorMove.bmp", 1600, 200, 16, 2, true, RGB(255, 0, 255));
		ANIMATIONMANAGER->addAnimation("tumor", "TumorIdle", 0, 15, 15, false, true);
		minionAni = ANIMATIONMANAGER->findAnimation("tumor");
		ANIMATIONMANAGER->resume("tumor");
	}
}

void MinionTumor::DeleteEnemy(int num)
{
	vMinionTumor.erase(vMinionTumor.begin() + num);
}

void MinionTumor::SetEnemyRectX(int enemyNum, int move)
{
	vMinionTumor[enemyNum].enemyRect.left += move;
	vMinionTumor[enemyNum].enemyRect.right += move;
}

void MinionTumor::SetEnemyRectY(int enemyNum, int move)
{
	vMinionTumor[enemyNum].enemyRect.top += move;
	vMinionTumor[enemyNum].enemyRect.bottom += move;
}
