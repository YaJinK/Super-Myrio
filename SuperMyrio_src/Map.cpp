#include "Map.h"

Map::Map(LPDIRECT3DDEVICE9 pD3DDevice, LPDIRECTSOUND8 pDirectSound, float gravity, float frictionFactor)
{
	m_Gravity = gravity;
	m_FrictionFactor = frictionFactor;
	m_RolesNum = 0;
	m_RoleCountDown = 5;
	m_StoneCountDown = 6;
	m_MonsterFrameCount = 20;
	m_MushRoomIndex = 0;
	m_GotGreenMush = 0;
	m_TotalGreenMush = 0;
	m_InfoBar = new InfoBar(pD3DDevice, "img/reward.png", 36, 35, 300, 480, 4);
	m_Story = new Story(pD3DDevice);
	m_MapStatus = MAP_STORYBEGIN;
	m_EndingNum = -1;
	m_DeadTimer = 300;
	m_Guide = new Guide(pD3DDevice, "img/guide.png", 100, 72, 630, 490);
	m_Roles[m_RolesNum] = new Role(pD3DDevice, pDirectSound, "img/mario.png", 0.5, -1, 38, 70, 550, 450);
	m_RolesNum++;

	srand(timeGetTime());
	short mapStoneColNum = 25;
	short mapStoneRowNum = 18;
	short adultMushroomNum = 0;
	short framePass = rand() % 3;
	char* backSound = NULL;
	DWORD samplesPerSec = 44100;
	DWORD second = 61;
	if (framePass == 1)
	{
		samplesPerSec = 22050;
		backSound = "sound/back2.wav";
		second = 13;
	}
	else
		backSound = "sound/back.wav";
	short monsterID = framePass * 3;
	short monsterRate = 30;
	if (framePass == 0)
		m_BackColor = D3DCOLOR_XRGB(92, 148, 252);
	else
		m_BackColor = D3DCOLOR_XRGB(0, 0, 0);

	m_DataLen = mapStoneRowNum * mapStoneColNum;
	m_Stone = new Stone*[m_DataLen];
	m_Monsters = new MushroomMonster*[m_DataLen];
	m_MonsterNum = 0;
	float x = -8;
	float y = -32;
	for (int index = 0; index < m_DataLen - 75; index++)
	{
		x += 32;
		if (index % mapStoneColNum == 0)
		{
			x = -8;
			y += 31;
		}
		
		if (index / mapStoneColNum == 0)
		{
			m_Stone[index] = new Stone(pD3DDevice, "img/stone.png", 32, 36.5, x, y, 12 + framePass);
		}
		else if (index / mapStoneColNum == mapStoneRowNum - 4)
		{
			if (!(index % mapStoneColNum > 3 && index % mapStoneColNum < 7))
				m_Stone[index] = new Stone(pD3DDevice, "img/stone.png", 32, 36.5, x, y, 12 + framePass);
			else
			{
				m_Stone[index] = NULL;
				short monsterTemp = rand() % monsterRate;
				if (monsterTemp == 0)
				{
					m_Monsters[m_MonsterNum] = new MushroomMonster(pD3DDevice, "img/monster.png", 36.5, 34.5, x, y, monsterID);
					m_MonsterNum++;
				}
			}
		}
		else
		{
			if (index % mapStoneColNum == 0 || index % mapStoneColNum == mapStoneColNum - 1 || index % mapStoneColNum == 1 || index % mapStoneColNum == mapStoneColNum - 2)
				m_Stone[index] = new Stone(pD3DDevice, "img/stone.png", 32, 36.5, x, y, 12 + framePass);
			else
			{
				if (index / mapStoneColNum > 2 && index / mapStoneColNum != mapStoneRowNum - 5)
				{
					short stoneFrame = rand() % 23;
					if (stoneFrame % 3 == 0)
					{
						if (stoneFrame == 12)
							stoneFrame = 18;
						if (framePass == 1)
						{
							if (stoneFrame == 18)
								stoneFrame = 21;
						}
						else
						{
							if (stoneFrame == 21)
								stoneFrame = 18;
						}
						m_Stone[index] = new Stone(pD3DDevice, "img/stone.png", 32, 36.5, x, y, stoneFrame + framePass);
					}
					else
					{
						m_Stone[index] = NULL;
						short monsterTemp = rand() % monsterRate;
						if (monsterTemp == 0)
						{
							m_Monsters[m_MonsterNum] = new MushroomMonster(pD3DDevice, "img/monster.png", 36.5, 34.5, x, y, monsterID);
							m_MonsterNum++;
						}
					}
				}
				else
				{
					m_Stone[index] = NULL;
					short monsterTemp = rand() % monsterRate;
					if (monsterTemp == 0)
					{
						m_Monsters[m_MonsterNum] = new MushroomMonster(pD3DDevice, "img/monster.png", 36.5, 34.5, x, y, monsterID);
						m_MonsterNum++;
					}
				}
			}
		}
	}
	for (int index = m_DataLen - 75; index < m_DataLen - 25; index++)
	{
		x += 32;
		if (index % mapStoneColNum == 0)
		{
			x = -8;
			y += 31;
		}
		if (index % mapStoneColNum == 0)
			m_Stone[index] = new Stone(pD3DDevice, "img/stone.png", 32, 36.5, x, y, 12 + framePass);
		else
		{
			m_Stone[index] = NULL;
		}
	}
	for (int index = m_DataLen - 25; index < m_DataLen; index++)
	{
		x += 32;
		if (index % mapStoneColNum == 0)
		{
			x = -8;
			y += 31;
		}
		m_Stone[index] = new Stone(pD3DDevice, "img/stone.png", 32, 36.5, x, y, 12 + framePass);
	}

	for (int index = 0; index < m_DataLen; index++)
	{
		if (m_Stone[index] == NULL)
			continue;
		if (m_Stone[index]->GetReward() != NULL)
		{
			if (m_Stone[index]->GetReward()->GetCurrentFrame() == 4)
				m_TotalGreenMush++;
		}
	}
	m_GameTimer = m_TotalGreenMush * 10 + 100;
	m_CurrentTime = timeGetTime();
	m_OldTime = m_CurrentTime;
	//backsound
	WAVEFORMATEX waveFormat;
	ZeroMemory(&waveFormat, sizeof(waveFormat));
	waveFormat.nChannels = 2;
	waveFormat.nSamplesPerSec = samplesPerSec;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nBlockAlign = 4;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwBufferBytes = second * waveFormat.nAvgBytesPerSec;
	dsbd.lpwfxFormat = &waveFormat;
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY
		| DSBCAPS_GLOBALFOCUS;

	pDirectSound->CreateSoundBuffer(&dsbd, &m_BackSound, NULL);
	ifstream soundStream;
	soundStream.open(backSound, ios::binary);
	LPVOID p;
	DWORD len;
	m_BackSound->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_BackSound->Unlock(&p, len, NULL, NULL);
	soundStream.close();
	//hurt
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	dsbd.dwBufferBytes = 2 * waveFormat.nAvgBytesPerSec;
	dsbd.lpwfxFormat = &waveFormat;
	pDirectSound->CreateSoundBuffer(&dsbd, &m_Hurt, NULL);
	soundStream.open("sound/hurt.wav", ios::binary);
	m_Hurt->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_Hurt->Unlock(&p, len, NULL, NULL);
	soundStream.close();
	//stonenormal
	dsbd.dwBufferBytes = 1 * waveFormat.nAvgBytesPerSec;
	pDirectSound->CreateSoundBuffer(&dsbd, &m_StoneNormal, NULL);
	soundStream.open("sound/stone_normal.wav", ios::binary);
	m_StoneNormal->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_StoneNormal->Unlock(&p, len, NULL, NULL);
	soundStream.close();
	//badend
	dsbd.dwBufferBytes = 10 * waveFormat.nAvgBytesPerSec;
	pDirectSound->CreateSoundBuffer(&dsbd, &m_BadEnd, NULL);
	soundStream.open("sound/bad_end.wav", ios::binary);
	m_BadEnd->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_BadEnd->Unlock(&p, len, NULL, NULL);
	soundStream.close();
	//monsterkill
	waveFormat.nSamplesPerSec = 22050;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	dsbd.dwBufferBytes = DWORD(0.5 * waveFormat.nAvgBytesPerSec);
	dsbd.lpwfxFormat = &waveFormat;
	pDirectSound->CreateSoundBuffer(&dsbd, &m_KillMonster, NULL);
	soundStream.open("sound/kill_monster.wav", ios::binary);
	m_KillMonster->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_KillMonster->Unlock(&p, len, NULL, NULL);
	soundStream.close();
	//getreward
	dsbd.dwBufferBytes = 1 * waveFormat.nAvgBytesPerSec;
	pDirectSound->CreateSoundBuffer(&dsbd, &m_GetMushroom, NULL);
	soundStream.open("sound/get_mushroom.wav", ios::binary);
	m_GetMushroom->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_GetMushroom->Unlock(&p, len, NULL, NULL);
	soundStream.close();
	//findreward
	dsbd.dwBufferBytes = DWORD(0.5 * waveFormat.nAvgBytesPerSec);
	pDirectSound->CreateSoundBuffer(&dsbd, &m_FindMushroom, NULL);
	soundStream.open("sound/find_mushroom.wav", ios::binary);
	m_FindMushroom->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_FindMushroom->Unlock(&p, len, NULL, NULL);
	soundStream.close();
	//getcoin
	dsbd.dwBufferBytes = DWORD(0.5 * waveFormat.nAvgBytesPerSec);
	pDirectSound->CreateSoundBuffer(&dsbd, &m_GetCoin, NULL);
	soundStream.open("sound/get_coin.wav", ios::binary);
	m_GetCoin->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_GetCoin->Unlock(&p, len, NULL, NULL);
	soundStream.close();
	//stonebroken
	dsbd.dwBufferBytes = DWORD(0.5 * waveFormat.nAvgBytesPerSec);
	pDirectSound->CreateSoundBuffer(&dsbd, &m_StoneBroken, NULL);
	soundStream.open("sound/stone_broken.wav", ios::binary);
	m_StoneBroken->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_StoneBroken->Unlock(&p, len, NULL, NULL);
	soundStream.close();
	//happyend
	dsbd.dwBufferBytes = 10 * waveFormat.nAvgBytesPerSec;
	pDirectSound->CreateSoundBuffer(&dsbd, &m_HappyEnd, NULL);
	soundStream.open("sound/happy_end.wav", ios::binary);
	m_HappyEnd->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_HappyEnd->Unlock(&p, len, NULL, NULL);
	soundStream.close();
	
}

