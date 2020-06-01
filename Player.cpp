#include "stdafx.h"
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

HRESULT Player::Init()
{
	// �÷��̾� �׸���
	player.playerShadowImage = IMAGEMANAGER->addImage("shadow", "images/player/playerShadow.bmp", 120 / 3, 49 / 3, true, RGB(255, 0, 255));
	// �÷��̾� HeadIdle
	player.playerHeadImage = IMAGEMANAGER->addFrameImage("playerHead", "images/player/player.bmp", 320 * 2, 124 * 2, 10, 4, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addAnimation("headIdle", "playerHead", arrHeadIdle, 1, 1, true);
	aniHead = ANIMATIONMANAGER->findAnimation("headIdle");
	// �÷��̾� BodyIdle
	player.playerBodyImage = IMAGEMANAGER->addFrameImage("playerBody", "images/player/player.bmp", 320 * 2, 124 * 2, 10, 4, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addAnimation("bodyIdle", "playerBody", arrBodyIdle, 1, 1, true);
	aniBody = ANIMATIONMANAGER->findAnimation("bodyIdle");
	// �÷��̾� Hit
	player.playerHitImage = IMAGEMANAGER->addFrameImage("PlayerHit", "images/player/PlayerHit.bmp", 944 / 1.5, 135 / 1.5, 8, 1, true, RGB(255, 0, 255));
	ANIMATIONMANAGER->addDefAnimation("playerHit", "PlayerHit", 25, false, true);
	aniHit = ANIMATIONMANAGER->findAnimation("playerHit");
	// �÷��̾� UI �̹���
	playerUINumber = IMAGEMANAGER->addFrameImage("playerUINumber", "images/UI/ingameUI/numbers.bmp", 110, 12, 11, 1, true, RGB(255, 0, 255));
	playerUIstats = IMAGEMANAGER->addFrameImage("playerUIStats", "images/UI/ingameUI/playerstats.bmp", 150, 30, 5, 1, true, RGB(255, 0, 255));
	playerUIpicks = IMAGEMANAGER->addFrameImage("playerUIpicks", "images/UI/ingameUI/uipicks.bmp", 102, 36, 3, 1, true, RGB(255, 0, 255));
	playerUIhearts = IMAGEMANAGER->addFrameImage("playerUIhearts", "images/UI/ingameUI/uiheart.bmp", 96, 32, 3, 1, true, RGB(255, 0, 255));
	
	// ������ �̹���
	blackBg = IMAGEMANAGER->addImage("blackBg", "images/UI/menu/blackBg.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	gameOver = IMAGEMANAGER->addImage("gameOver", "images/UI/menu/gameOver.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	// �÷��̾� ����
	player.playerHeadRect = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 32 * 2, 23 * 2);			// �Ӹ� ����
	player.playerBodyRect = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2 + 30, 32 * 2, 11 * 2);	// �� ����
	player.playerOffensePower = 5;																// ���ݷ�
	player.playerShotSpeed = 8.0f;																// ���ݼӵ�
	player.playerShotRange = 450.0f;															// ���ݻ�Ÿ�
	player.playerShotDelay = 25;																// �����ֱ�
	player.playerSpeed = 3.0f;																	// �̵��ӵ�
	player.playerSlideSpeed = 2.0f;																// �����̵� �ӵ�
	playerBulletCount = 0;																		// �ҷ� ī��Ʈ
	playerDeathCount = 0;																		// ��� ī��Ʈ

	player.playerMaxHp = 3.0f;
	player.playerHp = 3.0f;
	player.playerGold = 40;
	player.playerBomb = 1;
	player.playerKey = 1;

	// �÷��̾� ���� ���� �ʱ�ȭ
	isLeft = false;
	isRight = false;
	isUp = false;
	isDown = false;
	// �÷��̾� �����̵� ���� �ʱ�ȭ
	slideLeft = false;
	slideRight = false;
	slideUp = false;
	slideDown = false;
	// �÷��̾� ���� ���� �ʱ�ȭ
	playerLeftShot = false;
	playerRightShot = false;
	playerUpShot = false;
	playerDownShot = false;
	// �÷��̾� ��� ���� �ʱ�ȭ
	playerDeath = false;

	// �÷��̾� ������
	direction = PLAYER_IDLE;

	blackBg = IMAGEMANAGER->addImage("blackBg", "images/ending/blackBg.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	//������ ����
	vPlayerActiveItem = ITEMMANAGER->GetActiveItemInfo();
	vPlayerPassiveItem = ITEMMANAGER->GetPassiveItemInfo();
	vPlayerTrinkets = ITEMMANAGER->GetTrinketsInfo();
	vPlayerHeart = ITEMMANAGER->GetHeartInfo();
	//vPlayerBomb = ITEMMANAGER->GetBombInfo();
	vPlayerKey = ITEMMANAGER->GetKeyInfo();
	vPlayerCard = ITEMMANAGER->GetCardInfo();
	vPlayerPill = ITEMMANAGER->GetPillInfo();

	return S_OK;
}

void Player::Release()
{

}

void Player::Update()
{
	if (player.playerHp <= 0)
	{
		playerDeath = true;
	}

	if (playerDeath)
	{
		PlayerDeath();
	}
	else
	{
		PlayerAnimation();
		PlayerMove();
		PlayerShot();
		COLLISIONMANAGER->PlayerBulletCollision(vPlayerBullet, viPlayerBullet);
	}
	PlayerStatUpdate();

	if (deathCutSceneCount >= 150)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN) || KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			SCENEMANAGER->changeScene("MainMenu");
		}
	}
}

