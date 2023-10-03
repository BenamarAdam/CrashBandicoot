#pragma once

class DeathScene :
	public GameScene
{
public:

	DeathScene() :GameScene(L"DeathScene") {}
	~DeathScene() override = default;
	DeathScene(const DeathScene& other) = delete;
	DeathScene(DeathScene&& other) noexcept = delete;

	DeathScene& operator=(const DeathScene& other) = delete;
	DeathScene& operator=(DeathScene&& other) noexcept = delete;

	void SwitchToGame();
	void SetGameScene(GameScene* scene) { m_Game = scene; }
	void SetCheckPoint(XMFLOAT3 loc) { m_CheckPoint = loc; }
protected:
	void Initialize() override;
	void OnGUI() override;
	void Update() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
private:
	GameScene* m_Game{};
	XMFLOAT3 m_CheckPoint{};
};

