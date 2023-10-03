#pragma once
class Tree :
    public GameObject
{
public:
	Tree(XMFLOAT3 location,XMFLOAT3 size);
	~Tree() = default;
	Tree(const Tree& other) = delete;
	Tree(Tree&& other) noexcept = delete;
	Tree& operator=(const Tree& other) = delete;
	Tree& operator=(Tree&& other) noexcept = delete;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
private:
	XMFLOAT3 m_Loc{},
		m_Size{};
};