void Player::Render(HDC hdc)
{
	BULLETMANAGER->RenderBomb(hdc, vPlayerBomb, viPlayerBomb);
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		//Rectangle(hdc, player.playerHeadRect.left, player.playerHeadRect.top, player.playerHeadRect.right, player.playerHeadRect.bottom);
		//Rectangle(hdc, player.playerBodyRect.left, player.playerBodyRect.top, player.playerBodyRect.right, player.playerBodyRect.bottom);
		Rectangle(hdc, player.playerHitRect.left, player.playerHitRect.top, player.playerHitRect.right, player.playerHitRect.bottom);

		HBRUSH brush = CreateSolidBrush(RGB(255, 255, 153));
		FillRect(hdc, &player.playerHitRect, brush);
		DeleteObject(brush);
	}

	if (ENEMYMANAGER->GetGameEnd() == true)
	{
		player.playerBodyImage->aniRender(hdc, player.playerBodyRect.left - 10, player.playerBodyRect.top - 120, aniBody);
	}
	else
	{
		if (!playerDeath)
		{
			if (COLLISIONMANAGER->GetplayerHitAni() == true)
			{
				player.playerShadowImage->alphaRender(hdc, player.playerBodyRect.left + 12, player.playerBodyRect.top + 12, 70);
				player.playerHitImage->aniRender(hdc, player.playerBodyRect.left - 3, player.playerBodyRect.top - 63, aniHit);
			}
			else if (COLLISIONMANAGER->GetplayerHitAni() == false)
			{
				player.playerShadowImage->alphaRender(hdc, player.playerBodyRect.left + 12, player.playerBodyRect.top + 12, 70);
				player.playerBodyImage->aniRender(hdc, player.playerBodyRect.left, player.playerBodyRect.top - 20, aniBody);
				player.playerHeadImage->aniRender(hdc, player.playerHeadRect.left, player.playerHeadRect.top - 5, aniHead);
			}

			BULLETMANAGER->RenderBullet(hdc, vPlayerBullet, viPlayerBullet);

			// ���� ������ ǥ��
			if (vPlayerAllItem.size() > 0)
			{
				int j = 0;
				int k = 0;
				for (int i = 0; i < vPlayerAllItem.size(); i++)
				{
					if (i % 2 == 0 && i != 0)
					{
						k = 0;
						j++;
					}
					vPlayerAllItem[i].itemRect = RectMakeCenter(780 + 60 * k, 180 + 60 * j, 52, 52);
					//Rectangle(hdc, vPlayerAllItem[i].itemRect.left, vPlayerAllItem[i].itemRect.top, vPlayerAllItem[i].itemRect.right, vPlayerAllItem[i].itemRect.bottom);
					vPlayerAllItem[i].itemImage->alphaRender(hdc, vPlayerAllItem[i].itemRect.left, vPlayerAllItem[i].itemRect.top, 200);
					k++;
				}
			}
		}
		else
		{
			player.playerBodyImage->aniRender(hdc, player.playerBodyRect.left - 13, player.playerBodyRect.top - 349, aniBody);
			deathCutSceneCount++;
			if (deathCutSceneCount >= 150)
			{
				blackBg->alphaRender(hdc, 0, 0, 150);
				gameOver->render(hdc, 0, 0);
			}
		}
	}
	//void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);
	
	if (KEYMANAGER->isToggleKey(VK_F3))
	{
		sprintf_s((str), "Maxhp : %f", player.playerMaxHp);
		TextOut(hdc, 200, 80, str, strlen(str));

		sprintf_s((str), "hp : %f", player.playerHp);
		TextOut(hdc, 200, 100, str, strlen(str));

		sprintf_s((str), "gold : %d", player.playerGold);
		TextOut(hdc, 200, 120, str, strlen(str));

		sprintf_s((str), "bomb : %d", player.playerBomb);
		TextOut(hdc, 200, 140, str, strlen(str));

		sprintf_s((str), "playerKey : %d", player.playerKey);
		TextOut(hdc, 200, 160, str, strlen(str));

		sprintf_s((str), "playerOffensePower : %d", player.playerOffensePower);
		TextOut(hdc, 200, 180, str, strlen(str));

		sprintf_s((str), "playerShotSpeed : %f", player.playerShotSpeed);
		TextOut(hdc, 200, 200, str, strlen(str));

		sprintf_s((str), "playerShotRange : %f", player.playerShotRange);
		TextOut(hdc, 200, 220, str, strlen(str));

		sprintf_s((str), "playerShotDelay : %f", player.playerShotDelay);
		TextOut(hdc, 200, 240, str, strlen(str));

		sprintf_s((str), "playerSpeed : %f", player.playerSpeed);
		TextOut(hdc, 200, 260, str, strlen(str));

		sprintf_s((str), "playerSlideSpeed : %f", player.playerSlideSpeed);
		TextOut(hdc, 200, 280, str, strlen(str));

		sprintf_s((str), "Item : %d", vPlayerAllItem.size());
		TextOut(hdc, 200, 300, str, strlen(str));
	}

	//UI ���, ��ź, ����, ��Ʈ
	if (player.playerHp <= 0 && player.playerMaxHp == 3)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 2, 0);
	}
	else if (player.playerHp <= 0 && player.playerMaxHp == 4)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 3, 15, 2, 0);
	}
	else if (player.playerHp == 3 && player.playerMaxHp == 4)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 3, 15, 2, 0);
	}
	if (player.playerHp == 0.5 && player.playerMaxHp == 3)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 1, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 2, 0);
	}
	else if (player.playerHp == 0.5 && player.playerMaxHp == 4)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 1, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 3, 15, 2, 0);
	}
	if (player.playerHp == 1 && player.playerMaxHp == 3)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 2, 0);
	}
	else if (player.playerHp == 1 && player.playerMaxHp == 4)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 3, 15, 2, 0);
	}
	if (player.playerHp == 1.5 && player.playerMaxHp == 3)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 1, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 2, 0);
	}
	else if (player.playerHp == 1.5 && player.playerMaxHp == 4)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 1, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 3, 15, 2, 0);
	}
	if (player.playerHp == 2 && player.playerMaxHp == 3)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 2, 0);
	}
	else if (player.playerHp == 2 && player.playerMaxHp == 4)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 2, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 3, 15, 2, 0);
	}
	if (player.playerHp == 2.5 && player.playerMaxHp == 3)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 1, 0);
	}
	else if (player.playerHp == 2.5 && player.playerMaxHp == 4)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 1, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 3, 15, 2, 0);
	}
	if (player.playerHp == 3 && player.playerMaxHp == 3)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32*2, 15, 0, 0);
	}
	else if (player.playerHp == 3 && player.playerMaxHp == 4)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 3, 15, 2, 0);
	}
	if (player.playerHp == 3.5 && player.playerMaxHp == 4)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 3, 15, 1, 0);
	}
	if (player.playerHp == 4 && player.playerMaxHp == 4)
	{
		playerUIhearts->frameRender(hdc, 105, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 2, 15, 0, 0);
		playerUIhearts->frameRender(hdc, 105 + 32 * 3, 15, 0, 0);
	}


	for (int i = 0; i < 3; i++)
	{
		playerUIpicks->frameRender(hdc, 10, 80 + 30 * i, i, 0);
	}
	playerUINumber->frameRender(hdc, 42, 92, currentGold[0], 0);
	playerUINumber->frameRender(hdc, 52, 92, currentGold[1], 0);

	playerUINumber->frameRender(hdc, 42, 123, currentBomb[0], 0);
	playerUINumber->frameRender(hdc, 52, 123, currentBomb[1], 0);

	playerUINumber->frameRender(hdc, 42, 154, currentKey[0], 0);
	playerUINumber->frameRender(hdc, 52, 154, currentKey[1], 0);

	//UI �̹��� ����
	for (int i = 0; i < 5; i++)
	{
		playerUIstats->alphaRender(hdc, 10, 200 + 40 * i, 30 * i, 0, 30, 30, 125);
	}
	//UI ���ǵ�
	for (int i = 0; i < 5; i++)
	{
		if (i < 2)
		{
			playerUINumber->alphaRender(hdc, 50 + 8 * i, 210, 0 + 10 * currentSpeedFrame[i], 0, 10, 12, 125);
		}
		else if (i >= 2 && i < 4)
		{
			playerUINumber->alphaRender(hdc, 55 + 8 * i, 210, 0 + 10 * currentSpeedFrame[i], 0, 10, 12, 125);
		}
		else
		{
			playerUINumber->alphaRender(hdc, 66, 210, 0 + 10 * 10, 0, 10, 12, 125);
		}
	}
	//UI Range
	for (int i = 0; i < 6; i++)
	{
		if (i < 3)
		{
			playerUINumber->alphaRender(hdc, 50 + 8 * i, 250, 0 + 10 * currentRangeFrame[i], 0, 10, 12, 125);
		}
		else if (i >= 3 && i < 5)
		{
			playerUINumber->alphaRender(hdc, 58 + 8 * i, 250, 0 + 10 * currentRangeFrame[i], 0, 10, 12, 125);
		}
		else
		{
			playerUINumber->alphaRender(hdc, 76, 250, 0 + 10 * 10, 0, 10, 12, 125);
		}
	}
	//UI shotDelay
	for (int i = 0; i < 5; i++)
	{
		if (i < 2)
		{
			playerUINumber->alphaRender(hdc, 50 + 8 * i, 290, 0 + 10 * currentShotDelayFrame[i], 0, 10, 12, 125);
		}
		else if (i >= 2 && i < 4)
		{
			playerUINumber->alphaRender(hdc, 55 + 8 * i, 290, 0 + 10 * currentShotDelayFrame[i], 0, 10, 12, 125);
		}
		else
		{
			playerUINumber->alphaRender(hdc, 66, 290, 0 + 10 * 10, 0, 10, 12, 125);
		}
	}
	//UI shotSpeed
	for (int i = 0; i < 5; i++)
	{
		if (i < 2)
		{
			playerUINumber->alphaRender(hdc, 50 + 8 * i, 330, 0 + 10 * currentShotSpeedFrame[i], 0, 10, 12, 125);
		}
		else if (i >= 2 && i < 4)
		{
			playerUINumber->alphaRender(hdc, 55 + 8 * i, 330, 0 + 10 * currentShotSpeedFrame[i], 0, 10, 12, 125);
		}
		else
		{
			playerUINumber->alphaRender(hdc, 65, 330, 0 + 10 * 10, 0, 10, 12, 125);
		}
	}
	//UI ���ݷ�
	for (int i = 0; i < 5; i++)
	{
		if (i < 2)
		{
			playerUINumber->alphaRender(hdc, 50 + 8 * i, 370, 0 + 10 * currentOffenseFrame[i], 0, 10, 12, 125);
		}
		else if (i >= 2 && i < 4)
		{
			playerUINumber->alphaRender(hdc, 55 + 8 * i, 370, 0 + 10 * currentOffenseFrame[i], 0, 10, 12, 125);
		}
		else
		{
			playerUINumber->alphaRender(hdc, 66, 370, 0 + 10 * 10, 0, 10, 12, 125);
		}
	}


}

