#pragma once
class DeathBox :
    public GameObject
{
public:
	DeathBox(XMFLOAT3 location, XMFLOAT3 scale);
	~DeathBox() = default;
	DeathBox(const DeathBox& other) = delete;
	DeathBox(DeathBox&& other) noexcept = delete;
	DeathBox& operator=(const DeathBox& other) = delete;
	DeathBox& operator=(DeathBox&& other) noexcept = delete;
protected:
	void OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	XMFLOAT3 m_Location{},
		m_Scale{};
	GameObject* m_Trigger{};
	bool m_Switch = false;

};

