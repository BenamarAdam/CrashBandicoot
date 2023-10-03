#include "stdafx.h"
#include "SkyboxPre.h"

#include "Materials/SkyboxMaterial.h"

void SkyboxPre::Initialize(const SceneContext&)
{
	const auto pSkyMaterial = MaterialManager::Get()->CreateMaterial<SkyboxMaterial>();

	//Mesh
	//pSkyMaterial->SetTextureSkybox(L"Game/Textures/cubemap.png");

	auto model = AddChild(new GameObject());
	auto pModel = model->AddComponent(new ModelComponent(L"Game/Models/cube.ovm",false));
	pModel->SetMaterial(pSkyMaterial);
	GetTransform()->Scale(1000.f);
}

void SkyboxPre::Update(const SceneContext&)
{
	
}

