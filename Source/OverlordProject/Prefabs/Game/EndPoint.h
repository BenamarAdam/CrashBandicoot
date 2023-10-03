#pragma once
class EndPoint : public GameObject
{
public:
	EndPoint(XMFLOAT3 loc);
	~EndPoint() = default;
	EndPoint(const EndPoint& other) = delete;
	EndPoint(EndPoint&& other) noexcept = delete;
	EndPoint& operator=(const EndPoint& other) = delete;
	EndPoint& operator=(EndPoint&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	void OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);
private:
	XMFLOAT3 m_Location{};
	GameObject* m_Trigger{};
	ParticleEmitterComponent* m_Emitter{};
	bool m_Won = false;
	FMOD::Sound* m_WinSound{};
};

