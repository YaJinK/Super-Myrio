#include "Reward.h"

Reward::Reward(LPDIRECT3DDEVICE9 pD3DDevice, char * fileName, float srcWidth, float srcHeight, float x, float y, short currentFrame) : GameResource(pD3DDevice, fileName, srcWidth, srcHeight, x, y)
{
	m_CurrentFrame = currentFrame;
	m_Status = REWARD_HIDE;
	short vector = rand() % 2;
	if (vector == 0)
		m_Vector = VECTOR_RIGHT;
	else
		m_Vector = VECTOR_LEFT;
	m_OldY = m_Y;
	m_SpeedY = 0;
	m_SpeedX = 1.5;
}

RECT * Reward::GetPosRect(RECT * rect) const
{
	rect->left = (LONG)m_X - 1;
	rect->top = (LONG)m_Y + 20;
	rect->right = LONG(m_X + m_SrcWidth + 1);
	rect->bottom = LONG(m_Y + m_SrcHeight - 1);
	return rect;
}

void Reward::UpdateData(char* pKey, float frictionFactor, float gravite, float dragX, float dragY)
{
	if (m_CurrentFrame >= 0 && m_CurrentFrame <= 3)
	{
		m_Y -= 2;
		if (m_Y <= m_OldY - 105)
			m_Status = REWARD_DESTORY;
	}
	else
	{
		m_X += m_SpeedX * m_Vector;
		m_SpeedY = gravite - dragY;
		m_Y += m_SpeedY;
	}
	if (m_Y > 600)
		m_Status = REWARD_DESTORY;
}

void Reward::UpdateFrame(char * pKey)
{
	if (m_CurrentFrame >= 0 && m_CurrentFrame < 4)
	{
		m_CurrentFrame++;
		if (m_CurrentFrame == 4)
			m_CurrentFrame = 0;
	}
	
	m_Src.left = LONG((m_CurrentFrame % 4) * m_SrcWidth);
	m_Src.top = LONG((m_CurrentFrame / 4) * m_SrcHeight);
	m_Src.right = LONG(m_Src.left + m_SrcWidth);
	m_Src.bottom = LONG(m_Src.top + m_SrcHeight);
}

VECTOR_DIRECTION Reward::GetVector() const
{
	return m_Vector;
}

void Reward::SetVector(VECTOR_DIRECTION vector)
{
	m_Vector = vector;
}

REWARD_STATUS Reward::GetStatus() const
{
	return m_Status;
}

void Reward::SetStatus(REWARD_STATUS status)
{
	m_Status = status;
}
