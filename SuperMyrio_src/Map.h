#pragma once

#include "Role.h"
#include "Stone.h"
#include "MushroomMonster.h"
#include "InfoBar.h"
#include "Story.h"
#include "Guide.h"

class Map
{
public:
	Map(LPDIRECT3DDEVICE9 pD3DDevice, LPDIRECTSOUND8 pDirectSound, float gravity, float FrictionFactor);
	~Map();
	short ImpactCheck(RECT* rect1, RECT* rect2);
	void ImpactHandle(char* pKey);
	void AddPlayer(LPDIRECT3DDEVICE9 pD3DDevice, LPDIRECTSOUND8 pDirectSound, short roleId);
	void Draw(LPD3DXSPRITE pD3DXSprite);
	D3DCOLOR GetBackColor() const;
	MAP_STATUS GetMapStatus() const;
	Story* GetStory() const;
	void Release();
private:
	float m_Gravity;
	float m_FrictionFactor;
	short m_DataLen;
	Role* m_Roles[4];
	Stone** m_Stone;
	short m_RolesNum;
	short m_RoleCountDown;
	short m_StoneCountDown;
	Reward* m_MushRooms[100];
	short m_MushRoomIndex;
	MushroomMonster** m_Monsters;
	short m_MonsterNum;
	short m_MonsterFrameCount;
	short m_TotalGreenMush;
	short m_GotGreenMush;
	InfoBar* m_InfoBar;
	D3DCOLOR m_BackColor;
	Story* m_Story;
	MAP_STATUS m_MapStatus;
	short m_EndingNum;
	short m_DeadTimer;
	Guide* m_Guide;
	short m_GameTimer;
	DWORD m_OldTime;
	DWORD m_CurrentTime;
	LPDIRECTSOUNDBUFFER m_BackSound;
	LPDIRECTSOUNDBUFFER m_KillMonster;
	LPDIRECTSOUNDBUFFER m_GetMushroom;
	LPDIRECTSOUNDBUFFER m_FindMushroom;
	LPDIRECTSOUNDBUFFER m_GetCoin;
	LPDIRECTSOUNDBUFFER m_StoneBroken;
	LPDIRECTSOUNDBUFFER m_StoneNormal;
	LPDIRECTSOUNDBUFFER m_Hurt;
	LPDIRECTSOUNDBUFFER m_HappyEnd;
	LPDIRECTSOUNDBUFFER m_BadEnd;
};