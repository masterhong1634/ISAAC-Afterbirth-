#pragma once
#include "singletonBase.h"
// 
enum ItemKind
{
	ITEM_ACTIVE,
	ITEM_PASSIVE,
	ITEM_TRINKETS,
	ITEM_HEART,
	ITEM_GOLD,
	ITEM_BOMB,
	ITEM_KEY,
	ITEM_CARD,
	ITEM_PILL,
	ITEM_END
};
struct ItemInfo
{
	ItemKind	itemKind;				// 아이템 종류
	image*		itemImage;				// 아이템 이미지
	image*		itemInfoImage;			// 아이템 UI이미지
	RECT		itemRect;				// 아이템 생성위치
	RECT		itemPriceRect;			// 아이템 가격위치
	const char* itemName;				// 아이템 이름
	float		addPower;				// 플레이어 공격력
	float		addSpeed;				// 플레이어 속도 증감
	float		addShotSpeed;			// 샷 스피드(공격주기) 증감
	float		addShotRange;			// 샷 사정거리 증감
	int			addShotDelay;			// 샷 딜레이 증감

	float		addMaxHeart;
	float		addHeart;
	int			addGold;
	int			addBomb;
	int			addKey;

	int			price;					// 아이템 가격
};

class ItemManager : public singletonBase<ItemManager>
{
private:	
	
	vector <ItemInfo> vActiveItem;		// 액티브 아이템 벡터
	vector <ItemInfo> vPassiveItem;		// 패시브 아이템 벡터
	vector <ItemInfo> vTrinkets;		// 장신구 아이템 벡터

	vector <ItemInfo> vHeart;			// 체력 아이템 벡터

	vector <ItemInfo> vGold;			// 골드 아이템 벡터
	vector <ItemInfo> vBomb;			// 폭탄 아이템 벡터
	vector <ItemInfo> vKey;				// 열쇠 아이템 벡터

	vector <ItemInfo> vCard;			// 카드 아이템 벡터
	vector <ItemInfo> vPill;			// 알약 아이템 벡터

	vector <ItemInfo> vAllItem;			// 모든 아이템 벡터

	vector <ItemInfo> vTemp;			// 랜덤생성용 벡터
	int sour, dest;						// 랜덤생성용 변수

	ItemInfo heart;
	ItemInfo gold;
	ItemInfo bomb;
	ItemInfo key;

	bool isShop;
	bool isGoldRoom;

public:
	ItemManager();
	~ItemManager();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	// 상점에서 판매하는 아이템 벡터 전송용 함수 
	vector<ItemInfo> GetActiveItemInfo() { return vActiveItem; }
	vector<ItemInfo> GetPassiveItemInfo() { return vPassiveItem; }
	vector<ItemInfo> GetTrinketsInfo() { return vTrinkets; }
	vector<ItemInfo> GetHeartInfo() { return vHeart; }
	vector<ItemInfo> GetBombInfo() { return vBomb; }
	vector<ItemInfo> GetKeyInfo() { return vKey; }
	vector<ItemInfo> GetCardInfo() { return vCard; }
	vector<ItemInfo> GetPillInfo() { return vPill; }
	   				
	vector<ItemInfo> GetGoldInfo() { return vGold; }

	ItemInfo GetAllItemInfo(int x) { return vAllItem[x]; }
	vector<ItemInfo> GetAllItemInfo() { return vAllItem; }

	bool GetShop() {return isShop;}
	void SetShop(bool value) { isShop = value; }

	bool GetGoldRoom() { return isGoldRoom; }
	void SetGoldRoom(bool value) { isGoldRoom = value; }
};

