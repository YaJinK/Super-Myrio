#pragma once
#include "GameResource.h"

class Guide : public GameResource
{
public:
	Guide(LPDIRECT3DDEVICE9 pD3DDevice, char* fileName, float srcWidth, float srcHeight, float x, float y);
	~Guide();
	void DrawInfo();
	void UpdateData(char* pKey, float frictionFactor, float gravite, float dragX, float dragY);
	void UpdateFrame(char* pKey);
	void Release();
private:
	LPD3DXFONT m_GuideFont;
};