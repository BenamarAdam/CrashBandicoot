#include "stdafx.h"
#include "CrashGameScene.h"

#include "Materials/DiffuseMaterial.h"
#include "Prefabs/Game/EndPoint.h"
#include "Prefabs/Game/DeathBox.h"
#include "Prefabs/Game/HUD.h"
#include "Materials/Post/PostBlur.h"
#include "Prefabs/Game/Menu.h"
#include "Prefabs/Game/TNTCrate.h"
#include "Prefabs/Game/CheckPoint.h"
#include "Prefabs/Game/CrashBandicoot.h"
#include "Materials/Post/PostVignette.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/SkyboxPre.h"
#include "Prefabs/Game/CameraSwitch.h"
#include "Prefabs/Game/Crate.h"
#include "Prefabs/Game/LifePickup.h"
#include "Prefabs/Game/Obstacle.h"
#include "Prefabs/Game/PauseMenu.h"
#include "Prefabs/Game/Tree.h"
#include "Prefabs/Game/WumpaFruit.h"

CrashGameScene::~CrashGameScene()
{
	m_Theme->stop();
}


void CrashGameScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;

	//sky
	const auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	//Mesh
	pMaterial->SetTextureDiffuse(L"Game/Textures/Sky.jpg");

	auto model=  AddChild(new GameObject());
	const auto pModel = model->AddComponent(new ModelComponent(L"Game/Models/plane.ovm"));
	pModel->SetMaterial(pMaterial);
	pModel->GetTransform()->Scale(10);
	pModel->GetTransform()->Rotate(-40, 0, 0);
	pModel->GetTransform()->Translate(0, 100, 300);

	
	SoundManager::Get()->GetSystem()->createSound("Resources/Game/Sounds/theme.mp3", FMOD_2D | FMOD_LOOP_NORMAL, 0, &m_ThemeSound);
	SoundManager::Get()->GetSystem()->playSound(m_ThemeSound, 0, false, &m_Theme);
	m_Root = AddChild(new GameObject());

	m_SceneContext.pLights->SetDirectionalLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	//Level loading
	auto level = AddChild(new GameObject());
	//Model
	auto pLevelMat= MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	pLevelMat->SetDiffuseTexture(L"Game/Textures/Level/sand.jpeg");
	auto levelmodel = level->AddComponent(new ModelComponent(L"Game/Models/beach.ovm"));
	levelmodel->GetTransform()->Scale(0.04f);
	levelmodel->GetTransform()->Rotate(90, 0, 0);
	levelmodel->SetMaterial(pLevelMat);
	
	//Collision
	XMFLOAT3 rot = { 0,0,1.57f };
	XMFLOAT4 quat{};
	auto vec = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rot));
	XMStoreFloat4(&quat, vec);

	auto rb = level->AddComponent(new RigidBodyComponent());
	rb->SetKinematic(true);
	auto mesh = ContentManager::Load<PxConvexMesh>(L"Game/Models/Level/1_4.ovpc");
	rb->AddCollider(PxConvexMeshGeometry{ mesh,PxMeshScale{0.04f} }, *pDefaultMaterial,false, PxTransform( 0, 111.f,1.f,PxQuat{quat.x,quat.y,quat.z,quat.w }));
	rb->SetCollisionGroup(CollisionGroup::Group0);

	rb = level->AddComponent(new RigidBodyComponent());
	rb->SetKinematic(true);
	mesh = ContentManager::Load<PxConvexMesh>(L"Game/Models/Level/1_2_2.ovpc");
	rb->AddCollider(PxConvexMeshGeometry{ mesh,PxMeshScale{0.04f} }, *pDefaultMaterial, false, PxTransform(0, 111.f, 1.f, PxQuat{ quat.x,quat.y,quat.z,quat.w }));
	rb->SetCollisionGroup(CollisionGroup::Group0);

	rb = level->AddComponent(new RigidBodyComponent());
	rb->SetKinematic(true);
	mesh = ContentManager::Load<PxConvexMesh>(L"Game/Models/Level/2.ovpc");
	rb->AddCollider(PxConvexMeshGeometry{ mesh,PxMeshScale{0.04f} }, *pDefaultMaterial, false, PxTransform(0, 111.f, 1.f, PxQuat{ quat.x,quat.y,quat.z,quat.w }));
	rb->SetCollisionGroup(CollisionGroup::Group0);

	rb = level->AddComponent(new RigidBodyComponent());
	rb->SetKinematic(true);
	mesh = ContentManager::Load<PxConvexMesh>(L"Game/Models/Level/3.ovpc");
	rb->AddCollider(PxConvexMeshGeometry{ mesh,PxMeshScale{0.04f} }, *pDefaultMaterial, false, PxTransform(0, 111.f, 1.f, PxQuat{ quat.x,quat.y,quat.z,quat.w }));
	rb->SetCollisionGroup(CollisionGroup::Group0);

	rb = level->AddComponent(new RigidBodyComponent());
	rb->SetKinematic(true);
	mesh = ContentManager::Load<PxConvexMesh>(L"Game/Models/Level/4.ovpc");
	rb->AddCollider(PxConvexMeshGeometry{ mesh,PxMeshScale{0.04f} }, *pDefaultMaterial, false, PxTransform(0, 111.f, 1.f, PxQuat{ quat.x,quat.y,quat.z,quat.w }));
	rb->SetCollisionGroup(CollisionGroup::Group0);

	rb = level->AddComponent(new RigidBodyComponent());
	rb->SetKinematic(true);
	mesh = ContentManager::Load<PxConvexMesh>(L"Game/Models/Level/5.ovpc");
	rb->AddCollider(PxConvexMeshGeometry{ mesh,PxMeshScale{0.04f} }, *pDefaultMaterial, false, PxTransform(0, 111.f, 1.f, PxQuat{ quat.x,quat.y,quat.z,quat.w }));
	rb->SetCollisionGroup(CollisionGroup::Group0);


	//Side Walls
	auto Object = AddChild(new GameObject());
	Object->GetTransform()->Translate(-7.f, 10.f, 160.f);
	auto body = Object->AddComponent(new RigidBodyComponent());
	body->SetKinematic(true);
	body->SetCollisionGroup(CollisionGroup::Group0);
	body->AddCollider(PxBoxGeometry(2.f, 30.f, 100.f), *pDefaultMaterial);

	Object = AddChild(new GameObject());
	Object->GetTransform()->Translate(15.f, 10.f, 100.f);
	body = Object->AddComponent(new RigidBodyComponent());
	body->SetKinematic(true);
	body->SetCollisionGroup(CollisionGroup::Group0);
	body->AddCollider(PxBoxGeometry(2.f, 30.f, 50.f), *pDefaultMaterial);

	Object = AddChild(new GameObject());
	Object->GetTransform()->Translate(18.f, 10.f, 200.f);
	body = Object->AddComponent(new RigidBodyComponent());
	body->SetKinematic(true);
	body->SetCollisionGroup(CollisionGroup::Group0);
	body->AddCollider(PxBoxGeometry(2.f, 30.f, 50.f), *pDefaultMaterial);

	//Side block start
	Object = AddChild(new GameObject());
	Object->GetTransform()->Translate(27.f, 10.f, 10.f);
	body = Object->AddComponent(new RigidBodyComponent());
	body->SetKinematic(true);
	body->SetCollisionGroup(CollisionGroup::Group0);
	body->AddCollider(PxBoxGeometry(20.f, 30.f, 50.f), *pDefaultMaterial);

	Object = AddChild(new GameObject());
	Object->GetTransform()->Translate(-25.f, 10.f, 10.f);
	body = Object->AddComponent(new RigidBodyComponent());
	body->SetKinematic(true);
	body->SetCollisionGroup(CollisionGroup::Group0);
	body->AddCollider(PxBoxGeometry(20.f, 30.f, 50.f), *pDefaultMaterial);

	Object = AddChild(new GameObject());
	Object->GetTransform()->Translate(0, 0.f, -34.f);
	body = Object->AddComponent(new RigidBodyComponent());
	body->SetKinematic(true);
	body->SetCollisionGroup(CollisionGroup::Group0);
	body->AddCollider(PxBoxGeometry(50.f, 30.f, 5.f), *pDefaultMaterial);

	Object = AddChild(new GameObject());
	Object->GetTransform()->Translate(0, 20.f, 237.f);
	body = Object->AddComponent(new RigidBodyComponent());
	body->SetKinematic(true);
	body->SetCollisionGroup(CollisionGroup::Group0);
	body->AddCollider(PxBoxGeometry(50.f, 30.f, 5.f), *pDefaultMaterial);

	//Interactables
	AddToRoot(new Obstacle(XMFLOAT3{ -4,-11,82 }, XMFLOAT3{ 15,-11,82 }, XMFLOAT3{ 8,1,1 }));
	AddToRoot(new Obstacle(XMFLOAT3{ 15,-11,114 }, XMFLOAT3{ -4, -11, 114 }, XMFLOAT3{ 8,1,1 }));
	AddToRoot(new Obstacle(XMFLOAT3{ -7,-11,134 }, XMFLOAT3{ 13,-11,134 }, XMFLOAT3{ 8,1,1 }));
	AddToRoot(new Obstacle(XMFLOAT3{ 13, 2, 154 }, XMFLOAT3{ -7,2,154 }, XMFLOAT3{ 8,1,1 }));
	AddToRoot(new Obstacle(XMFLOAT3{ -7,11,174 }, XMFLOAT3{ 13,11,174 }, XMFLOAT3{ 8,1,1 }));
	AddToRoot(new Obstacle(XMFLOAT3{ 15, 15, 194 }, XMFLOAT3{ -5,15,194 }, XMFLOAT3{ 8,1,1 }));


	AddChild(new EndPoint(XMFLOAT3{ 4,17,225 }));

	Object = AddChild(new CheckPoint(XMFLOAT3{ 0,-12.8f,100 }));
	Object->GetTransform()->Rotate(0, 0, -20);
	Object = AddChild(new Crate(XMFLOAT3{ 10,-10.8f,90 }));
	Object->GetTransform()->Rotate(0, 0, 20);
	AddChild(new TNTCrate(XMFLOAT3{ 10,-11.8f,94 }));
	Object = AddChild(new TNTCrate(XMFLOAT3{ -4,0,10 }));
	Object->GetTransform()->Rotate(0, 0, -20);
	Object = AddChild(new Crate(XMFLOAT3{ 8,0.f,150 }));
	Object->GetTransform()->Rotate(-20, 0, 0);
	Object = AddChild(new TNTCrate(XMFLOAT3{ 10,-2.f,147 }));
	Object->GetTransform()->Rotate(-20, 0, 5);
	Object = AddChild(new CheckPoint(XMFLOAT3{ 0,10.5f,170 }));
	Object->GetTransform()->Rotate(-20, 0, -5);
	Object = AddChild(new Crate(XMFLOAT3{ 10,17.1f,198 }));
	Object->GetTransform()->Rotate(-5, 0, 10);
	
	//Deathboxes
	AddChild(new DeathBox(XMFLOAT3{ 5,-21,75 }, XMFLOAT3{ 20,15,7 }));
	AddChild(new DeathBox(XMFLOAT3{ 2,-21,128 }, XMFLOAT3{ 20,15,7 }));
	AddChild(new DeathBox(XMFLOAT3{ 6,5,188 }, XMFLOAT3{ 20,15,8 }));
	AddChild(new DeathBox(XMFLOAT3{ 2,5,204 }, XMFLOAT3{ 20,15,12 }));


	//CameraSwitches
	AddChild(new CameraSwitch(XMFLOAT3{20,0,0}, XMFLOAT3{5,5,-10}, XMFLOAT3{ 0,-10,70 },XMFLOAT3{30,10,5}));
	AddChild(new CameraSwitch(XMFLOAT3{ -20,0,0 }, XMFLOAT3{ 5,-5,-15 }, XMFLOAT3{ 5,-5,140 }, XMFLOAT3{ 30,10,5 }));
	AddChild(new CameraSwitch(XMFLOAT3{ 10,0,0 }, XMFLOAT3{ 0,5,-15 }, XMFLOAT3{ 5,13,180 }, XMFLOAT3{ 30,10,5 }));
	AddChild(new CameraSwitch(XMFLOAT3{ 30,0,0 }, XMFLOAT3{ 5,7,-10 }, XMFLOAT3{ 5,15,213 }, XMFLOAT3{ 30,10,5 }));

	//Decoration
	//TREES
	auto tree = AddChild(new Tree(XMFLOAT3{-13,1,-10},XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -30,1,10 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -20,1,30 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -16,1,48 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -16,-10,67 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -19,-10,85 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -23,-10,97 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -16,-10,109 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -19,-5,121 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -23,-4,139 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -16,5,157 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -19,12,172 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -23,19,193 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -13,19,206 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ -29,19,222 }, XMFLOAT3{}));

	tree = AddChild(new Tree(XMFLOAT3{ 18,1,-10 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 35,1,10 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 25,1,30 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 21,1,48 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 21,-10,67 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 24,-10,85 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 28,-10,97 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 21,-10,109 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 24,-9,121 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 28,-4,139 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 21,5,157 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 24,12,172 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 28,19,193 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 18,19,206 }, XMFLOAT3{}));
	tree = AddChild(new Tree(XMFLOAT3{ 34,19,222 }, XMFLOAT3{}));


	m_Crash = new CrashBandicoot();
	AddChild(m_Crash);
	m_Crash->GetTransform()->Translate(m_CheckPoint);
		
	m_Root->SetActive(false);

	//Post Processing
	m_PostVignette = MaterialManager::Get()->CreateMaterial<PostVignette>();

	AddPostProcessingEffect(m_PostVignette);
	m_PostVignette->SetIsEnabled(false);
	
}

