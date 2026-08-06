#include "pch.h"
#include "PlayerBullet.h"
#include "CircleCollider.h"
#include "AnimatorComponent.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
void PlayerBullet::Init()
{
    Super::Init();
   CircleCollider* collider = AddComponent<CircleCollider>();

    AnimatorComponent* animator = AddComponent<AnimatorComponent>();
    animator->LoadAnimationFromJson(L"Attack", L"../Resources/sprites/AttackEffect/Animation/Attack.json");
    animator->Play(L"Attack");

    this->SetScale(Vector(2.0f, 2.0f));
    collider->SetRadius(15.0f);
    collider->SetOffset(Vector(35.0f, 60.0f));
}

void PlayerBullet::Update(float deltaTime)
{
    Super::Update(deltaTime);
    Vector nextpos = GetPos() + (_dir * _state.speed* deltaTime);
    this->SetPos(nextpos);

    _lifeTime += deltaTime;

    if (_lifeTime > _state.maxLifeTIme)
    {
        Destroy();
    }
}

void PlayerBullet::Reset(Vector startPos, Vector dir, ChargeLevel level)
{
    this->SetPos(startPos);
    _dir = dir;
    _lifeTime = 0;

    Vector scale = this->GetScale();
    scale.x = abs(scale.x)* dir.x;
    this->SetScale(scale);

    // 1. JSON 파일 읽어오기
    std::ifstream f("../Resources/Data/BulletStats.json");
    if (!f.is_open()) return;

    json j;
    f >> j;
    
    // 2. 레벨에 해당하는 키(String) 찾기
    std::string levelKey = "Normal";
    if (level == ChargeLevel::Mid) levelKey = "Mid";
    else if (level == ChargeLevel::Max) levelKey = "Max";

    // 3. 해당 레벨의 데이터 블록 가져오기
    json statData = j[levelKey];
    
    _state.speed = statData["speed"];
    _state.damage = statData["damage"];
    _state.isPiercing = statData["pierce"];

    CircleCollider* collider = GetComponent<CircleCollider>();
    collider->SetRadius(statData["radius"]);

    Vector Scale = Vector(statData["scale"][0], statData["scale"][1]);
    Scale.x = Scale.x * dir.x;
    this->SetScale(Scale);

    // 원 콜라이더 오프셋
    Vector offset = Vector(statData["offset"][0], statData["offset"][1]);
    offset.x *= dir.x;
    this->GetCollider()->SetOffset(offset);

    AnimatorComponent* animator = GetComponent<AnimatorComponent>();
    string path = statData["animationPath"];
    string name = statData["animationName"];
    
    wstring wPath(path.begin(), path.end());
    wstring wName(name.begin(), name.end());

    animator->LoadAnimationFromJson(wName, wPath);
    animator->Play(wName);
   
}
