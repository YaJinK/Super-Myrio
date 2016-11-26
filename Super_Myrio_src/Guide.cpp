#include "Guide.h"

Guide::Guide(LPDIRECT3DDEVICE9 pD3DDevice, char * fileName, float srcWidth, float srcHeight, float x, float y) : GameResource(pD3DDevice, fileName, srcWidth, srcHeight, x, y)
{
	D3DXCreateFont(pD3DDevice, 18, 0, FW_BLACK, 0, true, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "", &m_GuideFont);
	m_CurrentFrame = 0;
	m_Src.left = LONG((m_CurrentFrame % 1) * m_SrcWidth);
	m_Src.top = LONG((m_CurrentFrame / 1) * m_SrcHeight);
	m_Src.right = LONG(m_Src.left + m_SrcWidth);
	m_Src.bottom = LONG(m_Src.top + m_SrcHeight);
}

Guide::~Guide()
{
	GameResource::Release();
	if (m_GuideFont != NULL)
	{
		m_GuideFont->Release();
		m_GuideFont = NULL;
	}
}

void Guide::DrawInfo()
{
	RECT rect;
	GetPosRect(&rect);
	rect.bottom -= 10;
	m_GuideFont->DrawTextA(NULL, "³Ç±¤", (int)strlen("³Ç±¤"), &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(173, 29, 22));
}

void Guide::UpdateData(char * pKey, float frictionFactor, float gravite, float dragX, float dragY)
{
}

void Guide::UpdateFrame(char * pKey)
{
}

void Guide::Release()
{
	GameResource::Release();
	if (m_GuideFont != NULL)
	{
		m_GuideFont->Release();
		m_GuideFont = NULL;
	}
}

