#pragma once
#include "Misc/Material.h"
class DiffuseMaterial : public Material<DiffuseMaterial>
{
public:
	DiffuseMaterial();
	~DiffuseMaterial() = default;

	DiffuseMaterial(const DiffuseMaterial& other) = delete;
	DiffuseMaterial(DiffuseMaterial&& other) noexcept = delete;
	DiffuseMaterial& operator=(const DiffuseMaterial& other) = delete;
	DiffuseMaterial& operator=(DiffuseMaterial&& other) noexcept = delete;

	void SetTextureDiffuse(const std::wstring& assetFile);
protected:
	void InitializeEffectVariables() override;
private:
	TextureData* m_pDiffuseTexture{};
};

