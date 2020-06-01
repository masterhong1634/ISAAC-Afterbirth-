#include "stdafx.h"
#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
	playerHitAni = false;
	playerHit = false;
	hitCount = 0;
}

CollisionManager::~CollisionManager()
{
}

HRESULT CollisionManager::Init()
{
	playerHitAni = false;
	playerHit = false;
	hitCount = 0;
	EFFECTMANAGER->addEffect("deathEffect", "images/bullet/deathEffect.bmp", 320 * 3, 320 * 3, 80 * 3, 80 * 3, 30, 1.0f, 10);

	return S_OK;
}

void CollisionManager::UpdateCollision()
{
	PlayerToMinionCollision();
	MinionToMinionCollision();
}

void CollisionManager::PlayerBulletCollision(vector<BulletInfo>& playerBulletVector, vector<BulletInfo>::iterator& playerBulletIter)
{
	RECT temp;

	if (!playerBulletVector.empty())
	{
		if (ENEMYMANAGER->GetMonstro()->GetJumpCollision() == false && ENEMYMANAGER->GetMonstro()->GetTeleportCollision() == false)
		{
			// Monstro 충돌
			for (int j = 0; j < ENEMYMANAGER->GetMonstro()->GetMinionVector().size(); j++)
			{
				for (playerBulletIter = playerBulletVector.begin(); playerBulletIter != playerBulletVector.end(); )
				{
					if (IntersectRect(&temp, &playerBulletIter->rect, &ENEMYMANAGER->GetMonstro()->GetMinionVector()[j].enemyRect))
					{
						// 오브젝트 풀로 총알을 돌려주는 함수
						OBJECTPOOL->SetBulletVector(playerBulletVector.front());
						EFFECTMANAGER->play("tearpoof", playerBulletIter->rect.left, playerBulletIter->rect.top + 20);
						playerBulletIter = playerBulletVector.erase(playerBulletIter);

						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMonstro()->GetMinionVector()[j].enemyRect.left &&
								ENEMYMANAGER->GetMonstro()->GetMinionVector()[j].enemyRect.right <= 770)
							{
								ENEMYMANAGER->GetMonstro()->SetEnemyRectX(j, tempW * KNOCKBACK);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMonstro()->GetMinionVector()[j].enemyRect.right &&
								ENEMYMANAGER->GetMonstro()->GetMinionVector()[j].enemyRect.left >= 110)
							{
								ENEMYMANAGER->GetMonstro()->SetEnemyRectX(j, -tempW * KNOCKBACK);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMonstro()->GetMinionVector()[j].enemyRect.top &&
								ENEMYMANAGER->GetMonstro()->GetMinionVector()[j].enemyRect.bottom <= 460)
							{
								ENEMYMANAGER->GetMonstro()->SetEnemyRectY(j, tempH * KNOCKBACK);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMonstro()->GetMinionVector()[j].enemyRect.bottom &&
								ENEMYMANAGER->GetMonstro()->GetMinionVector()[j].enemyRect.top >= 110)
							{
								ENEMYMANAGER->GetMonstro()->SetEnemyRectY(j, -tempH * KNOCKBACK);
							}
						}

						// 적 에너미의 HP 정보를 받아와 플레이어의 공격력만큼 HP를 감소시킨다
						ENEMYMANAGER->GetMonstro()->SetEnemyHp(j, ENEMYMANAGER->GetMonstro()->GetEnemyHp(j) - PLAYERMANAGER->GetPlayerOffensePower());

						if (ENEMYMANAGER->GetMonstro()->GetEnemyHp(j) <= 0)
						{
							// 피격이 된 적의 벡터를 지우는 함수
							//ENEMYMANAGER->GetMonstro()->DeleteEnemy(j);
						}

						// j번째 에너미 피격 시 j를 0으로 초기화
						j = 0;

						// 벡터값이 비어있으면 for문을 빠져나간다.
						if (ENEMYMANAGER->GetMonstro()->GetMinionVector().empty())
						{
							break;
						}

						// 체력 이미지를 깎아준다.
						ENEMYMANAGER->GetMonstro()->hitDamage(PLAYERMANAGER->GetPlayerOffensePower());
					}
					else
					{
						++playerBulletIter;
					}
				}
			}
		}

		// MinionAttackFly 충돌
		for (int j = 0; j < ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size(); j++)
		{
			for (playerBulletIter = playerBulletVector.begin(); playerBulletIter != playerBulletVector.end(); )
			{
				if (IntersectRect(&temp, &playerBulletIter->rect, &ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect))
				{
					// 오브젝트 풀로 총알을 돌려주는 함수
					OBJECTPOOL->SetBulletVector(playerBulletVector.front());
					EFFECTMANAGER->play("tearpoof", playerBulletIter->rect.left, playerBulletIter->rect.top + 20);
					playerBulletIter = playerBulletVector.erase(playerBulletIter);

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect.left &&
							ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect.right <= 770)
						{
							ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(j, tempW * KNOCKBACK);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect.right &&
							ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect.left >= 110)
						{
							ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(j, -tempW * KNOCKBACK);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect.top &&
							ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect.bottom <= 460)
						{
							ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(j, tempH * KNOCKBACK);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect.bottom &&
							ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect.top >= 110)
						{
							ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(j, -tempH * KNOCKBACK);
						}
					}

					// 적 에너미의 HP 정보를 받아와 플레이어의 공격력만큼 HP를 감소시킨다
					ENEMYMANAGER->GetMinionAttackFly()->SetEnemyHp(j, ENEMYMANAGER->GetMinionAttackFly()->GetEnemyHp(j) - PLAYERMANAGER->GetPlayerOffensePower());

					if (ENEMYMANAGER->GetMinionAttackFly()->GetEnemyHp(j) <= 0)
					{
						EFFECTMANAGER->play("deathEffect", ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect.left, ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect.top);

						// 피격이 된 적의 벡터를 지우는 함수
						ENEMYMANAGER->GetMinionAttackFly()->DeleteEnemy(j);
					}

					// j번째 에너미 피격 시 j를 0으로 초기화
					j = 0;

					// 벡터값이 비어있으면 for문을 빠져나간다.
					if (ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().empty())
					{
						break;
					}
				}
				else
				{
					++playerBulletIter;
				}
			}
		}

		// MinionBlackFly 충돌
		for (int j = 0; j < ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size(); j++)
		{

			for (playerBulletIter = playerBulletVector.begin(); playerBulletIter != playerBulletVector.end(); )
			{
				if (IntersectRect(&temp, &playerBulletIter->rect, &ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect))
				{
					// 오브젝트 풀로 총알을 돌려주는 함수
					OBJECTPOOL->SetBulletVector(playerBulletVector.front());
					EFFECTMANAGER->play("tearpoof", playerBulletIter->rect.left, playerBulletIter->rect.top + 20);
					playerBulletIter = playerBulletVector.erase(playerBulletIter);

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect.left &&
							ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect.right <= 770)
						{
							ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(j, tempW * KNOCKBACK);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect.right &&
							ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect.left >= 110)
						{
							ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(j, -tempW * KNOCKBACK);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect.top &&
							ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect.bottom <= 460)
						{
							ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(j, tempH * KNOCKBACK);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect.bottom &&
							ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect.top >= 110)
						{
							ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(j, -tempH * KNOCKBACK);
						}
					}

					// 적 에너미의 HP 정보를 받아와 플레이어의 공격력만큼 HP를 감소시킨다
					ENEMYMANAGER->GetMinionBlackFly()->SetEnemyHp(j, ENEMYMANAGER->GetMinionBlackFly()->GetEnemyHp(j) - PLAYERMANAGER->GetPlayerOffensePower());

					if (ENEMYMANAGER->GetMinionBlackFly()->GetEnemyHp(j) <= 0)
					{
						EFFECTMANAGER->play("deathEffect", ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect.left, ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect.top);

						// 피격이 된 적의 벡터를 지우는 함수
						ENEMYMANAGER->GetMinionBlackFly()->DeleteEnemy(j);
					}

					// j번째 에너미 피격 시 j를 0으로 초기화
					j = 0;

					// 벡터값이 비어있으면 for문을 빠져나간다.
					if (ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().empty())
					{
						break;
					}
				}
				else
				{
					++playerBulletIter;
				}
			}
		}

		// MinionMaw 충돌
		for (int j = 0; j < ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size(); j++)
		{

			for (playerBulletIter = playerBulletVector.begin(); playerBulletIter != playerBulletVector.end(); )
			{
				if (IntersectRect(&temp, &playerBulletIter->rect, &ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect))
				{
					// 오브젝트 풀로 총알을 돌려주는 함수
					OBJECTPOOL->SetBulletVector(playerBulletVector.front());
					EFFECTMANAGER->play("tearpoof", playerBulletIter->rect.left, playerBulletIter->rect.top + 20);
					playerBulletIter = playerBulletVector.erase(playerBulletIter);

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect.left &&
							ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect.right <= 770)
						{
							ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(j, tempW * KNOCKBACK);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect.right &&
							ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect.left >= 110)
						{
							ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(j, -tempW * KNOCKBACK);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect.top &&
							ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect.bottom <= 460)
						{
							ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(j, tempH * KNOCKBACK);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect.bottom &&
							ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect.top >= 110)
						{
							ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(j, -tempH * KNOCKBACK);
						}
					}

					// 적 에너미의 HP 정보를 받아와 플레이어의 공격력만큼 HP를 감소시킨다
					ENEMYMANAGER->GetMinionMaw()->SetEnemyHp(j, ENEMYMANAGER->GetMinionMaw()->GetEnemyHp(j) - PLAYERMANAGER->GetPlayerOffensePower());

					if (ENEMYMANAGER->GetMinionMaw()->GetEnemyHp(j) <= 0)
					{
						EFFECTMANAGER->play("deathEffect", ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect.left, ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect.top);

						// 피격이 된 적의 벡터를 지우는 함수
						ENEMYMANAGER->GetMinionMaw()->DeleteEnemy(j);
					}

					// j번째 에너미 피격 시 j를 0으로 초기화
					j = 0;

					// 벡터값이 비어있으면 for문을 빠져나간다.
					if (ENEMYMANAGER->GetMinionMaw()->GetMinionVector().empty())
					{
						break;
					}
				}
				else
				{
					++playerBulletIter;
				}
			}
		}

		// MinionTumor 충돌
		for (int j = 0; j < ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size(); j++)
		{
			for (playerBulletIter = playerBulletVector.begin(); playerBulletIter != playerBulletVector.end(); )
			{
				if (IntersectRect(&temp, &playerBulletIter->rect, &ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect))
				{
					// 오브젝트 풀로 총알을 돌려주는 함수
					OBJECTPOOL->SetBulletVector(playerBulletVector.front());
					EFFECTMANAGER->play("tearpoof", playerBulletIter->rect.left, playerBulletIter->rect.top + 20);
					playerBulletIter = playerBulletVector.erase(playerBulletIter);

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect.left &&
							ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect.right <= 770)
						{
							ENEMYMANAGER->GetMinionTumor()->SetEnemyRectX(j, tempW * KNOCKBACK);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect.right &&
							ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect.left >= 110)
						{
							ENEMYMANAGER->GetMinionTumor()->SetEnemyRectX(j, -tempW * KNOCKBACK);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect.top &&
							ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect.bottom <= 460)
						{
							ENEMYMANAGER->GetMinionTumor()->SetEnemyRectY(j, tempH * KNOCKBACK);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect.bottom &&
							ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect.top >= 110)
						{
							ENEMYMANAGER->GetMinionTumor()->SetEnemyRectY(j, -tempH * KNOCKBACK);
						}
					}

					// 적 에너미의 HP 정보를 받아와 플레이어의 공격력만큼 HP를 감소시킨다
					ENEMYMANAGER->GetMinionTumor()->SetEnemyHp(j, ENEMYMANAGER->GetMinionTumor()->GetEnemyHp(j) - PLAYERMANAGER->GetPlayerOffensePower());

					if (ENEMYMANAGER->GetMinionTumor()->GetEnemyHp(j) <= 0)
					{
						EFFECTMANAGER->play("deathEffect", ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect.left, ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect.top);
						
						// 피격이 된 적의 벡터를 지우는 함수
						ENEMYMANAGER->GetMinionTumor()->DeleteEnemy(j);
					}

					// j번째 에너미 피격 시 j를 0으로 초기화
					j = 0;

					// 벡터값이 비어있으면 for문을 빠져나간다.
					if (ENEMYMANAGER->GetMinionTumor()->GetMinionVector().empty())
					{
						break;
					}
				}
				else
				{
					++playerBulletIter;
				}
			}
		}

		// MinionClot 충돌
		for (int j = 0; j < ENEMYMANAGER->GetMinionClot()->GetMinionVector().size(); j++)
		{

			for (playerBulletIter = playerBulletVector.begin(); playerBulletIter != playerBulletVector.end(); )
			{
				if (IntersectRect(&temp, &playerBulletIter->rect, &ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect))
				{
					// 오브젝트 풀로 총알을 돌려주는 함수
					OBJECTPOOL->SetBulletVector(playerBulletVector.front());
					EFFECTMANAGER->play("tearpoof", playerBulletIter->rect.left, playerBulletIter->rect.top + 20);
					playerBulletIter = playerBulletVector.erase(playerBulletIter);

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.left &&
							ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.right <= 770)
						{
							ENEMYMANAGER->GetMinionClot()->SetEnemyRectX(j, tempW * KNOCKBACK);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.right &&
							ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.left >= 110)
						{
							ENEMYMANAGER->GetMinionClot()->SetEnemyRectX(j, -tempW * KNOCKBACK);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.top &&
							ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.bottom <= 460)
						{
							ENEMYMANAGER->GetMinionClot()->SetEnemyRectY(j, tempH * KNOCKBACK);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.bottom &&
							ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.top >= 110)
						{
							ENEMYMANAGER->GetMinionClot()->SetEnemyRectY(j, -tempH * KNOCKBACK);
						}
					}

					// 적 에너미의 HP 정보를 받아와 플레이어의 공격력만큼 HP를 감소시킨다
					ENEMYMANAGER->GetMinionClot()->SetEnemyHp(j, ENEMYMANAGER->GetMinionClot()->GetEnemyHp(j) - PLAYERMANAGER->GetPlayerOffensePower());

					if (ENEMYMANAGER->GetMinionClot()->GetEnemyHp(j) <= 0)
					{
						EFFECTMANAGER->play("deathEffect", ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.left, ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.top);

						// 피격이 된 적의 벡터를 지우는 함수
						ENEMYMANAGER->GetMinionClot()->DeleteEnemy(j);
					}

					// j번째 에너미 피격 시 j를 0으로 초기화
					j = 0;

					// 벡터값이 비어있으면 for문을 빠져나간다.
					if (ENEMYMANAGER->GetMinionClot()->GetMinionVector().empty())
					{
						break;
					}
				}
				else
				{
					++playerBulletIter;
				}
			}
		}

		// MinionClotty 충돌
		for (int j = 0; j < ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size(); j++)
		{

			for (playerBulletIter = playerBulletVector.begin(); playerBulletIter != playerBulletVector.end(); )
			{
				if (IntersectRect(&temp, &playerBulletIter->rect, &ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect))
				{
					// 오브젝트 풀로 총알을 돌려주는 함수
					OBJECTPOOL->SetBulletVector(playerBulletVector.front());
					EFFECTMANAGER->play("tearpoof", playerBulletIter->rect.left, playerBulletIter->rect.top + 20);
					playerBulletIter = playerBulletVector.erase(playerBulletIter);

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.left &&
							ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.right <= 770)
						{
							ENEMYMANAGER->GetMinionClotty()->SetEnemyRectX(j, tempW * KNOCKBACK);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.right &&
							ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.left >= 110)
						{
							ENEMYMANAGER->GetMinionClotty()->SetEnemyRectX(j, -tempW * KNOCKBACK);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.top &&
							ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.bottom <= 460)
						{
							ENEMYMANAGER->GetMinionClotty()->SetEnemyRectY(j, tempH * KNOCKBACK);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.bottom &&
							ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.top >= 110)
						{
							ENEMYMANAGER->GetMinionClotty()->SetEnemyRectY(j, -tempH * KNOCKBACK);
						}
					}

					// 적 에너미의 HP 정보를 받아와 플레이어의 공격력만큼 HP를 감소시킨다
					ENEMYMANAGER->GetMinionClotty()->SetEnemyHp(j, ENEMYMANAGER->GetMinionClotty()->GetEnemyHp(j) - PLAYERMANAGER->GetPlayerOffensePower());

					if (ENEMYMANAGER->GetMinionClotty()->GetEnemyHp(j) <= 0)
					{
						EFFECTMANAGER->play("deathEffect", ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.left, ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.top);

						// 피격이 된 적의 벡터를 지우는 함수
						ENEMYMANAGER->GetMinionClotty()->DeleteEnemy(j);
					}

					// j번째 에너미 피격 시 j를 0으로 초기화
					j = 0;

					// 벡터값이 비어있으면 for문을 빠져나간다.
					if (ENEMYMANAGER->GetMinionClotty()->GetMinionVector().empty())
					{
						break;
					}
				}
				else
				{
					++playerBulletIter;
				}
			}
		}

		// MinionGaper 충돌
		for (int j = 0; j < ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size(); j++)
		{

			for (playerBulletIter = playerBulletVector.begin(); playerBulletIter != playerBulletVector.end(); )
			{
				if (IntersectRect(&temp, &playerBulletIter->rect, &ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect))
				{
					// 오브젝트 풀로 총알을 돌려주는 함수
					OBJECTPOOL->SetBulletVector(playerBulletVector.front());
					EFFECTMANAGER->play("tearpoof", playerBulletIter->rect.left, playerBulletIter->rect.top + 20);
					playerBulletIter = playerBulletVector.erase(playerBulletIter);

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.left &&
							ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.right <= 770)
						{
							ENEMYMANAGER->GetMinionGaper()->SetEnemyRectX(j, tempW * KNOCKBACK);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.right &&
							ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.left >= 110)
						{
							ENEMYMANAGER->GetMinionGaper()->SetEnemyRectX(j, -tempW * KNOCKBACK);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.top &&
							ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.bottom <= 460)
						{
							ENEMYMANAGER->GetMinionGaper()->SetEnemyRectY(j, tempH * KNOCKBACK);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.bottom &&
							ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.top >= 110)
						{
							ENEMYMANAGER->GetMinionGaper()->SetEnemyRectY(j, -tempH * KNOCKBACK);
						}
					}

					// 적 에너미의 HP 정보를 받아와 플레이어의 공격력만큼 HP를 감소시킨다
					ENEMYMANAGER->GetMinionGaper()->SetEnemyHp(j, ENEMYMANAGER->GetMinionGaper()->GetEnemyHp(j) - PLAYERMANAGER->GetPlayerOffensePower());

					if (ENEMYMANAGER->GetMinionGaper()->GetEnemyHp(j) <= 0)
					{
						EFFECTMANAGER->play("deathEffect", ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.left, ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.top);

						// 피격이 된 적의 벡터를 지우는 함수
						ENEMYMANAGER->GetMinionGaper()->DeleteEnemy(j);
					}

					// j번째 에너미 피격 시 j를 0으로 초기화
					j = 0;

					// 벡터값이 비어있으면 for문을 빠져나간다.
					if (ENEMYMANAGER->GetMinionGaper()->GetMinionVector().empty())
					{
						break;
					}
				}
				else
				{
					++playerBulletIter;
				}
			}
		}

		// MinionHorf 충돌
		for (int j = 0; j < ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size(); j++)
		{

			for (playerBulletIter = playerBulletVector.begin(); playerBulletIter != playerBulletVector.end(); )
			{
				if (IntersectRect(&temp, &playerBulletIter->rect, &ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect))
				{
					// 오브젝트 풀로 총알을 돌려주는 함수
					OBJECTPOOL->SetBulletVector(playerBulletVector.front());
					EFFECTMANAGER->play("tearpoof", playerBulletIter->rect.left, playerBulletIter->rect.top + 20);
					playerBulletIter = playerBulletVector.erase(playerBulletIter);

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.left &&
							ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.right <= 770)
						{
							ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(j, tempW * KNOCKBACK);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.right &&
							ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.left >= 110)
						{
							ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(j, -tempW * KNOCKBACK);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.top &&
							ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.bottom <= 460)
						{
							ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(j, tempH * KNOCKBACK);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.bottom &&
							ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.top >= 110)
						{
							ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(j, -tempH * KNOCKBACK);
						}
					}

					// 적 에너미의 HP 정보를 받아와 플레이어의 공격력만큼 HP를 감소시킨다
					ENEMYMANAGER->GetMinionHorf()->SetEnemyHp(j, ENEMYMANAGER->GetMinionHorf()->GetEnemyHp(j) - PLAYERMANAGER->GetPlayerOffensePower());

					if (ENEMYMANAGER->GetMinionHorf()->GetEnemyHp(j) <= 0)
					{
						EFFECTMANAGER->play("deathEffect", ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.left, ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.top);

						// 피격이 된 적의 벡터를 지우는 함수
						ENEMYMANAGER->GetMinionHorf()->DeleteEnemy(j);
					}

					// j번째 에너미 피격 시 j를 0으로 초기화
					j = 0;

					// 벡터값이 비어있으면 for문을 빠져나간다.
					if (ENEMYMANAGER->GetMinionHorf()->GetMinionVector().empty())
					{
						break;
					}
				}
				else
				{
					++playerBulletIter;
				}
			}
		}

		// MinionMulligan 충돌
		for (int j = 0; j < ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size(); j++)
		{

			for (playerBulletIter = playerBulletVector.begin(); playerBulletIter != playerBulletVector.end(); )
			{
				if (IntersectRect(&temp, &playerBulletIter->rect, &ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect))
				{
					// 오브젝트 풀로 총알을 돌려주는 함수
					OBJECTPOOL->SetBulletVector(playerBulletVector.front());
					EFFECTMANAGER->play("tearpoof", playerBulletIter->rect.left, playerBulletIter->rect.top + 20);
					playerBulletIter = playerBulletVector.erase(playerBulletIter);

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.left &&
							ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.right <= 770)
						{
							ENEMYMANAGER->GetMinionMulligan()->SetEnemyRectX(j, tempW * KNOCKBACK);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.right &&
							ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.left >= 110)
						{
							ENEMYMANAGER->GetMinionMulligan()->SetEnemyRectX(j, -tempW * KNOCKBACK);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.top &&
							ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.bottom <= 460)
						{
							ENEMYMANAGER->GetMinionMulligan()->SetEnemyRectY(j, tempH * KNOCKBACK);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.bottom &&
							ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.top >= 110)
						{
							ENEMYMANAGER->GetMinionMulligan()->SetEnemyRectY(j, -tempH * KNOCKBACK);
						}
					}

					// 적 에너미의 HP 정보를 받아와 플레이어의 공격력만큼 HP를 감소시킨다
					ENEMYMANAGER->GetMinionMulligan()->SetEnemyHp(j, ENEMYMANAGER->GetMinionMulligan()->GetEnemyHp(j) - PLAYERMANAGER->GetPlayerOffensePower());

					if (ENEMYMANAGER->GetMinionMulligan()->GetEnemyHp(j) <= 0)
					{
						EFFECTMANAGER->play("deathEffect", ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.left, ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.top);

						// 피격이 된 적의 벡터를 지우는 함수
						ENEMYMANAGER->GetMinionMulligan()->DeleteEnemy(j);
					}

					// j번째 에너미 피격 시 j를 0으로 초기화
					j = 0;

					// 벡터값이 비어있으면 for문을 빠져나간다.
					if (ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().empty())
					{
						break;
					}
				}
				else
				{
					++playerBulletIter;
				}
			}
		}

		// MinionPacer 충돌
		for (int j = 0; j < ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size(); j++)
		{

			for (playerBulletIter = playerBulletVector.begin(); playerBulletIter != playerBulletVector.end(); )
			{
				if (IntersectRect(&temp, &playerBulletIter->rect, &ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect))
				{
					// 오브젝트 풀로 총알을 돌려주는 함수
					OBJECTPOOL->SetBulletVector(playerBulletVector.front());
					EFFECTMANAGER->play("tearpoof", playerBulletIter->rect.left, playerBulletIter->rect.top + 20);
					playerBulletIter = playerBulletVector.erase(playerBulletIter);

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.left &&
							ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.right <= 770)
						{
							ENEMYMANAGER->GetMinionPacer()->SetEnemyRectX(j, tempW * KNOCKBACK);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.right &&
							ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.left >= 110)
						{
							ENEMYMANAGER->GetMinionPacer()->SetEnemyRectX(j, -tempW * KNOCKBACK);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.top &&
							ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.bottom <= 460)
						{
							ENEMYMANAGER->GetMinionPacer()->SetEnemyRectY(j, tempH * KNOCKBACK);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.bottom &&
							ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.top >= 110)
						{
							ENEMYMANAGER->GetMinionPacer()->SetEnemyRectY(j, -tempH * KNOCKBACK);
						}
					}

					// 적 에너미의 HP 정보를 받아와 플레이어의 공격력만큼 HP를 감소시킨다
					ENEMYMANAGER->GetMinionPacer()->SetEnemyHp(j, ENEMYMANAGER->GetMinionPacer()->GetEnemyHp(j) - PLAYERMANAGER->GetPlayerOffensePower());

					if (ENEMYMANAGER->GetMinionPacer()->GetEnemyHp(j) <= 0)
					{
						EFFECTMANAGER->play("deathEffect", ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.left, ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.top);

						// 피격이 된 적의 벡터를 지우는 함수
						ENEMYMANAGER->GetMinionPacer()->DeleteEnemy(j);
					}

					// j번째 에너미 피격 시 j를 0으로 초기화
					j = 0;

					// 벡터값이 비어있으면 for문을 빠져나간다.
					if (ENEMYMANAGER->GetMinionPacer()->GetMinionVector().empty())
					{
						break;
					}
				}
				else
				{
					++playerBulletIter;
				}
			}
		}
	}
}

