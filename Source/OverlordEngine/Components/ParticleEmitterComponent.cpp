#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount):
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	TODO_W9(L"Implement Destructor")
	SafeDelete(m_ParticlesArray);
	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	TODO_W9(L"Implement Initialize")
	if (m_pParticleMaterial == nullptr)
	{
		m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();
	}
	CreateVertexBuffer(sceneContext);
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	TODO_W9(L"Implement CreateVertexBuffer")
	SafeRelease(m_pVertexBuffer);
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = m_ParticleCount * sizeof(VertexParticle);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	HANDLE_ERROR(sceneContext.d3dContext.pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pVertexBuffer));
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	TODO_W9(L"Implement Update")
		
		/*if (!m_IsPaused)
		{
			m_EmittedTime += sceneContext.pGameTime->GetElapsed();
			if (m_EmittedTime > m_EmitterSettings.timeToEmit)
			{
				SafeDelete(m_ParticlesArray);
				SafeRelease(m_pVertexBuffer);
				GetScene()->RemoveChild(GetGameObject(), true);
				return;
			}
		}*/
			float interval = (m_EmitterSettings.maxEnergy - m_EmitterSettings.minEnergy) / m_ParticleCount;
			m_LastParticleSpawn += sceneContext.pGameTime->GetElapsed();

			m_ActiveParticles = 0;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			const auto pBuffer = static_cast<VertexParticle*>(mappedResource.pData);
			
			for (UINT i = 0; i < m_ParticleCount; ++i)
			{
				Particle& particle = m_ParticlesArray[i];
				bool active = false;
				if (particle.isActive)
				{
					UpdateParticle(particle, sceneContext.pGameTime->GetElapsed());
					active = true;
				}
				else
				{
					if (m_LastParticleSpawn >= interval)
					{
						SpawnParticle(particle);
						active = true;
					}
				}
				if (active)
				{
					pBuffer[m_ActiveParticles] = particle.vertexInfo;
					++m_ActiveParticles;
				}
			}
			sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
		
}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	TODO_W9(L"Implement UpdateParticle")
		if (true)//!m_IsPaused)
		{
			if (!p.isActive)
			{
				return;
			}

			p.currentEnergy -= elapsedTime;
			if (p.currentEnergy < 0)
			{
				p.isActive = false;
				return;
			}

			XMVECTOR pos = XMLoadFloat3(&p.vertexInfo.Position);
			pos += XMLoadFloat3(&m_EmitterSettings.velocity) * elapsedTime;
			XMStoreFloat3(&p.vertexInfo.Position, pos);

			float energyPercent = p.currentEnergy / p.totalEnergy;
			p.vertexInfo.Color = m_EmitterSettings.color;
			p.vertexInfo.Color.w *= energyPercent;
			p.vertexInfo.Size = (p.initialSize - p.sizeChange) * energyPercent + p.sizeChange;
		}
}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	TODO_W9(L"Implement SpawnParticle")
	p.isActive = true;
	p.totalEnergy = p.currentEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);
	XMVECTOR randDir = XMVectorSet(0, 1, 0, 0);
	
	float randomDist = MathHelper::randF(m_EmitterSettings.minEmitterRadius,m_EmitterSettings.maxEmitterRadius - m_EmitterSettings.minEmitterRadius);
	XMMATRIX randomRot = XMMatrixRotationRollPitchYaw(MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI),MathHelper::randF(-XM_PI, XM_PI));
	randDir = XMVector3TransformNormal(randDir, randomRot);
	randomDist += m_EmitterSettings.minEmitterRadius;

	auto pos = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	pos += randDir * randomDist;
	XMStoreFloat3(&p.vertexInfo.Position, pos);

	p.vertexInfo.Size = p.initialSize = MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize);
	p.sizeChange = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);

	p.vertexInfo.Rotation = MathHelper::randF(-XM_PI, XM_PI);

}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	TODO_W9(L"Implement PostDraw")
		if (true)//!m_IsPaused)
		{
			m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
			m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());
			m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture);

			sceneContext.d3dContext.pDeviceContext->IASetInputLayout(m_pParticleMaterial->GetTechniqueContext().pInputLayout);
			sceneContext.d3dContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			const UINT offsets = 0;
			const UINT strides = sizeof(VertexParticle);
			sceneContext.d3dContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides,
				&offsets);

			D3DX11_TECHNIQUE_DESC technique{};
			m_pParticleMaterial->GetTechniqueContext().pTechnique->GetDesc(&technique);
			for (UINT p = 0; p < technique.Passes; ++p)
			{
				m_pParticleMaterial->GetTechniqueContext().pTechnique->GetPassByIndex(p)->Apply(0, sceneContext.d3dContext.pDeviceContext);
				sceneContext.d3dContext.pDeviceContext->DrawIndexed(m_ParticleCount, 0, 0);
			}
		}
}

void ParticleEmitterComponent::DrawImGui()
{
	if(ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}