Map::~Map()
{
	for (int index = 0; index < m_RolesNum; index++)
	{
		m_Roles[index]->Release();
		m_Roles[index] = NULL;
	}
	for (int index = 0; index < m_DataLen; index++)
	{
		if (m_Stone[index] != NULL)
		{
			m_Stone[index]->Release();
			m_Stone[index] = NULL;
		}
	}
	for (int index = 0; index < m_MushRoomIndex; index++)
	{
		if (m_MushRooms[index] != NULL)
		{
			m_MushRooms[index]->Release();
			m_MushRooms[index] = NULL;
		}
	}
	for (int index = 0; index < m_MonsterNum; index++)
	{
		if (m_Monsters[index] != NULL)
		{
			m_Monsters[index]->Release();
			m_Monsters[index] = NULL;
		}
	}
	if (m_InfoBar != NULL)
	{
		m_InfoBar->Release();
		m_InfoBar = NULL;
	}
	if (m_Story != NULL)
	{
		m_Story->Release();
		m_Story = NULL;
	}
	if (m_Guide != NULL)
	{
		m_Guide->Release();
		m_Guide = NULL;
	}
	if (m_BackSound != NULL)
	{
		m_BackSound->Release();
		m_BackSound = NULL;
	}
	if (m_KillMonster != NULL)
	{
		m_KillMonster->Release();
		m_KillMonster = NULL;
	}
	if (m_GetMushroom != NULL)
	{
		m_GetMushroom->Release();
		m_GetMushroom = NULL;
	}
	if (m_FindMushroom != NULL)
	{
		m_FindMushroom->Release();
		m_FindMushroom = NULL;
	}
	if (m_GetCoin != NULL)
	{
		m_GetCoin->Release();
		m_GetCoin = NULL;
	}
	if (m_StoneBroken != NULL)
	{
		m_StoneBroken->Release();
		m_StoneBroken = NULL;
	}
	if (m_Hurt != NULL)
	{
		m_Hurt->Release();
		m_Hurt = NULL;
	}
	if (m_HappyEnd != NULL)
	{
		m_HappyEnd->Release();
		m_HappyEnd = NULL;
	}
	if (m_BadEnd != NULL)
	{
		m_BadEnd->Release();
		m_BadEnd = NULL;
	}
	if (m_StoneNormal != NULL)
	{
		m_StoneNormal->Release();
		m_StoneNormal = NULL;
	}
}

