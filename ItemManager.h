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
	ItemKind	itemKind;				// ������ ����
	image*		itemImage;				// ������ �̹���
	image*		itemInfoImage;			// ������ UI�̹���
	RECT		itemRect;				// ������ ������ġ
	RECT		itemPriceRect;			// ������ ������ġ
	const char* itemName;				// ������ �̸�
	float		addPower;				// �÷��̾� ���ݷ�
	float		addSpeed;				// �÷��̾� �ӵ� ����
	float		addShotSpeed;			// �� ���ǵ�(�����ֱ�) ����
	float		addShotRange;			// �� �����Ÿ� ����
	int			addShotDelay;			// �� ������ ����

	float		addMaxHeart;
	float		addHeart;
	int			addGold;
	int			addBomb;
	int			addKey;

	int			price;					// ������ ����
};

class ItemManager : public singletonBase<ItemManager>
{
private:	
	
	vector <ItemInfo> vActiveItem;		// ��Ƽ�� ������ ����
	vector <ItemInfo> vPassiveItem;		// �нú� ������ ����
	vector <ItemInfo> vTrinkets;		// ��ű� ������ ����

	vector <ItemInfo> vHeart;			// ü�� ������ ����

	vector <ItemInfo> vGold;			// ��� ������ ����
	vector <ItemInfo> vBomb;			// ��ź ������ ����
	vector <ItemInfo> vKey;				// ���� ������ ����

	vector <ItemInfo> vCard;			// ī�� ������ ����
	vector <ItemInfo> vPill;			// �˾� ������ ����

	vector <ItemInfo> vAllItem;			// ��� ������ ����

	vector <ItemInfo> vTemp;			// ���������� ����
	int sour, dest;						// ���������� ����

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

	// �������� �Ǹ��ϴ� ������ ���� ���ۿ� �Լ� 
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

