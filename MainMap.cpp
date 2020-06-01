#include "stdafx.h"
#include "MainMap.h"

MainMap::MainMap()
{
}

MainMap::~MainMap()
{
}

HRESULT MainMap::init()
{
	_locationX = 0;
	_locationY = 0;
	_center = 0;
	memset(_tileMap, 0, sizeof(_tileMap));

	currentX = INIT_X;
	currentY = INIT_Y;

	openDoor = true;

	moveUp = false;
	moveDown = false;
	moveRight = false;
	moveLeft = false;

	stopCamera = true;
	isUsedKey = false;
	savePositionX = currentX;
	savePositionY = currentY;
	currentPositionImage = IMAGEMANAGER->addImage("miniMapCurrentPosition", "images/maptool/miniMapCurrentPosition.bmp", 18 * 1.5, 16 * 1.5, true, RGB(255, 0, 255));
	passedPositionImage = IMAGEMANAGER->addImage("miniMapPassedPosition", "images/maptool/miniMapPassedPosition.bmp", 18 * 1.5, 16 * 1.5, true, RGB(255, 0, 255));

	for (int i = 0; i < ROOM_MAX_X; i++)
	{
		for (int j = 0; j < ROOM_MAX_Y; j++)
		{
			currentPositionRect[i][j] = RectMake(WINSIZEX - 135 + j * 18 * 1.5, 0 + i * 16 * 1.5, currentPositionImage->getWidth(), currentPositionImage->getHeight());
			passedPositionRect[i][j] = RectMake(WINSIZEX - 135 + j * 18 * 1.5, 0 + i * 16 * 1.5, currentPositionImage->getWidth(), currentPositionImage->getHeight());
		}
	}
	miniMapBoardImage = IMAGEMANAGER->addImage("miniMapBoard", "images/maptool/miniMapBoard.bmp", 90 * 1.5, 80 * 1.5, true, RGB(255, 0, 255));
	miniMapBoardRect = RectMake(WINSIZEX - 135, 0, miniMapBoardImage->getWidth(), miniMapBoardImage->getHeight());
	loadData = RND->getInt(4);
	load(loadData);

	// 맵 데이터에 따라서 각 방의 위치의 bool 값을 지정해준다.
	switch (loadData)
	{
	case 0:
	{
		for (int i = 0; i < ROOM_MAX_X; i++)
		{
			for (int j = 0; j < ROOM_MAX_Y; j++)
			{
				isCheckClear[i][j] = false;
				isSummonEnemy[i][j] = false;

				isBoss[i][j] = false;
				isShop[i][j] = false;
				isGoldRoom[i][j] = false;
			}
		}
		isBoss[4][4] = true;
		isShop[0][0] = true;
		isGoldRoom[2][0] = true;

		// 초기시작위치에서 적이 소환되지 않게 해주는 부분
		isSummonEnemy[2][2] = true;
	}
	break;
	case 1:
	{
		for (int i = 0; i < ROOM_MAX_X; i++)
		{
			for (int j = 0; j < ROOM_MAX_Y; j++)
			{
				isCheckClear[i][j] = false;
				isSummonEnemy[i][j] = false;

				isBoss[i][j] = false;
				isShop[i][j] = false;
				isGoldRoom[i][j] = false;
			}
		}
		isBoss[1][0] = true;
		isShop[0][2] = true;
		isGoldRoom[4][4] = true;

		// 초기시작위치에서 적이 소환되지 않게 해주는 부분
		isSummonEnemy[2][2] = true;
	}
	break;
	case 2:
	{
		for (int i = 0; i < ROOM_MAX_X; i++)
		{
			for (int j = 0; j < ROOM_MAX_Y; j++)
			{
				isCheckClear[i][j] = false;
				isSummonEnemy[i][j] = false;

				isBoss[i][j] = false;
				isShop[i][j] = false;
				isGoldRoom[i][j] = false;
			}
		}
		isBoss[0][1] = true;
		isShop[3][0] = true;
		isGoldRoom[1][4] = true;

		// 초기시작위치에서 적이 소환되지 않게 해주는 부분
		isSummonEnemy[2][2] = true;
	}
		break;
	case 3:
	{
		for (int i = 0; i < ROOM_MAX_X; i++)
		{
			for (int j = 0; j < ROOM_MAX_Y; j++)
			{
				isCheckClear[i][j] = false;
				isSummonEnemy[i][j] = false;

				isBoss[i][j] = false;
				isShop[i][j] = false;
				isGoldRoom[i][j] = false;
			}
		}
		isBoss[0][4] = true;
		isShop[0][1] = true;
		isGoldRoom[4][1] = true;

		// 초기시작위치에서 적이 소환되지 않게 해주는 부분
		isSummonEnemy[2][2] = true;
	}
	break;
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	default:
		break;
	}

	return S_OK;

}

