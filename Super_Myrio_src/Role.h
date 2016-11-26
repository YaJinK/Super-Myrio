#pragma once

#include "GameResource.h"
#include "Enum.h"
#include <dsound.h>
#include <fstream>
using std::ifstream;
using std::ios;

#pragma comment(lib, "dsound.lib")

class Role: public GameResource
{
public:
	Role(LPDIRECT3DDEVICE9 pD3DDevice, LPDIRECTSOUND8 pDirectSound, char* fileName, float mass, short vector, float srcWidth, float srcHeight, float x, float y);
	~Role();
	void SetIsInAir(bool isInAir);
	void SetSpeedX(float speedX);
	void SetSpeedY(float speedY);
	void SetTractioinY(float tractionY);
	RECT* GetPosRect(RECT* rect) const;
	void ToAdult();
	void ToChild();
	short GetIsInvincible() const;
	void SetIsInvincible(bool isInvincible);
	float GetSpeedX() const;
	short GetVector() const;
	void Dead();
	ROLE_STATUS GetStatus() const;
	void UpdateData(char* pKey, float frictionFactor, float gravite, float dragX, float dragY);
	void UpdateFrame(char* pKey);
	short GetACCount() const;
	void Draw(LPD3DXSPRITE pD3DXSprite);
	void Release();
private:
	float m_Mass;
	float m_TractionX;
	float m_MaxTractionX;
	float m_SpeedX;
	float m_MaxSpeedX;
	float m_TractionY;
	float m_MaxTractionY;
	float m_SpeedY;
	float m_StandY;
	short m_FramePass;
	ROLE_STATUS m_Status;
	short m_PosRectTopPass;
	short m_Vector;
	bool m_IsInAir;
	bool m_IsDown;
	short m_AdultChildCount;
	bool m_IsDeadSeted;
	short m_InvincibleTime;
	short m_IsInvincible;
	LPDIRECTSOUNDBUFFER m_Dead;
	LPDIRECTSOUNDBUFFER m_Jump;
};