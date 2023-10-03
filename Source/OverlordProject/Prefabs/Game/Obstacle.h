#pragma once
class Obstacle :
    public GameObject
{
public:
	Obstacle(XMFLOAT3 startPos, XMFLOAT3 endPos, XMFLOAT3 scale);
	~Obstacle() = default;
	Obstacle(const Obstacle& other) = delete;
	Obstacle(Obstacle&& other) noexcept = delete;
	Obstacle& operator=(const Obstacle& other) = delete;
	Obstacle& operator=(Obstacle&& other) noexcept = delete;
protected:
	void Update(const SceneContext&) override;
	void Initialize(const SceneContext&) override;
	void OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);
private:
	XMFLOAT3 m_Start{},
		m_End{},
		m_Scale{};
	bool m_ToStart = false;
	float m_Speed = 1.5f;
	float m_StartTime{};
	float m_Rotation{};
	bool m_Init{false};

	GameObject* m_Trigger{};
	GameObject* m_Model{};
	RigidBodyComponent* m_RigidBody{};
};