void MainMap::release()
{
}

void MainMap::update()
{
	// 재시작
	if (KEYMANAGER->isStayKeyDown('U'))
	{
		count++;
		if (count == 30)
		{
			resetData = RND->getInt(4);
			loadData = resetData;
			ENEMYMANAGER->Init();
			PLAYERMANAGER->Init();
			COLLISIONMANAGER->Init();
			this->init();
			currentX = INIT_X;
			currentY = INIT_Y;
			load(loadData);
			count = 0;
		}
	}

	ENEMYMANAGER->SetLoadData(loadData);

	// 해당 방에 입장 했을 때
	for (int i = 0; i < ROOM_MAX_X; i++)
	{
		for (int j = 0; j < ROOM_MAX_Y; j++)
		{
			if (isSummonEnemy[i][j] == false)
			{
				if (savePositionX == (j * -884) && savePositionY == (i * -572))
				{
					isCheckClear[i][j] = true;
					ENEMYMANAGER->SetCheckClear(isCheckClear[i][j]);
				}
			}
		}
	}

	// 상점방 입장 했을 때
	switch (loadData)
	{
	case 0:
	{
		// 상점방에 입장 했을 시의 bool값
		if (isCheckClear[0][0] == true && isShop[0][0] == true)
		{
			isSummonEnemy[0][0] = true;
			isShop[0][0] = false;
			ITEMMANAGER->SetShop(isShop[0][0]);
		}
		if (isShop[0][0] == false)
		{
			if (savePositionX == (0 * -884) && savePositionY == (0 * -572))
			{
				isSummonEnemy[0][0] = true;
				isShop[0][0] = false;
				ITEMMANAGER->SetShop(isShop[0][0]);
			}
			else
			{
				isSummonEnemy[0][0] = true;
				isShop[0][0] = true;
				ITEMMANAGER->SetShop(isShop[0][0]);
			}
		}
	}
	break;
	case 1:
	{
		// 상점방에 입장 했을 시의 bool값
		if (isCheckClear[0][2] == true && isShop[0][2] == true)
		{
			isSummonEnemy[0][2] = true;
			isShop[0][2] = false;
			ITEMMANAGER->SetShop(isShop[0][2]);
		}
		if (isShop[0][2] == false)
		{
			if (savePositionX == (2 * -884) && savePositionY == (0 * -572))
			{
				isSummonEnemy[0][2] = true;
				isShop[0][2] = false;
				ITEMMANAGER->SetShop(isShop[0][2]);
			}
			else
			{
				isSummonEnemy[0][2] = true;
				isShop[0][2] = true;
				ITEMMANAGER->SetShop(isShop[0][2]);
			}
		}
	}
	break;
	case 2:
	{
		// 상점방에 입장 했을 시의 bool값
		if (isCheckClear[3][0] == true && isShop[3][0] == true)
		{
			isSummonEnemy[3][0] = true;
			isShop[3][0] = false;
			ITEMMANAGER->SetShop(isShop[3][0]);
		}
		if (isShop[3][0] == false)
		{
			if (savePositionX == (0 * -884) && savePositionY == (3 * -572))
			{
				isSummonEnemy[3][0] = true;
				isShop[3][0] = false;
				ITEMMANAGER->SetShop(isShop[3][0]);
			}
			else
			{
				isSummonEnemy[3][0] = true;
				isShop[3][0] = true;
				ITEMMANAGER->SetShop(isShop[3][0]);
			}
		}
	}
	break;
	case 3:
	{
		// 상점방에 입장 했을 시의 bool값
		if (isCheckClear[0][1] == true && isShop[0][1] == true)
		{
			isSummonEnemy[0][1] = true;
			isShop[0][1] = false;
			ITEMMANAGER->SetShop(isShop[3][0]);
		}
		if (isShop[0][1] == false)
		{
			if (savePositionX == (1 * -884) && savePositionY == (0 * -572))
			{
				isSummonEnemy[0][1] = true;
				isShop[0][1] = false;
				ITEMMANAGER->SetShop(isShop[0][1]);
			}
			else
			{
				isSummonEnemy[0][1] = true;
				isShop[0][1] = true;
				ITEMMANAGER->SetShop(isShop[0][1]);
			}
		}
	}
	break;
	}

	// 황금방 입장 했을 때
	switch (loadData)
	{
	case 0:
	{
		// 황금방 입장 했을 시의 bool값
		if (isCheckClear[2][0] == true && isGoldRoom[2][0] == true)
		{
			isSummonEnemy[2][0] = true;
			isGoldRoom[2][0] = false;
			ITEMMANAGER->SetGoldRoom(isGoldRoom[2][0]);
		}
		if (isGoldRoom[2][0] == false)
		{
			if (savePositionX == (0 * -884) && savePositionY == (2 * -572))
			{
				isSummonEnemy[2][0] = true;
				isGoldRoom[2][0] = false;
				ITEMMANAGER->SetGoldRoom(isGoldRoom[2][0]);
			}
			else
			{
				isSummonEnemy[2][0] = true;
				isGoldRoom[2][0] = true;
				ITEMMANAGER->SetGoldRoom(isGoldRoom[2][0]);
			}
		}
	}
	break;
	case 1:
	{
		// 황금방 입장 했을 시의 bool값
		if (isCheckClear[4][4] == true && isGoldRoom[4][4] == true)
		{
			isSummonEnemy[4][4] = true;
			isGoldRoom[4][4] = false;
			ITEMMANAGER->SetGoldRoom(isGoldRoom[4][4]);
		}
		if (isGoldRoom[4][4] == false)
		{
			if (savePositionX == (4 * -884) && savePositionY == (4 * -572))
			{
				isSummonEnemy[4][4] = true;
				isGoldRoom[4][4] = false;
				ITEMMANAGER->SetGoldRoom(isGoldRoom[4][4]);
			}
			else
			{
				isSummonEnemy[4][4] = true;
				isGoldRoom[4][4] = true;
				ITEMMANAGER->SetGoldRoom(isGoldRoom[4][4]);
			}
		}
	}
	break;
	case 2:
	{
		// 황금방 입장 했을 시의 bool값
		if (isCheckClear[1][4] == true && isGoldRoom[1][4] == true)
		{
			isSummonEnemy[1][4] = true;
			isGoldRoom[1][4] = false;
			ITEMMANAGER->SetGoldRoom(isGoldRoom[1][4]);
		}
		if (isGoldRoom[1][4] == false)
		{
			if (savePositionX == (4 * -884) && savePositionY == (1 * -572))
			{
				isSummonEnemy[1][4] = true;
				isGoldRoom[1][4] = false;
				ITEMMANAGER->SetGoldRoom(isGoldRoom[1][4]);
			}
			else
			{
				isSummonEnemy[1][4] = true;
				isGoldRoom[1][4] = true;
				ITEMMANAGER->SetGoldRoom(isGoldRoom[1][4]);
			}
		}
	}
	break;
	case 3:
	{
		// 황금방 입장 했을 시의 bool값
		if (isCheckClear[4][1] == true && isGoldRoom[4][1] == true)
		{
			isSummonEnemy[4][1] = true;
			isGoldRoom[4][1] = false;
			ITEMMANAGER->SetGoldRoom(isGoldRoom[4][1]);
		}
		if (isGoldRoom[4][1] == false)
		{
			if (savePositionX == (1 * -884) && savePositionY == (4 * -572))
			{
				isSummonEnemy[4][1] = true;
				isGoldRoom[4][1] = false;
				ITEMMANAGER->SetGoldRoom(isGoldRoom[4][1]);
			}
			else
			{
				isSummonEnemy[4][1] = true;
				isGoldRoom[4][1] = true;
				ITEMMANAGER->SetGoldRoom(isGoldRoom[4][1]);
			}
		}
	}
	break;
	}


	// 보스방에 입장 했을 때
	switch (loadData)
	{
	case 0:
	{
		// 보스방에 입장 했을 시의 bool값
		if (isCheckClear[4][4] == true && isBoss[4][4] == true && isSummonEnemy[4][4] == false)
		{
			SOUNDMANAGER->stop("BGM");
			SOUNDMANAGER->play("BossBGM", 1.0f);
			isBoss[4][4] = false;
			isSummonEnemy[4][4] = true;
			ENEMYMANAGER->SetBoss(isBoss[4][4]);
			ENEMYMANAGER->SetSummonEnemy(isSummonEnemy[4][4]);
		}
	}
	break;
	case 1:
	{
		// 보스방에 입장 했을 시의 bool값
		if (isCheckClear[1][0] == true && isBoss[1][0] == true && isSummonEnemy[1][0] == false)
		{
			SOUNDMANAGER->stop("BGM");
			SOUNDMANAGER->play("BossBGM", 1.0f);
			isBoss[1][0] = false;
			isSummonEnemy[1][0] = true;
			ENEMYMANAGER->SetBoss(isBoss[1][0]);
			ENEMYMANAGER->SetSummonEnemy(isSummonEnemy[1][0]);
		}
	}
	break;
	case 2:
	{
		// 보스방에 입장 했을 시의 bool값
		if (isCheckClear[0][1] == true && isBoss[0][1] == true && isSummonEnemy[0][1] == false)
		{
			SOUNDMANAGER->stop("BGM");
			SOUNDMANAGER->play("BossBGM", 1.0f);
			isBoss[0][1] = false;
			isSummonEnemy[0][1] = true;
			ENEMYMANAGER->SetBoss(isBoss[0][1]);
			ENEMYMANAGER->SetSummonEnemy(isSummonEnemy[0][1]);
		}
	}
	break;
	case 3:
	{
		// 보스방에 입장 했을 시의 bool값
		if (isCheckClear[0][4] == true && isBoss[0][4] == true && isSummonEnemy[0][4] == false)
		{
			SOUNDMANAGER->stop("BGM");
			SOUNDMANAGER->play("BossBGM", 1.0f);
			isBoss[0][4] = false;
			isSummonEnemy[0][4] = true;
			ENEMYMANAGER->SetBoss(isBoss[0][4]);
			ENEMYMANAGER->SetSummonEnemy(isSummonEnemy[0][4]);
		}
	}
	break;
	}

	// 일반방에 입장 했을 때
		for (int i = 0; i < ROOM_MAX_X; i++)
	{
		for (int j = 0; j < ROOM_MAX_Y; j++)
		{
			// isCheckClear -> 방에 입장했다면 true / isSummonEnemy -> 몬스터를 소환한 적이 없다면 false / 보스방이아니라면 false / 상점이 아니라면 false / 황금방이 아니라면 false
			if (isCheckClear[i][j] == true && isSummonEnemy[i][j] == false && isBoss[i][j] == false && isShop[i][j] == false && isGoldRoom[i][j] == false)
			{
				// 몬스터를 소환했다면 true
				isSummonEnemy[i][j] = true;
				ENEMYMANAGER->SetSummonEnemy(isSummonEnemy[i][j]);
			}
		}
	}


	// 충돌처리
	for (int i = 0; i < TILE_COUNT_X; i++)
	{
		for (int j = 0; j < TILE_COUNT_Y; j++)
		{
			for (int z = 0; z < TILE_MAX; z++)
			{
				// 보이는 오브젝트와의 충돌처리
				if (_tileMap[i][j].tileKind[z] == TILEKIND_OBJECT)
				{
					if (stopCamera)
					{
						COLLISIONMANAGER->PlayerToObstacleCollision(_tileMap[i][j].rect);
						COLLISIONMANAGER->EnemyToObstacleCollision(_tileMap[i][j].rect);
						COLLISIONMANAGER->PlayerBulletToObjectCollision(_tileMap[i][j].rect);
					}
				}

				// 열쇠로 잠긴방과의 충돌처리
				if (PLAYERMANAGER->GetPlayerKey() <= 0)
				{
					if (_tileMap[i][j].tileKind[z] == TILEKIND_LOCKED_DOOR)
					{
						COLLISIONMANAGER->PlayerToObstacleCollision(_tileMap[i][j].rect);
						COLLISIONMANAGER->EnemyToObstacleCollision(_tileMap[i][j].rect);
						COLLISIONMANAGER->PlayerBulletToObjectCollision(_tileMap[i][j].rect);
					}
				}
				else
				{
					// 열쇠가 있다면 해당 방의 문을 지우는 식으로 방을 연다
					if (_tileMap[i][j].tileKind[z] == TILEKIND_LOCKED_DOOR)
					{
						if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &_tileMap[i][j].rect))
						{
							isUsedKey = true;
							PLAYERMANAGER->SetPlayerKey(-1);
							_tileMap[i][j].tileNum[z] = 0;
							_tileMap[i][j].tileKind[z] = TILEKIND_NONE;
							_tileMap[i][j].tilePos[z] = PointMake(0, 0);
						}
						COLLISIONMANAGER->PlayerBulletToObjectCollision(_tileMap[i][j].rect);
					}
				}

				// 열렸다면 문안으로 들어갈 수 있다.
				if (isUsedKey)
				{
					if (_tileMap[i][j].tileKind[z] == TILEKIND_USEDKEY_DOOR)
					{
						if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &_tileMap[i][j].rect))
						{
							if (stopCamera)
							{
								if (COLLISIONMANAGER->PlayerCollisionNextDoor(_tileMap[i][j].rect) == 3)
								{
									PLAYERMANAGER->SetPlayerRectY(340);
									moveUp = true;
									stopCamera = false;
								}

								else if (COLLISIONMANAGER->PlayerCollisionNextDoor(_tileMap[i][j].rect) == 4)
								{
									PLAYERMANAGER->SetPlayerRectY(-340);
									moveDown = true;
									stopCamera = false;
								}

								else if (COLLISIONMANAGER->PlayerCollisionNextDoor(_tileMap[i][j].rect) == 1)
								{
									PLAYERMANAGER->SetPlayerRectX(620);
									moveLeft = true;
									stopCamera = false;
								}
								else if (COLLISIONMANAGER->PlayerCollisionNextDoor(_tileMap[i][j].rect) == 2)
								{
									PLAYERMANAGER->SetPlayerRectX(-620);
									moveRight = true;
									stopCamera = false;
								}
							}
							COLLISIONMANAGER->PlayerBulletToObjectCollision(_tileMap[i][j].rect);
						}
					}
				}

				if (ENEMYMANAGER->GetOpenDoor())
				{
					if (_tileMap[i][j].tileKind[z] == TILEKIND_OPEN_DOOR)
					{
						if (stopCamera)
						{
							if (COLLISIONMANAGER->PlayerCollisionNextDoor(_tileMap[i][j].rect) == 3)
							{
								PLAYERMANAGER->SetPlayerRectY(340);
								moveUp = true;
								stopCamera = false;
							}

							else if (COLLISIONMANAGER->PlayerCollisionNextDoor(_tileMap[i][j].rect) == 4)
							{
								PLAYERMANAGER->SetPlayerRectY(-340);
								moveDown = true;
								stopCamera = false;
							}

							else if (COLLISIONMANAGER->PlayerCollisionNextDoor(_tileMap[i][j].rect) == 1)
							{
								PLAYERMANAGER->SetPlayerRectX(620);
								moveLeft = true;
								stopCamera = false;
							}
							else if (COLLISIONMANAGER->PlayerCollisionNextDoor(_tileMap[i][j].rect) == 2)
							{
								PLAYERMANAGER->SetPlayerRectX(-620);
								moveRight = true;
								stopCamera = false;
							}
						}
						COLLISIONMANAGER->PlayerBulletToObjectCollision(_tileMap[i][j].rect);
					}
				}
				else
				{
					if (_tileMap[i][j].tileKind[z] == TILEKIND_CLOSE_DOOR)
					{
						COLLISIONMANAGER->PlayerToObstacleCollision(_tileMap[i][j].rect);
						COLLISIONMANAGER->EnemyToObstacleCollision(_tileMap[i][j].rect);
						COLLISIONMANAGER->PlayerBulletToObjectCollision(_tileMap[i][j].rect);
					}
				}

				if (_tileMap[i][j].tileKind[z] == TILEKIND_INVISIBLE_BLOCK)
				{
					COLLISIONMANAGER->PlayerToObstacleCollision(_tileMap[i][j].rect);
					COLLISIONMANAGER->PlayerBulletToObjectCollision(_tileMap[i][j].rect);
				}

				if (_tileMap[i][j].tileKind[z] == TILEKIND_ITEMHEART)
				{
					if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &_tileMap[i][j].rect))
					{
						PLAYERMANAGER->SetPlayerHp(1.0f);
						_tileMap[i][j].tileNum[z] = 0;
						_tileMap[i][j].tileKind[z] = TILEKIND_NONE;
						_tileMap[i][j].tilePos[z] = { 0 };
					}
				}

				if (_tileMap[i][j].tileKind[z] == TILEKIND_ITEMGOLD)
				{
					if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &_tileMap[i][j].rect))
					{
						PLAYERMANAGER->SetPlayerGold(1);
						_tileMap[i][j].tileNum[z] = 0;
						_tileMap[i][j].tileKind[z] = TILEKIND_NONE;
						_tileMap[i][j].tilePos[z] = { 0 };
					}
				}

				if (_tileMap[i][j].tileKind[z] == TILEKIND_ITEMBOMB)
				{
					if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &_tileMap[i][j].rect))
					{
						PLAYERMANAGER->SetPlayerBomb(1);
						_tileMap[i][j].tileNum[z] = 0;
						_tileMap[i][j].tileKind[z] = TILEKIND_NONE;
						_tileMap[i][j].tilePos[z] = { 0 };
					}
				}

				if (_tileMap[i][j].tileKind[z] == TILEKIND_ITEMKEY)
				{
					if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &_tileMap[i][j].rect))
					{
						PLAYERMANAGER->SetPlayerKey(1);
						_tileMap[i][j].tileNum[z] = 0;
						_tileMap[i][j].tileKind[z] = TILEKIND_NONE;
						_tileMap[i][j].tilePos[z] = { 0 };
					}
				}

				if (_tileMap[i][j].tileKind[z] == TILEKIND_POOP100)
				{
					COLLISIONMANAGER->PlayerToObstacleCollision(_tileMap[i][j].rect);

					if (COLLISIONMANAGER->PlayerBulletToObjectCollision(_tileMap[i][j].rect))
					{
						_tileMap[i][j].tileNum[z] = 0;
						_tileMap[i][j].tileKind[z] = TILEKIND_NONE;
						_tileMap[i][j].tilePos[z] = PointMake(0, 0);
					}
				}
			}
		}
	}


	if (!stopCamera)
	{
		if (moveUp)
		{
			currentY += 60;

			if (currentY >= savePositionY + WINSIZEY)
			{
				currentY = savePositionY + WINSIZEY;
				savePositionY = currentY;
				moveUp = false;
				stopCamera = true;
			}
		}
		if (moveDown)
		{
			currentY -= 60;

			if (currentY <= savePositionY - WINSIZEY)
			{
				currentY = savePositionY - WINSIZEY;
				savePositionY = currentY;
				moveDown = false;
				stopCamera = true;
			}
		}
		else if (moveLeft)
		{
			currentX += 100;

			if (currentX >= savePositionX + WINSIZEX)
			{
				currentX = savePositionX + WINSIZEX;
				savePositionX = currentX;
				moveLeft = false;
				stopCamera = true;
			}
		}
		else if (moveRight)
		{
			currentX -= 100;

			if (currentX <= savePositionX - WINSIZEX)
			{
				currentX = savePositionX - WINSIZEX;
				savePositionX = currentX;
				moveRight = false;
				stopCamera = true;
			}
		}
	}
}

