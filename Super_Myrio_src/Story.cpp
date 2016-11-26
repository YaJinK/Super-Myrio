#include "Story.h"

Story::Story(LPDIRECT3DDEVICE9 pD3DDevice)
{
	m_Begin = new char*[1];
	m_Begin[0] = "    碧琪公主得了很严重的病，勇敢的马里奥，快去野外寻找拯救公主的蘑菇吧！要小心成群的怪物和有毒的蘑菇！！";
	m_Ending = new char*[3];
	m_Ending[0] = "    马里奥成功的带回了蘑菇，碧琪公主恢复了以往的神采，和马利奥在城堡里过上了幸福的生活。";
	m_Ending[1] = "    碧琪公主日夜盼着马里奥回来，却不知道她永远也见不到马里奥了。几天之后，由于病情的加重，碧琪公主也永远的离开了这个世界。";
	m_Ending[2] = "    马里奥急切的赶回了城堡，却没能带回足够的蘑菇。碧琪公主没有埋怨他，说只要马里奥平安回来就已经很高兴了。但是，国王下令秘密处死马里奥，尸体被士兵抛在了野外。";
	D3DXCreateFont(pD3DDevice, 45, 0, 0, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "", &m_StoryFont);
	m_Area.left = 50;
	m_Area.right = m_Area.left + 700;
	m_Area.top = 120;
	m_Area.bottom = m_Area.top + 500;
	m_BackColor = D3DCOLOR_XRGB(255, 255, 255);
	m_Timer = 400;
}

Story::~Story()
{
	for (short index = 0; index < 1; index++)
	{
		delete m_Begin[index];
	}
	delete m_Begin;
	for (short index = 0; index < 3; index++)
	{
		delete m_Ending[index];
	}
	delete m_Ending;
	if (m_StoryFont != NULL)
	{
		m_StoryFont->Release();
		m_StoryFont = NULL;
	}
}

void Story::DrawBegin()
{
	m_StoryFont->DrawTextA(NULL, m_Begin[0], (int)strlen(m_Begin[0]), &m_Area, DT_WORDBREAK | DT_LEFT, D3DCOLOR_XRGB(0, 0, 0));
	m_Timer--;
}

void Story::DrawEnding(short endingNum)
{
	m_StoryFont->DrawTextA(NULL, m_Ending[endingNum], (int)strlen(m_Ending[endingNum]), &m_Area, DT_WORDBREAK | DT_LEFT, D3DCOLOR_XRGB(0, 0, 0));
	m_Timer--;
}

short Story::GetTimer() const
{
	return m_Timer;
}

void Story::SetTimer(short timer)
{
	m_Timer = timer;
}

D3DCOLOR Story::GetBackColor() const
{
	return m_BackColor;
}

void Story::Release()
{
	delete[] m_Begin;
	delete[] m_Ending;
	if (m_StoryFont != NULL)
	{
		m_StoryFont->Release();
		m_StoryFont = NULL;
	}
}
