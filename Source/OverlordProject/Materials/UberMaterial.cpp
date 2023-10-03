#include "stdafx.h"
#include "UberMaterial.h"


UberMaterial::UberMaterial() : Material<UberMaterial>(L"Effects/UberShader.fx")
{
	
}

void UberMaterial::InitializeEffectVariables()
{
	SetVariable_Scalar(L"gUseTextureDiffuse", true);
	SetVariable_Vector(L"gColorDiffuse", XMFLOAT4{ 1.f,0.f,0.f,1.f });
	TextureData* pTexture = ContentManager::Load<TextureData>(L"Textures/Skulls_Diffusemap.tga");
	SetVariable_Texture(L"gTextureDiffuse", pTexture);

	SetVariable_Scalar(L"gUseTextureNormal", true);
	pTexture = ContentManager::Load<TextureData>(L"Textures/Skulls_Normalmap.tga");
	SetVariable_Texture(L"gTextureNormal", pTexture);

	SetVariable_Scalar(L"gUseEnvironmentMapping", true);
	pTexture = ContentManager::Load<TextureData>(L"Textures/Sunol_Cubemap.dds");
	SetVariable_Texture(L"gCubeEnvironment", pTexture);
	SetVariable_Scalar(L"gReflectionStrength", 1.f);
	SetVariable_Scalar(L"gRefractionStrength", 1.f);

	SetVariable_Scalar(L"gUseFresnelFalloff", true);
	SetVariable_Scalar(L"gFresnelMultiplier", 1.8f);

	SetVariable_Scalar(L"gUseSpecularBlinn", true);

	pTexture = ContentManager::Load<TextureData>(L"Textures/Specular_Level.tga");
	//SetVariable_Texture(L"gTextureSpecularIntensity", pTexture);
	SetVariable_Texture(L"gTextureOpacity", pTexture);
}


