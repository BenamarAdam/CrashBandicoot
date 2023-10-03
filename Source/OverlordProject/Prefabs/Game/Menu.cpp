#include "stdafx.h"
#include "Menu.h"

#include "Controls.h"
#include "Components/AnimatedSpriteComponent.h"
#include "Scenes/Game/CrashGameScene.h"
#include "Scenes/Game/MenuScene.h"

Menu::Menu(MenuScene* menu)
	:m_MenuScene(menu)
{}

void Menu::Initialize(const SceneContext& sceneContext)
{
	m_SpriteFont = ContentManager::Load<SpriteFont>(L"Game/Font/Font48.fnt");

	
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/menu.mp3", FMOD_2D, 0, &m_MenuSound); 
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/theme.mp3", FMOD_2D|FMOD_LOOP_NORMAL, 0, &m_MenuTheme);
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/menuswitch.wav", FMOD_2D, 0, &m_ButtonSound);
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/select.wav", FMOD_2D, 0, &m_SelectSound);

	std::vector<std::wstring> fileLocs{};
	for (int i{ 1 }; i <= 194; ++i)
	{
		std::wstring str;
		str = L"Game/Textures/Menu/(" + std::to_wstring(i) + L").gif";
		fileLocs.push_back(str);
	}


	auto obj = AddChild(new GameObject());
	auto dae = obj->AddComponent(new SpriteComponent(L"Game/Textures/banner.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));
	dae->GetTransform()->Translate(XMFLOAT3{ 1280 / 2, 696 ,0});

	m_Ctrls = AddChild(new GameObject());
	m_Ctrls->AddComponent(new SpriteComponent(L"Game/Textures/Menu/crashctrl.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));
	m_Ctrls->GetTransform()->Translate(XMFLOAT3{ 1280 / 2, 720/2 ,0 });
	m_Ctrls->GetTransform()->Scale(0);

	obj = AddChild(new GameObject());
	auto sprite = obj->AddComponent(new AnimatedSpriteComponent(fileLocs, { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f },false));
	sprite->GetTransform()->Translate(1280/2, 720/2, 0);
	sprite->GetTransform()->Scale(2.15f, 2.15f, 0);
	sprite->SetFPS(20);

	SoundManager::Get()->GetSystem()->playSound(m_MenuSound, 0, false, 0);
	SoundManager::Get()->GetSystem()->playSound(m_MenuTheme, 0, false, &m_ThemeChannel);

	auto inputAction = InputAction(Input::jump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	sceneContext.pInput->AddInputAction(inputAction);
	
}

void Menu::Update(const SceneContext& sceneContext)
{
	
	auto pInput = sceneContext.pInput;
	if(m_Started)
	{
		m_ElapsedCtrlTime += sceneContext.pGameTime->GetElapsed();
		if (m_ElapsedCtrlTime > 5.f || pInput->IsActionTriggered(Input::jump))
		{
			m_ThemeChannel->stop();
			m_MenuScene->SwitchToGame();
		}
	}
	else
	{
		float gameTime = sceneContext.pGameTime->GetTotal();
		if (gameTime > 1.f)
		{
			if (pInput->IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_DPAD_UP) || pInput->IsKeyboardKey(InputState::pressed, 'W') || pInput->IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_DPAD_DOWN) || pInput->IsKeyboardKey(InputState::pressed, 'S'))
			{
				m_StartSelected = !m_StartSelected;
				SoundManager::Get()->GetSystem()->playSound(m_ButtonSound, 0, false, 0);
			}
			if (pInput->IsActionTriggered(Input::jump))
			{
				if (m_StartSelected)
				{
					SoundManager::Get()->GetSystem()->playSound(m_SelectSound, 0, false, 0);
					m_Started = true;
					m_Ctrls->GetTransform()->Scale(1.f);
					m_MenuSound->release();
				}
				else
					PostQuitMessage(-1);
			}
			float opacity = (1 + sin(gameTime * 4)) / 2;
			XMFLOAT4 defaultcolor{ 0.98f,0.27f,0.01f,0.2f };
			XMFLOAT4 colorhover = defaultcolor;
			colorhover.w = opacity;
			if (m_StartSelected)
			{
				TextRenderer::Get()->DrawText(m_SpriteFont, StringUtil::utf8_decode("Start Game"), XMFLOAT2{ 240,470 }, colorhover);
				TextRenderer::Get()->DrawText(m_SpriteFont, StringUtil::utf8_decode("Quit"), XMFLOAT2{ 240,520 }, defaultcolor);

			}
			else
			{
				TextRenderer::Get()->DrawText(m_SpriteFont, StringUtil::utf8_decode("Start Game"), XMFLOAT2{ 240,470 }, defaultcolor);
				TextRenderer::Get()->DrawText(m_SpriteFont, StringUtil::utf8_decode("Quit"), XMFLOAT2{ 240,520 }, colorhover);
			}
		}
	}
}

