#pragma once
class CrashCamera :
    public GameObject
{
public:
	CrashCamera() = default;
	~CrashCamera() = default;
	CrashCamera(const CrashCamera& other) = delete;
	CrashCamera(CrashCamera&& other) noexcept = delete;
	CrashCamera& operator=(const CrashCamera& other) = delete;
	CrashCamera& operator=(CrashCamera&& other) noexcept = delete;

	void SetCrash(GameObject* crash) { m_Crash = crash; }
	void SetView(XMFLOAT3 rotation, XMFLOAT3 distance);
	XMFLOAT3 GetRot() { return m_Rot; }
	XMFLOAT3 GetDist() { return m_Distance; }
protected:
	void Update(const SceneContext&) override;
	void Initialize(const SceneContext&) override;
private:
	void Clamp();
	GameObject* m_Crash{};
	XMFLOAT3 m_Rot{25,0,0},
	m_Distance{0,7,-15};
};

