#pragma once
class EndScreen :
    public GameObject
{
public:
	EndScreen() = default;
	~EndScreen() = default;
	EndScreen(const EndScreen& other) = delete;
	EndScreen(EndScreen&& other) noexcept = delete;
	EndScreen& operator=(const EndScreen& other) = delete;
	EndScreen& operator=(EndScreen&& other) noexcept = delete;
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

