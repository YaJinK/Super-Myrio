#pragma once

#include "GameResource.h"
#include "Enum.h"

class Flag : public GameResource
{
public:
	Flag(LPDIRECT3DDEVICE9 pD3DDevice, char* fileName, float srcWidth, float srcHeight, float x, float y);
	void UpdateData(char* pKey, float frictionFactor, float gravite, float dragX, float dragY);
	void UpdateFrame(char* pKey);
};