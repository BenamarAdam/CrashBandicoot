#include "stdafx.h"
#include "LifePickup.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/Game/CrashBandicoot.h"
#include "Scenes/Game/CrashGameScene.h"

LifePickup::LifePickup(XMFLOAT3 location)
	:m_Location(location)
{}

void LifePickup::Initialize(const SceneContext&)
{
	//Sound
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	const auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();

	GetTransform()->Translate(m_Location);

	//Mesh
	pMaterial->SetDiffuseTexture(L"Game/Textures/lifetex.png");
	
	m_Model = AddChild(new GameObject());
	const auto pModel = m_Model->AddComponent(new ModelComponent(L"Game/Models/Life.ovm"));
	pModel->SetMaterial(pMaterial);

	m_Model->GetTransform()->Scale(0.02f);

	//Trigger
	m_PickUpTrigger = AddChild(new GameObject());
	auto pTriggerActor = m_PickUpTrigger->AddComponent(new RigidBodyComponent());
	pTriggerActor->SetKinematic(true);
	pTriggerActor->AddCollider(PxBoxGeometry(0.7f, 0.7f, 0.7f), *pDefaultMaterial, true, PxTransform(PxVec3(m_Location.x, m_Location.y, m_Location.z)));
	pTriggerActor->SetCollisionGroup(CollisionGroup::Group1);

	m_PickUpTrigger->SetOnTriggerCallBack(std::bind(&LifePickup::OnTrigger, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

}

void LifePickup::OnTrigger(GameObject* /*pTriggerObject*/, GameObject* pOtherObject, PxTriggerAction action)
{
	CrashBandicoot* crash = dynamic_cast<CrashBandicoot*>(pOtherObject);
	if (crash)
	{
		if (action == PxTriggerAction::ENTER)
		{
			crash->AddLife();
			m_Model->GetTransform()->Scale(0);
			m_Active = false;
		}

	}

}
void LifePickup::Update(const SceneContext&)
{
	m_Rot += GetScene()->GetSceneContext().pGameTime->GetElapsed()*100;
	m_Model->GetTransform()->Rotate(90, m_Rot, 0);
	if (!m_Active)
		dynamic_cast<CrashGameScene*>(GetScene())->RemoveObject(this);

}
