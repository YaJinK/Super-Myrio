#pragma once
#include <d3dx9.h>

class Story
{
public:
	Story(LPDIRECT3DDEVICE9 pD3DDevice);
	~Story();
	void DrawBegin();
	void DrawEnding(short endingNum);
	short GetTimer() const;
	void SetTimer(short timer);
	D3DCOLOR GetBackColor() const;
	void Release();
private:
	char** m_Begin;
	char** m_Ending;
	LPD3DXFONT m_StoryFont;
	short m_Timer;
	RECT m_Area;
	D3DCOLOR m_BackColor;
};