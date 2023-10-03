#pragma once

class MenuScene :
	public GameScene
{
public:

	MenuScene() :GameScene(L"MenuScene") {}
	~MenuScene() override = default;
	MenuScene(const MenuScene& other) = delete;
	MenuScene(MenuScene&& other) noexcept = delete;
	
	MenuScene& operator=(const MenuScene& other) = delete;
	MenuScene& operator=(MenuScene&& other) noexcept = delete;

	void SwitchToGame();
protected:
	void Initialize() override;
	void OnGUI() override;
	void Update() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
private:
	GameObject* m_Menu{};
	GameScene* m_Game{};
};

