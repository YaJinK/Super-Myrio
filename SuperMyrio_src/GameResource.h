#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

class GameResource
{
public:
	GameResource(LPDIRECT3DDEVICE9 pD3DDevice, char* fileName, float srcWidth, float srcHeight, float x, float y);
	~GameResource();
	virtual void Draw(LPD3DXSPRITE pD3DXSprite);
	virtual void Draw(LPD3DXSPRITE pD3DXSprite, float x, float y);
	virtual short GetCurrentFrame() const;
	virtual RECT* GetPosRect(RECT* rect) const;
	float GetX() const;
	void SetX(float x);
	float GetY() const;
	void SetY(float y);
	float GetSrcWidth() const;
	float GetSrcHeight() const;
	void SetCurrectFrame(short currentFrame);
	virtual void UpdateData(char* pKey, float frictionFactor, float gravite, float dragX, float dragY) = 0;
	virtual void UpdateFrame(char* pKey) = 0;
	virtual void Release();
protected:
	LPDIRECT3DTEXTURE9 m_Texture;
	short m_CurrentFrame;
	RECT m_Src;
	float m_SrcWidth;
	float m_SrcHeight;
	float m_X;
	float m_Y;
};

