#pragma once

#include "GameResource.h"
#include "Enum.h"

class Reward : public GameResource
{
public:
	Reward(LPDIRECT3DDEVICE9 pD3DDevice, char* fileName, float srcWidth, float srcHeight, float x, float y, short currentFrame);
	void UpdateData(char* pKey, float frictionFactor, float gravite, float dragX, float dragY);
	void UpdateFrame(char* pKey);
	VECTOR_DIRECTION GetVector() const;
	void SetVector(VECTOR_DIRECTION vector);
	RECT * GetPosRect(RECT * rect) const;
	REWARD_STATUS GetStatus() const;
	void SetStatus(REWARD_STATUS status);
private:
	REWARD_STATUS m_Status;
	VECTOR_DIRECTION m_Vector;
	float m_OldY;
	float m_SpeedY;
	float m_SpeedX;
};