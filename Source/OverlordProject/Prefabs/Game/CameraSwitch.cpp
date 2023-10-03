#include "stdafx.h"
#include "CameraSwitch.h"

#include "CrashBandicoot.h"
#include "CrashCamera.h"
#include "Scenes/Game/CrashGameScene.h"

CameraSwitch::CameraSwitch(XMFLOAT3 rot, XMFLOAT3 dist, XMFLOAT3 loc, XMFLOAT3 size)
	:m_Distance{ dist },
	m_Rot{ rot },
	m_Location{ loc },
	m_Size{ size }
{
	
}

void CameraSwitch::Initialize(const SceneContext&)
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	m_Trigger = AddChild(new GameObject());
	auto pTriggerActor = m_Trigger->AddComponent(new RigidBodyComponent());
	pTriggerActor->SetKinematic(true);
	pTriggerActor->AddCollider(PxBoxGeometry(m_Size.x / 2, m_Size.y / 2, m_Size.z / 2), *pDefaultMaterial, true, PxTransform(PxVec3(m_Location.x, m_Location.y, m_Location.z)));
	pTriggerActor->SetCollisionGroup(CollisionGroup::Group1);

	m_Trigger->SetOnTriggerCallBack(std::bind(&CameraSwitch::OnTrigger, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

}

void CameraSwitch::Update(const SceneContext&)
{
	
}

void CameraSwitch::OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
{

	//on enter take old rot and dist and apply new
	//on leave if crash dir is -z apply old

	if (pTriggerObject == m_Trigger)
	{
		auto crash = dynamic_cast<CrashGameScene*>(GetScene())->GetCrash();
		if (pOtherObject == crash)
		{
			if (action == PxTriggerAction::ENTER)
			{
				auto cam = dynamic_cast<CrashBandicoot*>(crash)->GetCamera();
					if (!m_PreviousSaved)
					{
						m_PreviousRot = cam->GetRot();
						m_PreviousDist = cam->GetDist();
						m_PreviousSaved = true;
					}
				cam->SetView(m_Rot, m_Distance);
			}
			if (action == PxTriggerAction::LEAVE && dynamic_cast<CrashBandicoot*>(crash)->GetDirection().z<0)
			{
				auto cam = dynamic_cast<CrashBandicoot*>(crash)->GetCamera();
				cam->SetView(m_PreviousRot, m_PreviousDist);
			}
		}
	}
}
