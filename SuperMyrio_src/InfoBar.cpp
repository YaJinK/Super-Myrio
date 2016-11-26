#include "InfoBar.h"

InfoBar::InfoBar(LPDIRECT3DDEVICE9 pD3DDevice, char * fileName, float srcWidth, float srcHeight, float x, float y, short currentFrame) : GameResource(pD3DDevice, fileName, srcWidth, srcHeight, x, y)
{
	m_CurrentFrame = currentFrame;
	m_Src.left = LONG((m_CurrentFrame % 4) * m_SrcWidth);
	m_Src.top = LONG((m_CurrentFrame / 4) * m_SrcHeight);
	m_Src.right = LONG(m_Src.left + m_SrcWidth);
	m_Src.bottom = LONG(m_Src.top + m_SrcHeight);
	D3DXCreateFont(pD3DDevice, 30, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "", &m_pD3DXFont);
}

InfoBar::~InfoBar()
{
	GameResource::Release();
	if (m_pD3DXFont != NULL)
	{
		m_pD3DXFont->Release();
		m_pD3DXFont = NULL;
	}
}

void InfoBar::UpdateData(char * pKey, float frictionFactor, float gravite, float dragX, float dragY)
{
}

void InfoBar::UpdateFrame(char * pKey)
{
}

void InfoBar::DrawInfo(short time)
{
	char begin[10] = "Time ";
	char timeText[4];
	_itoa_s(int(time), timeText, 10);
	strcat_s(begin, timeText);
	RECT gotRect;
	gotRect.left = (LONG)m_X - 170;
	gotRect.top = (LONG)m_Y + 5;
	gotRect.right = gotRect.left + 200;
	gotRect.bottom = gotRect.top + 50;
	m_pD3DXFont->DrawText(NULL, begin, (int)strlen(begin), &gotRect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
}

void InfoBar::DrawInfo(short gotGM, short totalGM)
{
	char allText[6];
	_itoa_s(int(gotGM), allText, 10);
	char* division = "/";
	strcat_s(allText, division);
	char total[3];
	_itoa_s(int(totalGM), total, 10);
	strcat_s(allText, total);
	RECT gotRect;
	gotRect.left = (LONG)m_X + 50;
	gotRect.top = (LONG)m_Y + 5;
	gotRect.right = gotRect.left + 100;
	gotRect.bottom = gotRect.top + 50;
	m_pD3DXFont->DrawText(NULL, allText, (int)strlen(allText), &gotRect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
}

void InfoBar::Release()
{
	GameResource::Release();
	if (m_pD3DXFont != NULL)
	{
		m_pD3DXFont->Release();
		m_pD3DXFont = NULL;
	}
}
