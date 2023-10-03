#pragma once
class MenuScene;
class CrashGameScene;

class Menu :
    public GameObject
{
	
public:
	Menu(MenuScene* menu);
	~Menu() = default;
	Menu(const Menu& other) = delete;
	Menu(Menu&& other) noexcept = delete;
	Menu& operator=(const Menu& other) = delete;
	Menu& operator=(Menu&& other) noexcept = delete;

	void SwitchToGame();
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	void Reset();


	FMOD::Sound* m_MenuSound{},
		* m_MenuTheme{},
	*m_ButtonSound{},
	*m_SelectSound{};
	FMOD::Channel* m_ThemeChannel{};
	SpriteFont* m_SpriteFont{};
	bool m_StartSelected = true;
	bool m_Started = false;
	float m_ElapsedCtrlTime{};
	GameObject* m_Ctrls{};
	MenuScene* m_MenuScene{};
	
};

