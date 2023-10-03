#pragma once
class CrashGameScene;
class CrashBandicoot;

class Crate : public GameObject
{
public:
	Crate(XMFLOAT3 location);
	~Crate()=default;

	Crate(const Crate& other) = delete;
	Crate(Crate&& other) noexcept = delete;
	Crate& operator=(const Crate& other) = delete;
	Crate& operator=(Crate&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	void OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);
	void SpawnParticle();
	XMFLOAT3 m_Location{};
	GameObject *m_TopTrigger{}
	, *m_SpinTrigger{}
	, *m_CrateModel;

	CrashBandicoot* m_Crash = nullptr;
	FMOD::Sound* m_Sound{};
	bool m_CrashInside = false;
	bool m_Delete = false;
};