void MainMap::render()
{
	DrawTileMap();

	for (int i = 0; i < TILE_COUNT_X; i++)
	{
		for (int j = 0; j < TILE_COUNT_Y; j++)
		{
			for (int z = 0; z < TILE_MAX; z++)
			{
				if (KEYMANAGER->isToggleKey(VK_TAB))
				{
					if (_tileMap[i][j].tileKind[z] == TILEKIND_OBJECT)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);

						HBRUSH brush = CreateSolidBrush(RGB(204, 0, 102));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (ENEMYMANAGER->GetOpenDoor())
					{
						if (_tileMap[i][j].tileKind[z] == TILEKIND_OPEN_DOOR)
						{
							Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
							HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
							FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
							DeleteObject(brush);
						}
					}
					else
					{
						if (_tileMap[i][j].tileKind[z] == TILEKIND_CLOSE_DOOR)
						{
							Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
							HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
							FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
							DeleteObject(brush);
						}
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_LOCKED_DOOR)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(200, 200, 0));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_USEDKEY_DOOR)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(0, 255, 200));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}


					if (_tileMap[i][j].tileKind[z] == TILEKIND_INVISIBLE_BLOCK)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_ITEMPOINT)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(51, 0, 51));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_ITEMHEART)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(204, 204, 102));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_ITEMGOLD)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(51, 102, 0));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_ITEMBOMB)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(204, 255, 204));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_ITEMATTACKBOMB)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(204, 100, 204));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_ITEMKEY)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(0, 255, 255));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_ITEMPILL)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(0, 102, 153));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_ITEMCARD)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(0, 51, 102));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_BLACKITEMBOXOPEN)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(153, 255, 203));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_BLACKITEMBOXCLOSE)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(0, 102, 0));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_GOLDITEMBOXOPEN)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(153, 51, 102));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_GOLDITEMBOXCLOSE)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(153, 153, 204));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_REDITEMBOXOPEN)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(153, 0, 102));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_REDITEMBOXCLOSE)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(255, 204, 153));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_CRACKEDITEMBOXOPEN)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(51, 0, 255));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_CRACKEDITEMBOXCLOSE)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(204, 204, 204));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_POOP100)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(100, 100, 100));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_POOP50)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(130, 130, 130));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}

					if (_tileMap[i][j].tileKind[z] == TILEKIND_POOP10)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(180, 180, 180));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}
					if (_tileMap[i][j].tileKind[z] == TILEKIND_FIREPLACE)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(200, 200, 200));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}
					if (_tileMap[i][j].tileKind[z] == TILEKIND_SHOP_HOST)
					{
						Rectangle(getMemDC(), _tileMap[i][j].rect.left, _tileMap[i][j].rect.top, _tileMap[i][j].rect.right, _tileMap[i][j].rect.bottom);
						HBRUSH brush = CreateSolidBrush(RGB(230, 0, 150));
						FillRect(getMemDC(), &_tileMap[i][j].rect, brush);
						DeleteObject(brush);
					}
				}
			}
		}
	}

	miniMapBoardImage->alphaRender(getMemDC(), miniMapBoardRect.left, miniMapBoardRect.top, 170);

	for (int i = 0; i < ROOM_MAX_X; i++)
	{
		for (int j = 0; j < ROOM_MAX_Y; j++)
		{
			if (isSummonEnemy[i][j] == true)
			{
				passedPositionImage->alphaRender(getMemDC(), passedPositionRect[i][j].left, passedPositionRect[i][j].top, 170);
			}
			if (isCheckClear[i][j] == true && isShop[i][j] == true)
			{
				passedPositionImage->alphaRender(getMemDC(), passedPositionRect[i][j].left, passedPositionRect[i][j].top, 170);
			}
			if (isCheckClear[i][j] == true && isGoldRoom[i][j] == true)
			{
				passedPositionImage->alphaRender(getMemDC(), passedPositionRect[i][j].left, passedPositionRect[i][j].top, 170);
			}

		}
	}

	for (int i = 0; i < ROOM_MAX_X; i++)
	{
		for (int j = 0; j < ROOM_MAX_Y; j++)
		{
			if (savePositionX == (j * -884) && savePositionY == (i * -572))
			{
				currentPositionImage->alphaRender(getMemDC(), currentPositionRect[i][j].left, currentPositionRect[i][j].top, 170);
			}
		}
	}
}

