#pragma once
class SkyboxPre :
    public GameObject
{
public:
	SkyboxPre() = default;
	~SkyboxPre() = default;
	SkyboxPre(const SkyboxPre& other) = delete;
	SkyboxPre(SkyboxPre&& other) noexcept = delete;
	SkyboxPre& operator=(const SkyboxPre& other) = delete;
	SkyboxPre& operator=(SkyboxPre&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:

};

