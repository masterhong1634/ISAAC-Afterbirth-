#pragma once
#include "gameNode.h"
#include "TitleScene.h"
#include "CharacterSelect.h"
#include "MainMenu.h"
#include "GameScene.h"
#include "MapToolScene.h"

class mainGame :  public gameNode
{
private:

public:
	mainGame();
	~mainGame();

	virtual HRESULT init();	//�ʱ�ȭ
	virtual void release();//����
	virtual void update();//�����ϴ°�
	virtual void render(/*HDC hdc*/);//�׸��°�
};