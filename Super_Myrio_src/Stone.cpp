#include "Stone.h"

Stone::Stone(LPDIRECT3DDEVICE9 pD3DDevice, char* fileName, float srcWidth, float srcHeight, float x, float y, short currentFrame) : GameResource(pD3DDevice, fileName, srcWidth, srcHeight, x, y)
{
	m_DragX = 1;
	m_DragY = 3;
	m_CurrentFrame = currentFrame;
	m_OldY = y;
	m_OldX = x;
	m_XPass = 5;
	m_Status = STONE_NORMAL;
	m_FrameAdd = 1;
	if (18 <= m_CurrentFrame)
	{
		srand(timeGetTime());
		int frame = rand() % 7;
		if (frame == 0)
			frame = 4;
		m_Reward = new Reward(pD3DDevice, "img/reward.png", 36, 35, m_X - 1, m_OldY - 30, frame);
	}
	else
		m_Reward = NULL;
}

void Stone::UpdateData(char * pKey, float frictionFactor, float gravite, float dragX, float dragY)
{
	if (m_Y > m_OldY)
		m_Y = m_OldY;
	if (m_Y < m_OldY)
		m_Y += 5;
	if (m_Status == STONE_DESDORYING)
	{
		m_XPass += 5;
		if (m_Y == m_OldY)
			m_Status = STONE_DESDORYED;
	}
}

void Stone::UpdateFrame(char * pKey)
{
	if (18 <= m_CurrentFrame && m_CurrentFrame <= 23)
	{
		if (18 == m_CurrentFrame || 21 == m_CurrentFrame)
			m_FrameAdd = 1;
		if (20 == m_CurrentFrame || 23 == m_CurrentFrame)
			m_FrameAdd = -1;
		m_CurrentFrame += m_FrameAdd;
	}
	
	m_Src.left = LONG((m_CurrentFrame % 3) * m_SrcWidth);
	m_Src.top = LONG((m_CurrentFrame / 3) * m_SrcHeight);
	m_Src.right = LONG(m_Src.left + m_SrcWidth);
	m_Src.bottom = LONG(m_Src.top + m_SrcHeight);
}

void Stone::Destory()
{
	m_SrcWidth = 16;
	m_SrcHeight = 16;
}

float Stone::GetDragY() const
{
	return m_DragY;
}

float Stone::GetDragX() const
{
	return m_DragX;
}

float Stone::GetXPass() const
{
	return m_XPass;
}

Reward * Stone::GetReward() const
{
	return m_Reward;
}

void Stone::Action(ROLE_STATUS roleStatus)
{
	if ((0 <= m_CurrentFrame && m_CurrentFrame <= 11) || (15 <= m_CurrentFrame && m_CurrentFrame <= 17))
	{
		if (roleStatus == ROLE_NORMAL)
		{
			if (!(15 <= m_CurrentFrame && m_CurrentFrame <= 17) && !(3 <= m_CurrentFrame && m_CurrentFrame <= 8))
				m_Y = m_Y - 25;
		}
		else
		{
			Destory();
			m_Src.left = LONG((m_CurrentFrame % 3) * m_SrcWidth);
			m_Src.top = LONG((m_CurrentFrame / 3) * m_SrcHeight);
			m_Src.right = LONG(m_Src.left + m_SrcWidth);
			m_Src.bottom = LONG(m_Src.top + m_SrcHeight);
			m_Status = STONE_DESDORYING;
			m_Y = m_Y - 100;
			
		}
	}
	if (18 <= m_CurrentFrame && m_CurrentFrame <= 23)
	{
		if (m_CurrentFrame >= 18 && m_CurrentFrame <= 20)
			m_CurrentFrame = 3;
		else
			m_CurrentFrame = 7;
		m_Y = m_Y - 20;
	}
}

STONE_STATUS Stone::GetStatus() const
{
	return m_Status;
}

void Stone::SetStatus(STONE_STATUS status)
{
	m_Status = status;
}

