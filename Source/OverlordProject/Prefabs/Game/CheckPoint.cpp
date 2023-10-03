#include "stdafx.h"
#include "CheckPoint.h"

#include "Components/ParticleExplosion.h"
#include "Prefabs/Game/CrashBandicoot.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Scenes/Game/CrashGameScene.h"

CheckPoint::CheckPoint(XMFLOAT3 location)
	:m_Location(location)
,m_CheckPointModel{}
{

}

void CheckPoint::Initialize(const SceneContext&)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	const auto pCheckPointMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();

	GetTransform()->Translate(m_Location);

	//Mesh
	pCheckPointMaterial->SetDiffuseTexture(L"Game/Textures/crate_checkpoint.bmp");
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/cratebreak.mp3", FMOD_2D, 0, &m_Sound);
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/check.mp3", FMOD_2D, 0, &m_CheckSound);
	
	m_CheckPointModel = AddChild(new GameObject());
	const auto pModel = m_CheckPointModel->AddComponent(new ModelComponent(L"Game/Models/Crate.ovm"));
	pModel->SetMaterial(pCheckPointMaterial);

	m_CheckPointModel->GetTransform()->Scale(0.04f);
	m_CheckPointModel->GetTransform()->Translate(XMFLOAT3{ -20.5f,1,0 });

	//Collision
	auto pCollision = AddComponent(new RigidBodyComponent());
	pCollision->SetKinematic(true);
	auto pCheckPointTriangle = ContentManager::Load<PxConvexMesh>(L"Game/Models/Crate.ovpc");
	pCollision->AddCollider(PxConvexMeshGeometry{ pCheckPointTriangle,PxMeshScale(0.04f) }, *pDefaultMaterial, false, PxTransform{ -20.5f,1,0 });
	pCollision->SetCollisionGroup(CollisionGroup::Group0);

	//Trigger
	m_SpinTrigger = AddChild(new GameObject());
	auto pTriggerActor = m_SpinTrigger->AddComponent(new RigidBodyComponent());
	pTriggerActor->SetKinematic(true);
	pTriggerActor->AddCollider(PxBoxGeometry(3, 3, 3), *pDefaultMaterial, true, PxTransform(PxVec3(m_Location.x, m_Location.y, m_Location.z)));
	pTriggerActor->SetCollisionGroup(CollisionGroup::Group1);

	m_SpinTrigger->SetOnTriggerCallBack(std::bind(&CheckPoint::OnTrigger, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void CheckPoint::Update(const SceneContext&)
{
	if (m_CrashInside && m_Crash)
	{
		if (m_Crash->GetState() == CrashBandicoot::spinning)
		{
			auto scene = dynamic_cast<CrashGameScene*>(GetScene());
			SpawnParticle();
			SoundManager::Get()->GetSystem()->playSound(m_Sound, 0, false, 0);
			SoundManager::Get()->GetSystem()->playSound(m_CheckSound, 0, false, 0);
			scene->SetCheckPoint(GetTransform()->GetWorldPosition());
			scene->RemoveObject(this);
			dynamic_cast<CrashBandicoot*>(scene->GetCrash())->GetHUD()->CheckPoint();
		}
	}

}

void CheckPoint::OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
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


void CheckPoint::SpawnParticle()
{
	veloParticleEmitterSettings velosettings{};
	velosettings.velocity = { 0.f,0.f,0.f };
	velosettings.minSize = .5f;
	velosettings.maxSize = 1.f;
	velosettings.minEnergy = 15.f;
	velosettings.maxEnergy = 20.f;
	velosettings.minScale = 1.5f;
	velosettings.maxScale = 3.5f;
	velosettings.minEmitterRadius = .1f;
	velosettings.maxEmitterRadius = 1.f;
	velosettings.color = { 1.f,1.f,1.f, 1.f };
	velosettings.timeToEmit = 1.f;

	auto obj = GetScene()->AddChild(new GameObject());
	obj->GetTransform()->Translate(m_Location);
	auto emitter = obj->AddComponent(new ParticleExplosion(L"Game/Textures/crateparticle.png", velosettings, 20));
	emitter->Play();
}
