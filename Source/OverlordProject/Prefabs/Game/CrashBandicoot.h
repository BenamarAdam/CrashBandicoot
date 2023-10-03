#pragma once
#include "HUD.h"

class CrashCamera;
class HUD;

class CrashBandicoot : public GameObject
{

	

	public:

	enum CrashState
	{
		idle,
		moving,
		spinning,
		jumping,
		paused
	};
		CrashBandicoot();
		~CrashBandicoot() override;

		CrashBandicoot(const CrashBandicoot& other) = delete;
		CrashBandicoot(CrashBandicoot&& other) noexcept = delete;
		CrashBandicoot& operator=(const CrashBandicoot& other) = delete;
		CrashBandicoot& operator=(CrashBandicoot&& other) noexcept = delete;

		void DrawImGui();
		void SwitchAnim(int clipId);
		void Pause(bool ispaused) { if (ispaused)m_State = paused; else m_State = idle; }

		CrashState GetState() { return m_State; }

		void Damage(int lives);
		void AddWumpa(int amt = 10);
		void SetWumpa(int amt) {m_WumpaFruit = amt;}
		void AddLife(int amt = 1);
		void SetHUD(HUD* hudObj);
		HUD* GetHUD() { return m_HUD; }
		int GetWumpaAmt() { return m_WumpaFruit; }
		int GetLives() { return m_Lives; }
		CrashCamera* GetCamera() { return m_Camera; }
		XMFLOAT3 GetDirection() { return m_CurrentDirection; }
		void Kill() { m_Dead = true; }
	protected:
		void Initialize(const SceneContext&) override;
		void Update(const SceneContext&) override;
		
	private:
		bool CheckGround();
		bool CheckSlope();
		void HandleSpin();
		void HandleHurt();
	//VARIABLES
		

		CameraComponent* m_pCameraComponent{};
		ControllerComponent* m_pControllerComponent{};

		GameObject* m_pCrashModel{};
		CrashCamera* m_Camera{};

		CrashState m_State=idle;

		ModelAnimator* m_pAnimator = nullptr;

		HUD* m_HUD{};

		FMOD::Sound* m_SpinSound{}
		,*m_DamageSound{}
		, * m_JumpSound{}
		,*m_WumpaSound{}
		, * m_LifeSound{}
		,*m_StepSound{}
		,*m_DeathSound{};

		FMOD::Channel* m_StepChannel{};
		//MOVEMENT
		float maxMoveSpeed{ 25.f };
		float maxFallSpeed{ 20.f };
		float JumpSpeed{ 25.f };
		float moveAccelerationTime{ .1f };
		float fallAccelerationTime{ .3f };
		float rotationSpeed{ 60.f };

		float m_TotalPitch{}, m_TotalYaw{};				//Total camera Pitch(X) and Yaw(Y) rotation
		float m_MoveAcceleration{},						//Acceleration required to reach maxMoveVelocity after 1 second (maxMoveVelocity / moveAccelerationTime)
			m_FallAcceleration{2.f},						//Acceleration required to reach maxFallVelocity after 1 second (maxFallVelocity / fallAccelerationTime)
			m_MoveSpeed{};
		float m_Radius = 1.f;
		bool m_Dead = false;

	//GAMEPLAY
		int m_Lives = 3;
		int m_WumpaFruit = 0;

	//ANIM
		float m_SpinnedTime{};
		float m_HurtTime{};
		bool m_Hurt = false;
		int m_CurrentClipId = -1;//MoveSpeed > Horizontal Velocity = MoveDirection * MoveVelocity (= TotalVelocity.xz)


		XMFLOAT3 m_TotalVelocity{};						//TotalVelocity with X/Z for Horizontal Movement AND Y for Vertical Movement (fall/jump)
		XMFLOAT3 m_CurrentDirection{};
														//Current/Last Direction based on Camera forward/right (Stored for deacceleration)
		

};

