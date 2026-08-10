#include "pch.h"
#include "PlayerFactory.h"
#include "AnimatorComponent.h"
#include "FSMComponent.h"
#include "Texture.h"
#include "Player.h"
#include "IdleState.h"
#include "MoveState.h"
#include "JumpState.h"
#include "SlideState.h"
#include "HitState.h"
#include "DeathState.h"
#include "SpawnState.h"


// TODO: Player.cpp에 있던 각종 State 헤더파일들을 이곳으로 가져오세요.
Actor* PlayerFactory::CreateActor(Vector pos)
{
	Player* player = new Player();
	player->Init();
	player->SetPos(pos);

	AnimatorComponent* animator = player->GetComponent<AnimatorComponent>();
	FSMComponent* fsm = player->GetComponent<FSMComponent>();

	// Animation 폴더 안의 모든 파일을 순회
	for (const auto& entry : std::filesystem::directory_iterator("../Resources/sprites/Player/Animation/"))
	{
		// 확장자가 .json 인 경우에만
		if (entry.path().extension() == ".json")
		{
			std::wstring stateName = entry.path().stem().wstring(); // 파일 이름만 추출 (예: idle.json -> idle)
			std::wstring filePath = entry.path().wstring();         // 전체 경로

			animator->LoadAnimationFromJson(stateName, filePath);
		}
	}

	auto setupFSM = [fsm]() {
		// TODO: 기존 상태 추가 로직들을 이곳으로 옮기세요.
		fsm->AddState("Idle", new IdleState(fsm));
		fsm->AddState("Move", new MoveState(fsm));
		fsm->AddState("Jump", new JumpState(fsm));
		fsm->AddState("Sliding", new SlideState(fsm));
		fsm->AddState("Hit", new HitState(fsm));
		fsm->AddState("Death", new DeathState(fsm));
		fsm->AddState("Spawn", new SpawnState(fsm));
		};
	setupFSM();
	fsm->ChangeState("Spawn");

	std::unordered_map<uint32, uint32> yellowPalette = {
		{ 0xFF5084F4, 0xFFF0C030 },
		{ 0xFF2040D4, 0xFFF08000 },
		{ 0xFF30C4D4, 0xFFF0F0C0 }
	};

	// TODO: 상태 이름과 파일 경로 데이터를 쌍으로 묶어서 배열에 추가하세요.
	std::vector<std::pair<std::wstring, std::wstring>> textureData = {
		{L"Idle", L"../Resources/sprites/Player/State/Player_Idle_Sheet.png"},
		{L"Move", L"../Resources/sprites/Player/State/Player_Walk_Sheet.png"},
		{L"Jump", L"../Resources/sprites/Player/State/Player_Jump_Sheet.png"},
		{L"Sliding", L"../Resources/sprites/Player/State/Player_Sliding_Sheet.png"}
	};

	for (const auto& data : textureData)
	{
		Texture* tex = new Texture();
		tex->LoadWithPaletteSwap(data.second, yellowPalette, 1, 1, 1.0f, false);
		animator->SetSwapTextureForState(data.first, tex);
	}

	return player;
}
