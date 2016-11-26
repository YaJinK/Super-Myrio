#include "Role.h"

Role::Role(LPDIRECT3DDEVICE9 pD3DDevice, LPDIRECTSOUND8 pDirectSound, char* fileName, float mass, short vector, float srcWidth, float srcHeight, float x, float y) : GameResource(pD3DDevice, fileName, srcWidth, srcHeight, x, y)
{
	m_Mass = mass;
	m_MaxSpeedX = 3;
	m_SpeedX = 0;
	m_MaxTractionX = 0.1f;
	m_TractionX = 0;
	m_FramePass = 14;
	m_SpeedY = 0;
	m_MaxTractionY = 5.8f;
	m_TractionY = 0;
	m_PosRectTopPass = 25;
	m_CurrentFrame = 14;
	m_Status = ROLE_ADULT;
	m_Vector = vector;
	m_IsInAir = true;
	m_AdultChildCount = 7;
	m_IsDeadSeted = false;
	m_InvincibleTime = 180;
	m_IsInvincible = true;
	//dead
	WAVEFORMATEX waveFormat;
	ZeroMemory(&waveFormat, sizeof(waveFormat));
	waveFormat.nChannels = 2;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nBlockAlign = 4;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwBufferBytes = 4 * waveFormat.nAvgBytesPerSec;
	dsbd.lpwfxFormat = &waveFormat;
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY
		| DSBCAPS_GLOBALFOCUS;

	pDirectSound->CreateSoundBuffer(&dsbd, &m_Dead, NULL);
	ifstream soundStream;
	soundStream.open("sound/dead.wav", ios::binary);
	LPVOID p;
	DWORD len;
	m_Dead->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_Dead->Unlock(&p, len, NULL, NULL);
	soundStream.close();
	//jump
	dsbd.dwBufferBytes = DWORD(0.5 * waveFormat.nAvgBytesPerSec);
	dsbd.lpwfxFormat->nSamplesPerSec = 22050;
	dsbd.lpwfxFormat->nAvgBytesPerSec = dsbd.lpwfxFormat->nSamplesPerSec * dsbd.lpwfxFormat->nBlockAlign;
	pDirectSound->CreateSoundBuffer(&dsbd, &m_Jump, NULL);
	soundStream.open("sound/jump.wav", ios::binary);
	m_Jump->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_Jump->Unlock(&p, len, NULL, NULL);
}

Role::~Role()
{
	m_Texture->Release();
	m_Texture = NULL;
	m_Dead->Release();
	m_Dead = NULL;
	m_Jump->Release();
	m_Jump = NULL;
}

void Role::SetIsInAir(bool isInAir)
{
	m_IsInAir = isInAir;
}

void Role::SetSpeedX(float speedX)
{
	if (speedX != -1)
		m_SpeedX = speedX;
}

void Role::Draw(LPD3DXSPRITE pD3DXSprite)
{
	unsigned int alpha = 0;
	if (m_IsInvincible)
		alpha = 0x80ffffff;
	else
		alpha = 0xffffffff;
	D3DXVECTOR3 pos = { (float)m_X, (float)m_Y, 0 };
	pD3DXSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pD3DXSprite->Draw(m_Texture, &m_Src, NULL, &pos, alpha);
	pD3DXSprite->End();
}

void Role::Release()
{
	GameResource::Release();
	m_Dead->Release();
	m_Dead = NULL;
	m_Jump->Release();
	m_Jump = NULL;
}

void Role::SetSpeedY(float speedY)
{
	if (speedY != -1)
		m_SpeedY = speedY;
}

void Role::SetTractioinY(float tractionY)
{
	m_TractionY = tractionY;
}

RECT * Role::GetPosRect(RECT * rect) const
{
	rect->left = (LONG)m_X + 5;
	rect->top = (LONG)m_Y + m_PosRectTopPass;
	rect->right = LONG(m_X + m_SrcWidth) - 5;
	rect->bottom = LONG(m_Y + m_SrcHeight - 2);
	return rect;
}

void Role::ToAdult()
{
	if (m_Status == ROLE_NORMAL)
	{
		m_Status = ROLE_ADULT;
	}
}

void Role::ToChild()
{
	if (m_Status == ROLE_ADULT)
	{
		m_Status = ROLE_NORMAL;
	}
}

short Role::GetIsInvincible() const
{
	return m_IsInvincible;
}

void Role::SetIsInvincible(bool isInvincible)
{
	m_IsInvincible = isInvincible;
}

float Role::GetSpeedX() const
{
	return m_SpeedX;
}

short Role::GetVector() const
{
	return m_Vector;
}

void Role::Dead()
{
	m_Dead->Play(0, 0, NULL);
	m_Status = ROLE_DEAD;
}

ROLE_STATUS Role::GetStatus() const
{
	return m_Status;
}

