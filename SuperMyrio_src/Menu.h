#pragma once

#include "MenuItem.h"

class Menu
{
public:
	Menu(LPDIRECT3DDEVICE9 pD3DDevice, LPDIRECTSOUND8 pDirectSound, MenuItem* item, char* fileName);
	~Menu();
	bool GetShow() const;
	MenuItem* GetItem() const;
	void SetItem(MenuItem* item);
	void SetShow(bool show);
	void Init();
	void Draw(LPD3DXSPRITE pD3DXSprite);
	void Release();
private:
	MenuItem* m_Item;
	MenuItem* m_OriginalItem;
	bool m_IsShow;
	LPDIRECT3DTEXTURE9 m_Background;
	LPDIRECTSOUNDBUFFER m_BackSound;
};

