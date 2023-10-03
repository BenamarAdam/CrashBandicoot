#pragma once
class UberMaterial : public Material<UberMaterial>
{
public:
	UberMaterial();
	~UberMaterial() = default;
	UberMaterial(const UberMaterial& other) = delete;
	UberMaterial(UberMaterial&& other) noexcept = delete;
	UberMaterial& operator=(const UberMaterial& other) = delete;
	UberMaterial& operator=(UberMaterial&& other) noexcept = delete;
	
protected:
	void InitializeEffectVariables() override;
	

};

