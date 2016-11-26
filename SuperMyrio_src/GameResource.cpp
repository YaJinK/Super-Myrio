#include "GameResource.h"

GameResource::GameResource(LPDIRECT3DDEVICE9 pD3DDevice, char* fileName, float srcWidth, float srcHeight, float x, float y)
{
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(fileName, &info);
	D3DXCreateTextureFromFileEx(pD3DDevice, fileName, info.Width * 2, info.Height * 2, 1,
		D3DUSAGE_RENDERTARGET, info.Format, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		NULL, &info, NULL, &m_Texture);
	m_SrcHeight = srcHeight;
	m_SrcWidth = srcWidth;
	m_CurrentFrame = 1;
	m_X = x;
	m_Y = y;
}

GameResource::~GameResource()
{
	if (NULL != m_Texture)
	{
		m_Texture->Release();
		m_Texture = NULL;
	}
}

void GameResource::Draw(LPD3DXSPRITE pD3DXSprite)
{
	Draw(pD3DXSprite, m_X, m_Y);
}

void GameResource::Draw(LPD3DXSPRITE pD3DXSprite, float x, float y)
{
	D3DXVECTOR3 pos = { (float)x, (float)y, 0 };
	pD3DXSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pD3DXSprite->Draw(m_Texture, &m_Src, NULL, &pos, D3DCOLOR_XRGB(255, 255, 255));
	pD3DXSprite->End();
}

short GameResource::GetCurrentFrame() const
{
	return m_CurrentFrame;
}

RECT * GameResource::GetPosRect(RECT * rect) const
{
	rect->left = (LONG)m_X + 2;
	rect->top = (LONG)m_Y;
	rect->right = LONG(m_X + m_SrcWidth) - 2;
	rect->bottom = LONG(m_Y + m_SrcHeight);
	return rect;
}

float GameResource::GetX() const
{
	return m_X;
}

void GameResource::SetX(float x)
{
	if (x != -1)
		m_X = x;
}

float GameResource::GetY() const
{
	return m_Y;
}

void GameResource::SetY(float y)
{
	if (y != -1)
		m_Y = y;
}

float GameResource::GetSrcWidth() const
{
	return m_SrcWidth;
}

float GameResource::GetSrcHeight() const
{
	return m_SrcHeight;
}

void GameResource::SetCurrectFrame(short currentFrame)
{
	m_CurrentFrame = currentFrame;
}

void GameResource::Release()
{
	if (NULL != m_Texture)
	{
		m_Texture->Release();
		m_Texture = NULL;
	}
}