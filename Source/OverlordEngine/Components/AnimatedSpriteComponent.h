#pragma once

class TextureData;

class AnimatedSpriteComponent : public BaseComponent
{
public:
	AnimatedSpriteComponent(const std::vector<std::wstring>& spriteAsset, const XMFLOAT2& pivot = XMFLOAT2{ 0, 0 }, const XMFLOAT4& color = XMFLOAT4{ Colors::White },bool looping = true);
	~AnimatedSpriteComponent() override = default;
	AnimatedSpriteComponent(const AnimatedSpriteComponent& other) = delete;
	AnimatedSpriteComponent(AnimatedSpriteComponent&& other) noexcept = delete;
	AnimatedSpriteComponent& operator=(const AnimatedSpriteComponent& other) = delete;
	AnimatedSpriteComponent& operator=(AnimatedSpriteComponent&& other) noexcept = delete;

	const XMFLOAT2& GetPivot() const { return m_Pivot; }
	const XMFLOAT4& GetColor() const { return m_Color; }

	void SetPivot(const XMFLOAT2& pivot) { m_Pivot = pivot; }
	void SetColor(const XMFLOAT4& color) { m_Color = color; }
	void SetTexture(const std::wstring& spriteAsset);
	void SetFPS(int fps) { m_FPS = fps; }
protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Draw(const SceneContext& sceneContext) override;
	void Update(const SceneContext&) override;
private:
	std::vector<TextureData*>m_pTexture{};
	TextureData* m_pCurrentTexture{};
	std::vector<std::wstring> m_AnimatedSpriteAsset{};
	XMFLOAT2 m_Pivot{};
	XMFLOAT4 m_Color{};
	int m_Idx{}
	,m_FrameAmt{}
	,m_FPS{30};
	float m_FrameTime{};
	bool m_Looping{};
};
