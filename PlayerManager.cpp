#include "stdafx.h"
#include "PlayerManager.h"

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
}

HRESULT PlayerManager::Init()
{
	m_Player = new Player;
	m_Player->Init();
	return S_OK;
}

void PlayerManager::Release()
{
	SAFE_DELETE(m_Player);
}

void PlayerManager::Update()
{
	m_Player->Update();
}

void PlayerManager::Render(HDC hdc)
{
	m_Player->Render(hdc);
}
