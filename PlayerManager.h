#pragma once
#include "singletonBase.h"
#include "Player.h"

class PlayerManager : public singletonBase<PlayerManager>
{
private:
	Player* m_Player;

public:
	PlayerManager();
	~PlayerManager();

	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	vector<BulletInfo>& GetPlayerBulletVector() { return  m_Player->GetPlayerBulletVector(); }
	void DeleteBullet(int num) { m_Player->DeleteBullet(num); }


	RECT GetPlayerHeadRect() { return m_Player->GetPlayerHeadRect(); }
	float GetPlayerHeadRectX() { return m_Player->GetPlayerHeadRectX(); }
	float GetPlayerHeadRectY() { return m_Player->GetPlayerHeadRectY(); }

	RECT GetPlayerHitRect() { return m_Player->GetPlayerHitRect(); }
	float GetPlayerHitRectX() { return m_Player->GetPlayerHitRectX(); }
	float GetPlayerHitRectY() { return m_Player->GetPlayerHitRectY(); }

	void SetPlayerRectX(float num) { m_Player->SetPlayerRectX(num); }
	void SetPlayerRectY(float num) { m_Player->SetPlayerRectY(num); }

	int GetPlayerOffensePower() { return m_Player->GetPlayerOffensePower(); }
	void SetPlayerOffensePower(int addPower) { m_Player->SetPlayerOffensePower(addPower); }

	int GetPlayerShotSpeed() { return m_Player->GetPlayerShotSpeed(); }
	void SetPlayerShotSpeed(int addShotSpeed) { m_Player->SetPlayerShotSpeed(addShotSpeed); }

	int GetPlayerShotRange() { return m_Player->GetPlayerShotRange(); }
	void SetPlayerShotRange(int addShotRange) { m_Player->SetPlayerShotRange(addShotRange); }

	int GetPlayerSpeed() { return m_Player->GetPlayerSpeed(); }
	void SetPlayerSpeed(int addSpeed) { m_Player->SetPlayerSpeed(addSpeed); }

	int GetPlayerMaxHp() { return m_Player->GetPlayerMaxHp(); }
	void SetPlayerMaxHp(int addMaxHp) { m_Player->SetPlayerMaxHp(addMaxHp); }

	void SetPlayerHp(float num) { m_Player->SetPlayerHp(num); }
	int GetPlayerGold() { return m_Player->GetPlayerGold(); }
	void SetPlayerGold(int num) { m_Player->SetPlayerGold(num); }
	int GetPlayerBomb() { return m_Player->GetPlayerBomb(); }
	void SetPlayerBomb(int num) { m_Player->SetPlayerBomb(num); }
	int GetPlayerKey() { return m_Player->GetPlayerKey(); }
	void SetPlayerKey(int num) { m_Player->SetPlayerKey(num); }

	void SetPlayerAllItem(ItemInfo itemInfo) { m_Player->SetPlayerAllItem(itemInfo); }

	bool GetPlayerDeath() { return m_Player->GetPlayerDeath(); }
};