short Map::ImpactCheck(RECT* rect1, RECT* rect2)
{
	RECT temp;
	if (IntersectRect(&temp, rect1, rect2))
	{
		if (temp.bottom - temp.top < temp.right - temp.left && temp.right - temp.left > 2)
		{
			if (rect1->bottom > rect2->bottom)
				return 1;
			else
				return 2;
			
		}
			
		if (temp.bottom - temp.top > temp.right - temp.left + 3)
		{
			if (rect1->right > rect2->right)
				return 3;
			else
				return 4;
		}
			
	}
	return 0;
}

void Map::ImpactHandle(char* pKey)
{
	if (m_MapStatus != MAP_INIT)
		return;
	if (0 == m_StoneCountDown)
	{
		//石头和奖励的精灵更新
		for (int stoneIndex = 0; stoneIndex < m_DataLen; stoneIndex++)
		{
			if (NULL != m_Stone[stoneIndex] && m_Stone[stoneIndex]->GetStatus() == STONE_NORMAL)
			{
				m_Stone[stoneIndex]->UpdateFrame(NULL);
				if (m_Stone[stoneIndex]->GetReward() != NULL)
				{
					if (m_Stone[stoneIndex]->GetReward()->GetStatus() == REWARD_SHOW)
						m_Stone[stoneIndex]->GetReward()->UpdateFrame(NULL);
				}
			}
		}
		m_StoneCountDown = 6;
	}
	if (0 == m_MonsterFrameCount)
	{
		
		for (int monsterIndex = 0; monsterIndex < m_MonsterNum; monsterIndex++)
		{
			if (m_Monsters[monsterIndex]->GetStatus() != MONSTER_HIDE)
			{
				m_Monsters[monsterIndex]->UpdateFrame(NULL);
			}
		}
		m_MonsterFrameCount = 20;
	}
	for (int index = 0; index < m_RolesNum; index++)
	{
		RECT role;
		m_Roles[index]->GetPosRect(&role);
		float speedX = -1;
		float dragX = 0;
		float dragY = 0;
		
		bool isInAir = true;
		bool findX = false;
		bool findY = false;
		if (0 == m_RoleCountDown)
		{
			m_Roles[index]->UpdateFrame(pKey);
			m_RoleCountDown = 5;
		}
		
		//石头角色碰撞判定以及石头数据更新
		if (m_Roles[index]->GetStatus() != ROLE_DEAD)
		{
			for (int stoneIndex = 0; stoneIndex < m_DataLen; stoneIndex++)
			{
				RECT stone;
				if (NULL == m_Stone[stoneIndex])
					continue;

				if (m_Stone[stoneIndex]->GetStatus() == STONE_NORMAL)
				{
					m_Stone[stoneIndex]->GetPosRect(&stone);

					//障碍下方判定
					if (!findY && 1 == ImpactCheck(&role, &stone))
					{
						isInAir = true;
						m_Roles[index]->SetTractioinY(0);
						findY = true;
						m_Stone[stoneIndex]->Action(m_Roles[index]->GetStatus());
						
						if (m_Stone[stoneIndex]->GetReward() != NULL)
						{
							if (m_Stone[stoneIndex]->GetReward()->GetStatus() == REWARD_HIDE)
							{
								if (m_Stone[stoneIndex]->GetReward()->GetCurrentFrame() > 3)
								{
									m_FindMushroom->SetCurrentPosition(0);
									m_FindMushroom->Play(0, 0, NULL);
								}
								else
								{
									m_GetCoin->SetCurrentPosition(0);
									m_GetCoin->Play(0, 0, NULL);
								}
								m_MushRooms[m_MushRoomIndex] = m_Stone[stoneIndex]->GetReward();
								m_MushRooms[m_MushRoomIndex]->SetStatus(REWARD_SHOW);
								m_MushRoomIndex++;
							}
						}
						
						if (m_Roles[index]->GetStatus() == ROLE_ADULT)
						{
							if ((0 <= m_Stone[stoneIndex]->GetCurrentFrame() && m_Stone[stoneIndex]->GetCurrentFrame() <= 2)
								|| (9 <= m_Stone[stoneIndex]->GetCurrentFrame() && m_Stone[stoneIndex]->GetCurrentFrame() <= 11))
							{
								m_StoneBroken->SetCurrentPosition(0);
								m_StoneBroken->Play(0, 0, NULL);
							}
							else
							{
								m_StoneNormal->SetCurrentPosition(0);
								m_StoneNormal->Play(0, 0, NULL);
							}
						}
						else
						{
							m_StoneNormal->SetCurrentPosition(0);
							m_StoneNormal->Play(0, 0, NULL);
						}
					}
					//障碍上方判定
					if (!findY && 2 == ImpactCheck(&role, &stone))
					{
						isInAir = false;
						dragY = m_Stone[stoneIndex]->GetDragY();
						m_Roles[index]->SetSpeedY(0);
						findY = true;
					}
					//障碍右边判定
					if (!findX && 3 == ImpactCheck(&role, &stone))
					{
						speedX = 0;
						dragX = -m_Stone[stoneIndex]->GetDragX();
						findX = true;
					}
					//障碍左边判定
					if (!findX && 4 == ImpactCheck(&role, &stone))
					{
						speedX = 0;
						dragX = m_Stone[stoneIndex]->GetDragX();
						findX = true;
					}

				}
				m_Stone[stoneIndex]->UpdateData(NULL, 0, 0, 0, 0);
			}
		}
		
		//上方 蘑菇判定
		for (int mushRoomIndex = 0; mushRoomIndex < m_MushRoomIndex; mushRoomIndex++)
		{
			if (m_MushRooms[mushRoomIndex]->GetStatus() == REWARD_DESTORY)
				continue;
			RECT mushroom;
			float dragYMushRoom = 0;
			m_MushRooms[mushRoomIndex]->GetPosRect(&mushroom);
			for (int stoneIndex = 0; stoneIndex < m_DataLen; stoneIndex++)
			{
				RECT stone;
				if (NULL == m_Stone[stoneIndex])
					continue;

				if (m_Stone[stoneIndex]->GetStatus() == STONE_NORMAL)
				{
					m_Stone[stoneIndex]->GetPosRect(&stone);
					if (2 == ImpactCheck(&mushroom, &stone))
					{
						m_MushRooms[mushRoomIndex]->SetY(m_Stone[stoneIndex]->GetY() - 32);
						dragYMushRoom = m_Gravity + 0.1f;
					}
					m_Stone[stoneIndex]->GetPosRect(&stone);
					if (3 == ImpactCheck(&mushroom, &stone))
					{
						m_MushRooms[mushRoomIndex]->SetVector(VECTOR_RIGHT);
					}
					if (4 == ImpactCheck(&mushroom, &stone))
					{
						m_MushRooms[mushRoomIndex]->SetVector(VECTOR_LEFT);
					}
				}
			}
			m_MushRooms[mushRoomIndex]->UpdateData(NULL, m_FrictionFactor, m_Gravity, 0, dragYMushRoom);
			if (m_Roles[index]->GetStatus() != ROLE_DEAD)
			{
				if (0 != ImpactCheck(&mushroom, &role))
				{
					m_MushRooms[mushRoomIndex]->SetStatus(REWARD_DESTORY);
					if (m_MushRooms[mushRoomIndex]->GetCurrentFrame() == 4)
					{
						m_GotGreenMush++;
					}
					if (m_MushRooms[mushRoomIndex]->GetCurrentFrame() == 5)
					{
						m_GetMushroom->Play(0, 0, NULL);
						m_Roles[index]->ToAdult();
					}
					if (!m_Roles[index]->GetIsInvincible())
					{
						if (m_MushRooms[mushRoomIndex]->GetCurrentFrame() == 6)
						{
							if (m_Roles[index]->GetStatus() == ROLE_ADULT)
							{
								m_Hurt->Play(0, 0, NULL);
								m_Roles[index]->ToChild();
								m_Roles[index]->SetIsInvincible(true);
							}
							else
								m_Roles[index]->Dead();
						}
					}
					
				}
			}
		}
		//怪物判定
		for (int monsterIndex = 0; monsterIndex < m_MonsterNum; monsterIndex++)
		{
			if (m_Monsters[monsterIndex]->GetStatus() == MONSTER_SHOW)
			{
				RECT monster;
				m_Monsters[monsterIndex]->GetPosRect(&monster);
				float dragMonster = 0;

				for (int stoneIndex = 0; stoneIndex < m_DataLen; stoneIndex++)
				{
					RECT stone;
					if (NULL == m_Stone[stoneIndex])
						continue;


					if (m_Stone[stoneIndex]->GetStatus() == STONE_NORMAL)
					{
						m_Stone[stoneIndex]->GetPosRect(&stone);
						if (2 == ImpactCheck(&monster, &stone))
						{
							m_Monsters[monsterIndex]->SetY(m_Stone[stoneIndex]->GetY() - 32);
							dragMonster = m_Gravity + 0.1f;
						}
						m_Stone[stoneIndex]->GetPosRect(&stone);
						if (3 == ImpactCheck(&monster, &stone))
						{
							m_Monsters[monsterIndex]->SetVector(VECTOR_RIGHT);
						}
						if (4 == ImpactCheck(&monster, &stone))
						{
							m_Monsters[monsterIndex]->SetVector(VECTOR_LEFT);
						}
					}
				}
				for (int monsterIndex2 = 0; monsterIndex2 < m_MonsterNum; monsterIndex2++)
				{
					if (monsterIndex2 == monsterIndex || m_Monsters[monsterIndex2]->GetStatus() == MONSTER_DEAD)
						continue;
					RECT monster2;
					m_Monsters[monsterIndex2]->GetPosRect(&monster2);
					if (3 == ImpactCheck(&monster, &monster2))
					{
						m_Monsters[monsterIndex2]->Back();
						m_Monsters[monsterIndex]->Back();
					}
				}
				if (m_Monsters[monsterIndex]->GetStatus() != MONSTER_DEAD && m_Roles[index]->GetStatus() != ROLE_DEAD)
				{
					if (!m_Roles[index]->GetIsInvincible())
					{
						if (2 == ImpactCheck(&role, &monster))
						{
							m_KillMonster->SetCurrentPosition(0);
							m_KillMonster->Play(0, 0, NULL);
							m_Monsters[monsterIndex]->SetStatus(MONSTER_DEAD);
							m_Roles[index]->SetTractioinY(5.0f);
						}
						if (0 != ImpactCheck(&role, &monster) && m_Monsters[monsterIndex]->GetStatus() != MONSTER_DEAD)
						{
							if (m_Roles[index]->GetStatus() == ROLE_ADULT)
							{
								m_Hurt->Play(0, 0, NULL);
								m_Roles[index]->ToChild();
								m_Roles[index]->SetIsInvincible(true);
							}
							else
								m_Roles[index]->Dead();
						}
					}
				}
				m_Monsters[monsterIndex]->UpdateData(NULL, m_FrictionFactor, m_Gravity, 0, dragMonster);
			}
		}
	
		m_Roles[index]->UpdateData(pKey, m_FrictionFactor, m_Gravity, dragX, dragY);
		m_Roles[index]->SetSpeedX(speedX);
		m_Roles[index]->SetIsInAir(isInAir);
		if (m_Roles[index]->GetStatus() == ROLE_DEAD)
		{
			m_EndingNum = 1;
		}
		if (m_Roles[index]->GetX() >= 785)
		{
			if (m_TotalGreenMush == m_GotGreenMush)
				m_EndingNum = 0;
			else
				m_EndingNum = 2;
		}
		if (m_EndingNum != -1)
		{
			m_DeadTimer--;
			if (m_DeadTimer == 0)
				m_MapStatus = MAP_STORYENDING;
		}
		if (m_GameTimer == 0 && m_Roles[index]->GetX() < 785 && m_Roles[index]->GetStatus() != ROLE_DEAD)
			m_Roles[index]->Dead();
	}
	m_RoleCountDown--;
	m_StoneCountDown--;
	m_MonsterFrameCount--;
	m_CurrentTime = timeGetTime();
	if (m_CurrentTime - m_OldTime >= 1000)
	{
		m_OldTime = m_CurrentTime;
		if (m_GameTimer > 0)
			m_GameTimer--;
	}
}

