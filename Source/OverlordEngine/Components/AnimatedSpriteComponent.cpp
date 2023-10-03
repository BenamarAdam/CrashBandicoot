#include "stdafx.h"
#include "AnimatedSpriteComponent.h"

AnimatedSpriteComponent::AnimatedSpriteComponent(const std::vector<std::wstring>& spriteAsset, const XMFLOAT2& pivot, const XMFLOAT4& color,bool looping) :
	m_AnimatedSpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color),
	m_Looping(looping)
{}

void AnimatedSpriteComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	int idx{};
	for(std::wstring loc:m_AnimatedSpriteAsset)
	{
		m_pTexture.push_back(ContentManager::Load<TextureData>(loc));
		++idx;
	}
		m_pCurrentTexture = m_pTexture[0];
		m_FrameAmt = idx;
	
}

void AnimatedSpriteComponent::SetTexture(const std::wstring&) //spriteAsset)
{
	//m_AnimatedSpriteAsset = spriteAsset;
	//m_pTexture = ContentManager::Load<TextureData>(m_AnimatedSpriteAsset);
}

void AnimatedSpriteComponent::Draw(const SceneContext& /*sceneContext*/)
{
	if (!m_pCurrentTexture)
		return;

	TODO_W4(L"Draw the sprite with AnimatedSpriteRenderer::Draw")
		XMFLOAT3 pos = m_pGameObject->GetTransform()->GetWorldPosition();
	XMFLOAT3 scale = m_pGameObject->GetTransform()->GetScale();
	SpriteRenderer::Get()->Draw(m_pCurrentTexture, XMFLOAT2{ pos.x,pos.y }, m_Color, m_Pivot, XMFLOAT2{ scale.x,scale.y }
		, MathHelper::QuaternionToEuler(m_pGameObject->GetTransform()->GetRotation()).z
		, 0.9f);
	//Here you need to draw the AnimatedSpriteComponent using the Draw of the sprite renderer
	// The sprite renderer is a singleton
	// you will need to position (X&Y should be in screenspace, Z contains the depth between [0,1]), the rotation and the scale from the owning GameObject
	// You can use the MathHelper::QuaternionToEuler function to help you with the z rotation 
}

void AnimatedSpriteComponent::Update(const SceneContext& sceneContext)
{
	m_FrameTime += sceneContext.pGameTime->GetElapsed();
	if(m_FrameTime>=1.f/m_FPS)
	{
		m_FrameTime = 0;
		++m_Idx;
		if (m_Idx >= m_FrameAmt)
			if (m_Looping)
				m_Idx = 0;
			else
				m_Idx = m_FrameAmt - 1;
		m_pCurrentTexture = m_pTexture[m_Idx];
	}

}
