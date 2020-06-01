#pragma once

//ĳ���� ���� enum��
enum pDirection
{
	PLAYER_IDLE,
	PLAYER_LEFT,
	PLAYER_RIGHT,
	PLAYER_UP,
	PLAYER_DOWN
};

//ĳ���� �Ӽ� ����ü
struct PlayerInfo
{
	image*	playerHeadImage;		// �÷��̾� �Ӹ� �̹���
	image*	playerBodyImage;		// �÷��̾� �� �̹���
	image*	playerHitImage;			// �÷��̾� �ǰ� �̹���
	image*	playerShadowImage;		// �÷��̾� �׸��� �̹���
	image*	playerDeathImage;		// �÷��̾� ��� �̹���
	RECT	playerHeadRect;			// �÷��̾� �Ӹ� ����
	RECT	playerBodyRect;			// �÷��̾� �� ����
	RECT	playerHitRect;			// �÷��̾� �ǰݿ� ����
	int     playerOffensePower;		// �÷��̾� ���ݷ�
	int     playerShotDelay;		// �����ֱ�
	float   playerShotSpeed;		// ���ݼӵ�
	float   playerShotRange;		// ���ݻ�Ÿ�
	float   playerSpeed;			// �̵��ӵ�
	float   playerSlideSpeed;		// �����̵� �ӵ�

	// ������
	int		playerGold;				// �÷��̾� ���
	int		playerBomb;				// �÷��̾� ��ź
	int		playerKey;				// �÷��̾� Ű
	float	playerHp;				// �÷��̾� ü��
	float	playerMaxHp;			// �÷��̾� �ִ� ü��
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

	//UI�� �迭
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

	int playerBulletCount;			// �÷��̾� �ҷ� ī��Ʈ
	int playerDeathCount;			// �÷��̾� ��� ī��Ʈ
	int playerBombDelay = 125;		// ��ź ī��Ʈ

	// �ִϸ��̼��� ������ ���� �迭
	int arrHeadIdle[1] = { 0 };
	int arrHeadRight[1] = { 2 };
	int arrHeadUp[1] = { 4 };
	int arrHeadLeft[1] = { 7 };
	int arrBodyIdle[1] = { 22 };

private:
	// �÷��̾� ���� ����
	bool isLeft;
	bool isRight;
	bool isUp;
	bool isDown;
	// �÷��̾� �����̵� ����
	bool slideLeft;
	bool slideRight;
	bool slideUp;
	bool slideDown;
	// �÷��̾� ���� ����
	bool playerLeftShot;
	bool playerRightShot;
	bool playerUpShot;
	bool playerDownShot;
	// �÷��̾� ��� ����
	bool playerDeath;

	char str[128];

private:
	RECT temp;

	vector <ItemInfo> vPlayerActiveItem;				// ��Ƽ�� ������ ����
	vector <ItemInfo>::iterator viPlayerActiveItem;		// ��Ƽ�� ������ ����
	vector <ItemInfo> vPlayerPassiveItem;				// �нú� ������ ����
	vector <ItemInfo>::iterator viPlayerPassiveItem;	// �нú� ������ ����
	vector <ItemInfo> vPlayerTrinkets;					// ��ű� ������ ����
	vector <ItemInfo>::iterator viPlayerTrinkets;		// ��ű� ������ ����

	vector <ItemInfo> vPlayerHeart;						// ü�� ������ ����
	vector <ItemInfo>::iterator viPlayerHeart;			// ü�� ������ ����

	vector <ItemInfo> vPlayerGold;						// ��� ������ ����
	vector <ItemInfo>::iterator viPlayerGold;			// ��� ������ ����
	//vector <ItemInfo> vPlayerBomb;						// ��ź ������ ����
	//vector <ItemInfo>::iterator viPlayerBomb;			// ��ź ������ ����
	vector <ItemInfo> vPlayerKey;						// ���� ������ ����
	vector <ItemInfo>::iterator viPlayerKey;			// ���� ������ ����

	vector <ItemInfo> vPlayerCard;						// ī�� ������ ����
	vector <ItemInfo>::iterator viPlayerCard;			// ī�� ������ ����
	vector <ItemInfo> vPlayerPill;						// �˾� ������ ����
	vector <ItemInfo>::iterator viPlayerPill;			// �˾� ������ ����

	vector <ItemInfo> vPlayerAllItem;						// ��� ������ ����
	vector <ItemInfo>::iterator viPlayerAllItem;			// ��� ������ ����

public:
	Player();
	~Player();

	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void PlayerDeath();			// �÷��̾� ���	
	void PlayerMove();			// �÷��̾� �̵�Ű �Լ�
	void PlayerSilde();			// �÷��̾� �̵��� �����̵� �Լ�
	void PlayerShot();			// �÷��̾� ����Ű �Լ�
	void PlayerShotMove();		// �÷��̾� ���ݽ� ���� �Լ�
	void PlayerAnimation();		// �÷��̾� �ִϸ��̼�
	void PlayerStatUpdate();	// �÷��̾� ���� ������Ʈ

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