void Map::AddPlayer(LPDIRECT3DDEVICE9 pD3DDevice, LPDIRECTSOUND8 pDirectSound, short roleId)
{
	if (m_RolesNum > 3)
		return;
	if (0 == roleId)
	{
		m_Roles[m_RolesNum] = new Role(pD3DDevice, pDirectSound, "img/mario.png", 0.5, -1, 38, 70, 550, 450);
	}
	m_RolesNum++;
}

void Map::Draw(LPD3DXSPRITE pD3DXSprite)
{
	if (m_MapStatus == MAP_INIT)
	{
		
		for (int stoneIndex = 0; stoneIndex < m_DataLen; stoneIndex++)
		{
			if (NULL == m_Stone[stoneIndex])
				continue;
			if (m_Stone[stoneIndex]->GetStatus() != STONE_DESDORYED)
			{

				if (m_Stone[stoneIndex]->GetStatus() == STONE_NORMAL)
					m_Stone[stoneIndex]->Draw(pD3DXSprite);
				else
				{
					m_Stone[stoneIndex]->Draw(pD3DXSprite, m_Stone[stoneIndex]->GetX() - m_Stone[stoneIndex]->GetXPass(), m_Stone[stoneIndex]->GetY());
					m_Stone[stoneIndex]->Draw(pD3DXSprite, m_Stone[stoneIndex]->GetX() + m_Stone[stoneIndex]->GetXPass(), m_Stone[stoneIndex]->GetY());
					m_Stone[stoneIndex]->Draw(pD3DXSprite, m_Stone[stoneIndex]->GetX() - m_Stone[stoneIndex]->GetXPass(), m_Stone[stoneIndex]->GetY() + 35);
					m_Stone[stoneIndex]->Draw(pD3DXSprite, m_Stone[stoneIndex]->GetX() + m_Stone[stoneIndex]->GetXPass(), m_Stone[stoneIndex]->GetY() + 35);
				}
			}
		}
		m_InfoBar->Draw(pD3DXSprite);
		m_InfoBar->DrawInfo(m_GameTimer);
		m_InfoBar->DrawInfo(m_GotGreenMush, m_TotalGreenMush);
		
		for (int mushroomIndex = 0; mushroomIndex < m_MushRoomIndex; mushroomIndex++)
		{
			if (m_MushRooms[mushroomIndex]->GetStatus() == REWARD_SHOW)
				m_MushRooms[mushroomIndex]->Draw(pD3DXSprite);
		}
		for (int monsterIndex = 0; monsterIndex < m_MonsterNum; monsterIndex++)
		{
			if (m_Monsters[monsterIndex]->GetDeadCount() != 0 || m_Monsters[monsterIndex]->GetStatus() == MONSTER_SHOW)
			{
				m_Monsters[monsterIndex]->Draw(pD3DXSprite);
			}
		}

		short deadNum = 0;
		for (int index = 0; index < m_RolesNum; index++)
		{
			m_Roles[index]->Draw(pD3DXSprite);
			if (m_Roles[index]->GetStatus() == ROLE_DEAD)
				deadNum++;
		}
		m_Guide->Draw(pD3DXSprite);
		m_Guide->DrawInfo();
		if (deadNum != m_RolesNum)
			m_BackSound->Play(0, 0, DSBPLAY_LOOPING);
		else
			m_BackSound->Stop();
	}
	else if (m_MapStatus == MAP_STORYBEGIN)
	{
		m_Story->DrawBegin();
		if (m_Story->GetTimer() == 0)
		{
			m_MapStatus = MAP_INIT;
			m_Story->SetTimer(500);
		}
	}
	else
	{
		m_BackSound->Stop();
		m_Story->DrawEnding(m_EndingNum);
		if (m_EndingNum == 0)
			m_HappyEnd->Play(0, 0, NULL);
		else
			m_BadEnd->Play(0, 0, NULL);
		if (m_Story->GetTimer() == 0)
			m_MapStatus = MAP_DESTORY;
	}
}

