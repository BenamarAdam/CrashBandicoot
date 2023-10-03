#include "stdafx.h"
#include "CrashBandicoot.h"
#include "stdafx.h"
#include "CrashBandicoot.h"
#include <cmath>

#include "Controls.h"
#include "CrashCamera.h"
#include "HUD.h"
#include "WumpaFruit.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Scenes/Game/CrashGameScene.h"
#include "Scenes/Game/DeathScene.h"

CrashBandicoot::CrashBandicoot() :
	m_MoveAcceleration(maxMoveSpeed/moveAccelerationTime),
	m_FallAcceleration(maxFallSpeed/fallAccelerationTime)
{}

CrashBandicoot::~CrashBandicoot()
{
	m_StepChannel->stop();
}


void CrashBandicoot::Initialize(const SceneContext& sceneContext)
{
	//Sound
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/crashspin.mp3", FMOD_2D, 0, &m_SpinSound);
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/woah.mp3", FMOD_2D, 0, &m_DamageSound);
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/wumpa.mp3", FMOD_2D, 0, &m_WumpaSound);
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/life.mp3", FMOD_2D, 0, &m_LifeSound);
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/jump.mp3", FMOD_2D, 0, &m_JumpSound);
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/death.mp3", FMOD_2D, 0, &m_DeathSound);
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/footsteps.mp3", FMOD_2D|FMOD_LOOP_NORMAL, 0, &m_StepSound);

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	//Controller
	PxCapsuleControllerDesc controller{};
	controller.setToDefault();
	controller.radius = m_Radius;
	controller.height = 2.3f;
	controller.material = pDefaultMaterial;
	controller.climbingMode = PxCapsuleClimbingMode::eEASY;
	controller.stepOffset = 0.f;
	controller.slopeLimit = 0.9f;
	m_pControllerComponent = AddComponent(new ControllerComponent(controller));
	m_pControllerComponent->Translate(XMFLOAT3{ 0,10,0 });

	m_Camera = GetScene()->AddChild(new CrashCamera());
	m_Camera->SetCrash(this);

	////Mesh
	const auto pCrashMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();

	pCrashMaterial->SetDiffuseTexture(L"Game/Textures/crash_diffuse.png");
	m_pCrashModel = AddChild(new GameObject());
	const auto pModel = m_pCrashModel->AddComponent(new ModelComponent(L"Game/Models/Crash.ovm"));
	pModel->SetMaterial(pCrashMaterial);
	m_pCrashModel->GetTransform()->Rotate(0, 180, 0);
	m_pCrashModel->GetTransform()->Translate(0, -1.7f, -0.3f);
	m_pCrashModel->GetTransform()->Scale(0.015f);

	GetTransform()->Translate(0, 20.f, 0);
	SetTag(L"CRASH");

	//Input
	auto inputAction = InputAction(Input::left, InputState::down, 'A', -1, XINPUT_GAMEPAD_DPAD_LEFT);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Input::right, InputState::down, 'D', -1, XINPUT_GAMEPAD_DPAD_RIGHT);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Input::forward, InputState::down, 'W', -1, XINPUT_GAMEPAD_DPAD_UP);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Input::backward, InputState::down, 'S',-1,XINPUT_GAMEPAD_DPAD_DOWN);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Input::jump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	sceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Input::spin, InputState::pressed, VK_SHIFT, -1, XINPUT_GAMEPAD_B);
	sceneContext.pInput->AddInputAction(inputAction);
	m_pAnimator = pModel->GetAnimator();
	
	if (m_pAnimator)
	{
		m_pAnimator->SetAnimation(2);
		m_pAnimator->Play();
	}
}

