#include "FPSTools.h"

FPSTools::FPSTools(LPDIRECT3DDEVICE9 pDevice): FPSTools(pDevice, 20, D3DCOLOR_XRGB(0, 255, 0)) {}

FPSTools::FPSTools(LPDIRECT3DDEVICE9 pDevice, int height, D3DCOLOR color)
{
	if (NULL != pDevice)
	{
		D3DXCreateFont(pDevice, height, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "FPSFont", &m_font);
		m_color = color;
		m_frame = 0;
		m_FPS = 0;
		m_currentTime = 0;
		m_lastTime = 0;
	}
}

FPSTools::~FPSTools()
{
	if (NULL != m_font)
	{
		m_font->Release();
		m_font = NULL;
	}
}

LPD3DXFONT FPSTools::getFont() const
{
	return m_font;
}

void FPSTools::setFont(LPD3DXFONT font)
{
	if (NULL != font)
	{
		m_font = font;
	}
}

void FPSTools::setFont(LPDIRECT3DDEVICE9 pDevice, int height)
{
	if (NULL != pDevice)
	{
		D3DXCreateFont(pDevice, height, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "FPSFont", &m_font);
	}
}

D3DCOLOR FPSTools::getColor() const
{
	return m_color;
}

void FPSTools::setColor(D3DCOLOR color)
{
	m_color = color;
}

void FPSTools::updateFPS()
{
	m_frame++;
	m_currentTime = timeGetTime() * 0.001f;
	float distance = m_currentTime - m_lastTime;
	if (1 <= distance)
	{
		m_FPS = int(m_frame / distance);
		m_frame = 0;
		m_lastTime = m_currentTime;
	}
}

int FPSTools::getFPS() const
{
	return m_FPS;
}

void FPSTools::Release()
{
	if (NULL != m_font)
	{
		m_font->Release();
		m_font = NULL;
	}
}