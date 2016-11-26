#include "Story.h"

Story::Story(LPDIRECT3DDEVICE9 pD3DDevice)
{
	m_Begin = new char*[1];
	m_Begin[0] = "    �����������˺����صĲ����¸ҵ�����£���ȥҰ��Ѱ�����ȹ�����Ģ���ɣ�ҪС�ĳ�Ⱥ�Ĺ�����ж���Ģ������";
	m_Ending = new char*[3];
	m_Ending[0] = "    ����³ɹ��Ĵ�����Ģ�������������ָ�����������ɣ����������ڳǱ���������Ҹ������";
	m_Ending[1] = "    ����������ҹ��������»�����ȴ��֪������ԶҲ������������ˡ�����֮�����ڲ���ļ��أ���������Ҳ��Զ���뿪��������硣";
	m_Ending[2] = "    ����¼��еĸϻ��˳Ǳ���ȴû�ܴ����㹻��Ģ������������û����Թ����˵ֻҪ�����ƽ���������Ѿ��ܸ����ˡ����ǣ������������ܴ�������£�ʬ�屻ʿ��������Ұ�⡣";
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
