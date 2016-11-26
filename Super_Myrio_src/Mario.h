#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

class Mario
{
public:
	Mario(LPDIRECT3DDEVICE9 pD3DDevice);
	~Mario();
	void go(char* pKey, LPD3DXSPRITE pD3DXSprite);
	void Release();
private:
	void updateSrc();
	LPDIRECT3DTEXTURE9 m_Sprite;
	short m_CurrentFrame;
	RECT m_Src;
	short m_SrcWidth;
	short m_SrcHeight;
	POINT m_Pos;
	short m_MaxSpeed;
	short m_Speed;
	short m_Acce;
	//short m_InertiaCount;
};