#include "stdafx.h"
#include "Crate.h"

#include "CrashBandicoot.h"
#include "LifePickup.h"
#include "WumpaFruit.h"
#include "Components/ParticleExplosion.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Scenes/Game/CrashGameScene.h"

Crate::Crate(XMFLOAT3 location)
	:m_Location(location)
	,m_CrateModel{}
{
	
}

void Crate::Initialize(const SceneContext&)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	const auto pCrateMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();

	GetTransform()->Translate(m_Location);
	
	//Mesh
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/cratebreak.mp3", FMOD_2D,0, &m_Sound);
	pCrateMaterial->SetDiffuseTexture(L"Game/Textures/crate_question.png");
	
	m_CrateModel = AddChild(new GameObject());
	const auto pModel = m_CrateModel->AddComponent(new ModelComponent(L"Game/Models/Crate.ovm"));
	pModel->SetMaterial(pCrateMaterial);
	
	m_CrateModel->GetTransform()->Scale(0.04f);
	m_CrateModel->GetTransform()->Translate(XMFLOAT3{ -20.5f,1,0 });

	//Collision
	auto pCollision = AddComponent(new RigidBodyComponent());
	pCollision->SetKinematic(true);
	auto pCrateTriangle = ContentManager::Load<PxConvexMesh>(L"Game/Models/Crate.ovpc");
	pCollision->AddCollider(PxConvexMeshGeometry{pCrateTriangle,PxMeshScale(0.04f)},*pDefaultMaterial,false,PxTransform{ -20.5f,1,0 });
	pCollision->SetCollisionGroup(CollisionGroup::Group0);

	//Trigger
	m_SpinTrigger = AddChild(new GameObject());
	auto pTriggerActor = m_SpinTrigger->AddComponent(new RigidBodyComponent());
	pTriggerActor->SetKinematic(true);
	pTriggerActor->AddCollider(PxBoxGeometry(3, 3, 3), *pDefaultMaterial, true, PxTransform(PxVec3(m_Location.x, m_Location.y, m_Location.z)));
	pTriggerActor->SetCollisionGroup(CollisionGroup::Group1);

	m_SpinTrigger->SetOnTriggerCallBack(std::bind(&Crate::OnTrigger, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	
}

void Crate::Update(const SceneContext&)
{
	if(m_Delete)
	{

	}
	if(m_CrashInside && m_Crash)
	{
		if (m_Crash->GetState() == CrashBandicoot::spinning)
		{
			CrashGameScene* scene = dynamic_cast<CrashGameScene*>(GetScene());
			SpawnParticle();
			SoundManager::Get()->GetSystem()->playSound(m_Sound, 0, false, 0);
			if (rand() % 3 == 0)
				GetScene()->AddChild(new LifePickup(m_Location));
			else
				GetScene()->AddChild(new WumpaFruit(m_Location));
			scene->RemoveObject(this);
		}
	}

}

void Crate::OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
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


void Crate::SpawnParticle()
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

	auto obj = dynamic_cast<CrashGameScene*>(GetScene())->AddChild(new GameObject());
	obj->GetTransform()->Translate(m_Location);
	auto emitter = obj->AddComponent(new ParticleExplosion(L"Game/Textures/crateparticle.png", velosettings, 50));
	emitter->Play();
}