#include "stdafx.h"
#include "Tree.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

Tree::Tree(XMFLOAT3 location, XMFLOAT3 size)
	:m_Loc{location},
	m_Size{size}
{
	
}


void Tree::Initialize(const SceneContext&)
{

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	const auto pTreeMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();

	GetTransform()->Translate(m_Loc);

	//Mesh
	pTreeMaterial->SetDiffuseTexture(L"Game/Textures/wood.jpeg");

	auto TreeModel = AddChild(new GameObject());
	const auto pModel =  TreeModel->AddComponent(new ModelComponent(L"Game/Models/tree1.ovm"));
	pModel->SetMaterial(pTreeMaterial);

	TreeModel->GetTransform()->Scale(0.015f);
	TreeModel->GetTransform()->Rotate(90, float(rand()%360), 0);

	//Collision
	XMFLOAT3 rot = { 1.5f,0,0 };
	XMFLOAT4 quat{};
	auto vec = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rot));
	XMStoreFloat4(&quat, vec);

	auto pCollision = AddComponent(new RigidBodyComponent());
	pCollision->SetKinematic(true);
	auto pTreeTriangle = ContentManager::Load<PxConvexMesh>(L"Game/Models/tree1.ovpc");
	pCollision->AddCollider(PxConvexMeshGeometry{ pTreeTriangle,PxMeshScale(0.015f) }, *pDefaultMaterial, false,PxTransform{PxQuat{quat.x,quat.y,quat.z,quat.w}});
	pCollision->SetCollisionGroup(CollisionGroup::Group1);
	
}

void Tree::Update(const SceneContext&)
{
	
}