void Role::UpdateData(char* pKey, float frictionFactor, float gravite, float dragX, float dragY)
{
	if (m_AdultChildCount == 0 || m_AdultChildCount == 7)
	{
		m_TractionX = 0;
		float tractionYReduce = 0.5f;
		if (!m_IsDown || m_IsInAir)
		{
			if (pKey[DIK_A] & 0x80)
			{
				m_Vector = VECTOR_LEFT;
				m_TractionX = m_MaxTractionX;
			}

			if (pKey[DIK_D] & 0x80)
			{
				m_Vector = VECTOR_RIGHT;
				m_TractionX = m_MaxTractionX;
			}
		}

		if (!m_IsInAir)
		{
			if (pKey[DIK_J] & 0x80)
				m_MaxSpeedX = 4;
			else
				m_MaxSpeedX = 2;
		}

		m_TractionX *= m_Vector;
		if (m_Status == ROLE_DEAD)
		{
			if (!m_IsDeadSeted)
			{
				m_TractionY = m_MaxTractionY;
				m_IsDeadSeted = true;
			}
			m_TractionX = 0;
			m_IsInAir = true;
		}
		float friction = m_Mass * frictionFactor * gravite;
		if (m_SpeedX > 0)
			friction = -friction;
		if (0 == m_SpeedX)
			friction = 0;
		float temp = m_SpeedX;
		m_SpeedX += (m_TractionX + friction - dragX) / m_Mass;
		if (temp * m_SpeedX < 0)
			m_SpeedX = 0;
		if (m_SpeedX > m_MaxSpeedX)
			m_SpeedX = m_MaxSpeedX;
		if (m_SpeedX < -m_MaxSpeedX)
			m_SpeedX = -m_MaxSpeedX;
		m_X += m_SpeedX;

		
		if (pKey[DIK_K] & 0x80)
		{
			tractionYReduce = 0.1f;
			if (!m_IsInAir)
			{
				m_Jump->SetCurrentPosition(0);
				m_Jump->Play(0, 0, NULL);
				m_TractionY = m_MaxTractionY;
			}
			m_IsInAir = true;
		}
		if (m_IsInAir)
		{
			m_SpeedY = gravite - (m_TractionY + dragY) / m_Mass;
			m_Y += m_SpeedY;
			m_TractionY -= tractionYReduce;
			if (m_TractionY < 0)
				m_TractionY = 0;
		}
	}
	if (m_IsInvincible)
	{
		m_InvincibleTime--;
		if (m_InvincibleTime == 0)
		{
			m_InvincibleTime = 180;
			m_IsInvincible = false;
		}
	}
}

void Role::UpdateFrame(char* pKey)
{
	if (m_Status == ROLE_ADULT && m_AdultChildCount < 7)
	{
		m_FramePass = 14;
		if (m_AdultChildCount % 2 == 0)
			m_CurrentFrame += 14;
		else
			m_CurrentFrame -= 14;
		m_AdultChildCount++;
	}
	if (m_Status == ROLE_NORMAL && m_AdultChildCount > 0)
	{
		m_AdultChildCount--;
		m_FramePass = 0;
		if (m_AdultChildCount % 2 == 0)
			m_CurrentFrame -= 14;
		else
			m_CurrentFrame += 14;
	}
	if (m_AdultChildCount == 0 || m_AdultChildCount == 7)
	{
		if (!m_IsInAir)
		{
			m_IsDown = false;
			if (m_Status == ROLE_NORMAL)
				m_PosRectTopPass = 55;
			else
				m_PosRectTopPass = 25;
		}
		if (pKey[DIK_S] & 0x80)
		{
			if (m_CurrentFrame > 14)
			{
				m_IsDown = true;
				if (VECTOR_RIGHT == m_Vector)
					m_CurrentFrame = 26;
				else
					m_CurrentFrame = 27;
				m_PosRectTopPass = 55;
			}
		}
		if (!m_IsInAir)
		{
			if (!m_IsDown)
			{
				if (pKey[DIK_A] & 0x80)
				{
					if (m_CurrentFrame >= 0 + m_FramePass && m_CurrentFrame < 8 + m_FramePass)
						m_CurrentFrame = 10 + m_FramePass;
					m_CurrentFrame--;
					if (m_SpeedX * m_Vector < 0)
						m_CurrentFrame = 5 + m_FramePass;
				}
				if (pKey[DIK_D] & 0x80)
				{
					if (m_CurrentFrame > 3 + m_FramePass && m_CurrentFrame <= 13 + m_FramePass)
						m_CurrentFrame = 1 + m_FramePass;
					m_CurrentFrame++;
					if (m_SpeedX * m_Vector < 0)
						m_CurrentFrame = 6 + m_FramePass;
				}
				if (0 == m_TractionX && 1 == m_Vector)
				{
					if (m_CurrentFrame >= 4 + m_FramePass && m_CurrentFrame <= 11 + m_FramePass)
						m_CurrentFrame = 1 + m_FramePass;
					m_CurrentFrame++;
					if (0 == m_SpeedX)
						m_CurrentFrame = 1 + m_FramePass;
				}
				if (0 == m_TractionX && -1 == m_Vector)
				{
					if (m_CurrentFrame >= 0 + m_FramePass && m_CurrentFrame <= 7 + m_FramePass)
						m_CurrentFrame = 10 + m_FramePass;
					m_CurrentFrame--;
					if (0 == m_SpeedX)
						m_CurrentFrame = 10 + m_FramePass;
				}
			}

		}
		if (pKey[DIK_K] & 0x80)
		{
			if (!m_IsDown)
			{
				if (1 == m_Vector && 11 + m_FramePass != m_CurrentFrame)
					m_CurrentFrame = 0 + m_FramePass;
				if (-1 == m_Vector && 0 + m_FramePass != m_CurrentFrame)
					m_CurrentFrame = 11 + m_FramePass;
			}
		}
		if (m_Status == ROLE_DEAD)
		{
			m_CurrentFrame = 12;
		}
	}
	
	//精灵矩形更新
	m_Src.left = LONG((m_CurrentFrame % 14) * m_SrcWidth);
	m_Src.top = LONG((m_CurrentFrame / 14) * m_SrcHeight);
	m_Src.right = LONG(m_Src.left + m_SrcWidth);
	m_Src.bottom = LONG(m_Src.top + m_SrcHeight);
}

short Role::GetACCount() const
{
	return m_AdultChildCount;
}