void CrashBandicoot::Update(const SceneContext& /*sceneContext*/)
{
	if (m_State != paused)
	{
		if (m_Dead)
		{
			SoundManager::Get()->GetSystem()->playSound(m_DeathSound, 0, 0, 0);
			FMOD::Channel* chan;
			SoundManager::Get()->GetSystem()->getChannel(0, &chan);
			chan->stop();
			auto scene = GetScene();
			auto deathscene = new DeathScene();
			deathscene->SetCheckPoint(dynamic_cast<CrashGameScene*>(scene)->GetCheckPoint());
			deathscene->SetGameScene(scene);
			SceneManager::Get()->AddGameScene(deathscene);
			SceneManager::Get()->SetActiveGameScene(L"DeathScene");
		}
		
		auto pInput = GetScene()->GetSceneContext().pInput;
		XMFLOAT2 move{};
		bool isMoving = false;
		
		if (pInput->IsActionTriggered(Input::forward))
		{
			move.y += 1;
			isMoving = true;
		}
		if (pInput->IsActionTriggered(Input::backward))
		{
			move.y -= 1;
			isMoving = true;
		}
		
		if (pInput->IsActionTriggered(Input::right))
		{
			move.x += 1;
			isMoving = true;
		}
		if (pInput->IsActionTriggered(Input::left))
		{
			move.x -= 1;
			isMoving = true;
		}
	

		TransformComponent* pTransform = GetComponent<TransformComponent>();

		XMVECTOR forward = XMLoadFloat3(&pTransform->GetForward());
		XMVECTOR right = XMLoadFloat3(&pTransform->GetRight());
		
		float time = GetScene()->GetSceneContext().pGameTime->GetElapsed();

		float angle = atan2(m_CurrentDirection.x, m_CurrentDirection.z);
		m_pCrashModel->GetTransform()->Rotate(0.0f, angle + XM_PI, 0.0f, false);

		//********
	
		float acceleration = m_MoveAcceleration * time;
		
		if (isMoving && m_State != spinning)
		{
			if (!m_StepChannel)
				SoundManager::Get()->GetSystem()->playSound(m_StepSound, 0, false, &m_StepChannel);
			m_State = moving;
			m_CurrentDirection = { 0,0,0 };
			XMVECTOR pos = XMLoadFloat3(&m_CurrentDirection);
			pos += forward * move.y * m_MoveSpeed * time;
			pos += right * move.x * m_MoveSpeed * time;
			XMStoreFloat3(&m_CurrentDirection, pos);

			m_MoveSpeed += acceleration;

			if (m_MoveSpeed > maxMoveSpeed)
			{
				m_MoveSpeed = maxMoveSpeed;
			}

		}
		
		else
		{
			m_StepChannel->stop();
			m_StepChannel = nullptr;
			m_MoveSpeed = 0;
			if (m_State != spinning)
				m_State = idle;
		}
		
		XMVECTOR velo = XMLoadFloat3(&m_CurrentDirection) * m_MoveSpeed;
		XMFLOAT3 horizontalVelocity{};
		XMStoreFloat3(&horizontalVelocity, velo);
		
		m_TotalVelocity.x = horizontalVelocity.x;
		m_TotalVelocity.z = horizontalVelocity.z;
		
		if (pInput->IsActionTriggered(Input::jump)&&(CheckGround()||CheckSlope()))
		{
			SoundManager::Get()->GetSystem()->playSound(m_JumpSound, 0, false, 0);

			m_TotalVelocity.y = JumpSpeed;
			m_State = jumping;
		}
		if (!CheckGround() && m_pControllerComponent->GetCollisionFlags() != PxControllerCollisionFlag::eCOLLISION_DOWN)
		{
			m_TotalVelocity.y -= m_FallAcceleration * time;
			if (m_TotalVelocity.y < -maxFallSpeed)
				m_TotalVelocity.y = -maxFallSpeed;
			if (!CheckSlope())
			{
				m_State = jumping;
				m_StepChannel->stop();
				m_StepChannel = nullptr;
				
			}
		}
		
		else if (pInput->IsActionTriggered(Input::jump)){}
		
		else if (pInput->IsActionTriggered(Input::spin) && (m_State == idle || m_State == moving))
		{
			
			m_State = spinning;
			SoundManager::Get()->GetSystem()->playSound(m_SpinSound, 0, false, 0);
		}
		else
		{
			m_TotalVelocity.y = 0;
		}
		
		XMVECTOR velocity = XMLoadFloat3(&m_TotalVelocity);
		velocity *= time;
		XMFLOAT3 displacement{};
		XMStoreFloat3(&displacement, velocity);
		m_pControllerComponent->Move(displacement);
	

		HandleHurt();

		switch (m_State)
		{
		case idle:
			SwitchAnim(1);
			break;
		case moving:
			SwitchAnim(2);
			break;
		case jumping:
			SwitchAnim(0);
			break;
		case spinning:
			SwitchAnim(3);
			m_pAnimator->SetAnimationSpeed(2.2f);
			HandleSpin();
			break;

			
		}
	}
}

