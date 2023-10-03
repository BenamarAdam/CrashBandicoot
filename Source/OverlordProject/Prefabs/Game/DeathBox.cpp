#include "stdafx.h"
#include "DeathBox.h"

#include "CrashBandicoot.h"
#include "Scenes/Game/CrashGameScene.h"
#include "Scenes/Game/DeathScene.h"

DeathBox::DeathBox(XMFLOAT3 location, XMFLOAT3 scale)
	:m_Location{location},
m_Scale{scale}
{
	
}


void DeathBox::Initialize(const SceneContext&)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	m_Trigger = AddChild(new GameObject());
	auto pTriggerActor = m_Trigger->AddComponent(new RigidBodyComponent());
	pTriggerActor->SetKinematic(true);
	pTriggerActor->AddCollider(PxBoxGeometry(m_Scale.x/2,m_Scale.y/2,m_Scale.z/2), *pDefaultMaterial, true, PxTransform(PxVec3(m_Location.x, m_Location.y, m_Location.z)));
	pTriggerActor->SetCollisionGroup(CollisionGroup::Group1);

	m_Trigger->SetOnTriggerCallBack(std::bind(&DeathBox::OnTrigger, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

}

void DeathBox::Update(const SceneContext&)
{
	if (m_Switch)
	{
		FMOD::Channel* chan;
		SoundManager::Get()->GetSystem()->getChannel(0, &chan);
		chan->stop();
		auto scene = GetScene();
		dynamic_cast<CrashBandicoot*>(dynamic_cast<CrashGameScene*>(scene)->GetCrash())->Kill();		
	}
}

void DeathBox::OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
{
	if (pTriggerObject == m_Trigger)
	{
		auto crash = dynamic_cast<CrashGameScene*>(GetScene())->GetCrash();
		if (pOtherObject == crash)
		{
			if (action == PxTriggerAction::ENTER)
			{
				m_Switch = true;
			}
		}
	}
}
