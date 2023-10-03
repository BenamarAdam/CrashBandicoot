#pragma once
class CrashBandicoot;
class PostVignette;
class Character;

class CrashGameScene :
    public GameScene
{
    public:
	enum class GameState
	{
		menu,
		game,
		death,
		pause
	};
		CrashGameScene() :GameScene(L"CrashGameScene") {}
		~CrashGameScene() override;
		CrashGameScene(const CrashGameScene & other) = delete;
		CrashGameScene(CrashGameScene && other) noexcept = delete;
		CrashGameScene& operator=(const CrashGameScene & other) = delete;
		CrashGameScene& operator=(CrashGameScene && other) noexcept = delete;

		void EnableVignette(bool enabled);

		GameObject* AddToRoot(GameObject* obj);
		GameObject* GetRoot() { return m_Root; }
		GameObject* GetCrash() { return (GameObject*)m_Crash; }
		void SetGameState(GameState state);
		void InitGame();
		void DeleteDeathScene(GameScene* scene);
		void RemoveObject(GameObject* object);

		void SetCrashLoc(XMFLOAT3 crashLoc);
		void SetCheckPoint(XMFLOAT3 checkLoc) { m_CheckPoint = checkLoc; }
		XMFLOAT3 GetCheckPoint() { return m_CheckPoint; }

		
protected:
	void SetActive();
	void Initialize() override;
	void OnGUI() override;
	void Update() override;
private:
	bool m_MenuOff = false;
	GameScene* m_DeathScene{};
	FMOD::Channel* m_Theme{};
	FMOD::Sound* m_ThemeSound{};
	CrashBandicoot* m_Crash{};
	GameObject* m_Menu{};
	GameObject* m_Root{};
	GameObject* m_PauseMenu{};
	PostVignette* m_PostVignette{};
	GameState m_State{};
	XMFLOAT3 m_temp{};
	XMFLOAT3 m_CheckPoint{0,0,-20.f};
	RigidBodyComponent* m_temprb{};
	XMFLOAT2 m_Temprot{};

	//delete
	std::vector<GameObject*> m_ToBeDeleted{};
	bool m_ToDelete = false;
private:
	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump
	};
};

