#include "stdafx.h"
#include "MenuScene.h"

#include "Prefabs/Game/Menu.h"
#include "Scenes/Game/CrashGameScene.h"

void MenuScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;
}

void MenuScene::Update()
{
	

}

void MenuScene::OnGUI()
{
	
}

void MenuScene::OnSceneActivated()
{
	m_Menu = AddChild(new Menu(this));
	SceneManager::Get()->RemoveGameScene(m_Game, true);
}

void MenuScene::OnSceneDeactivated()
{
	RemoveChild(m_Menu, true);
}

void MenuScene::SwitchToGame()
{
	m_Game = new CrashGameScene();
	SceneManager::Get()->AddGameScene(m_Game);
	SceneManager::Get()->SetActiveGameScene(L"CrashGameScene");
	dynamic_cast<CrashGameScene*>(m_Game)->SetGameState(CrashGameScene::GameState::game);
}