D3DCOLOR Map::GetBackColor() const
{
	return m_BackColor;
}

MAP_STATUS Map::GetMapStatus() const
{
	return m_MapStatus;
}

Story * Map::GetStory() const
{
	return m_Story;
}

void Map::Release()
{
	for (int index = 0; index < m_RolesNum; index++)
	{
		m_Roles[index]->Release();
		m_Roles[index] = NULL;
	}
	for (int index = 0; index < m_DataLen; index++)
	{
		if (m_Stone[index] != NULL)
		{
			m_Stone[index]->Release();
			m_Stone[index] = NULL;
		}
	}
	for (int index = 0; index < m_MushRoomIndex; index++)
	{
		if (m_MushRooms[index] != NULL)
		{
			m_MushRooms[index]->Release();
			m_MushRooms[index] = NULL;
		}
	}
	for (int index = 0; index < m_MonsterNum; index++)
	{
		if (m_Monsters[index] != NULL)
		{
			m_Monsters[index]->Release();
			m_Monsters[index] = NULL;
		}
	}
	if (m_InfoBar != NULL)
	{
		m_InfoBar->Release();
		m_InfoBar = NULL;
	}
	if (m_Story != NULL)
	{
		m_Story->Release();
		m_Story = NULL;
	}
	if (m_Guide != NULL)
	{
		m_Guide->Release();
		m_Guide = NULL;
	}
	if (m_BackSound != NULL)
	{
		m_BackSound->Release();
		m_BackSound = NULL;
	}
	if (m_KillMonster != NULL)
	{
		m_KillMonster->Release();
		m_KillMonster = NULL;
	}
	if (m_GetMushroom != NULL)
	{
		m_GetMushroom->Release();
		m_GetMushroom = NULL;
	}
	if (m_FindMushroom != NULL)
	{
		m_FindMushroom->Release();
		m_FindMushroom = NULL;
	}
	if (m_GetCoin != NULL)
	{
		m_GetCoin->Release();
		m_GetCoin = NULL;
	}
	if (m_StoneBroken != NULL)
	{
		m_StoneBroken->Release();
		m_StoneBroken = NULL;
	}
	if (m_Hurt != NULL)
	{
		m_Hurt->Release();
		m_Hurt = NULL;
	}
	if (m_HappyEnd != NULL)
	{
		m_HappyEnd->Release();
		m_HappyEnd = NULL;
	}
	if (m_BadEnd != NULL)
	{
		m_BadEnd->Release();
		m_BadEnd = NULL;
	}
	if (m_StoneNormal != NULL)
	{
		m_StoneNormal->Release();
		m_StoneNormal = NULL;
	}
}