void MainMap::DrawTileMap()
{
	for (int i = 0; i < TILE_COUNT_X; i++)
	{
		for (int j = 0; j < TILE_COUNT_Y; j++)
		{
			int left = currentX + (i * CELL_WIDTH);
			int top = currentY + (j * CELL_HEIGHT);
			int right = currentX + (i * CELL_WIDTH) + CELL_WIDTH;
			int bottom = currentY + (j * CELL_HEIGHT) + CELL_HEIGHT;

			_tileMap[i][j].left = left;
			_tileMap[i][j].top = top;
			_tileMap[i][j].right = right;
			_tileMap[i][j].bottom = bottom;

			_tileMap[i][j].rect = { _tileMap[i][j].left , _tileMap[i][j].top , _tileMap[i][j].right , _tileMap[i][j].bottom };


			for (int z = 0; z <= _tileMap[i][j].index; z++)
			{
				if (_tileMap[i][j].tileKind[z] != TILEKIND_NONE)
				{
					switch (_tileMap[i][j].tileKind[z])
					{
					case TILEKIND_TERRAIN:
						IMAGEMANAGER->frameRender("mapTile", getMemDC(),
							_tileMap[i][j].left,
							_tileMap[i][j].top - _tileMap[i][j].height*z,
							_tileMap[i][j].tilePos[z].x,
							_tileMap[i][j].tilePos[z].y);
						break;
					case TILEKIND_OBJECT:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_OPEN_DOOR:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							if (ENEMYMANAGER->GetOpenDoor())
							{
								if (_tileMap[i][j].tilePos[z].x % 2 == 1 && _tileMap[i][j].tilePos[z].y % 2 == 0)
								{
									IMAGEMANAGER->frameRender("door", getMemDC(),
										_tileMap[i][j].left - 50,
										_tileMap[i][j].top - _tileMap[i][j].height * z - 26,
										_tileMap[i][j].tilePos[z].x,
										_tileMap[i][j].tilePos[z].y);
									break;
								}
								IMAGEMANAGER->frameRender("door", getMemDC(),
									_tileMap[i][j].left - 26,
									_tileMap[i][j].top - _tileMap[i][j].height * z - 26,
									_tileMap[i][j].tilePos[z].x,
									_tileMap[i][j].tilePos[z].y);
								break;
							}
						}
						break;
					case TILEKIND_CLOSE_DOOR:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							if (!ENEMYMANAGER->GetOpenDoor())
							{
								if (_tileMap[i][j].tilePos[z].x % 2 == 1 && _tileMap[i][j].tilePos[z].y % 2 == 0)
								{
									IMAGEMANAGER->frameRender("door", getMemDC(),
										_tileMap[i][j].left - 50,
										_tileMap[i][j].top - _tileMap[i][j].height * z - 26,
										_tileMap[i][j].tilePos[z].x,
										_tileMap[i][j].tilePos[z].y);
									break;
								}
								IMAGEMANAGER->frameRender("door", getMemDC(),
									_tileMap[i][j].left - 26,
									_tileMap[i][j].top - _tileMap[i][j].height * z - 26,
									_tileMap[i][j].tilePos[z].x,
									_tileMap[i][j].tilePos[z].y);
								break;
							}
						}
						break;
					case TILEKIND_LOCKED_DOOR:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							if (_tileMap[i][j].tilePos[z].x % 2 == 1 && _tileMap[i][j].tilePos[z].y % 2 == 0)
							{
								IMAGEMANAGER->frameRender("door", getMemDC(),
									_tileMap[i][j].left - 50,
									_tileMap[i][j].top - _tileMap[i][j].height * z - 26,
									_tileMap[i][j].tilePos[z].x,
									_tileMap[i][j].tilePos[z].y);
								break;
							}
							IMAGEMANAGER->frameRender("door", getMemDC(),
								_tileMap[i][j].left - 26,
								_tileMap[i][j].top - _tileMap[i][j].height * z - 26,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_USEDKEY_DOOR:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							if (_tileMap[i][j].tilePos[z].x % 2 == 1 && _tileMap[i][j].tilePos[z].y % 2 == 0)
							{
								IMAGEMANAGER->frameRender("door", getMemDC(),
									_tileMap[i][j].left - 50,
									_tileMap[i][j].top - _tileMap[i][j].height * z - 26,
									_tileMap[i][j].tilePos[z].x,
									_tileMap[i][j].tilePos[z].y);
								break;
							}
							IMAGEMANAGER->frameRender("door", getMemDC(),
								_tileMap[i][j].left - 26,
								_tileMap[i][j].top - _tileMap[i][j].height * z - 26,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;

					case TILEKIND_ITEMPOINT:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_ITEMHEART:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_ITEMGOLD:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_ITEMBOMB:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_ITEMATTACKBOMB:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_ITEMKEY:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_ITEMPILL:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_ITEMCARD:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_BLACKITEMBOXOPEN:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_BLACKITEMBOXCLOSE:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_GOLDITEMBOXOPEN:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_GOLDITEMBOXCLOSE:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_REDITEMBOXOPEN:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_REDITEMBOXCLOSE:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_CRACKEDITEMBOXOPEN:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_CRACKEDITEMBOXCLOSE:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					case TILEKIND_POOP100:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;

					case TILEKIND_POOP50:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;

					case TILEKIND_POOP10:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;

					case TILEKIND_FIREPLACE:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;

					case TILEKIND_SHOP_HOST:
						if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
						{
							IMAGEMANAGER->frameRender("blocks", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top - _tileMap[i][j].height * z,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
							break;
						}
						break;
					}
				}
			}

			if (IntersectRect(&temp, &cameraRect, &_tileMap[i][j].rect))
			{

				if (KEYMANAGER->isToggleKey(VK_TAB))
				{
					if (j % 11 == 0)
					{
						Draw_Line_X(left, top);
					}
					if (i % 17 == 0)
					{
						Draw_Line_Y(left, top);
					}

					if (j % 1 == 0)
					{
						Draw_Line_X(left, top);
					}
					if (i % 1 == 0)
					{
						Draw_Line_Y(left, top);
					}
					SetTextColor(getMemDC(), RGB(255, 0, 0));
					sprintf_s(str, "(%d,%d)", i, j);
					TextOut(getMemDC(),
						left + CELL_WIDTH / 2 - 20,
						top + CELL_HEIGHT / 2 - 10, str, strlen(str));
				}
			}
		}
	}
}

void MainMap::load(int loadCount)
{
	file = CreateFile(fileName[loadCount], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tileMap, sizeof(TagTile) * TILE_COUNT_X * TILE_COUNT_Y, &read, NULL);

	CloseHandle(file);
}

void MainMap::Draw_Line_X(int left, int top)
{
	int centerX = left;
	int centerY = top;

	LineMake(getMemDC(), centerX, centerY, centerX + CELL_WIDTH, centerY);
}

void MainMap::Draw_Line_Y(int left, int top)
{
	int centerX = left;
	int centerY = top;

	LineMake(getMemDC(), centerX, centerY, centerX, centerY + CELL_HEIGHT);
}
