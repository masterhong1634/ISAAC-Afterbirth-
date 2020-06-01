#pragma once
//SHOP 초기좌표
#define INIT_X -1768
#define INIT_Y -1716

class Shop 
{
private:

	vector <ItemInfo> vShopActiveItem;				// 액티브 아이템 벡터
	vector <ItemInfo>::iterator viShopActiveItem;	// 액티브 아이템 벡터
	vector <ItemInfo> vShopPassiveItem;				// 패시브 아이템 벡터
	vector <ItemInfo>::iterator viShopPassiveItem;	// 패시브 아이템 벡터
	vector <ItemInfo> vShopTrinkets;				// 장신구 아이템 벡터
	vector <ItemInfo>::iterator viShopTrinkets;		// 장신구 아이템 벡터

	vector <ItemInfo> vShopHeart;					// 체력 아이템 벡터
	vector <ItemInfo>::iterator viShopHeart;		// 체력 아이템 벡터

	vector <ItemInfo> vShopGold;					// 골드 아이템 벡터
	vector <ItemInfo>::iterator viShopGold;			// 골드 아이템 벡터
	vector <ItemInfo> vShopBomb;					// 폭탄 아이템 벡터
	vector <ItemInfo>::iterator viShopBomb;			// 폭탄 아이템 벡터
	vector <ItemInfo> vShopKey;						// 열쇠 아이템 벡터
	vector <ItemInfo>::iterator viShopKey;			// 열쇠 아이템 벡터

	vector <ItemInfo> vShopCard;					// 카드 아이템 벡터
	vector <ItemInfo>::iterator viShopCard;			// 카드 아이템 벡터
	vector <ItemInfo> vShopPill;					// 알약 아이템 벡터
	vector <ItemInfo>::iterator viShopPill;			// 알약 아이템 벡터

	vector <ItemInfo> vShopAllItem;					// 모든 아이템 벡터
	vector <ItemInfo>::iterator viShopAllItem;		// 모든 아이템 벡터

	vector <ItemInfo> vGoldRoomAllItem;					// 모든 아이템 벡터
	vector <ItemInfo>::iterator viGoldRoomAllItem;		// 모든 아이템 벡터

	int sour, dest;

	RECT itemRect[3];
	RECT itemPriceRect[3];
	RECT goldRoomItemRect;

	RECT temp;

public:
	Shop();
	~Shop();
	
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);
	RECT GetShopItemRect(int x) { return itemRect[x]; }

	vector<ItemInfo> GetShopActiveItemInfo() { return vShopActiveItem; }
	vector<ItemInfo> GetShopPassiveItemInfo() { return vShopPassiveItem; }
	vector<ItemInfo> GetShopTrinketsInfo() { return vShopTrinkets; }
	vector<ItemInfo> GetShopHeartInfo() { return vShopHeart; }
	vector<ItemInfo> GetShopBombInfo() { return vShopBomb; }
	vector<ItemInfo> GetShopKeyInfo() { return vShopKey; }
	vector<ItemInfo> GetShopCardInfo() { return vShopCard; }
	vector<ItemInfo> GetShopPillInfo() { return vShopPill; }
	vector<ItemInfo> GetShopGoldInfo() { return vShopGold; }

};

