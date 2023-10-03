#pragma once
class PauseMenu : public GameObject
{

public:
	PauseMenu() = default;
	~PauseMenu() = default;
	PauseMenu(const PauseMenu& other) = delete;
	PauseMenu(PauseMenu&& other) noexcept = delete;
	PauseMenu& operator=(const PauseMenu& other) = delete;
	PauseMenu& operator=(PauseMenu&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	enum Selection
	{
		resume,
		menu,
		quit
	};
	Selection m_Selection = resume;
	FMOD::Sound* m_ButtonSound{},
		* m_SelectSound{};
	SpriteFont* m_SpriteFont{};
};

