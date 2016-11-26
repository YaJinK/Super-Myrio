#pragma once

#include <d3dx9.h>

#pragma comment (lib, "winmm.lib")

class FPSTools
{
public:
	FPSTools(LPDIRECT3DDEVICE9 pDevice);
	FPSTools(LPDIRECT3DDEVICE9 pDevice, int height, D3DCOLOR color);
	LPD3DXFONT getFont() const;
	void setFont(LPD3DXFONT font);
	void setFont(LPDIRECT3DDEVICE9 pDevice, int height);
	D3DCOLOR getColor() const;
	void setColor(D3DCOLOR color);
	void updateFPS();
	int getFPS() const;
	void Release();
	~FPSTools();
private:
	LPD3DXFONT m_font;
	D3DCOLOR m_color;
	int m_frame;
	int m_FPS;
	float m_lastTime;
	float m_currentTime;
};