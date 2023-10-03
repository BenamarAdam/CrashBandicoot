#pragma once
class CrashBandicoot;

class CheckPoint : public GameObject
{
public:
	CheckPoint(XMFLOAT3 location);
	~CheckPoint() = default;

	CheckPoint(const CheckPoint& other) = delete;
	CheckPoint(CheckPoint&& other) noexcept = delete;
	CheckPoint& operator=(const CheckPoint& other) = delete;
	CheckPoint& operator=(CheckPoint&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	void OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);
	void SpawnParticle();
	XMFLOAT3 m_Location{};
	GameObject* m_TopTrigger{}
		, * m_SpinTrigger{}
	, * m_CheckPointModel;

	CrashBandicoot* m_Crash = nullptr;
	
	FMOD::Sound* m_Sound{}
	,*m_CheckSound{};
	bool m_CrashInside = false;
};

