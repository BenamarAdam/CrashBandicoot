#include "stdafx.h"
#include "CrashCamera.h"

void CrashCamera::Initialize(const SceneContext&)
{
	auto cam = 
		AddChild(new FixedCamera());
	cam->GetComponent<CameraComponent>()->SetActive();
}

void CrashCamera::Update(const SceneContext& sc)
{
	
	GetTransform()->Rotate(m_Rot);
	XMFLOAT3 camLoc = GetTransform()->GetWorldPosition();
	XMFLOAT3 crashLoc = m_Crash->GetTransform()->GetWorldPosition();
	float dt = sc.pGameTime->GetElapsed()*10;

	float zloc = crashLoc.z + m_Distance.z;
	float yloc = crashLoc.y + m_Distance.y;
	if (zloc < -30.f)
	zloc = -30;
	if (zloc > 217.f)
		zloc = 217.f;
	if (yloc > 29)
		yloc = 29;


	XMFLOAT3 loc{ m_Distance.x,yloc,zloc };
	XMStoreFloat3(&loc, XMVectorLerp(XMLoadFloat3(&camLoc), XMLoadFloat3(&loc),dt));

	GetTransform()->Translate(loc);
}

void CrashCamera::SetView(XMFLOAT3 rotation, XMFLOAT3 distance)
{
	m_Rot = rotation;
	m_Distance = distance;
}

void CrashCamera::Clamp()
{
	XMFLOAT3 pos = GetTransform()->GetWorldPosition();
	if (pos.z < -40)
		GetTransform()->Translate(pos.x, pos.y, -40);
}