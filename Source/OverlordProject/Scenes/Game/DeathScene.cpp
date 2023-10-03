#include "stdafx.h"
#include "DeathScene.h"

#include "CrashGameScene.h"
#include "Prefabs/Game/DeathScreen.h"

void DeathScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;
}

void DeathScene::Update()
{


}

void DeathScene::OnGUI()
{

}

void DeathScene::OnSceneActivated()
{
	AddChild(new DeathScreen());
	SceneManager::Get()->RemoveGameScene(m_Game,true);
}

void DeathScene::OnSceneDeactivated()
{
}

void DeathScene::SwitchToGame()
{
	auto scene = new CrashGameScene();
	scene->SetCheckPoint(m_CheckPoint);

	scene->DeleteDeathScene(this);
	SceneManager::Get()->AddGameScene(scene);
	SceneManager::Get()->SetActiveGameScene(L"CrashGameScene");
	scene->SetGameState(CrashGameScene::GameState::game);
}