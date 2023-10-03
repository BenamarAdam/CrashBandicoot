#pragma once

class CrashBandicoot;

class HUD :
    public GameObject
{
public:
	HUD() = default;
	~HUD() = default;
	HUD(const HUD& other) = delete;
	HUD(HUD&& other) noexcept = delete;
	HUD& operator=(const HUD& other) = delete;
	HUD& operator=(HUD&& other) noexcept = delete;
	void AddWumpa(int amt);
	void AddLife();
	void SetCrash(CrashBandicoot* crash) { m_Crash = crash; }
	void CheckPoint();
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	FMOD::Sound* m_WumpaSound{};
	SpriteFont* m_SpriteFont{};
	CrashBandicoot* m_Crash{};
	float m_ElapsedCounterTime{},
	m_ElapsedSlideTime{},
	m_ElapsedShownTime{},
	m_yPos{};
	int m_WumpaToAdd{};

	bool m_IsShowing{}
	, m_SlideUp = false;
	GameObject* m_WumpaSprite{},
		m_LifeSprite{};

	bool m_CheckPoint = false;
	float m_ElapsedCheckTime{};
};

