#pragma once

#include "GameResource.h"
#include "Enum.h"

class MushroomMonster : public GameResource
{
public:
	MushroomMonster(LPDIRECT3DDEVICE9 pD3DDevice, char* fileName, float srcWidth, float srcHeight, float x, float y, short currentFrame);
	void UpdateData(char* pKey, float frictionFactor, float gravite, float dragX, float dragY);
	void UpdateFrame(char* pKey);
	MONSTER_STATUS GetStatus() const;
	void SetStatus(MONSTER_STATUS status);
	void SetVector(VECTOR_DIRECTION vector);
	void Back();
	RECT * GetPosRect(RECT * rect) const;
	short GetDeadCount() const;
private:
	VECTOR_DIRECTION m_Vector;
	float m_SpeedX;
	float m_SpeedY;
	short m_FramePass;
	MONSTER_STATUS m_Status;
	short m_DeadCount;
};