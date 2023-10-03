#pragma once
#include "Scenes/Game/CrashGameScene.h"
class ParticleExplosion;
class CrashBandicoot;

class TNTCrate : public GameObject
{
public:
	TNTCrate(XMFLOAT3 location);
	~TNTCrate() = default;

	TNTCrate(const TNTCrate& other) = delete;
	TNTCrate(TNTCrate&& other) noexcept = delete;
	TNTCrate& operator=(const TNTCrate& other) = delete;
	TNTCrate& operator=(TNTCrate&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	void OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action);
	void SpawnParticle();
	XMFLOAT3 m_Location{};
	GameObject* m_TopTrigger{}
		, * m_SpinTrigger{}
	, * m_TNTCrateModel{};

	CrashBandicoot* m_Crash = nullptr;
	CrashGameScene* m_Scene = nullptr;
	ParticleExplosion* m_ParticleEmitter{};
	FMOD::Sound* m_Sound{};
	bool m_CrashInside = false;
};

