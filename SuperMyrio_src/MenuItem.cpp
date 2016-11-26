#include "MenuItem.h"

MenuItem::MenuItem(LPDIRECT3DDEVICE9 pD3DDevice, char* data, char* id)
{
	m_Data = data;
	m_ItemID = id;
	m_SubNum = 0;
	m_CurrentItem = 0;
	D3DXCreateFont(pD3DDevice, 27, 0, FW_BOLD, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "свт╡", &m_Font);
	m_KeyTimer = 1;
	ZeroMemory(&m_SubItems, sizeof(m_SubItems));
	m_Parent = NULL;
}

MenuItem::~MenuItem()
{
	if (m_Font != NULL)
	{
		m_Font->Release();
		m_Font = NULL;
	}
	for (short index = 0; index < m_SubNum; index++)
	{
		if (m_SubItems[index] != NULL)
		{
			m_SubItems[index]->Release();
			m_SubItems[index] = NULL;
		}
	}
}

void MenuItem::AddItem(MenuItem* item)
{
	if (m_SubNum < 10)
	{
		m_SubItems[m_SubNum] = item;
		m_SubNum++;
		item->m_Parent = this;
	}
}

void MenuItem::Draw()
{
	RECT rect;
	rect.left = 240;
	rect.right = rect.left + 300;
	rect.top = 350;
	rect.bottom = rect.top + 100;
	for (short itemIndex = 0; itemIndex < m_SubNum; itemIndex++)
	{
		D3DCOLOR itemColor = D3DCOLOR_XRGB(110, 110, 110);
		if (itemIndex == m_CurrentItem)
			itemColor = D3DCOLOR_XRGB(255, 50, 50);
		m_Font->DrawTextA(NULL, m_SubItems[itemIndex]->m_Data, (int)strlen(m_SubItems[itemIndex]->m_Data), &rect, DT_CENTER, itemColor);
		rect.top += 50;
		rect.bottom = rect.top + 100;
	}
}

char * MenuItem::GetData() const
{
	return m_Data;
}

char * MenuItem::GetItemId() const
{
	return m_ItemID;
}

MenuItem* MenuItem::Update(char * pKey)
{
	m_KeyTimer--;
	if (m_KeyTimer == 0)
	{
		if (pKey[DIK_W] & 0x80 || pKey[DIK_UP] & 0x80)
			m_CurrentItem--;
		if (pKey[DIK_S] & 0x80 || pKey[DIK_DOWN] & 0x80)
			m_CurrentItem++;
		
		if (m_CurrentItem < 0)
			m_CurrentItem = m_SubNum - 1;
		if (m_CurrentItem >= m_SubNum)
			m_CurrentItem = 0;
		m_KeyTimer = 5;
		if (pKey[DIK_J] & 0x80 || pKey[DIK_RETURN] & 0x80)
		{
			if (m_CurrentItem == m_SubNum - 1)
				return m_Parent;
			else
			{
				return m_SubItems[m_CurrentItem];
			}
		}
		if (pKey[DIK_K] & 0x80 || pKey[DIK_ESCAPE] & 0x80)
		{
			return m_Parent;
		}
	}
	return this;
}

void MenuItem::Release()
{
	if (m_Font != NULL)
	{
		m_Font->Release();
		m_Font = NULL;
	}
	for (short index = 0; index < m_SubNum; index++)
	{
		if (m_SubItems[index] != NULL)
		{
			m_SubItems[index]->Release();
			m_SubItems[index] = NULL;
		}
	}
}
