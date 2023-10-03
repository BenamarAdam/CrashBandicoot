#pragma once
class CameraSwitch : public GameObject
{
public:
	CameraSwitch(XMFLOAT3 rot, XMFLOAT3 dist,XMFLOAT3 loc, XMFLOAT3 size);
	~CameraSwitch() = default;
	CameraSwitch(const CameraSwitch& other) = delete;
	CameraSwitch(CameraSwitch&& other) noexcept = delete;
	CameraSwitch& operator=(const CameraSwitch& other) = delete;
	CameraSwitch& operator=(CameraSwitch&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	void OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);
private:
	XMFLOAT3 m_Rot{},
		m_Distance{},
		m_PreviousRot{},
		m_PreviousDist{};

	XMFLOAT3 m_Location{},
		m_Size{};
	GameObject* m_Trigger{};

	bool m_PreviousSaved = false;
};