void Player::PlayerDeath()
{
	playerDeathCount++;

	if (playerDeathCount < 167)
	{
		player.playerBodyImage = IMAGEMANAGER->addFrameImage("PlayerDeath", "images/player/playerDeath.bmp", 1935, 1568, 15, 4, true, RGB(255, 0, 255));
		ANIMATIONMANAGER->addAnimation("playerDeath", "PlayerDeath", 0, 56, 20, false, true);
		aniBody = ANIMATIONMANAGER->findAnimation("playerDeath");
		ANIMATIONMANAGER->resume("playerDeath");
	}
	else
	{
		ANIMATIONMANAGER->pause("playerDeath");
	}
}

void Player::PlayerMove()
{
	//����
	if (KEYMANAGER->isStayKeyDown('F'))
	{
		direction = PLAYER_LEFT;
		isLeft = true;

		player.playerHeadRect.left -= player.playerSpeed;
		player.playerHeadRect.right -= player.playerSpeed;

		player.playerBodyRect.left -= player.playerSpeed;
		player.playerBodyRect.right -= player.playerSpeed;
	}
	if (KEYMANAGER->isOnceKeyUp('F'))
	{
		direction = PLAYER_IDLE;
		slideLeft = true;
	}

	//������
	if (KEYMANAGER->isStayKeyDown('H'))
	{
		direction = PLAYER_RIGHT;
		isRight = true;

		player.playerHeadRect.left += player.playerSpeed;
		player.playerHeadRect.right += player.playerSpeed;

		player.playerBodyRect.left += player.playerSpeed;
		player.playerBodyRect.right += player.playerSpeed;
	}
	if (KEYMANAGER->isOnceKeyUp('H'))
	{
		direction = PLAYER_IDLE;
		slideRight = true;
	}

	//��
	if (KEYMANAGER->isStayKeyDown('T'))
	{
		direction = PLAYER_UP;
		isUp = true;

		player.playerHeadRect.top -= player.playerSpeed;
		player.playerHeadRect.bottom -= player.playerSpeed;

		player.playerBodyRect.top -= player.playerSpeed;
		player.playerBodyRect.bottom -= player.playerSpeed;
	}
	if (KEYMANAGER->isOnceKeyUp('T'))
	{
		direction = PLAYER_IDLE;
		slideUp = true;
	}

	//�Ʒ�
	if (KEYMANAGER->isStayKeyDown('G'))
	{
		direction = PLAYER_DOWN;
		isDown = true;

		player.playerHeadRect.top += player.playerSpeed;
		player.playerHeadRect.bottom += player.playerSpeed;

		player.playerBodyRect.top += player.playerSpeed;
		player.playerBodyRect.bottom += player.playerSpeed;
	}
	if (KEYMANAGER->isOnceKeyUp('G'))
	{
		direction = PLAYER_IDLE;
		slideDown = true;
	}

	PlayerSilde();	//�÷��̾� �����̵�

	//�÷��̾� �ǰ� ����
	player.playerHitRect = RectMakeCenter(player.playerBodyRect.left + (player.playerBodyRect.right - player.playerBodyRect.left) / 2,
		player.playerBodyRect.top + (player.playerBodyRect.bottom - player.playerBodyRect.top) / 2, 40, 20);
}

