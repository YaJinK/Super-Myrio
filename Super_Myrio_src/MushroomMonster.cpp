#include "MushroomMonster.h"

MushroomMonster::MushroomMonster(LPDIRECT3DDEVICE9 pD3DDevice, char * fileName, float srcWidth, float srcHeight, float x, float y, short currentFrame) : GameResource(pD3DDevice, fileName, srcWidth, srcHeight, x, y)
{
	if (currentFrame == 2 || currentFrame == 5 || currentFrame == 8)
		m_CurrentFrame = currentFrame - 1;
	else
		m_CurrentFrame = currentFrame;
	m_FramePass = m_CurrentFrame / 3 * 3;
	m_Status = MONSTER_SHOW;
	m_Vector = VECTOR_LEFT;
	m_SpeedX = 1;
	m_DeadCount = 5;
}

void MushroomMonster::UpdateData(char * pKey, float frictionFactor, float gravite, float dragX, float dragY)
{
	m_X += m_SpeedX * m_Vector;
	m_SpeedY = gravite - dragY;
	m_Y += m_SpeedY;
}

RECT * MushroomMonster::GetPosRect(RECT * rect) const
{
	rect->left = (LONG)m_X + 5;
	rect->top = (LONG)m_Y + 20;
	rect->right = LONG(m_X + m_SrcWidth - 5);
	rect->bottom = LONG(m_Y + m_SrcHeight - 1);
	return rect;
}

short MushroomMonster::GetDeadCount() const
{
	return m_DeadCount;
}

void MushroomMonster::UpdateFrame(char * pKey)
{
	if (m_Status == MONSTER_SHOW)
	{
		if (m_CurrentFrame == 0 + m_FramePass)
			m_CurrentFrame++;
		else
			m_CurrentFrame--;
	}
	if (m_Status == MONSTER_DEAD)
	{
		m_CurrentFrame = 2 + m_FramePass;
		if (m_DeadCount > 0)
		{
			m_DeadCount--;
		}
	}
	
	m_Src.left = LONG((m_CurrentFrame % 9) * m_SrcWidth);
	m_Src.top = LONG((m_CurrentFrame / 9) * m_SrcHeight);
	m_Src.right = LONG(m_Src.left + m_SrcWidth);
	m_Src.bottom = LONG(m_Src.top + m_SrcHeight);
}

MONSTER_STATUS MushroomMonster::GetStatus() const
{
	return m_Status;
}

void MushroomMonster::SetStatus(MONSTER_STATUS status)
{
	m_Status = status;
}

void MushroomMonster::SetVector(VECTOR_DIRECTION vector)
{
	m_Vector = vector;
}

void MushroomMonster::Back()
{
	if (m_Vector == VECTOR_LEFT)
		m_Vector = VECTOR_RIGHT;
	else
		m_Vector = VECTOR_LEFT;
}
