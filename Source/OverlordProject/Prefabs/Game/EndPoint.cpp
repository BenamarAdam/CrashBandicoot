#include "stdafx.h"
#include "EndPoint.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/Game/CrashBandicoot.h"
#include "Scenes/Game/WinScene.h"

EndPoint::EndPoint(XMFLOAT3 loc)
	:m_Location{loc}
{
	
}

void EndPoint::Initialize(const SceneContext&)
{
	//Particle System
	ParticleEmitterSettings settings{};
	settings.velocity = { 0.f,6.f,0.f };
	settings.minSize = .5f;
	settings.maxSize = 1.f;
	settings.minEnergy = .5f;
	settings.maxEnergy = 1.f;
	settings.minScale = .1f;
	settings.maxScale = 1.f;
	settings.minEmitterRadius = .2f;
	settings.maxEmitterRadius = 4.5f;
	settings.color = { 1.f,1.f,1.f, .6f };
	GetTransform()->Translate(m_Location);

	const auto pObject = GetScene()->AddChild(new GameObject);
	m_Emitter = pObject->AddComponent(new ParticleEmitterComponent(L"Game/Textures/orb.png", settings, 100));
	pObject->GetTransform()->Translate(m_Location.x,m_Location.y+4,m_Location.z);

	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/win.mp3", FMOD_2D, 0, &m_WinSound);

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	const auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();

	//Mesh
	pMaterial->SetDiffuseTexture(L"Game/Textures/stone.jpg");

	auto model = AddChild(new GameObject());
	const auto pModel = model->AddComponent(new ModelComponent(L"Game/Models/End.ovm"));
	pModel->SetMaterial(pMaterial);

	model->GetTransform()->Scale(0.04f);
	model->GetTransform()->Rotate(90, 0, 0);
	//Collision
	auto pCollision = AddComponent(new RigidBodyComponent());
	pCollision->SetKinematic(true);
	auto pCrateTriangle = ContentManager::Load<PxConvexMesh>(L"Game/Models/End.ovpc");

	XMFLOAT3 rot = { 1.5f,0,0 };
	XMFLOAT4 quat{};
	auto vec = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rot));
	XMStoreFloat4(&quat, vec);

	pCollision->AddCollider(PxConvexMeshGeometry{ pCrateTriangle,PxMeshScale(0.04f) }, *pDefaultMaterial, false, PxTransform{ PxQuat{quat.x,quat.y,quat.z,quat.w} });
	pCollision->SetCollisionGroup(CollisionGroup::Group0);
	pCollision->Translate(m_Location);
	//Trigger
	m_Trigger = AddChild(new GameObject());
	auto rb = m_Trigger->AddComponent(new RigidBodyComponent());
	rb->SetKinematic(true);
	rb->AddCollider(PxBoxGeometry{ 3,2,3 }, *pDefaultMaterial, true, PxTransform{ PxVec3{m_Location.x,m_Location.y+4,m_Location.z} });

	rb->SetCollisionGroup(CollisionGroup::Group1);
	m_Trigger->SetOnTriggerCallBack(std::bind(&EndPoint::OnTrigger, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	
}

void EndPoint::Update(const SceneContext&)
{
	if(m_Won)
	{
		FMOD::Channel* chan;
		SoundManager::Get()->GetSystem()->getChannel(0, &chan);
		chan->stop();
		SoundManager::Get()->GetSystem()->playSound(m_WinSound,0,0,0);
		auto scene = GetScene();
		auto winscene = new WinScene();
		winscene->SetGameScene(scene);
		SceneManager::Get()->AddGameScene(winscene);
		SceneManager::Get()->SetActiveGameScene(L"WinScene");
	}
}

void EndPoint::OnTrigger(GameObject* /*pTriggerObject*/, GameObject* pOtherObject, PxTriggerAction action)
{
	CrashBandicoot* crash = dynamic_cast<CrashBandicoot*>(pOtherObject);
	if (crash)
	{
		if (action == PxTriggerAction::ENTER)
		{
			m_Won = true;
		}

	}
}