void CollisionManager::EnemyBulletCollision(vector<BulletInfo>& enemyBulletVector, vector<BulletInfo>::iterator & enemyBulletIter)
{
	RECT temp;

	if (PLAYERMANAGER->GetPlayerDeath() == false)
	{
		if (!playerHit)
		{
			for (enemyBulletIter = enemyBulletVector.begin(); enemyBulletIter != enemyBulletVector.end();)
			{
				if (IntersectRect(&temp, &enemyBulletIter->rect, &PLAYERMANAGER->GetPlayerHitRect()))
				{
					PLAYERMANAGER->SetPlayerHp(-0.5f);
					playerHitAni = true;
					playerHit = true;
					EFFECTMANAGER->play("enemyBloodpoof", enemyBulletIter->rect.left, enemyBulletIter->rect.top + 20);

					// 오브젝트 풀로 총알을 돌려주는 함수
					OBJECTPOOL->SetBulletVector(enemyBulletVector.front());
					enemyBulletIter = enemyBulletVector.erase(enemyBulletIter);
				}
				else
				{
					++enemyBulletIter;
				}
			}
		}

		if (playerHit)
		{
			hitCount++;

			if (hitCount % PLAYERHITANIMATION == 0)
			{
				playerHitAni = false;
			}
			if (hitCount % PLAYERINVINCIBILITYTIME == 0)
			{
				playerHit = false;
				hitCount = 0;
			}
		}
	}
}

