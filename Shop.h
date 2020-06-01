#pragma once
//SHOP �ʱ���ǥ
#define INIT_X -1768
#define INIT_Y -1716

class Shop 
{
private:

	vector <ItemInfo> vShopActiveItem;				// ��Ƽ�� ������ ����
	vector <ItemInfo>::iterator viShopActiveItem;	// ��Ƽ�� ������ ����
	vector <ItemInfo> vShopPassiveItem;				// �нú� ������ ����
	vector <ItemInfo>::iterator viShopPassiveItem;	// �нú� ������ ����
	vector <ItemInfo> vShopTrinkets;				// ��ű� ������ ����
	vector <ItemInfo>::iterator viShopTrinkets;		// ��ű� ������ ����

	vector <ItemInfo> vShopHeart;					// ü�� ������ ����
	vector <ItemInfo>::iterator viShopHeart;		// ü�� ������ ����

	vector <ItemInfo> vShopGold;					// ��� ������ ����
	vector <ItemInfo>::iterator viShopGold;			// ��� ������ ����
	vector <ItemInfo> vShopBomb;					// ��ź ������ ����
	vector <ItemInfo>::iterator viShopBomb;			// ��ź ������ ����
	vector <ItemInfo> vShopKey;						// ���� ������ ����
	vector <ItemInfo>::iterator viShopKey;			// ���� ������ ����

	vector <ItemInfo> vShopCard;					// ī�� ������ ����
	vector <ItemInfo>::iterator viShopCard;			// ī�� ������ ����
	vector <ItemInfo> vShopPill;					// �˾� ������ ����
	vector <ItemInfo>::iterator viShopPill;			// �˾� ������ ����

	vector <ItemInfo> vShopAllItem;					// ��� ������ ����
	vector <ItemInfo>::iterator viShopAllItem;		// ��� ������ ����

	vector <ItemInfo> vGoldRoomAllItem;					// ��� ������ ����
	vector <ItemInfo>::iterator viGoldRoomAllItem;		// ��� ������ ����

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

