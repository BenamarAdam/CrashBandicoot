#include "stdafx.h"
#include "HUD.h"

#include "Components/AnimatedSpriteComponent.h"
#include "Prefabs/Game/CrashBandicoot.h"
void HUD::Initialize(const SceneContext&)
{
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/blip.mp3", FMOD_2D, 0, &m_WumpaSound);

	m_SpriteFont = ContentManager::Load<SpriteFont>(L"Game/Font/Font48.fnt");
	
	std::vector<std::wstring> fileLocs{};
	for (int i{ 1 }; i <= 75; ++i)
	{
		std::wstring str;
		str = L"Game/Textures/HUD/Wumpa/(" + std::to_wstring(i) + L").gif";
		fileLocs.push_back(str);
	}
	m_WumpaSprite = AddChild(new GameObject());
	auto sprite = m_WumpaSprite->AddComponent(new AnimatedSpriteComponent(fileLocs, { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));
	sprite->GetTransform()->Translate(70, 60, 0);
	sprite->GetTransform()->Scale(0.6f, 0.6f, 0.f);
	sprite->SetFPS(30);
	fileLocs.clear();
	for (int i{ 1 }; i <= 56; ++i)
	{
		std::wstring str;
		str = L"Game/Textures/HUD/Life/(" + std::to_wstring(i) + L").gif";
		fileLocs.push_back(str);
	}
	m_WumpaSprite = AddChild(new GameObject());
	sprite = m_WumpaSprite->AddComponent(new AnimatedSpriteComponent(fileLocs, { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));
	sprite->GetTransform()->Translate(1280-70, 60, 0);
	sprite->GetTransform()->Scale(0.5f, 0.5f, 0.f);
	sprite->SetFPS(30);

	GetTransform()->Translate(0, -130, 0);
}

void HUD::Update(const SceneContext& sceneContext)
{
	if(m_CheckPoint)
	{
		m_ElapsedCheckTime += sceneContext.pGameTime->GetElapsed();
		if(m_ElapsedCheckTime>=2.f)
		{
			m_CheckPoint = false;
			m_ElapsedCheckTime = 0;
			return;
		}
		XMFLOAT4 defaultcolor{ 0.98f,0.27f,0.01f,1.f };
		TextRenderer::Get()->DrawText(m_SpriteFont, L"CHECKPOINT!", XMFLOAT2{1280/2-100.f,720/2}, defaultcolor);

	}
		XMFLOAT3 pos = GetTransform()->GetWorldPosition();
	if(m_IsShowing)
	{
	float dt = sceneContext.pGameTime->GetElapsed();
	
	m_ElapsedSlideTime += dt;
		if(m_ElapsedSlideTime<1.f)
		{
			if (!m_SlideUp)			
				m_yPos += dt * 130;
			
			else
				m_yPos -= dt * 130;
			pos.y = -130 + m_yPos;
				GetTransform()->Translate(pos);
		}
		else if(!m_SlideUp)
		{
			m_ElapsedShownTime += dt;
			if(m_ElapsedShownTime>0.05f)
			{
				if (m_WumpaToAdd > 0)
				{
					--m_WumpaToAdd;
					SoundManager::Get()->GetSystem()->playSound(m_WumpaSound, 0, false, 0);

				}
				if (m_WumpaToAdd == 0) {
					if (m_ElapsedShownTime > 4.f)
					{
						m_SlideUp = true;
						m_ElapsedSlideTime = 0;
						
					}
					if (m_Crash->GetWumpaAmt() >= 100)
					{
						m_Crash->AddLife();
						m_Crash->SetWumpa(0);
					}
				}
				else {
					m_ElapsedShownTime = 0.f;
				}
			}

		}
		else
		{
			m_SlideUp = false;
			m_IsShowing = false;
			m_ElapsedShownTime = 0;
			m_ElapsedSlideTime = 0;
		}
		XMFLOAT4 defaultcolor{ 0.98f,0.27f,0.01f,1.f };
		TextRenderer::Get()->DrawText(m_SpriteFont, std::to_wstring(m_Crash->GetWumpaAmt()-m_WumpaToAdd), XMFLOAT2{ 140,pos.y+30 }, defaultcolor);
		TextRenderer::Get()->DrawText(m_SpriteFont, std::to_wstring(m_Crash->GetLives()), XMFLOAT2{ 1280-180,pos.y+30 }, defaultcolor);

	}
}

void HUD::AddWumpa(int amt)
{
	if (m_SlideUp)
	{
		m_yPos = 0;
		m_ElapsedSlideTime = 0;
	}
	m_IsShowing = true;
	m_WumpaToAdd = amt;
	m_SlideUp = false;
	m_ElapsedShownTime = 0;
	
}
void HUD::AddLife()
{
	if (m_SlideUp)
	{
		m_yPos = 0;
		m_ElapsedSlideTime = 0;
	}
	m_IsShowing = true;
	m_SlideUp = false;
	m_ElapsedShownTime = 0;
	
}

void HUD::CheckPoint()
{
	m_CheckPoint = true;
}
