#include "Mario.h"

Mario::Mario(LPDIRECT3DDEVICE9 pD3DDevice)
{
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile("img/mc/mario.png", &info);
	D3DXCreateTextureFromFileEx(pD3DDevice, "img/mc/mario.png", info.Width*2.5, info.Height*2.5, 1,
		D3DUSAGE_RENDERTARGET, info.Format, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
		D3DCOLOR_XRGB(0, 0, 0), &info, NULL, &m_Sprite);
	m_CurrentFrame = 0;
	m_SrcWidth = 50;
	m_SrcHeight = 50;
	m_Pos = { 400, 400 };
	m_MaxSpeed = 10;
	m_Speed = 0;
	m_Acce = 0;
	updateSrc();
}

Mario::~Mario()
{
	m_Sprite->Release();
	m_Sprite = NULL;
}

void Mario::updateSrc()
{
	m_Src.left = (m_CurrentFrame % 13) * m_SrcWidth;
	m_Src.top = (m_CurrentFrame / 13) * m_SrcHeight;
	m_Src.right = m_Src.left + m_SrcWidth;
	m_Src.bottom = m_Src.top + m_SrcHeight;
}

void Mario::go(char* pKey, LPD3DXSPRITE pD3DXSprite)
{
	if (pKey[DIK_A] & 0x80)
	{
		m_Acce = -2;
		if (m_Speed > 0)
		{
			if (m_Speed >= 10)
				m_CurrentFrame = 5;
			m_Speed += m_Acce * 2;
			m_Pos.x += m_Speed;
		}
		else
		{
			m_MaxSpeed = 10;
			if (5 == m_CurrentFrame || 8 == m_CurrentFrame || (0 <= m_CurrentFrame && 3 >= m_CurrentFrame))
				m_CurrentFrame = 11;
			if (pKey[DIK_J] & 0x80)
			{
				m_MaxSpeed = 25;
				m_Acce -= 2;
			}
			if (m_Speed > -m_MaxSpeed)
				m_Speed += m_Acce;
			m_Pos.x += m_Speed;
			m_CurrentFrame--;
		}
		
	}
	else if (pKey[DIK_D] & 0x80)
	{
		m_Acce = 2;
		if (m_Speed < 0)
		{
			if (m_Speed <= -10)
				m_CurrentFrame = 6;
			m_Speed += m_Acce * 2;
			m_Pos.x += m_Speed;
		}
		else
		{
			m_MaxSpeed = 10;
			if (3 == m_CurrentFrame || 6 == m_CurrentFrame || (11 >= m_CurrentFrame && 8 <= m_CurrentFrame))
				m_CurrentFrame = 0;
			if (pKey[DIK_J] & 0x80)
			{
				m_MaxSpeed = 25;
				m_Acce += 2;
			}
			if (m_Speed < m_MaxSpeed)
				m_Speed += m_Acce;
			m_Pos.x += m_Speed;
			m_CurrentFrame++;
		}
	}
	else
	{
		if (m_Speed > 0)
		{
			if (3 == m_CurrentFrame || 6 == m_CurrentFrame || (11 >= m_CurrentFrame && 8 <= m_CurrentFrame))
			{
				m_CurrentFrame = 0;
			}
			m_Acce = -2;
			m_Pos.x += m_Speed;
			m_Speed += m_Acce;
			m_CurrentFrame++;
		} else if (m_Speed < 0)
		{
			if (5 == m_CurrentFrame || 8 == m_CurrentFrame || (0 <= m_CurrentFrame && 3 >= m_CurrentFrame))
				m_CurrentFrame = 11;
			m_Acce = 2;
			m_Pos.x += m_Speed;
			m_Speed += m_Acce;
			m_CurrentFrame--;
		} else
		{
			if (m_Acce < 0)
				m_CurrentFrame = 0;
			else
				m_CurrentFrame = 11;
		}
	}

	updateSrc();
	//RECT rect = { m_Pos.x, m_Pos.y, m_Pos.x + m_SrcWidth, m_Pos.y + m_SrcHeight };
	
	D3DXVECTOR3 pos = { (float)m_Pos.x, (float)m_Pos.y, 0 };
	pD3DXSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pD3DXSprite->Draw(m_Sprite, &m_Src, NULL, &pos, D3DCOLOR_XRGB(255, 255, 255));
	pD3DXSprite->End();
}

void Mario::Release()
{
	if (NULL != m_Sprite)
	{
		m_Sprite->Release();
		m_Sprite = NULL;
	}
}