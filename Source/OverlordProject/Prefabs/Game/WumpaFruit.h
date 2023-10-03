#pragma once
class WumpaFruit :
    public GameObject
{
public:
	WumpaFruit(XMFLOAT3 location);
	~WumpaFruit() = default;
	WumpaFruit(const WumpaFruit& other) = delete;
	WumpaFruit(WumpaFruit&& other) noexcept = delete;
	WumpaFruit& operator=(const WumpaFruit& other) = delete;
	WumpaFruit& operator=(WumpaFruit&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	void OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);

private:
	XMFLOAT3 m_Location{};
	GameObject* m_Model{}
	,*m_PickUpTrigger{};
	bool m_Active = true;

	float m_Rot{};
};

