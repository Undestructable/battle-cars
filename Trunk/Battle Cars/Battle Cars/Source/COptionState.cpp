
#include "COptionState.h"
#include "CMainMenuState.h"
#include "CGame.h"
#include "CPrintFont.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CXboxInput.h"
enum options{WS_EFFECTS,WS_MUSIC,WS_INPUT,WS_EXIT};

COptionState::COptionState(void)
{
	m_pD3D = NULL;
	m_pTM = NULL;
	m_pPF = NULL;
	m_pDI = NULL;
	m_pFM = NULL;
}

COptionState::~COptionState()
{

}

COptionState* COptionState::GetInstance(void)
{
	static COptionState instance;
	return &instance;
}

void COptionState::Enter(void)
{
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_nFontID = m_pTM->LoadTexture("resource/graphics/FontPlaceholder.png",D3DCOLOR_XRGB(0, 0, 0));

	m_pPF = new CPrintFont(m_nFontID);
	m_pController1 = CGame::GetInstance()->GetController1();
	m_pController2 = CGame::GetInstance()->GetController2();
	m_pFM = CSGD_FModManager::GetInstance();
	m_nBackgroundMusicID = m_pFM->LoadSound("resource/sounds/FeelSoNumb.mp3",SGD_FMOD_LOOPING);
	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	m_pFM->SetVolume(m_nBackgroundMusicID,0.5f);
	//m_pFM->PlaySound(m_nBackgroundMusicID);

}

bool COptionState::Input(void)
{
	if(CGame::GetInstance()->ControllerInput())
	{
		//m_pController1->ReadInputState();
		XINPUT_STATE xState = m_pController1->GetState();
		BYTE rTrig = xState.Gamepad.bRightTrigger;
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;
		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetInputDelay();
		if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			return this->HandleEnter();
		}
		else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			CGame::GetInstance()->RemoveState(this);
		}
		}
		if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetThumbDelay();
		if(x < 8000 && x > -8000 && y > 16000)
		{
			
			m_nSelection--;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection < 0)
				m_nSelection = 3;
		}
		else if(x < 8000 && x > -8000 && y < -16000)
		{
			m_nSelection++;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection > 3)
				m_nSelection = 0;
		}
		}
	}
	else
	{

	if(m_pDI->KeyPressed(DIK_ESCAPE))
	{
		CGame::GetInstance()->RemoveState(this);
	}
	if(m_pDI->KeyPressed(DIK_RETURN))
	{
		return this->HandleEnter();
	}
	if(m_pDI->KeyPressed(DIK_UP))
	{
		m_nSelection--;
		m_pFM->PlaySound(m_nMenuMove);
		if(m_nSelection < 0)
			m_nSelection = 3;
	}

	if(m_pDI->KeyPressed(DIK_DOWN))
	{
		m_nSelection++;
		m_pFM->PlaySound(m_nMenuMove);
		if(m_nSelection > 3)
			m_nSelection = 0;
	}
	}
	return true;
}

void COptionState::Update(float fElapsedTime)
{
	m_pFM->Update();
}

void COptionState::Render(void)
{								 //
	RECT temp;					 ////
	temp.left = 0;				 //////
	temp.top = 0;				 // replace this with background image
	temp.right = 800;			 ////////
	temp.bottom = 600;			 //////
	m_pD3D->GetSprite()->Flush();////
	m_pD3D->DrawRect(temp,0,0,0);//
	
	m_pPF->Print("OPTIONS",220,50,1.0f,D3DCOLOR_XRGB(200, 0, 0));

	m_pPF->Print("EFFECTS",300,200,0.5f,D3DCOLOR_XRGB(200, 0, 0));
	m_pPF->Print("MUSIC",300,250,0.5f,D3DCOLOR_XRGB(200, 0, 0));	
	m_pPF->Print("INPUT DEVICE",300,300,0.5f,D3DCOLOR_XRGB(200, 0, 0));
	m_pPF->Print("EXIT",300,350,0.5f,D3DCOLOR_XRGB(200, 0, 0));

	switch(m_nSelection)
		{
		case WS_EFFECTS:			
			m_pPF->Print("EFFECTS",300,200,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		case WS_MUSIC:
			m_pPF->Print("MUSIC",300,250,0.5f,D3DCOLOR_XRGB(0, 255, 0));	
			break;
		case WS_INPUT:
			m_pPF->Print("INPUT DEVICE",300,300,0.5f,D3DCOLOR_XRGB(0, 255, 0));	
			break;
		case WS_EXIT:
			m_pPF->Print("EXIT",300,350,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		}
}

void COptionState::Exit(void)
{
	//m_pFM->StopSound(m_nBackgroundMusicID);
	m_pTM->UnloadTexture(m_nFontID);
	delete m_pPF;
}

bool COptionState::HandleEnter(void)
{
	m_pFM->PlaySound(m_nMenuSelect);
		if(m_nSelection == WS_EXIT)
		{
			CGame::GetInstance()->RemoveState(this);
		}
	return true;
}