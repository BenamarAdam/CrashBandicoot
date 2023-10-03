#include "stdafx.h"
#include "SkyboxMaterial.h"

SkyboxMaterial::SkyboxMaterial()
	: Material{ L"Effects/Skybox.fx" }
{
	m_pSkyboxTexture = ContentManager::Load<TextureData>(L"Textures/sunol_cubemap.dds");
}

void SkyboxMaterial::InitializeEffectVariables()
{	
	SetVariable_Texture(L"gCubeMap", m_pSkyboxTexture);
}

void SkyboxMaterial::SetTextureSkybox(const std::wstring& assetFile)
{
	m_pSkyboxTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gCubeMap", m_pSkyboxTexture);
}