void Player::PlayerSilde()
{
	//���� �����̵�
	if (slideLeft)
	{
		player.playerSlideSpeed = player.playerSlideSpeed * 0.9f;

		player.playerHeadRect.left -= player.playerSlideSpeed;
		player.playerHeadRect.right -= player.playerSlideSpeed;

		player.playerBodyRect.left -= player.playerSlideSpeed;
		player.playerBodyRect.right -= player.playerSlideSpeed;

		if (player.playerSlideSpeed < 0.8f)
		{
			player.playerSlideSpeed = 2.0f;
			isLeft = false;
			slideLeft = false;
		}
	}

	//������ �����̵�
	else if (slideRight)
	{
		player.playerSlideSpeed = player.playerSlideSpeed * 0.9f;

		player.playerHeadRect.left += int(player.playerSlideSpeed + 1);
		player.playerHeadRect.right += int(player.playerSlideSpeed + 1);

		player.playerBodyRect.left += int(player.playerSlideSpeed + 1);
		player.playerBodyRect.right += int(player.playerSlideSpeed + 1);

		if (player.playerSlideSpeed < 0.8f)
		{
			player.playerSlideSpeed = 2.0f;
			isRight = false;
			slideRight = false;
		}
	}

	//���� �����̵�
	else if (slideUp)
	{
		player.playerSlideSpeed = player.playerSlideSpeed * 0.9f;

		player.playerHeadRect.top -= player.playerSlideSpeed;
		player.playerHeadRect.bottom -= player.playerSlideSpeed;

		player.playerBodyRect.top -= player.playerSlideSpeed;
		player.playerBodyRect.bottom -= player.playerSlideSpeed;

		if (player.playerSlideSpeed < 0.8f)
		{
			player.playerSlideSpeed = 2.0f;
			isUp = false;
			slideUp = false;
		}
	}

	//�Ʒ��� �����̵�
	else if (slideDown)
	{
		player.playerSlideSpeed = player.playerSlideSpeed * 0.9f;

		player.playerHeadRect.top += int(player.playerSlideSpeed + 1);
		player.playerHeadRect.bottom += int(player.playerSlideSpeed + 1);

		player.playerBodyRect.top += int(player.playerSlideSpeed + 1);
		player.playerBodyRect.bottom += int(player.playerSlideSpeed + 1);

		if (player.playerSlideSpeed < 0.8f)
		{
			player.playerSlideSpeed = 2.0f;
			isDown = false;
			slideDown = false;
		}
	}
}

