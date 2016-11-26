#pragma once

#include "GameResource.h"
#include "Enum.h"
#include "Reward.h"

class Stone : public GameResource
{
public:
	Stone(LPDIRECT3DDEVICE9 pD3DDevice, char* fileName, float srcWidth, float srcHeight, float x, float y, short currentFrame);
	void UpdateData(char* pKey, float frictionFactor, float gravite, float dragX, float dragY);
	void UpdateFrame(char* pKey);
	void Destory();
	float GetDragY() const;
	float GetDragX() const;
	float GetXPass() const;
	Reward* GetReward() const;
	void Action(ROLE_STATUS roleStatus);
	STONE_STATUS GetStatus() const;
	void SetStatus(STONE_STATUS status);
private:
	float m_DragX;
	float m_DragY;
	float m_OldY;
	float m_OldX;
	float m_XPass;
	STONE_STATUS m_Status;
	short m_FrameAdd;
	Reward* m_Reward;
};