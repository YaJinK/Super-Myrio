#include "Menu.h"

Menu::Menu(LPDIRECT3DDEVICE9 pD3DDevice, LPDIRECTSOUND8 pDirectSound, MenuItem * item, char* fileName)
{
	m_Item = item;
	m_OriginalItem = item;
	m_IsShow = true;
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(fileName, &info);
	D3DXCreateTextureFromFileEx(pD3DDevice, fileName, info.Width, info.Height, 1,
		D3DUSAGE_RENDERTARGET, info.Format, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		NULL, &info, NULL, &m_Background);
	//backsound
	WAVEFORMATEX waveFormat;
	ZeroMemory(&waveFormat, sizeof(waveFormat));
	waveFormat.nChannels = 2;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nBlockAlign = 4;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwBufferBytes = 88 * waveFormat.nAvgBytesPerSec;
	dsbd.lpwfxFormat = &waveFormat;
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY
		| DSBCAPS_GLOBALFOCUS;

	pDirectSound->CreateSoundBuffer(&dsbd, &m_BackSound, NULL);
	ifstream soundStream;
	soundStream.open("sound/menu.wav", ios::binary);
	LPVOID p;
	DWORD len;
	m_BackSound->Lock(0, 0, &p, &len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	soundStream.read((char*)p, len);
	m_BackSound->Unlock(&p, len, NULL, NULL);
	soundStream.close();
	m_BackSound->Play(0, 0, DSBPLAY_LOOPING);
}

Menu::~Menu()
{
	if (m_Background != NULL)
	{
		m_Background->Release();
		m_Background = NULL;
	}
	if (m_OriginalItem != NULL)
	{
		m_OriginalItem->Release();
		m_OriginalItem = NULL;
		m_Item = NULL;
	}
	if (m_BackSound != NULL)
	{
		m_BackSound->Release();
		m_BackSound = NULL;
	}
}

bool Menu::GetShow() const
{
	return m_IsShow;
}

MenuItem * Menu::GetItem() const
{
	return m_Item;
}

void Menu::SetItem(MenuItem * item)
{
	m_Item = item;
}

void Menu::SetShow(bool show)
{
	m_IsShow = show;
	if (m_IsShow)
	{
		m_BackSound->SetCurrentPosition(0);
		m_BackSound->Play(0, 0, DSBPLAY_LOOPING);
	}
	else
		m_BackSound->Stop();
}

void Menu::Init()
{
	m_Item = m_OriginalItem;
}

void Menu::Draw(LPD3DXSPRITE pD3DXSprite)
{
	D3DXVECTOR3 pos = { 0, 0, 0 };
	pD3DXSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pD3DXSprite->Draw(m_Background, NULL, NULL, &pos, D3DCOLOR_XRGB(255, 255, 255));
	pD3DXSprite->End();
	
}

void Menu::Release()
{
	if (m_Background != NULL)
	{
		m_Background->Release();
		m_Background = NULL;
	}
	if (m_OriginalItem != NULL)
	{
		m_OriginalItem->Release();
		m_OriginalItem = NULL;
		m_Item = NULL;
	}
	if (m_BackSound != NULL)
	{
		m_BackSound->Release();
		m_BackSound = NULL;
	}
}