void Player::PlayerShot()
{
	//���� �Ѿ� �߻�
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		playerLeftShot = true;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
	{
		playerLeftShot = false;

		//�ִϸ��̼� ������
		aniHead = ANIMATIONMANAGER->findAnimation("shotLeft");
		ANIMATIONMANAGER->stop("shotLeft");
	}

	//������ �Ѿ� �߻�
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		playerRightShot = true;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
	{
		playerRightShot = false;

		//�ִϸ��̼� ������
		aniHead = ANIMATIONMANAGER->findAnimation("shotRight");
		ANIMATIONMANAGER->stop("shotRight");
	}

	//���� �Ѿ� �߻�
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		playerUpShot = true;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_UP))
	{
		playerUpShot = false;

		//�ִϸ��̼� ������
		aniHead = ANIMATIONMANAGER->findAnimation("shotUp");
		ANIMATIONMANAGER->stop("shotUp");
	}

	//�Ʒ��� �Ѿ� �߻�
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		playerDownShot = true;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
	{
		playerDownShot = false;

		//�ִϸ��̼� ������
		aniHead = ANIMATIONMANAGER->findAnimation("shotDown");
		ANIMATIONMANAGER->stop("shotDown");
	}

	//��ź �����ð�
	playerBombDelay++;

	if (KEYMANAGER->isOnceKeyDown('Y'))
	{
		//string imageName, vector<BombInfo>& bombVector, int x, int y, int damage, int range, int delayTime
		if (playerBombDelay > 125)
		{
			BULLETMANAGER->ShootBomb("playerBomb", vPlayerBomb,
				player.playerHeadRect.left + (player.playerHeadRect.right - player.playerHeadRect.left) / 2 + 780,
				player.playerHeadRect.top + (player.playerHeadRect.bottom - player.playerHeadRect.top) / 2 + 385,
				1, 500, 3);
			playerBombDelay = 0;
		}

		//��ź �ִϸ��̼�
		ANIMATIONMANAGER->findAnimation("playerBomb");
		ANIMATIONMANAGER->start("playerBomb");
	}

	//��ź ����
	BULLETMANAGER->RemoveBomb(vPlayerBomb, viPlayerBomb);

	PlayerShotMove();	//�÷��̾� ���� ����
}

