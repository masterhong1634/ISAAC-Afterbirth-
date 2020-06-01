#include "stdafx.h"
#include "Shop.h"
#include<time.h>

Shop::Shop()
{
}


Shop::~Shop()
{
}

HRESULT Shop::Init()
{

	IMAGEMANAGER->addImage("price5", "images/item/price5.bmp", 50, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("price10", "images/item/price10.bmp", 50, 32, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("price15", "images/item/price15.bmp", 50, 32, true, RGB(255, 0, 255));
	vShopActiveItem = ITEMMANAGER->GetActiveItemInfo();
	vShopPassiveItem = ITEMMANAGER->GetPassiveItemInfo();
	vShopTrinkets = ITEMMANAGER->GetTrinketsInfo();

	vShopGold = ITEMMANAGER->GetGoldInfo();
	vShopHeart = ITEMMANAGER->GetHeartInfo();

	vShopBomb = ITEMMANAGER->GetBombInfo();
	vShopKey = ITEMMANAGER->GetKeyInfo();

	vShopCard = ITEMMANAGER->GetCardInfo();
	vShopPill = ITEMMANAGER->GetPillInfo();

	for (int i = 0; i < 3; i++)
	{
		vShopAllItem.push_back(ITEMMANAGER->GetAllItemInfo(i));
	}

	for (int i = 0; i < 3; i++)
	{
		itemRect[i] = { 315 + i * 105, 270, 363 + i * 105, 318 };
		vShopAllItem[i].itemRect = itemRect[i];
		itemPriceRect[i] = { 315 + i * 105, 360, 363 + i * 105, 390 };
		vShopAllItem[i].itemPriceRect = itemPriceRect[i];
	}

	vGoldRoomAllItem.push_back(ITEMMANAGER->GetAllItemInfo(4));
	goldRoomItemRect = { 415, 215, 463, 263 };

	return S_OK;
}

void Shop::Release()
{
}

void Shop::Update()
{
	if (!ITEMMANAGER->GetShop())
	{
		for (int i = 0; i < vShopAllItem.size(); i++)
		{
			if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &vShopAllItem[i].itemRect))
			{
				if (PLAYERMANAGER->GetPlayerGold() >= vShopAllItem[i].price)
				{
					PLAYERMANAGER->SetPlayerAllItem(vShopAllItem[i]);
					PLAYERMANAGER->SetPlayerOffensePower(vShopAllItem[i].addPower);
					PLAYERMANAGER->SetPlayerShotSpeed(vShopAllItem[i].addShotSpeed);
					PLAYERMANAGER->SetPlayerShotRange(vShopAllItem[i].addShotRange);
					PLAYERMANAGER->SetPlayerSpeed(vShopAllItem[i].addSpeed);
					PLAYERMANAGER->SetPlayerMaxHp(vShopAllItem[i].addMaxHeart);
					PLAYERMANAGER->SetPlayerHp(vShopAllItem[i].addHeart);
					PLAYERMANAGER->SetPlayerGold(-vShopAllItem[i].price);
					PLAYERMANAGER->SetPlayerGold(vShopAllItem[i].addGold);
					PLAYERMANAGER->SetPlayerBomb(vShopAllItem[i].addBomb);
					PLAYERMANAGER->SetPlayerKey(vShopAllItem[i].addKey);
					vShopAllItem.erase(vShopAllItem.begin() + i);
					break;
				}
				break;
			}
		}
	}

	if (!ITEMMANAGER->GetGoldRoom())
	{
		if (vGoldRoomAllItem.size() > 0)
		{
			if (IntersectRect(&temp, &PLAYERMANAGER->GetPlayerHitRect(), &goldRoomItemRect))
			{
				PLAYERMANAGER->SetPlayerAllItem(vGoldRoomAllItem[0]);
				PLAYERMANAGER->SetPlayerOffensePower(vGoldRoomAllItem[0].addPower);
				PLAYERMANAGER->SetPlayerShotSpeed(vGoldRoomAllItem[0].addShotSpeed);
				PLAYERMANAGER->SetPlayerShotRange(vGoldRoomAllItem[0].addShotRange);
				PLAYERMANAGER->SetPlayerSpeed(vGoldRoomAllItem[0].addSpeed);
				PLAYERMANAGER->SetPlayerMaxHp(vGoldRoomAllItem[0].addMaxHeart);
				PLAYERMANAGER->SetPlayerHp(vGoldRoomAllItem[0].addHeart);
				PLAYERMANAGER->SetPlayerGold(vGoldRoomAllItem[0].addGold);
				PLAYERMANAGER->SetPlayerBomb(vGoldRoomAllItem[0].addBomb);
				PLAYERMANAGER->SetPlayerKey(vGoldRoomAllItem[0].addKey);
				vGoldRoomAllItem.erase(vGoldRoomAllItem.begin());
			}
		}
	}
}

void Shop::Render(HDC hdc)
{
	if (!ITEMMANAGER->GetShop())
	{
		if (vShopAllItem.size() > 0)
		{
			for (int i = 0; i < vShopAllItem.size(); i++)
			{
				vShopAllItem[i].itemImage->render(hdc, vShopAllItem[i].itemRect.left, vShopAllItem[i].itemRect.top);

				if (vShopAllItem[i].price == 5)
				{
					IMAGEMANAGER->render("price5", hdc, vShopAllItem[i].itemPriceRect.left, vShopAllItem[i].itemPriceRect.top);
				}
				if (vShopAllItem[i].price == 10)
				{
					IMAGEMANAGER->render("price10", hdc, vShopAllItem[i].itemPriceRect.left, vShopAllItem[i].itemPriceRect.top);
				}
				if (vShopAllItem[i].price == 15)
				{
					IMAGEMANAGER->render("price15", hdc, vShopAllItem[i].itemPriceRect.left, vShopAllItem[i].itemPriceRect.top);
				}

				if (KEYMANAGER->isToggleKey(VK_TAB))
				{
					if (KEYMANAGER->isToggleKey(VK_TAB))
					{
						Rectangle(hdc, itemRect[i].left, itemRect[i].top, itemRect[i].right, itemRect[i].bottom);
					}
				}
			}
		}

	}

	if (!ITEMMANAGER->GetGoldRoom())
	{
		if (vGoldRoomAllItem.size() > 0)
		{
			vGoldRoomAllItem[0].itemImage->render(hdc, goldRoomItemRect.left, goldRoomItemRect.top);
		}
	}
}
