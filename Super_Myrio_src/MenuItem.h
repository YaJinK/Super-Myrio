#pragma once

#include <d3dx9.h>
#include <dinput.h>
#include <dsound.h>
#include <fstream>
using std::ifstream;
using std::ios;

class MenuItem
{
public:
	MenuItem(LPDIRECT3DDEVICE9 pD3DDevice, char* data, char* id);
	~MenuItem();
	void AddItem(MenuItem* item);
	void Draw();
	char* GetData() const;
	char* GetItemId() const;
	MenuItem* Update(char* pKey);
	void Release();
private:
	char* m_Data;
	MenuItem* m_Parent;
	MenuItem* m_SubItems[10];
	short m_SubNum;
	char* m_ItemID;
	LPD3DXFONT m_Font;
	short m_CurrentItem;
	short m_KeyTimer;
};