void Player::PlayerShotMove()
{
	//�ҷ� ����
	if (playerLeftShot)
	{
		//���� ���� �ִϸ��̼�
		ANIMATIONMANAGER->addAnimation("shotLeft", "playerHead", 6, 7, 4, false, true);
		aniHead = ANIMATIONMANAGER->findAnimation("shotLeft");
		ANIMATIONMANAGER->resume("shotLeft");

		//�ҷ� ����
		BULLETMANAGER->ShootBullet("playerBullet", vPlayerBullet,
			player.playerHeadRect.left + (player.playerHeadRect.right - player.playerHeadRect.left) / 2 - 20,
			player.playerHeadRect.top + (player.playerHeadRect.bottom - player.playerHeadRect.top) / 2,
			ANGLE_180, player.playerShotSpeed, player.playerShotRange, playerBulletCount++, player.playerShotDelay);

	}
	else if (playerRightShot)
	{
		//������ ���� �ִϸ��̼�
		ANIMATIONMANAGER->addAnimation("shotRight", "playerHead", 2, 3, 4, false, true);
		aniHead = ANIMATIONMANAGER->findAnimation("shotRight");
		ANIMATIONMANAGER->resume("shotRight");

		//�ҷ� ����
		BULLETMANAGER->ShootBullet("playerBullet", vPlayerBullet,
			player.playerHeadRect.left + (player.playerHeadRect.right - player.playerHeadRect.left) / 2 + 20,
			player.playerHeadRect.top + (player.playerHeadRect.bottom - player.playerHeadRect.top) / 2,
			ANGLE_0, player.playerShotSpeed, player.playerShotRange, playerBulletCount++, player.playerShotDelay);
	}
	else if (playerUpShot)
	{
		//���� ���� �ִϸ��̼�
		ANIMATIONMANAGER->addAnimation("shotUp", "playerHead", 4, 5, 4, false, true);
		aniHead = ANIMATIONMANAGER->findAnimation("shotUp");
		ANIMATIONMANAGER->resume("shotUp");

		//�ҷ� ����
		BULLETMANAGER->ShootBullet("playerBullet", vPlayerBullet,
			player.playerHeadRect.left + (player.playerHeadRect.right - player.playerHeadRect.left) / 2,
			player.playerHeadRect.top + (player.playerHeadRect.bottom - player.playerHeadRect.top) / 2 - 20,
			ANGLE_90, player.playerShotSpeed, player.playerShotRange, playerBulletCount++, player.playerShotDelay);
	}
	else if (playerDownShot)
	{
		//�Ʒ��� ���� �ִϸ��̼�
		ANIMATIONMANAGER->addAnimation("shotDown", "playerHead", 0, 1, 4, false, true);
		aniHead = ANIMATIONMANAGER->findAnimation("shotDown");
		ANIMATIONMANAGER->resume("shotDown");

		//�ҷ� ����
		BULLETMANAGER->ShootBullet("playerBullet", vPlayerBullet,
			player.playerHeadRect.left + (player.playerHeadRect.right - player.playerHeadRect.left) / 2,
			player.playerHeadRect.top + (player.playerHeadRect.bottom - player.playerHeadRect.top) / 2 + 20,
			ANGLE_270, player.playerShotSpeed, player.playerShotRange, playerBulletCount++, player.playerShotDelay);
	}

	//�ҷ� ����
	BULLETMANAGER->MovePlayerBullet(vPlayerBullet, viPlayerBullet);
}

