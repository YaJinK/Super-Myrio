#include "Flag.h"

Flag::Flag(LPDIRECT3DDEVICE9 pD3DDevice, char * fileName,  float srcWidth, float srcHeight, float x, float y) : GameResource(pD3DDevice, fileName, srcWidth, srcHeight, x, y)
{
	m_Src.left = 0;
	m_Src.right = srcWidth;
	m_Src.top = 0;
	m_Src.bottom = srcHeight;
}

void Flag::UpdateData(char * pKey, float frictionFactor, float gravite, float dragX, float dragY)
{
}

void Flag::UpdateFrame(char * pKey)
{
}
