#include "stdafx.h"
#include "TNTCrate.h"

#include "Components/ParticleExplosion.h"
#include "Prefabs/Game/CrashBandicoot.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Scenes/Game/CrashGameScene.h"

TNTCrate::TNTCrate(XMFLOAT3 location)
	:m_Location(location)
{
}

void TNTCrate::Initialize(const SceneContext&)
{

	//Sound
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	const auto pTNTCrateMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();

	GetTransform()->Translate(m_Location);

	//Mesh
	pTNTCrateMaterial->SetDiffuseTexture(L"Game/Textures/crate_tnt.bmp");
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/explosion.wav", FMOD_2D, 0, &m_Sound);

	m_TNTCrateModel = AddChild(new GameObject());
	const auto pModel = m_TNTCrateModel->AddComponent(new ModelComponent(L"Game/Models/Crate.ovm"));
	pModel->SetMaterial(pTNTCrateMaterial);

	m_TNTCrateModel->GetTransform()->Scale(0.04f);
	m_TNTCrateModel->GetTransform()->Translate(XMFLOAT3{ -20.5f,1,0 });

	//Collision
	auto pCollision = AddComponent(new RigidBodyComponent());
	pCollision->SetKinematic(true);
	auto pTNTCrateTriangle = ContentManager::Load<PxConvexMesh>(L"Game/Models/Crate.ovpc");
	pCollision->AddCollider(PxConvexMeshGeometry{ pTNTCrateTriangle,PxMeshScale(0.04f) }, *pDefaultMaterial, false, PxTransform{ -20.5f,1,0 });
	pCollision->SetCollisionGroup(CollisionGroup::Group0);
	//Trigger
	m_SpinTrigger = AddChild(new GameObject());
	auto pTriggerActor = m_SpinTrigger->AddComponent(new RigidBodyComponent());
	pTriggerActor->SetKinematic(true);
	pTriggerActor->AddCollider(PxBoxGeometry(3, 3, 3), *pDefaultMaterial, true, PxTransform(PxVec3(m_Location.x, m_Location.y, m_Location.z)));
	pTriggerActor->SetCollisionGroup(CollisionGroup::Group1);

	m_SpinTrigger->SetOnTriggerCallBack(std::bind(&TNTCrate::OnTrigger, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

}

void TNTCrate::Update(const SceneContext&)
{
	if (m_CrashInside && m_Crash)
	{
		if (m_Crash->GetState() == CrashBandicoot::spinning)
		{
			SpawnParticle();
			SoundManager::Get()->GetSystem()->playSound(m_Sound, 0, false, 0);
			m_Crash->Damage(1); 
				dynamic_cast<CrashGameScene*>(GetScene())->RemoveObject(this);

		}
	}

}

void TNTCrate::OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
{
	if (pTriggerObject == m_SpinTrigger)
	{
		m_Crash = dynamic_cast<CrashBandicoot*>(pOtherObject);
		if (m_Crash)
		{
			if (action == PxTriggerAction::ENTER)
			{
				m_CrashInside = true;

			}
			if (action == PxTriggerAction::LEAVE)
			{
				m_CrashInside = false;
				m_Crash = nullptr;
			}
		}
	}
}

void TNTCrate::SpawnParticle()
{
	veloParticleEmitterSettings velosettings{};
	velosettings.velocity = { 0,0,0 };
	velosettings.minSize = .5f;
	velosettings.maxSize = 3.f;
	velosettings.minEnergy = 15.f;
	velosettings.maxEnergy = 20.f;
	velosettings.minScale = 1.5f;
	velosettings.maxScale = 5.5f;
	velosettings.minEmitterRadius = .1f;
	velosettings.maxEmitterRadius = 1.5f;
	velosettings.color = { 1.f,1.f,1.f, 1.f };
	velosettings.timeToEmit = 1.f;

	auto obj = GetScene()->AddChild(new GameObject());
	m_ParticleEmitter= obj->AddComponent(new ParticleExplosion(L"Game/Textures/explosion.png", velosettings, 50));
	m_ParticleEmitter->GetTransform()->Translate(XMFLOAT3(m_Location.x, m_Location.y, m_Location.z));
	m_ParticleEmitter->Play();
}
