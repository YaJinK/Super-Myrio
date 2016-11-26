#pragma once

#include "GameResource.h"

class InfoBar : public GameResource
{
public:
	InfoBar(LPDIRECT3DDEVICE9 pD3DDevice, char* fileName, float srcWidth, float srcHeight, float x, float y, short currentFrame);
	~InfoBar();
	virtual void UpdateData(char* pKey, float frictionFactor, float gravite, float dragX, float dragY);
	virtual void UpdateFrame(char* pKey);
	void DrawInfo(short time);
	void DrawInfo(short gotGM, short totalGM);
	void Release();
private:
	LPD3DXFONT m_pD3DXFont;
};