#include "stdafx.h"
#include "WinScene.h"

#include "Prefabs/Game/EndScreen.h"
#include "Scenes/Game/CrashGameScene.h"

void WinScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;
}

void WinScene::Update()
{


}

void WinScene::OnGUI()
{

}

void WinScene::OnSceneActivated()
{
	m_Win = AddChild(new EndScreen());
	SceneManager::Get()->RemoveGameScene(m_Game, true);
}

void WinScene::OnSceneDeactivated()
{
	RemoveChild(m_Win, true);
}

void WinScene::SwitchToGame()
{
	auto scene = new CrashGameScene();
	scene->DeleteDeathScene(this);
	SceneManager::Get()->AddGameScene(scene);
	SceneManager::Get()->SetActiveGameScene(L"CrashGameScene");
	scene->SetGameState(CrashGameScene::GameState::game);
}