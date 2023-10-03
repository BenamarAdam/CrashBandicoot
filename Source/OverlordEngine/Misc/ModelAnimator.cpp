#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter}
{
	SetAnimation(0);
}


void ModelAnimator::Update(const SceneContext& sceneContext)
{
	TODO_W7_();

	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		auto passedTicks = sceneContext.pGameTime->GetElapsed() * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		passedTicks = fmod(passedTicks, m_CurrentClip.duration);

		//2. 
		//IF m_Reversed is true
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount<0)
			{
				m_TickCount += m_CurrentClip.duration;
			}
		}
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount >m_CurrentClip.duration)
			{
				m_TickCount -= m_CurrentClip.duration;
			}
		}
		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		//Iterate all the keys of the clip and find the following keys:
		for (auto& key:m_CurrentClip.keys)
		{
			if (key.tick < m_TickCount)
			{
				keyA = key;
			}
			else
			{
				keyB = key;
				break;
			}
		}
		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		//Clear the m_Transforms vector
		//FOR every boneTransform in a key (So for every bone)
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector
		float BlendFactor = (m_TickCount - keyA.tick) / (keyB.tick - keyA.tick);
		m_Transforms.clear();

		for (size_t i{};i<m_pMeshFilter->m_BoneCount;++i)
		{
			auto transformA = keyA.boneTransforms[i];
			auto transformB = keyB.boneTransforms[i];

			XMVECTOR posA{}, scaleA{}, rotA{};
			XMVECTOR posB{}, scaleB{}, rotB{};
			XMMatrixDecompose(&scaleA, &rotA, &posA, XMLoadFloat4x4(&transformA));
			XMMatrixDecompose(&scaleB, &rotB, &posB, XMLoadFloat4x4(&transformB));

			XMVECTOR endPos = XMVectorLerp(posA, posB, BlendFactor);
			XMVECTOR endScale = XMVectorLerp(scaleA, scaleB, BlendFactor);
			XMVECTOR endRot = XMQuaternionSlerp(rotA, rotB, BlendFactor);

			XMMATRIX pos = XMMatrixTranslationFromVector(endPos);
			XMMATRIX scale = XMMatrixScalingFromVector(endScale);
			XMMATRIX rot = XMMatrixRotationQuaternion(endRot);
			XMFLOAT4X4 trans;
			XMStoreFloat4x4(&trans, scale * rot * pos);
			
			m_Transforms.push_back(trans);
		}
	
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	TODO_W7_()
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	//If found,
	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message
	for (auto& clip:m_pMeshFilter->m_AnimationClips)
	{
		if (clip.name==clipName)
		{
			SetAnimation(clip);
		}
		else
		{
			Reset();
			Logger::LogWarning(L"Clip not found.");
		}
	}
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	TODO_W7_()
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	//If not,
		//	Call Reset
		//	Log a warning with an appropriate message
		//	return
	if (!(clipNumber<m_pMeshFilter->m_AnimationClips.size()))
	{
		Reset();
		
		Logger::LogWarning(L"Clip number is out of range of the animation-clips vector.");
		return;
	}
	//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)
	else
	{
		AnimationClip clip{ m_pMeshFilter->m_AnimationClips[clipNumber] };
		SetAnimation(clip);
	}
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	TODO_W7_()
	//Set m_ClipSet to true
	m_ClipSet = true;
	//Set m_CurrentClip
	m_CurrentClip = clip;
	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	TODO_W7_()
	//If pause is true, set m_IsPlaying to false
	if (pause)
	{
		m_IsPlaying = false;
	}
	//Set m_TickCount to zero
	m_TickCount = 0.f;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.f;

	//If m_ClipSet is true
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	if (m_ClipSet)
	{
		auto& trans{ m_CurrentClip.keys[0].boneTransforms };
		m_Transforms.assign(trans.begin(), trans.end());
	}
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
	else
	{
		XMFLOAT4X4 identityMatrix{};
		XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity());
		m_Transforms.assign(m_Transforms.size(), identityMatrix);
	}
}