void CollisionManager::PlayerToMinionCollision()
{
	RECT temp;

	if (PLAYERMANAGER->GetPlayerDeath() == false)
	{
		if (!playerHit)
		{
			if (ENEMYMANAGER->GetMonstro()->GetJumpCollision() == false && ENEMYMANAGER->GetMonstro()->GetTeleportCollision() == false)
			{
				// Monstro 충돌
				for (int j = 0; j < ENEMYMANAGER->GetMonstro()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &ENEMYMANAGER->GetMonstro()->GetMinionVector()[j].enemyRect))
					{
						PLAYERMANAGER->SetPlayerHp(-0.5f);
						playerHitAni = true;
						playerHit = true;

						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
							{
								PLAYERMANAGER->SetPlayerRectX(tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
							{
								PLAYERMANAGER->SetPlayerRectX(-tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
							{
								PLAYERMANAGER->SetPlayerRectY(tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
							{
								PLAYERMANAGER->SetPlayerRectY(-tempH);
							}
						}
					}
				}
			}

			// MinionAttackFly 충돌
			for (int j = 0; j < ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size(); j++)
			{
				if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect))
				{
					PLAYERMANAGER->SetPlayerHp(-0.5f);
					playerHitAni = true;
					playerHit = true;

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
						{
							PLAYERMANAGER->SetPlayerRectX(tempW);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
						{
							PLAYERMANAGER->SetPlayerRectX(-tempW);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
						{
							PLAYERMANAGER->SetPlayerRectY(tempH);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
						{
							PLAYERMANAGER->SetPlayerRectY(-tempH);
						}
					}
				}
			}

			// MinionBlackFly 충돌
			for (int j = 0; j < ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size(); j++)
			{
				if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect))
				{
					PLAYERMANAGER->SetPlayerHp(-0.5f);
					playerHitAni = true;
					playerHit = true;

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
						{
							PLAYERMANAGER->SetPlayerRectX(tempW);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
						{
							PLAYERMANAGER->SetPlayerRectX(-tempW);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
						{
							PLAYERMANAGER->SetPlayerRectY(tempH);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
						{
							PLAYERMANAGER->SetPlayerRectY(-tempH);
						}
					}
				}
			}

			// MinionMaw 충돌
			for (int j = 0; j < ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size(); j++)
			{
				if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect))
				{
					PLAYERMANAGER->SetPlayerHp(-0.5f);
					playerHitAni = true;
					playerHit = true;

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
						{
							PLAYERMANAGER->SetPlayerRectX(tempW);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
						{
							PLAYERMANAGER->SetPlayerRectX(-tempW);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
						{
							PLAYERMANAGER->SetPlayerRectY(tempH);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
						{
							PLAYERMANAGER->SetPlayerRectY(-tempH);
						}
					}
				}
			}

			// MinionTumor 충돌
			for (int j = 0; j < ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size(); j++)
			{
				if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect))
				{
					PLAYERMANAGER->SetPlayerHp(-0.5f);
					playerHitAni = true;
					playerHit = true;

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
						{
							PLAYERMANAGER->SetPlayerRectX(tempW);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
						{
							PLAYERMANAGER->SetPlayerRectX(-tempW);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
						{
							PLAYERMANAGER->SetPlayerRectY(tempH);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
						{
							PLAYERMANAGER->SetPlayerRectY(-tempH);
						}
					}
				}
			}

			// MinionClot 충돌
			for (int j = 0; j < ENEMYMANAGER->GetMinionClot()->GetMinionVector().size(); j++)
			{
				if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect))
				{
					PLAYERMANAGER->SetPlayerHp(-0.5f);
					playerHitAni = true;
					playerHit = true;

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
						{
							PLAYERMANAGER->SetPlayerRectX(tempW);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
						{
							PLAYERMANAGER->SetPlayerRectX(-tempW);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
						{
							PLAYERMANAGER->SetPlayerRectY(tempH);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
						{
							PLAYERMANAGER->SetPlayerRectY(-tempH);
						}
					}
				}
			}

			// MinionClotty 충돌
			for (int j = 0; j < ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size(); j++)
			{
				if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect))
				{
					PLAYERMANAGER->SetPlayerHp(-0.5f);
					playerHitAni = true;
					playerHit = true;

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
						{
							PLAYERMANAGER->SetPlayerRectX(tempW);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
						{
							PLAYERMANAGER->SetPlayerRectX(-tempW);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
						{
							PLAYERMANAGER->SetPlayerRectY(tempH);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
						{
							PLAYERMANAGER->SetPlayerRectY(-tempH);
						}
					}
				}
			}

			// MinionGaper 충돌
			for (int j = 0; j < ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size(); j++)
			{
				if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect))
				{
					PLAYERMANAGER->SetPlayerHp(-0.5f);
					playerHitAni = true;
					playerHit = true;

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
						{
							PLAYERMANAGER->SetPlayerRectX(tempW);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
						{
							PLAYERMANAGER->SetPlayerRectX(-tempW);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
						{
							PLAYERMANAGER->SetPlayerRectY(tempH);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
						{
							PLAYERMANAGER->SetPlayerRectY(-tempH);
						}
					}
				}
			}

			// MinionHorf 충돌
			for (int j = 0; j < ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size(); j++)
			{
				if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect))
				{
					PLAYERMANAGER->SetPlayerHp(-0.5f);
					playerHitAni = true;
					playerHit = true;

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
						{
							PLAYERMANAGER->SetPlayerRectX(tempW);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
						{
							PLAYERMANAGER->SetPlayerRectX(-tempW);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
						{
							PLAYERMANAGER->SetPlayerRectY(tempH);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
						{
							PLAYERMANAGER->SetPlayerRectY(-tempH);
						}
					}
				}
			}

			// MinionMulligan 충돌
			for (int j = 0; j < ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size(); j++)
			{
				if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect))
				{
					PLAYERMANAGER->SetPlayerHp(-0.5f);
					playerHitAni = true;
					playerHit = true;

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
						{
							PLAYERMANAGER->SetPlayerRectX(tempW);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
						{
							PLAYERMANAGER->SetPlayerRectX(-tempW);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
						{
							PLAYERMANAGER->SetPlayerRectY(tempH);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
						{
							PLAYERMANAGER->SetPlayerRectY(-tempH);
						}
					}
				}
			}

			// MinionPacer 충돌
			for (int j = 0; j < ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size(); j++)
			{
				if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect))
				{
					PLAYERMANAGER->SetPlayerHp(-0.5f);
					playerHitAni = true;
					playerHit = true;

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
						{
							PLAYERMANAGER->SetPlayerRectX(tempW);
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
						{
							PLAYERMANAGER->SetPlayerRectX(-tempW);
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
						{
							PLAYERMANAGER->SetPlayerRectY(tempH);
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
						{
							PLAYERMANAGER->SetPlayerRectY(-tempH);
						}
					}
				}
			}
		}
		else
		{
			hitCount++;

			if (hitCount % PLAYERHITANIMATION == 0)
			{
				playerHitAni = false;
			}
			if (hitCount % PLAYERINVINCIBILITYTIME == 0)
			{
				playerHit = false;
				hitCount = 0;
			}
		}
	}
}

void CollisionManager::SameVectorMinionCollision(vector<EnemyInfo>& enemyVector)
{
	RECT temp;

	if (!enemyVector.empty())
	{
		for (int i = 0; i < enemyVector.size(); i++)
		{
			for (int j = 0; j < enemyVector.size(); j++)
			{
				if (IntersectRect(&temp, &enemyVector[i].enemyRect, &enemyVector[j].enemyRect))
				{
					if (i == j) continue;

					// temp의 Width와 Height 선언
					int tempW = temp.right - temp.left;
					int tempH = temp.bottom - temp.top;

					if (tempH > tempW)
					{
						// 왼쪽 충돌시 오른쪽으로 밀어줌
						if (temp.left == enemyVector[i].enemyRect.left)
						{
							enemyVector[i].enemyRect.left += tempW;
							enemyVector[i].enemyRect.right += tempW;
						}
						// 오른쪽 충돌시 왼쪽으로 밀어줌
						else if (temp.right == enemyVector[i].enemyRect.right)
						{
							enemyVector[i].enemyRect.left -= tempW;
							enemyVector[i].enemyRect.right -= tempW;
						}
					}
					else
					{
						// 위쪽 충돌시 아래쪽으로 밀어줌
						if (temp.top == enemyVector[i].enemyRect.top)
						{
							enemyVector[i].enemyRect.top += tempH;
							enemyVector[i].enemyRect.bottom += tempH;
						}
						// 아래쪽 충돌시 위쪽으로 밀어줌
						else if (temp.bottom == enemyVector[i].enemyRect.bottom)
						{
							enemyVector[i].enemyRect.top -= tempH;
							enemyVector[i].enemyRect.bottom -= tempH;
						}
					}
				}
			}
		}
	}
}

void CollisionManager::MinionToMinionCollision()
{
	RECT temp;

	// MinionHorf을 기준으로 다른 에너미 충돌
	if (ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size() > 0)
	{
		//MinionHorf To MinionAttackFly 충돌
		if (ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionHorf To MinionBlackFly 충돌
		if (ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionHorf To MinionMaw 충돌
		if (ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionHorf To MinionTumor 충돌
		if (ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionHorf To MinionClot 충돌
		if (ENEMYMANAGER->GetMinionClot()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionClot()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionHorf To MinionClotty 충돌
		if (ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionHorf To MinionGaper 충돌
		if (ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionHorf To MinionMulligan 충돌
		if (ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionHorf To MinionPacer 충돌
		if (ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}
	}

	// MinionAttackFly을 기준으로 다른 에너미 충돌
	if (ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size() > 0)
	{
		//MinionAttackFly To MinionBlackFly 충돌
		if (ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionAttackFly To MinionMaw 충돌
		if (ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionAttackFly To MinionTumor 충돌
		if (ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionAttackFly To MinionClot 충돌
		if (ENEMYMANAGER->GetMinionClot()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionClot()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionAttackFly To MinionClotty 충돌
		if (ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionAttackFly To MinionGaper 충돌
		if (ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionAttackFly To MinionMulligan 충돌
		if (ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionAttackFly To MinionPacer 충돌
		if (ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionAttackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionAttackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}
	}

	// MinionBlackFly을 기준으로 다른 에너미 충돌
	if (ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size() > 0)
	{
		//MinionBlackFly To MinionMaw 충돌
		if (ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionBlackFly To MinionTumor 충돌
		if (ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionBlackFly To MinionClot 충돌
		if (ENEMYMANAGER->GetMinionClot()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionClot()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionBlackFly To MinionClotty 충돌
		if (ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionBlackFly To MinionGaper 충돌
		if (ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionBlackFly To MinionMulligan 충돌
		if (ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionBlackFly To MinionPacer 충돌
		if (ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionBlackFly()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionBlackFly()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}
	}

	// MinionMaw을 기준으로 다른 에너미 충돌
	if (ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size() > 0)
	{
		//MinionMaw To MinionTumor 충돌
		if (ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionMaw To MinionClot 충돌
		if (ENEMYMANAGER->GetMinionClot()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionClot()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionMaw To MinionClotty 충돌
		if (ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionMaw To MinionGaper 충돌
		if (ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionMaw To MinionMulligan 충돌
		if (ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionMaw To MinionPacer 충돌
		if (ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionMaw()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionMaw()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionMaw()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}
	}

	// MinionTumor을 기준으로 다른 에너미 충돌
	if (ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size() > 0)
	{
		//MinionTumor To MinionClot 충돌
		if (ENEMYMANAGER->GetMinionClot()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionClot()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionTumor To MinionClotty 충돌
		if (ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionTumor To MinionGaper 충돌
		if (ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionTumor To MinionMulligan 충돌
		if (ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionTumor To MinionPacer 충돌
		if (ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionTumor()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionTumor()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionTumor()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}
	}

	// MinionClot을 기준으로 다른 에너미 충돌
	if (ENEMYMANAGER->GetMinionClot()->GetMinionVector().size() > 0)
	{
		//MinionClot To MinionClotty 충돌
		if (ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionClot()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionClot To MinionGaper 충돌
		if (ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionClot()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionClot To MinionMulligan 충돌
		if (ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionClot()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionClot To MinionPacer 충돌
		if (ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionClot()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionClot()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}
	}

	// MinionClotty을 기준으로 다른 에너미 충돌
	if (ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size() > 0)
	{
		//MinionClotty To MinionGaper 충돌
		if (ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionClotty()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionClotty()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionClotty()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionClotty()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionClotty To MinionMulligan 충돌
		if (ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionClotty()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionClotty()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionClotty()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionClotty()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionClotty To MinionPacer 충돌
		if (ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionClotty()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionClotty()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionClotty()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionClotty()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

	}

	// MinionGaper을 기준으로 다른 에너미 충돌
	if (ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size() > 0)
	{
		//MinionGaper To MinionMulligan 충돌
		if (ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionGaper()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionGaper()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionGaper()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionGaper()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}

		//MinionGaper To MinionPacer 충돌
		if (ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionGaper()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionGaper()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionGaper()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionGaper()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}
	}

	// MinionMulligan을 기준으로 다른 에너미 충돌
	if (ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size() > 0)
	{
		//MinionMulligan To MinionPacer 충돌
		if (ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size() > 0)
		{
			for (int i = 0; i < ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size(); i++)
			{
				for (int j = 0; j < ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size(); j++)
				{
					if (IntersectRect(&temp, &ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[i].enemyRect, &ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect))
					{
						// temp의 Width와 Height 선언
						int tempW = temp.right - temp.left;
						int tempH = temp.bottom - temp.top;

						if (tempH > tempW)
						{
							// 왼쪽 충돌시 오른쪽으로 밀어줌
							if (temp.left == ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[i].enemyRect.left)
							{
								ENEMYMANAGER->GetMinionMulligan()->SetEnemyRectX(i, tempW);
							}
							// 오른쪽 충돌시 왼쪽으로 밀어줌
							else if (temp.right == ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[i].enemyRect.right)
							{
								ENEMYMANAGER->GetMinionMulligan()->SetEnemyRectX(i, -tempW);
							}
						}
						else
						{
							// 위쪽 충돌시 아래쪽으로 밀어줌
							if (temp.top == ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[i].enemyRect.top)
							{
								ENEMYMANAGER->GetMinionMulligan()->SetEnemyRectY(i, tempH);
							}
							// 아래쪽 충돌시 위쪽으로 밀어줌
							else if (temp.bottom == ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[i].enemyRect.bottom)
							{
								ENEMYMANAGER->GetMinionMulligan()->SetEnemyRectY(i, -tempH);
							}
						}
					}
				}
			}
		}
	}
}

void CollisionManager::PlayerToObstacleCollision(RECT obstacleRect)
{
	RECT temp;

	if (PLAYERMANAGER->GetPlayerDeath() == false)
	{
		if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &obstacleRect))
		{
			// temp의 Width와 Height 선언
			int tempW = temp.right - temp.left;
			int tempH = temp.bottom - temp.top;

			if (tempH > tempW)
			{
				// 왼쪽 충돌시 오른쪽으로 밀어줌
				if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
				{
					PLAYERMANAGER->SetPlayerRectX(tempW);
				}
				// 오른쪽 충돌시 왼쪽으로 밀어줌
				else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
				{
					PLAYERMANAGER->SetPlayerRectX(-tempW);
				}
			}
			else
			{
				// 위쪽 충돌시 아래쪽으로 밀어줌
				if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
				{
					PLAYERMANAGER->SetPlayerRectY(tempH);
				}
				// 아래쪽 충돌시 위쪽으로 밀어줌
				else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
				{
					PLAYERMANAGER->SetPlayerRectY(-tempH);
				}
			}
		}
	}
}

void CollisionManager::EnemyToObstacleCollision(RECT obstacleRect)
{
	RECT temp;

	// MinionClot 충돌
	for (int j = 0; j < ENEMYMANAGER->GetMinionClot()->GetMinionVector().size(); j++)
	{
		if (IntersectRect(&temp, &obstacleRect, &ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect))
		{
			// temp의 Width와 Height 선언
			int tempW = temp.right - temp.left;
			int tempH = temp.bottom - temp.top;

			if (tempH > tempW)
			{
				// 왼쪽 충돌시 오른쪽으로 밀어줌
				if (temp.left == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.left)
				{
					ENEMYMANAGER->GetMinionClot()->SetEnemyRectX(j, tempW);
				}
				// 오른쪽 충돌시 왼쪽으로 밀어줌
				else if (temp.right == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.right)
				{
					ENEMYMANAGER->GetMinionClot()->SetEnemyRectX(j, -tempW);
				}
			}
			else
			{
				// 위쪽 충돌시 아래쪽으로 밀어줌
				if (temp.top == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.top)
				{
					ENEMYMANAGER->GetMinionClot()->SetEnemyRectY(j, tempH);
				}
				// 아래쪽 충돌시 위쪽으로 밀어줌
				else if (temp.bottom == ENEMYMANAGER->GetMinionClot()->GetMinionVector()[j].enemyRect.bottom)
				{
					ENEMYMANAGER->GetMinionClot()->SetEnemyRectY(j, -tempH);
				}
			}
		}
	}

	// MinionClotty 충돌
	for (int j = 0; j < ENEMYMANAGER->GetMinionClotty()->GetMinionVector().size(); j++)
	{
		if (IntersectRect(&temp, &obstacleRect, &ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect))
		{
			// temp의 Width와 Height 선언
			int tempW = temp.right - temp.left;
			int tempH = temp.bottom - temp.top;

			if (tempH > tempW)
			{
				// 왼쪽 충돌시 오른쪽으로 밀어줌
				if (temp.left == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.left)
				{
					ENEMYMANAGER->GetMinionClotty()->SetEnemyRectX(j, tempW);
				}
				// 오른쪽 충돌시 왼쪽으로 밀어줌
				else if (temp.right == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.right)
				{
					ENEMYMANAGER->GetMinionClotty()->SetEnemyRectX(j, -tempW);
				}
			}
			else
			{
				// 위쪽 충돌시 아래쪽으로 밀어줌
				if (temp.top == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.top)
				{
					ENEMYMANAGER->GetMinionClotty()->SetEnemyRectY(j, tempH);
				}
				// 아래쪽 충돌시 위쪽으로 밀어줌
				else if (temp.bottom == ENEMYMANAGER->GetMinionClotty()->GetMinionVector()[j].enemyRect.bottom)
				{
					ENEMYMANAGER->GetMinionClotty()->SetEnemyRectY(j, -tempH);
				}
			}
		}
	}

	// MinionGaper 충돌
	for (int j = 0; j < ENEMYMANAGER->GetMinionGaper()->GetMinionVector().size(); j++)
	{
		if (IntersectRect(&temp, &obstacleRect, &ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect))
		{
			// temp의 Width와 Height 선언
			int tempW = temp.right - temp.left;
			int tempH = temp.bottom - temp.top;

			if (tempH > tempW)
			{
				// 왼쪽 충돌시 오른쪽으로 밀어줌
				if (temp.left == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.left)
				{
					ENEMYMANAGER->GetMinionGaper()->SetEnemyRectX(j, tempW);
				}
				// 오른쪽 충돌시 왼쪽으로 밀어줌
				else if (temp.right == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.right)
				{
					ENEMYMANAGER->GetMinionGaper()->SetEnemyRectX(j, -tempW);
				}
			}
			else
			{
				// 위쪽 충돌시 아래쪽으로 밀어줌
				if (temp.top == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.top)
				{
					ENEMYMANAGER->GetMinionGaper()->SetEnemyRectY(j, tempH);
				}
				// 아래쪽 충돌시 위쪽으로 밀어줌
				else if (temp.bottom == ENEMYMANAGER->GetMinionGaper()->GetMinionVector()[j].enemyRect.bottom)
				{
					ENEMYMANAGER->GetMinionGaper()->SetEnemyRectY(j, -tempH);
				}
			}
		}
	}

	// MinionHorf 충돌
	for (int j = 0; j < ENEMYMANAGER->GetMinionHorf()->GetMinionVector().size(); j++)
	{
		if (IntersectRect(&temp, &obstacleRect, &ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect))
		{
			// temp의 Width와 Height 선언
			int tempW = temp.right - temp.left;
			int tempH = temp.bottom - temp.top;

			if (tempH > tempW)
			{
				// 왼쪽 충돌시 오른쪽으로 밀어줌
				if (temp.left == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.left)
				{
					ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(j, tempW);
				}
				// 오른쪽 충돌시 왼쪽으로 밀어줌
				else if (temp.right == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.right)
				{
					ENEMYMANAGER->GetMinionHorf()->SetEnemyRectX(j, -tempW);
				}
			}
			else
			{
				// 위쪽 충돌시 아래쪽으로 밀어줌
				if (temp.top == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.top)
				{
					ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(j, tempH);
				}
				// 아래쪽 충돌시 위쪽으로 밀어줌
				else if (temp.bottom == ENEMYMANAGER->GetMinionHorf()->GetMinionVector()[j].enemyRect.bottom)
				{
					ENEMYMANAGER->GetMinionHorf()->SetEnemyRectY(j, -tempH);
				}
			}
		}
	}

	// MinionMulligan 충돌
	for (int j = 0; j < ENEMYMANAGER->GetMinionMulligan()->GetMinionVector().size(); j++)
	{
		if (IntersectRect(&temp, &obstacleRect, &ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect))
		{
			// temp의 Width와 Height 선언
			int tempW = temp.right - temp.left;
			int tempH = temp.bottom - temp.top;

			if (tempH > tempW)
			{
				// 왼쪽 충돌시 오른쪽으로 밀어줌
				if (temp.left == ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.left)
				{
					ENEMYMANAGER->GetMinionMulligan()->SetEnemyRectX(j, tempW);
				}
				// 오른쪽 충돌시 왼쪽으로 밀어줌
				else if (temp.right == ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.right)
				{
					ENEMYMANAGER->GetMinionMulligan()->SetEnemyRectX(j, -tempW);
				}
			}
			else
			{
				// 위쪽 충돌시 아래쪽으로 밀어줌
				if (temp.top == ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.top)
				{
					ENEMYMANAGER->GetMinionMulligan()->SetEnemyRectY(j, tempH);
				}
				// 아래쪽 충돌시 위쪽으로 밀어줌
				else if (temp.bottom == ENEMYMANAGER->GetMinionMulligan()->GetMinionVector()[j].enemyRect.bottom)
				{
					ENEMYMANAGER->GetMinionMulligan()->SetEnemyRectY(j, -tempH);
				}
			}
		}
	}

	// MinionPacer 충돌
	for (int j = 0; j < ENEMYMANAGER->GetMinionPacer()->GetMinionVector().size(); j++)
	{
		if (IntersectRect(&temp, &obstacleRect, &ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect))
		{
			// temp의 Width와 Height 선언
			int tempW = temp.right - temp.left;
			int tempH = temp.bottom - temp.top;

			if (tempH > tempW)
			{
				// 왼쪽 충돌시 오른쪽으로 밀어줌
				if (temp.left == ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.left)
				{
					ENEMYMANAGER->GetMinionPacer()->SetEnemyRectX(j, tempW);
				}
				// 오른쪽 충돌시 왼쪽으로 밀어줌
				else if (temp.right == ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.right)
				{
					ENEMYMANAGER->GetMinionPacer()->SetEnemyRectX(j, -tempW);
				}
			}
			else
			{
				// 위쪽 충돌시 아래쪽으로 밀어줌
				if (temp.top == ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.top)
				{
					ENEMYMANAGER->GetMinionPacer()->SetEnemyRectY(j, tempH);
				}
				// 아래쪽 충돌시 위쪽으로 밀어줌
				else if (temp.bottom == ENEMYMANAGER->GetMinionPacer()->GetMinionVector()[j].enemyRect.bottom)
				{
					ENEMYMANAGER->GetMinionPacer()->SetEnemyRectY(j, -tempH);
				}
			}
		}
	}
}

int CollisionManager::PlayerCollisionNextDoor(RECT nextDoor)
{
	RECT temp;
	if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &nextDoor))
	{

		// temp의 Width와 Height 선언
		int tempW = temp.right - temp.left;
		int tempH = temp.bottom - temp.top;

		if (tempH >= tempW)
		{
			// 왼쪽 충돌시 오른쪽으로 밀어줌
			if (temp.left == PLAYERMANAGER->GetPlayerHitRect().left)
			{
				//PLAYERMANAGER->SetPlayerRectX(tempW * 50);
				direction = 1;
				return direction;
			}
			// 오른쪽 충돌시 왼쪽으로 밀어줌
			else if (temp.right == PLAYERMANAGER->GetPlayerHitRect().right)
			{
				//PLAYERMANAGER->SetPlayerRectX(-tempW * 50);
				direction = 2;

				return direction;
			}
		}
		else
		{
			// 위쪽 충돌시 아래쪽으로 밀어줌
			if (temp.top == PLAYERMANAGER->GetPlayerHitRect().top)
			{
				//PLAYERMANAGER->SetPlayerRectY(tempH * 50);
				direction = 3;

				return direction;
			}
			// 아래쪽 충돌시 위쪽으로 밀어줌
			else if (temp.bottom == PLAYERMANAGER->GetPlayerHitRect().bottom)
			{
				//PLAYERMANAGER->SetPlayerRectY(-tempH * 50);
				direction = 4;

				return direction;
			}
		}
	}
}

bool CollisionManager::PlayerBulletToObjectCollision(RECT obstacleRect)
{
	RECT temp;
	for (int i = 0; i < PLAYERMANAGER->GetPlayerBulletVector().size() > 0; i++)
	{
		if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerBulletVector()[i].rect, &obstacleRect))
		{
			OBJECTPOOL->SetBulletVector(PLAYERMANAGER->GetPlayerBulletVector()[i]);
			EFFECTMANAGER->play("tearpoof", PLAYERMANAGER->GetPlayerBulletVector()[i].rect.left, PLAYERMANAGER->GetPlayerBulletVector()[i].rect.top + 20);
			PLAYERMANAGER->DeleteBullet(i);
			return true;
		}
	}
	return false;
}
