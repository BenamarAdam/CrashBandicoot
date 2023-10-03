#include "stdafx.h"
#include "Obstacle.h"
#include <cmath>

#include "CrashBandicoot.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

Obstacle::Obstacle(XMFLOAT3 startPos, XMFLOAT3 endPos, XMFLOAT3 scale)
	:m_Start{startPos},
	m_End{endPos},
	m_Scale{scale}
{
	
}

void Obstacle::Initialize(const SceneContext& sc)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	const auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();

	//Mesh
	pMaterial->SetDiffuseTexture(L"Game/Textures/metal.jpeg");
	
	m_Model = AddChild(new GameObject());
	const auto pModel = m_Model->AddComponent(new ModelComponent(L"Game/Models/spike.ovm"));
	pModel->SetMaterial(pMaterial);

	m_Model->GetTransform()->Scale(0.07f);
	m_Model->GetTransform()->Rotate(90, 0, 0);

	//Collision
	auto pCollision = AddComponent(new RigidBodyComponent());
	pCollision->SetKinematic(true);
	auto pCrateTriangle = ContentManager::Load<PxConvexMesh>(L"Game/Models/body.ovpc");

	XMFLOAT3 rot = { 1.5f,0,0 };
	XMFLOAT4 quat{};
	auto vec = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rot));
	XMStoreFloat4(&quat, vec);
	
	pCollision->AddCollider(PxConvexMeshGeometry{ pCrateTriangle,PxMeshScale(0.07f) }, *pDefaultMaterial, false, PxTransform{PxQuat{quat.x,quat.y,quat.z,quat.w}});
	pCollision->SetCollisionGroup(CollisionGroup::Group1);

	//Trigger
	m_Trigger = AddChild(new GameObject());
	m_RigidBody = m_Trigger->AddComponent(new RigidBodyComponent());
	m_RigidBody->SetKinematic(true);

	m_RigidBody->AddCollider(PxConvexMeshGeometry{ pCrateTriangle,PxMeshScale(PxVec3{0.13f,0.13f,0.07f}) }, *pDefaultMaterial, true, PxTransform{ PxQuat{quat.x,quat.y,quat.z,quat.w} });
	m_RigidBody->Translate(m_Start);
	m_RigidBody->SetCollisionGroup(CollisionGroup::Group1);

	m_Trigger->SetOnTriggerCallBack(std::bind(&Obstacle::OnTrigger, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_StartTime=sc.pGameTime->GetTotal();
	GetTransform()->Translate(m_Start);
}

void Obstacle::Update(const SceneContext& sceneContext)
{
	if (sceneContext.pGameTime->GetTotal() - m_StartTime > 1.f)
		m_Init = true;
	m_Rotation += sceneContext.pGameTime->GetElapsed()*100;
	m_Model->GetTransform()->Rotate(90,m_Rotation,0);
	if(!m_ToStart)
	{
		XMFLOAT3 pos = GetTransform()->GetWorldPosition();
		float dt = sceneContext.pGameTime->GetElapsed()*m_Speed;
		float x = std::lerp(pos.x, m_End.x, dt);
		float y = std::lerp(pos.y, m_End.y, dt);
		float z = std::lerp(pos.z, m_End.z, dt);
		if (abs(x - m_End.x) < 1 && abs(z - m_End.z) < 1 && abs(z - m_End.z) < 1)
			m_ToStart = true;
		GetTransform()->Translate(x, y, z);
		m_RigidBody->Translate(XMFLOAT3{ x,y,z });
	}
	else
	{
		XMFLOAT3 pos = GetTransform()->GetWorldPosition();
		float dt = sceneContext.pGameTime->GetElapsed() * m_Speed;
		float x = std::lerp(pos.x, m_Start.x, dt);
		float y = std::lerp(pos.y, m_Start.y, dt);
		float z = std::lerp(pos.z, m_Start.z, dt);
		if (abs(x - m_Start.x) < 1 && abs(z - m_Start.z) < 1 && abs(z - m_Start.z) < 1)
			m_ToStart = false;
		GetTransform()->Translate(x, y, z);
		m_RigidBody->Translate(XMFLOAT3{ x,y,z });
	}
}


void Obstacle::OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action )
{
	if (pOtherObject != pTriggerObject && m_Init)
	{
		CrashBandicoot* crash = dynamic_cast<CrashBandicoot*>(pOtherObject);
		if (crash)
		{
			if (action == PxTriggerAction::ENTER)
			{
				crash->Damage(1);
			}

		}
	}
}