void CrashGameScene::OnGUI()
{
	ImGui::SliderFloat3("Position", ConvertUtil::ToImFloatPtr(m_temp),-50 , 50);
	ImGui::SliderFloat("Rotation", ConvertUtil::ToImFloatPtr(m_Temprot), 0, 360);

}

void CrashGameScene::Update()
{
	if(m_ToDelete)
	{
		for(auto obj: m_ToBeDeleted)
		{
			RemoveChild(obj, true);
		}
		m_ToBeDeleted.clear();
		m_ToDelete = false;
	}

	if(m_DeathScene)
	{
		SceneManager::Get()->RemoveGameScene(m_DeathScene, true);
		m_DeathScene = nullptr;
	}
	switch(m_State)
	{
	case GameState::game:
		if (m_SceneContext.pInput->IsKeyboardKey(InputState::pressed, VK_ESCAPE) || m_SceneContext.pInput->IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_START))
		{
				m_PauseMenu = AddChild(new PauseMenu());
				m_Crash->Pause(true);
				m_Root->SetActive(false);
				m_State = GameState::menu;
			
		}
		break;
	case GameState::pause:
		if (m_SceneContext.pInput->IsKeyboardKey(InputState::pressed, VK_ESCAPE) || m_SceneContext.pInput->IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_START))
		{
				m_Crash->Pause(false);
				m_Root->SetActive(true);
				RemoveObject(m_PauseMenu);
				m_PauseMenu = nullptr;
				m_State = GameState::game;
			
			
				
			
		}
		break;
	}
	XMFLOAT3 crashpos = m_Crash->GetTransform()->GetWorldPosition();
	crashpos.x += -95;
	crashpos.y += 66;
	crashpos.z += -41;
	m_SceneContext.pLights->SetDirectionalLight(crashpos, { 0.740129888f, -0.597205281f, 0.309117377f });

}

void CrashGameScene::EnableVignette(bool enabled)
{
	m_PostVignette->SetIsEnabled(enabled);
}

void CrashGameScene::SetActive()
{
	m_Root->SetActive(true);
}

void CrashGameScene::SetGameState(GameState state)
{
	if (m_State == GameState::menu && state == GameState::game)
	{
		HUD* hud = new HUD();
		AddToRoot(hud);
		m_Crash->SetHUD(hud);
		m_MenuOff=true;
		SetActive();
	}
		m_State = state;
}

GameObject* CrashGameScene::AddToRoot(GameObject* obj)
{
	m_Root->AddChild(obj);
	return obj;
}

void CrashGameScene::InitGame()
{
	
}

void CrashGameScene::DeleteDeathScene(GameScene* scene)
{
	m_DeathScene = scene;
}

void CrashGameScene::RemoveObject(GameObject* object)
{
	m_ToBeDeleted.push_back(object);
	m_ToDelete = true;
}

void CrashGameScene::SetCrashLoc(XMFLOAT3 crashLoc)
{
	m_Crash->GetTransform()->Translate(crashLoc);
}
