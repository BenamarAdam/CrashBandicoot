#include "stdafx.h"
#include "EndScreen.h"

#include "Scenes/Game/WinScene.h"

void EndScreen::Initialize(const SceneContext&)//sceneContext)
{
	m_SpriteFont = ContentManager::Load<SpriteFont>(L"Game/Font/Font48.fnt");

	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/menuswitch.wav", FMOD_2D, 0, &m_ButtonSound);
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/select.wav", FMOD_2D, 0, &m_SelectSound);

	auto obj = AddChild(new GameObject());
	auto sprite = obj->AddComponent(new SpriteComponent(L"Game/Textures/Menu/End.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));
	sprite->GetTransform()->Translate(1280 / 2, 720 / 2, 0);

}

void EndScreen::Update(const SceneContext& sceneContext)
{
	auto pInput = sceneContext.pInput;
	float gameTime = sceneContext.pGameTime->GetTotal();
	float opacity = (1 + sin(gameTime * 4)) / 2;
	XMFLOAT4 menucolor{ 0.98f,0.27f,0.01f,0.2f };
	XMFLOAT4 restartcolor = menucolor;
	XMFLOAT4 quitcolor = menucolor;
	switch (m_Selection)
	{
	case restart:
		restartcolor.w = opacity;
		break;
	case menu:
		menucolor.w = opacity;
		break;
	case quit:
		quitcolor.w = opacity;
		break;
	}
	TextRenderer::Get()->DrawText(m_SpriteFont, StringUtil::utf8_decode("Restart"), XMFLOAT2{ 1280 / 2 - 50,470 }, restartcolor);
	TextRenderer::Get()->DrawText(m_SpriteFont, StringUtil::utf8_decode("Menu"), XMFLOAT2{ 1280 / 2 - 30,520 }, menucolor);
	TextRenderer::Get()->DrawText(m_SpriteFont, StringUtil::utf8_decode("Quit"), XMFLOAT2{ 1280 / 2 - 30,570 }, quitcolor);


	if (pInput->IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_DPAD_UP) || pInput->IsKeyboardKey(InputState::pressed, 'W'))
	{
		switch (m_Selection)
		{
		case restart:
			m_Selection = quit;
			break;
		case menu:
			m_Selection = restart;
			break;
		case quit:
			m_Selection = menu;
			break;
		}
		SoundManager::Get()->GetSystem()->playSound(m_ButtonSound, 0, false, 0);
	}

	if (pInput->IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_DPAD_DOWN) || pInput->IsKeyboardKey(InputState::pressed, 'S'))
	{
		switch (m_Selection)
		{
		case restart:
			m_Selection = menu;
			break;
		case menu:
			m_Selection = quit;
			break;
		case quit:
			m_Selection = restart;
			break;
		}
		SoundManager::Get()->GetSystem()->playSound(m_ButtonSound, 0, false, 0);
	}
	if (pInput->IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_A) || pInput->IsKeyboardKey(InputState::pressed, VK_SPACE))
	{

		switch (m_Selection)
		{
		case restart:
			dynamic_cast<WinScene*>(GetScene())->SwitchToGame();

			return;
		case menu:
			SceneManager::Get()->SetActiveGameScene(L"MenuScene");
			return;
		case quit:
			PostQuitMessage(-1);
			return;
		}
		SoundManager::Get()->GetSystem()->playSound(m_SelectSound, 0, false, 0);
	}
}