void CrashBandicoot::SwitchAnim(int clipId)
{
	if (m_CurrentClipId != clipId)
	{
		m_pAnimator->SetAnimation(clipId);
		m_pAnimator->Play();
		m_CurrentClipId = clipId;
	}
}

bool CrashBandicoot::CheckGround()
{
	PxQueryFilterData filterData;
	PxRaycastBuffer hit;

	PxVec3 startVec = PhysxHelper::ToPxVec3(m_pControllerComponent->GetFootPosition());
	
	PxVec3 dirVec{ 0,-1.f,0 };
	filterData.data.word0 = (UINT32)CollisionGroup::Group0;

	bool isHit = GetScene()->GetPhysxProxy()->Raycast(startVec, dirVec,0.1f, hit, PxHitFlag::eDEFAULT, filterData);
	
	return (isHit); 
}

bool CrashBandicoot::CheckSlope()
{

	PxQueryFilterData filterData;
	PxRaycastBuffer hit;

	PxVec3 startVec = PhysxHelper::ToPxVec3(m_pControllerComponent->GetFootPosition());

	PxVec3 dirVec{ 0,-1.f,0 };
	filterData.data.word0 = (UINT32)CollisionGroup::Group0;

	startVec.x -= m_CurrentDirection.x * m_Radius;
	startVec.z -= m_CurrentDirection.z * m_Radius;

	bool isHit = GetScene()->GetPhysxProxy()->Raycast(startVec, dirVec, 0.9f, hit, PxHitFlag::eDEFAULT, filterData);

	return (isHit);
}


void CrashBandicoot::HandleSpin()
{
	m_SpinnedTime += GetScene()->GetSceneContext().pGameTime->GetElapsed();
	if (m_SpinnedTime >= 0.7f)
	{
		m_State = idle;
		m_SpinnedTime = 0;
	}
}

void CrashBandicoot::HandleHurt()
{
	if(m_Hurt)
	{
		m_HurtTime+=GetScene()->GetSceneContext().pGameTime->GetElapsed();
		if (m_HurtTime < 0.1f || (m_HurtTime < 0.3f && m_HurtTime>0.2f) || (m_HurtTime < 0.5f && m_HurtTime>0.4f) || (m_HurtTime < 0.9f && m_HurtTime>0.8f))
		{
			m_pCrashModel->GetTransform()->Scale(0);
		}
		else if ((m_HurtTime < 0.2f && m_HurtTime>0.1f) || (m_HurtTime < 0.4f && m_HurtTime>0.3f)|| (m_HurtTime < 0.6f && m_HurtTime>0.5f) || (m_HurtTime < 0.8f && m_HurtTime>0.7f) || (m_HurtTime < 1.f && m_HurtTime>0.9f))
		{
			m_pCrashModel->GetTransform()->Scale(0.015f);
		}
		else if (m_HurtTime >= 1.f)
		{
			m_Hurt = false;
				m_HurtTime = 0;
				dynamic_cast<CrashGameScene*>(GetScene())->EnableVignette(false);
		}
	}
}

void CrashBandicoot::Damage(int lives)
{
	SoundManager::Get()->GetSystem()->playSound(m_DamageSound, 0, false, 0);

	dynamic_cast<CrashGameScene*>(GetScene())->EnableVignette(true);
	 m_Lives -= lives;
	if(m_Lives<=0)
	{
		m_Dead = true;
		return;
	}
		m_Hurt = true;
		m_HUD->AddLife();
}

void CrashBandicoot::AddWumpa(int amt)
{
	m_WumpaFruit += amt;
	SoundManager::Get()->GetSystem()->playSound(m_WumpaSound, 0, false, 0);
	m_HUD->AddWumpa(amt);
}

void CrashBandicoot::AddLife(int amt)
{
	m_Lives += amt;
	SoundManager::Get()->GetSystem()->playSound(m_LifeSound, 0, false, 0);
	m_HUD->AddLife();

}

void CrashBandicoot::SetHUD(HUD* hudObj)
{
	m_HUD = hudObj;
	m_HUD->SetCrash(this);
}