void Player::PlayerAnimation()
{
	if (ENEMYMANAGER->GetGameEnd() == true)
	{
		// END
		player.playerBodyImage = IMAGEMANAGER->addFrameImage("GameEnd", "images/player/playerTrapdoor.bmp", 504, 387, 6, 3, true, RGB(255, 0, 255));
		ANIMATIONMANAGER->addAnimation("end", "GameEnd", 0, 15, 15, false, false);
		aniBody = ANIMATIONMANAGER->findAnimation("end");
		ANIMATIONMANAGER->resume("end");
	}
	else
	{
		if (COLLISIONMANAGER->GetplayerHitAni() == true)
		{
			aniHit = ANIMATIONMANAGER->findAnimation("playerHit");
			ANIMATIONMANAGER->resume("playerHit");
		}
		else
		{
			switch (direction)
			{
			case PLAYER_IDLE:
				// �÷��̾� HeadIdle
				aniHead = ANIMATIONMANAGER->findAnimation("headIdle");
				ANIMATIONMANAGER->stop("headIdle");
				// �÷��̾� BodyIdle
				ANIMATIONMANAGER->addAnimation("bodyIdle", "playerBody", arrBodyIdle, 1, 1, true);
				aniBody = ANIMATIONMANAGER->findAnimation("bodyIdle");
				ANIMATIONMANAGER->stop("bodyIdle");
				break;
			case PLAYER_LEFT:
				// �÷��̾� HeadLeft
				ANIMATIONMANAGER->addAnimation("headLeft", "playerHead", arrHeadLeft, 1, 1, true);
				aniHead = ANIMATIONMANAGER->findAnimation("headLeft");
				ANIMATIONMANAGER->stop("headLeft");
				// �÷��̾� BodyLeft
				ANIMATIONMANAGER->addAnimation("bodyLeft", "playerBody", 10, 19, 12, false, true);
				aniBody = ANIMATIONMANAGER->findAnimation("bodyLeft");
				ANIMATIONMANAGER->resume("bodyLeft");
				break;
			case PLAYER_RIGHT:
				// �÷��̾� HeadRight
				ANIMATIONMANAGER->addAnimation("headRight", "playerHead", arrHeadRight, 1, 1, true);
				aniHead = ANIMATIONMANAGER->findAnimation("headRight");
				ANIMATIONMANAGER->stop("headRight");
				// �÷��̾� BodyRight
				ANIMATIONMANAGER->addAnimation("bodyRight", "playerBody", 30, 39, 12, false, true);
				aniBody = ANIMATIONMANAGER->findAnimation("bodyRight");
				ANIMATIONMANAGER->resume("bodyRight");
				break;
			case PLAYER_UP:
				// �÷��̾� HeadUp
				ANIMATIONMANAGER->addAnimation("headUp", "playerHead", arrHeadUp, 1, 1, true);
				aniHead = ANIMATIONMANAGER->findAnimation("headUp");
				ANIMATIONMANAGER->stop("headUp");
				// �÷��̾� BodyUp
				ANIMATIONMANAGER->addAnimation("bodyUp", "playerBody", 20, 29, 12, false, true);
				aniBody = ANIMATIONMANAGER->findAnimation("bodyUp");
				ANIMATIONMANAGER->resume("bodyUp");
				break;
			case PLAYER_DOWN:
				// �÷��̾� HeadDown
				ANIMATIONMANAGER->addAnimation("headIdle", "playerHead", arrHeadIdle, 1, 1, true);
				aniHead = ANIMATIONMANAGER->findAnimation("headIdle");
				ANIMATIONMANAGER->stop("headIdle");
				// �÷��̾� BodyDown
				ANIMATIONMANAGER->addAnimation("bodyDown", "playerBody", 20, 29, 12, false, true);
				aniBody = ANIMATIONMANAGER->findAnimation("bodyDown");
				ANIMATIONMANAGER->resume("bodyDown");
				break;
			}
		}
	}
}

