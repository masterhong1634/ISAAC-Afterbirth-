#include "stdafx.h"
#include "ItemManager.h"


ItemManager::ItemManager()
{
}


ItemManager::~ItemManager()
{
}

HRESULT ItemManager::Init()
{
	isShop = true;
	isGoldRoom = true;

	// 액티브 아이템 초기화 및 벡터 푸쉬백
	ItemInfo activeItem_Dice;
	activeItem_Dice.itemKind = ITEM_ACTIVE;
	activeItem_Dice.itemImage = IMAGEMANAGER->addImage("ActiveItem_Dice", "images/item/ActiveItem_Dice.bmp", 50, 50, true, RGB(255, 0, 255));
	//activeItem_Dice.itemInfoImage = 
	activeItem_Dice.itemName = "ActiveItem_Dice";
	activeItem_Dice.itemRect = { 0, };
	activeItem_Dice.itemPriceRect = { 0, };
	activeItem_Dice.addPower = 5.0f;
	activeItem_Dice.addSpeed = 2.0f;
	activeItem_Dice.addShotSpeed = 0.0f;
	activeItem_Dice.addShotRange = 50.0f;
	activeItem_Dice.addShotDelay = 0;
	activeItem_Dice.addMaxHeart = 0.0f;
	activeItem_Dice.addHeart = 0.0f;
	activeItem_Dice.addGold = 0;
	activeItem_Dice.addBomb = 0;
	activeItem_Dice.addKey = 0;
	activeItem_Dice.price = 15;
	vActiveItem.push_back(activeItem_Dice);

	ItemInfo activeItem_GuppysPaw;
	activeItem_GuppysPaw.itemKind = ITEM_ACTIVE;
	activeItem_GuppysPaw.itemImage = IMAGEMANAGER->addImage("ActiveItem_GuppysPaw", "images/item/ActiveItem_GuppysPaw.bmp", 50, 50, true, RGB(255, 0, 255));
	//activeItem_GuppysPaw.itemInfoImage = 
	activeItem_GuppysPaw.itemName = "ActiveItem_GuppysPaw";
	activeItem_GuppysPaw.itemRect = { 0, };
	activeItem_GuppysPaw.itemPriceRect = { 0, };
	activeItem_GuppysPaw.addPower = 0.0f;
	activeItem_GuppysPaw.addSpeed = 2.0f;
	activeItem_GuppysPaw.addShotSpeed = 8.0f;
	activeItem_GuppysPaw.addShotRange = 150.0f;
	activeItem_GuppysPaw.addShotDelay = 0;
	activeItem_GuppysPaw.addMaxHeart = 0.0f;
	activeItem_GuppysPaw.addHeart = 0.0f;
	activeItem_GuppysPaw.addGold = 0;
	activeItem_GuppysPaw.addBomb = 0;
	activeItem_GuppysPaw.addKey = 0;
	activeItem_GuppysPaw.price = 15;
	vActiveItem.push_back(activeItem_GuppysPaw);

	// 패시브 아이템 초기화 및 벡터 푸쉬백
	ItemInfo passiveItem_Dessert;
	passiveItem_Dessert.itemKind = ITEM_PASSIVE;
	passiveItem_Dessert.itemImage = IMAGEMANAGER->addImage("PassiveItem_Dessert", "images/item/PassiveItem_Dessert.bmp", 50, 50, true, RGB(255, 0, 255));
	//passiveItem_Dessert.itemInfoImage = 
	passiveItem_Dessert.itemName = "PassiveItem_Dessert";
	passiveItem_Dessert.itemRect = { 0, };
	passiveItem_Dessert.itemPriceRect = { 0, };
	passiveItem_Dessert.addPower = 0.0f;
	passiveItem_Dessert.addSpeed = 0.0f;
	passiveItem_Dessert.addShotSpeed = 0.0f;
	passiveItem_Dessert.addShotRange = 0.0f;
	passiveItem_Dessert.addShotDelay = 0;
	passiveItem_Dessert.addMaxHeart = 1.0f;
	passiveItem_Dessert.addHeart = 1.0f;
	passiveItem_Dessert.addGold = 0;
	passiveItem_Dessert.addBomb = 0;
	passiveItem_Dessert.addKey = 0;
	passiveItem_Dessert.price = 10;
	vPassiveItem.push_back(passiveItem_Dessert);

	ItemInfo passiveItem_Momslipstick;
	passiveItem_Momslipstick.itemKind = ITEM_PASSIVE;
	passiveItem_Momslipstick.itemImage = IMAGEMANAGER->addImage("PassiveItem_Momslipstick", "images/item/PassiveItem_Momslipstick.bmp", 50, 50, true, RGB(255, 0, 255));
	//passiveItem_Momslipstick.itemInfoImage = 
	passiveItem_Momslipstick.itemName = "PassiveItem_Momslipstick";
	passiveItem_Momslipstick.itemRect = { 0, };
	passiveItem_Momslipstick.itemPriceRect = { 0, };
	passiveItem_Momslipstick.addPower = 0.0f;
	passiveItem_Momslipstick.addSpeed = 0.0f;
	passiveItem_Momslipstick.addShotSpeed = 0.0f;
	passiveItem_Momslipstick.addShotRange = 100.0f;
	passiveItem_Momslipstick.addShotDelay = 0;
	passiveItem_Momslipstick.addMaxHeart = 0.0f;
	passiveItem_Momslipstick.addHeart = 0.0f;
	passiveItem_Momslipstick.addGold = 0;
	passiveItem_Momslipstick.addBomb = 0;
	passiveItem_Momslipstick.addKey = 0;
	passiveItem_Momslipstick.price = 10;
	vPassiveItem.push_back(passiveItem_Momslipstick);

	ItemInfo passiveItem_Dollar;
	passiveItem_Dollar.itemKind = ITEM_PASSIVE;
	passiveItem_Dollar.itemImage = IMAGEMANAGER->addImage("PassiveItem_Dollar", "images/item/PassiveItem_Dollar.bmp", 50, 50, true, RGB(255, 0, 255));
	//passiveItem_Dollar.itemInfoImage = 
	passiveItem_Dollar.itemName = "PassiveItem_Dollar";
	passiveItem_Dollar.itemRect = { 0, };
	passiveItem_Dollar.itemPriceRect = { 0, };
	passiveItem_Dollar.addPower = 0.0f;
	passiveItem_Dollar.addSpeed = 0.0f;
	passiveItem_Dollar.addShotSpeed = 0.0f;
	passiveItem_Dollar.addShotRange = 0.0f;
	passiveItem_Dollar.addShotDelay = 0;
	passiveItem_Dollar.addMaxHeart = 0.0f;
	passiveItem_Dollar.addHeart = 0.0f;
	passiveItem_Dollar.addGold = 99;
	passiveItem_Dollar.addBomb = 0;
	passiveItem_Dollar.addKey = 0;
	passiveItem_Dollar.price = 10;
	vPassiveItem.push_back(passiveItem_Dollar);

	ItemInfo passiveItem_Pyro;
	passiveItem_Pyro.itemKind = ITEM_PASSIVE;
	passiveItem_Pyro.itemImage = IMAGEMANAGER->addImage("PassiveItem_Pyro", "images/item/PassiveItem_Pyro.bmp", 50, 50, true, RGB(255, 0, 255));
	//passiveItem_Pyro.itemInfoImage = 
	passiveItem_Pyro.itemName = "PassiveItem_Pyro";
	passiveItem_Pyro.itemRect = { 0, };
	passiveItem_Pyro.itemPriceRect = { 0, };
	passiveItem_Pyro.addPower = 0.0f;
	passiveItem_Pyro.addSpeed = 0.0f;
	passiveItem_Pyro.addShotSpeed = 0.0f;
	passiveItem_Pyro.addShotRange = 0.0f;
	passiveItem_Pyro.addShotDelay = 0;
	passiveItem_Pyro.addMaxHeart = 0.0f;
	passiveItem_Pyro.addHeart = 0.0f;
	passiveItem_Pyro.addGold = 0;
	passiveItem_Pyro.addBomb = 99;
	passiveItem_Pyro.addKey = 0;
	passiveItem_Pyro.price = 10;
	vPassiveItem.push_back(passiveItem_Pyro);

	// 장신구 아이템 초기화 및 벡터 푸쉬백
	ItemInfo trinkets_CurvedHorn;
	trinkets_CurvedHorn.itemKind = ITEM_TRINKETS;
	trinkets_CurvedHorn.itemImage = IMAGEMANAGER->addImage("Trinkets_CurvedHorn", "images/item/Trinkets_CurvedHorn.bmp", 50, 50, true, RGB(255, 0, 255));
	//trinkets_CurvedHorn.itemInfoImage = 
	trinkets_CurvedHorn.itemName = "Trinkets_CurvedHorn";
	trinkets_CurvedHorn.itemRect = { 0, };
	trinkets_CurvedHorn.itemPriceRect = { 0, };
	trinkets_CurvedHorn.addPower = 10.0f;
	trinkets_CurvedHorn.addSpeed = 0.0f;
	trinkets_CurvedHorn.addShotSpeed = 0.0f;
	trinkets_CurvedHorn.addShotRange = 0.0f;
	trinkets_CurvedHorn.addShotDelay = 0;
	trinkets_CurvedHorn.addMaxHeart = 0.0f;
	trinkets_CurvedHorn.addHeart = 0.0f;
	trinkets_CurvedHorn.addGold = 0;
	trinkets_CurvedHorn.addBomb = 0;
	trinkets_CurvedHorn.addKey = 0;
	trinkets_CurvedHorn.price = 5;
	vTrinkets.push_back(trinkets_CurvedHorn);

	ItemInfo trinkets_GoatHoof;
	trinkets_GoatHoof.itemKind = ITEM_TRINKETS;
	trinkets_GoatHoof.itemImage = IMAGEMANAGER->addImage("Trinkets_GoatHoof", "images/item/Trinkets_GoatHoof.bmp", 50, 50, true, RGB(255, 0, 255));
	//trinkets_GoatHoof.itemInfoImage = 
	trinkets_GoatHoof.itemName = "Trinkets_GoatHoof";
	trinkets_GoatHoof.itemRect = { 0, };
	trinkets_GoatHoof.itemPriceRect = { 0, };
	trinkets_GoatHoof.addPower = 0.0f;
	trinkets_GoatHoof.addSpeed = 1.0f;
	trinkets_GoatHoof.addShotSpeed = 0.0f;
	trinkets_GoatHoof.addShotRange = 0.0f;
	trinkets_GoatHoof.addShotDelay = 0;
	trinkets_GoatHoof.addMaxHeart = 0.0f;
	trinkets_GoatHoof.addHeart = 0.0f;
	trinkets_GoatHoof.addGold = 0;
	trinkets_GoatHoof.addBomb = 0;
	trinkets_GoatHoof.addKey = 0;
	trinkets_GoatHoof.price = 5;
	vTrinkets.push_back(trinkets_GoatHoof);

	ItemInfo trinkets_IsaacsFork;
	trinkets_IsaacsFork.itemKind = ITEM_TRINKETS;
	trinkets_IsaacsFork.itemImage = IMAGEMANAGER->addImage("Trinkets_IsaacsFork", "images/item/Trinkets_IsaacsFork.bmp", 50, 50, true, RGB(255, 0, 255));
	trinkets_IsaacsFork.itemName = "Trinkets_IsaacsFork";
	trinkets_IsaacsFork.itemRect = { 0, };
	trinkets_IsaacsFork.itemPriceRect = { 0, };
	//trinkets_IsaacsFork.itemInfoImage = 
	trinkets_IsaacsFork.addPower = 0.0f;
	trinkets_IsaacsFork.addSpeed = 0.0f;
	trinkets_IsaacsFork.addShotSpeed = 4.0f;
	trinkets_IsaacsFork.addShotRange = 0.0f;
	trinkets_IsaacsFork.addShotDelay = 0;
	trinkets_IsaacsFork.addMaxHeart = 0.0f;
	trinkets_IsaacsFork.addHeart = 0.0f;
	trinkets_IsaacsFork.addGold = 0;
	trinkets_IsaacsFork.addBomb = 0;
	trinkets_IsaacsFork.addKey = 0;
	trinkets_IsaacsFork.price = 5;
	vTrinkets.push_back(trinkets_IsaacsFork);

	ItemInfo trinkets_MatchStick;
	trinkets_MatchStick.itemKind = ITEM_TRINKETS;
	trinkets_MatchStick.itemImage = IMAGEMANAGER->addImage("Trinkets_MatchStick", "images/item/Trinkets_MatchStick.bmp", 50, 50, true, RGB(255, 0, 255));
	//trinkets_MatchStick.itemInfoImage = 
	trinkets_MatchStick.itemName = "Trinkets_MatchStick";
	trinkets_MatchStick.itemRect = { 0, };
	trinkets_MatchStick.itemPriceRect = { 0, };
	trinkets_MatchStick.addPower = 10.0f;
	trinkets_MatchStick.addSpeed = 0.0f;
	trinkets_MatchStick.addShotSpeed = 0.0f;
	trinkets_MatchStick.addShotRange = -100.0f;
	trinkets_MatchStick.addShotDelay = 0;
	trinkets_MatchStick.addMaxHeart = 0.0f;
	trinkets_MatchStick.addHeart = 0.0f;
	trinkets_MatchStick.addGold = 0;
	trinkets_MatchStick.addBomb = 0;
	trinkets_MatchStick.addKey = 0;
	trinkets_MatchStick.price = 5;
	vTrinkets.push_back(trinkets_MatchStick);

	heart.itemKind = ITEM_HEART;
	//heart.itemImage = IMAGEMANAGER->addImage("Trinkets_MatchStick", "images/item/Trinkets_MatchStick.bmp", 50, 50, true, RGB(255, 0, 255));
	//heart.itemName = "Trinkets_MatchStick";
	//heart.itemInfoImage = 
	heart.addPower = 0.0f;
	heart.addSpeed = 0.0f;
	heart.addShotSpeed = 0.0f;
	heart.addShotRange = 0.0f;
	heart.addShotDelay = 0;
	heart.addMaxHeart = 0.0f;
	heart.addHeart = 0.5f;
	heart.addGold = 0;
	heart.addBomb = 0;
	heart.addKey = 0;
	heart.price = 5;
	vHeart.push_back(heart);

	gold.itemKind = ITEM_GOLD;
	//gold.itemImage = IMAGEMANAGER->addImage("Trinkets_MatchStick", "images/item/Trinkets_MatchStick.bmp", 50, 50, true, RGB(255, 0, 255));
	//gold.itemName = "Trinkets_MatchStick";
	//gold.itemInfoImage = 
	gold.addPower = 0.0f;
	gold.addSpeed = 0.0f;
	gold.addShotSpeed = 0.0f;
	gold.addShotRange = 0.0f;
	gold.addShotDelay = 0;
	gold.addMaxHeart = 0.0f;
	gold.addHeart = 0.5f;
	gold.addGold = 1;
	gold.addBomb = 0;
	gold.addKey = 0;
	gold.price = 0;
	vGold.push_back(gold);

	bomb.itemKind = ITEM_BOMB;
	//bomb.itemImage = IMAGEMANAGER->addImage("Trinkets_MatchStick", "images/item/Trinkets_MatchStick.bmp", 50, 50, true, RGB(255, 0, 255));
	//bomb.itemName = "Trinkets_MatchStick";
	//bomb.itemInfoImage = 
	bomb.addPower = 0.0f;
	bomb.addSpeed = 0.0f;
	bomb.addShotSpeed = 0.0f;
	bomb.addShotRange = 0.0f;
	bomb.addShotDelay = 0;
	bomb.addMaxHeart = 0.0f;
	bomb.addHeart = 0.5f;
	bomb.addGold = 1;
	bomb.addBomb = 0;
	bomb.addKey = 0;
	bomb.price = 0;
	vBomb.push_back(bomb);

	key.itemKind = ITEM_KEY;
	//key.itemImage = IMAGEMANAGER->addImage("Trinkets_MatchStick", "images/item/Trinkets_MatchStick.bmp", 50, 50, true, RGB(255, 0, 255));
	//key.itemName = "Trinkets_MatchStick";
	//key.itemInfoImage = 
	key.addPower = 0.0f;
	key.addSpeed = 0.0f;
	key.addShotSpeed = 0.0f;
	key.addShotRange = 0.0f;
	key.addShotDelay = 0;
	key.addMaxHeart = 0.0f;
	key.addHeart = 0.5f;
	key.addGold = 0;
	key.addBomb = 0;
	key.addKey = 1;
	key.price = 0;
	vKey.push_back(key);

	ItemInfo card;
	card.itemKind = ITEM_KEY;
	//card.itemImage = IMAGEMANAGER->addImage("Trinkets_MatchStick", "images/item/Trinkets_MatchStick.bmp", 50, 50, true, RGB(255, 0, 255));
	//card.itemName = "Trinkets_MatchStick";
	//card.itemInfoImage = 
	card.addPower = 0.0f;
	card.addSpeed = 0.0f;
	card.addShotSpeed = 0.0f;
	card.addShotRange = 0.0f;
	card.addShotDelay = 0;
	card.addMaxHeart = 0.0f;
	card.addHeart = 0.5f;
	card.addGold = 0;
	card.addBomb = 0;
	card.addKey = 0;
	card.price = 0;
	vCard.push_back(card);

	ItemInfo pill;
	pill.itemKind = ITEM_KEY;
	//pill.itemImage = IMAGEMANAGER->addImage("Trinkets_MatchStick", "images/item/Trinkets_MatchStick.bmp", 50, 50, true, RGB(255, 0, 255));
	//pill.itemName = "Trinkets_MatchStick";
	//pill.itemInfoImage = 
	pill.addPower = 0.0f;
	pill.addSpeed = 0.0f;
	pill.addShotSpeed = 0.0f;
	pill.addShotRange = 0.0f;
	pill.addShotDelay = 0;
	pill.addMaxHeart = 0.0f;
	pill.addHeart = 0.5f;
	pill.addGold = 0;
	pill.addBomb = 0;
	pill.addKey = 0;
	pill.price = 0;
	vPill.push_back(pill);

	// 모든아이템에 추가
	vAllItem.push_back(activeItem_Dice);
	vAllItem.push_back(activeItem_GuppysPaw);
	vAllItem.push_back(passiveItem_Dessert);
	vAllItem.push_back(passiveItem_Momslipstick);
	vAllItem.push_back(passiveItem_Dollar);
	vAllItem.push_back(passiveItem_Pyro);
	vAllItem.push_back(trinkets_CurvedHorn);
	vAllItem.push_back(trinkets_GoatHoof);
	vAllItem.push_back(trinkets_IsaacsFork);
	vAllItem.push_back(trinkets_MatchStick);

	//vAllItem.push_back(heart);
	//vAllItem.push_back(gold);
	//vAllItem.push_back(bomb);
	//vAllItem.push_back(key);
	//vAllItem.push_back(card);
	//vAllItem.push_back(pill);

	for (int i = 1; i < 100; i++)
	{
		dest = RND->getInt(10);
		sour = RND->getInt(10);

		vTemp.insert(vTemp.begin(), vAllItem[dest]);
		vAllItem.insert(vAllItem.begin() + dest, vAllItem[sour]);
		vAllItem.erase(vAllItem.begin() + dest + 1);
		vAllItem.insert(vAllItem.begin() + sour, vTemp[0]);
		vAllItem.erase(vAllItem.begin() + sour + 1);
		vTemp.clear();
	}

	return S_OK;
}

void ItemManager::Release()
{
}

void ItemManager::Update()
{
}

void ItemManager::Render()
{
}
