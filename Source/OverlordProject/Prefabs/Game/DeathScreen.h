#pragma once
class DeathScreen : public GameObject
{

public:
	DeathScreen() = default;
	~DeathScreen() = default;
	DeathScreen(const DeathScreen& other) = delete;
	DeathScreen(DeathScreen&& other) noexcept = delete;
	DeathScreen& operator=(const DeathScreen& other) = delete;
	DeathScreen& operator=(DeathScreen&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	enum Selection
	{
		restart,
		menu,
		quit
	};
	Selection m_Selection = restart;
	FMOD::Sound* m_ButtonSound{},
		* m_SelectSound{};
	SpriteFont* m_SpriteFont{};
};