void Player::PlayerStatUpdate()
{
	currentGold[0] = (player.playerGold % 100) / 10;
	currentGold[1] = player.playerGold % 10;

	currentBomb[0] = (player.playerBomb % 100) / 10;
	currentBomb[1] = player.playerBomb % 10;

	currentKey[0] = (player.playerKey % 100) / 10;
	currentKey[1] = player.playerKey % 10;

	currentSpeedFrame[0] = ((int)player.playerSpeed % 100) / 10;
	currentSpeedFrame[1] = (int)player.playerSpeed % 10;
	currentSpeedFrame[2] = (float)(((int)player.playerSpeed * 10) % 10);
	currentSpeedFrame[3] = (float)(((int)player.playerSpeed * 100) % 10);

	currentRangeFrame[0] = ((int)player.playerShotRange % 1000) / 100;
	currentRangeFrame[1] = ((int)player.playerShotRange % 100) / 10;
	currentRangeFrame[2] = (int)player.playerShotRange % 10;
	currentRangeFrame[3] = (float)(((int)player.playerShotRange * 10) % 10);
	currentRangeFrame[4] = (float)(((int)player.playerShotRange * 100) % 10);
	   
	currentShotDelayFrame[0] = (player.playerShotDelay % 100) / 10;
	currentShotDelayFrame[1] = player.playerShotDelay % 10;
	currentShotDelayFrame[2] = 0;
	currentShotDelayFrame[3] = 0;

	currentShotSpeedFrame[0] = ((int)player.playerShotSpeed % 100) / 10;
	currentShotSpeedFrame[1] = (int)player.playerShotSpeed % 10;
	currentShotSpeedFrame[2] = (float)(((int)player.playerShotSpeed * 10) % 10);
	currentShotSpeedFrame[3] = (float)(((int)player.playerShotSpeed * 100) % 10);

	currentOffenseFrame[0] = (player.playerOffensePower % 100) / 10;
	currentOffenseFrame[1] = player.playerOffensePower % 10;
	currentOffenseFrame[2] = 0;
	currentOffenseFrame[3] = 0;
}

void Player::SetPlayerHp(float num)
{
	player.playerHp += num;
	if (player.playerHp >= player.playerMaxHp)
	{
		player.playerHp = player.playerMaxHp;
	}
}

void Player::SetPlayerGold(int num)
{
	player.playerGold += num;
	if (player.playerGold > 99)
	{
		player.playerGold = 99;
	}
}

void Player::SetPlayerBomb(int num)
{
	player.playerBomb += num;
	if (player.playerBomb > 99)
	{
		player.playerBomb = 99;
	}
}

void Player::SetPlayerKey(int num)
{
	player.playerKey += num;
	if (player.playerKey > 99)
	{
		player.playerKey = 99;
	}
}

void Player::SetPlayerRectX(int num)
{
	player.playerHeadRect.left += num;
	player.playerHeadRect.right += num;

	player.playerBodyRect.left += num;
	player.playerBodyRect.right += num;

	player.playerHitRect.left += num;
	player.playerHitRect.right += num;
}

void Player::SetPlayerRectY(int num)
{
	player.playerHeadRect.top += num;
	player.playerHeadRect.bottom += num;

	player.playerBodyRect.top += num;
	player.playerBodyRect.bottom += num;

	player.playerHitRect.top += num;
	player.playerHitRect.bottom += num;
}

void Player::DeleteBullet(int num)
{
	vPlayerBullet.erase(vPlayerBullet.begin() + num);
}