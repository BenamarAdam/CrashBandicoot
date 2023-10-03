#pragma once

class WinScene :
	public GameScene
{
public:

	WinScene() :GameScene(L"WinScene") {}
	~WinScene() override = default;
	WinScene(const WinScene& other) = delete;
	WinScene(WinScene&& other) noexcept = delete;

	WinScene& operator=(const WinScene& other) = delete;
	WinScene& operator=(WinScene&& other) noexcept = delete;

	void SetGameScene(GameScene* scene) { m_Game = scene; }
	void SwitchToGame();
protected:
	void Initialize() override;
	void OnGUI() override;
	void Update() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
private:
	GameObject* m_Win{};
	GameScene* m_Game{};
};

