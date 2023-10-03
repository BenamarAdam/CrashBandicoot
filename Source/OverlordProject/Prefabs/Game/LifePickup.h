#pragma once
class LifePickup :
	public GameObject
{
public:
	LifePickup(XMFLOAT3 location);
	~LifePickup() = default;
	LifePickup(const LifePickup& other) = delete;
	LifePickup(LifePickup&& other) noexcept = delete;
	LifePickup& operator=(const LifePickup& other) = delete;
	LifePickup& operator=(LifePickup&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	void OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);

private:
	XMFLOAT3 m_Location{};
	GameObject* m_Model{}
	, * m_PickUpTrigger{};
	float m_Rot{};
	bool m_Active = true;
};

