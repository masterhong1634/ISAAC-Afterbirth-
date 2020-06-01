#pragma once

//캐릭터 방향 enum문
enum pDirection
{
	PLAYER_IDLE,
	PLAYER_LEFT,
	PLAYER_RIGHT,
	PLAYER_UP,
	PLAYER_DOWN
};

//캐릭터 속성 구조체
struct PlayerInfo
{
	image*	playerHeadImage;		// 플레이어 머리 이미지
	image*	playerBodyImage;		// 플레이어 몸 이미지
	image*	playerHitImage;			// 플레이어 피격 이미지
	image*	playerShadowImage;		// 플레이어 그림자 이미지
	image*	playerDeathImage;		// 플레이어 사망 이미지
	RECT	playerHeadRect;			// 플레이어 머리 상자
	RECT	playerBodyRect;			// 플레이어 몸 상자
	RECT	playerHitRect;			// 플레이어 피격용 상자
	int     playerOffensePower;		// 플레이어 공격력
	int     playerShotDelay;		// 공격주기
	float   playerShotSpeed;		// 공격속도
	float   playerShotRange;		// 공격사거리
	float   playerSpeed;			// 이동속도
	float   playerSlideSpeed;		// 슬라이딩 속도

	// 아이템
	int		playerGold;				// 플레이어 골드
	int		playerBomb;				// 플레이어 폭탄
	int		playerKey;				// 플레이어 키
	float	playerHp;				// 플레이어 체력
	float	playerMaxHp;			// 플레이어 최대 체력
};

class Player
{
private:
	animation* aniHead;
	animation* aniBody;
	animation* aniHit;
	image*	   playerUINumber;
	image*	   playerUIstats;
	image*	   playerUIpicks;
	image*	   playerUIhearts;

	image* blackBg;
	image* gameOver;
	int deathCutSceneCount;

	//UI용 배열
	int		   currentGold[2];
	int		   currentBomb[2];
	int		   currentKey[2];
	   
	float	   currentSpeedFrame[4];
	float	   currentRangeFrame[5];
	float	   currentShotSpeedFrame[4];
	int		   currentShotDelayFrame[4];
	int		   currentOffenseFrame[4];

private:
	pDirection direction;
	PlayerInfo player;

	vector<BulletInfo> vPlayerBullet;
	vector<BulletInfo>::iterator viPlayerBullet;

	vector<BombInfo> vPlayerBomb;
	vector<BombInfo>::iterator viPlayerBomb;

	int playerBulletCount;			// 플레이어 불렛 카운트
	int playerDeathCount;			// 플레이어 사망 카운트
	int playerBombDelay = 125;		// 폭탄 카운트

	// 애니메이션을 돌리기 위한 배열
	int arrHeadIdle[1] = { 0 };
	int arrHeadRight[1] = { 2 };
	int arrHeadUp[1] = { 4 };
	int arrHeadLeft[1] = { 7 };
	int arrBodyIdle[1] = { 22 };

private:
	// 플레이어 무브 변수
	bool isLeft;
	bool isRight;
	bool isUp;
	bool isDown;
	// 플레이어 슬라이딩 변수
	bool slideLeft;
	bool slideRight;
	bool slideUp;
	bool slideDown;
	// 플레이어 슈팅 변수
	bool playerLeftShot;
	bool playerRightShot;
	bool playerUpShot;
	bool playerDownShot;
	// 플레이어 사망 변수
	bool playerDeath;

	char str[128];

private:
	RECT temp;

	vector <ItemInfo> vPlayerActiveItem;				// 액티브 아이템 벡터
	vector <ItemInfo>::iterator viPlayerActiveItem;		// 액티브 아이템 벡터
	vector <ItemInfo> vPlayerPassiveItem;				// 패시브 아이템 벡터
	vector <ItemInfo>::iterator viPlayerPassiveItem;	// 패시브 아이템 벡터
	vector <ItemInfo> vPlayerTrinkets;					// 장신구 아이템 벡터
	vector <ItemInfo>::iterator viPlayerTrinkets;		// 장신구 아이템 벡터

