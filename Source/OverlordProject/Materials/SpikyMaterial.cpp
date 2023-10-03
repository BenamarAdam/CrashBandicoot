#include "stdafx.h"
#include "SpikyMaterial.h"

SpikyMaterial::SpikyMaterial() : Material<SpikyMaterial>(L"Effects/SpikyShader.fx")
{}

void SpikyMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"m_LightDirection", XMFLOAT3{ -5.f,5.f,-5.f });
	SetVariable_Vector(L"gColorDiffuse", XMFLOAT3{ 1.f,0.f,0.f });
}