	vector <ItemInfo> vPlayerHeart;						// 체력 아이템 벡터
	vector <ItemInfo>::iterator viPlayerHeart;			// 체력 아이템 벡터

	vector <ItemInfo> vPlayerGold;						// 골드 아이템 벡터
	vector <ItemInfo>::iterator viPlayerGold;			// 골드 아이템 벡터
	//vector <ItemInfo> vPlayerBomb;						// 폭탄 아이템 벡터
	//vector <ItemInfo>::iterator viPlayerBomb;			// 폭탄 아이템 벡터
	vector <ItemInfo> vPlayerKey;						// 열쇠 아이템 벡터
	vector <ItemInfo>::iterator viPlayerKey;			// 열쇠 아이템 벡터

	vector <ItemInfo> vPlayerCard;						// 카드 아이템 벡터
	vector <ItemInfo>::iterator viPlayerCard;			// 카드 아이템 벡터
	vector <ItemInfo> vPlayerPill;						// 알약 아이템 벡터
	vector <ItemInfo>::iterator viPlayerPill;			// 알약 아이템 벡터

	vector <ItemInfo> vPlayerAllItem;						// 모든 아이템 벡터
	vector <ItemInfo>::iterator viPlayerAllItem;			// 모든 아이템 벡터

public:
	Player();
	~Player();

	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void PlayerDeath();			// 플레이어 사망	
	void PlayerMove();			// 플레이어 이동키 함수
	void PlayerSilde();			// 플레이어 이동시 슬라이딩 함수
	void PlayerShot();			// 플레이어 공격키 함수
	void PlayerShotMove();		// 플레이어 공격시 방향 함수
	void PlayerAnimation();		// 플레이어 애니메이션
	void PlayerStatUpdate();	// 플레이어 스탯 업데이트

	RECT GetPlayerHeadRect() { return player.playerHeadRect; }
	float GetPlayerHeadRectX() { return (player.playerHeadRect.left + (player.playerHeadRect.right - player.playerHeadRect.left) / 2); }
	float GetPlayerHeadRectY() { return (player.playerHeadRect.top + (player.playerHeadRect.bottom - player.playerHeadRect.top) / 2); }

	RECT GetPlayerHitRect() { return player.playerHitRect; }
	float GetPlayerHitRectX() { return (player.playerHitRect.left + (player.playerHitRect.right - player.playerHitRect.left) / 2); }
	float GetPlayerHitRectY() { return (player.playerHitRect.top + (player.playerHitRect.bottom - player.playerHitRect.top) / 2); }

	void SetPlayerHp(float num);
	int GetPlayerGold() { return player.playerGold; }
	void SetPlayerGold(int num);
	int GetPlayerBomb() { return player.playerBomb; }
	void SetPlayerBomb(int num);
	int GetPlayerKey() { return player.playerKey; }
	void SetPlayerKey(int num);

	void SetPlayerRectX(int num);
	void SetPlayerRectY(int num);

	vector<BulletInfo>& GetPlayerBulletVector() { return vPlayerBullet; }
	void DeleteBullet(int num);

	int GetPlayerOffensePower() { return player.playerOffensePower; }
	void SetPlayerOffensePower(int addPower) { player.playerOffensePower += addPower; }

	int GetPlayerShotSpeed() { return player.playerShotSpeed; }
	void SetPlayerShotSpeed(int addShotSpeed) { player.playerShotSpeed += addShotSpeed; }

	int GetPlayerShotRange() { return player.playerShotRange; }
	void SetPlayerShotRange(int addShotRange) { player.playerShotRange += addShotRange; }

	int GetPlayerSpeed() { return player.playerSpeed; }
	void SetPlayerSpeed(int addSpeed) { player.playerSpeed += addSpeed; }

	int GetPlayerMaxHp() { return player.playerMaxHp; }
	void SetPlayerMaxHp(int addMaxHp) { player.playerMaxHp += addMaxHp; }

	void SetPlayerAllItem(ItemInfo itemInfo) { vPlayerAllItem.push_back(itemInfo); }

	bool GetPlayerDeath() { return playerDeath; }
};
