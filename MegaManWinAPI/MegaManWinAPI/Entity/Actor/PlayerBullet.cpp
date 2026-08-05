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
    BulletState state;
    // 구조체 state로 변경 필요
    Vector nextpos = GetPos() + (_dir * _speed * deltaTime);
    this->SetPos(nextpos);

    _lifeTime += deltaTime;

    if (_lifeTime > _maxLifeTime)
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

    // TODO(USER): statData 에서 값을 꺼내어 내 멤버 변수에 적용하세요!
    
    // state 구조체로 변경 필요
    _speed = statData["speed"];
    _damage = statData["damage"];
    _isPiercing = statData["pierce"];

    CircleCollider* collider = GetComponent<CircleCollider>();
    Vector Scale = Vector(statData["scale"][0], statData["scale"][1]);
    collider->SetRadius(20.0f);

    AnimatorComponent* animator = GetComponent<AnimatorComponent>();
    string path = statData["animationPath"];
    string name = statData["animationName"];
    
    wstring wPath(path.begin(), path.end());
    wstring wName(name.begin(), name.end());

    animator->LoadAnimationFromJson(wName, wPath);
    animator->Play(wName);
    // 예시: 
    // 
    // _speed = statData["speed"];
    // 

    // (1) 충돌체 크기(Radius) 설정하기
    // CircleCollider* collider = GetComponent<CircleCollider>();
    // collider->SetRadius(...);
    // (2) 크기(Scale) 설정하기 
    // Vector scale = Vector(statData["scale"][0], statData["scale"][1]);
    // this->SetScale(scale); // 방향에 따른 X반전 로직(기존 코드 참고)도 함께 적용해보세요!
    // (3) 애니메이터 설정하기 (경로와 이름)
    // AnimatorComponent* animator = GetComponent<AnimatorComponent>();
    // std::string path = statData["animationPath"];
    // std::string name = statData["animationName"];
    // std::wstring wPath(path.begin(), path.end()); // string -> wstring 변환
    // std::wstring wName(name.begin(), name.end());
    // animator->LoadAnimationFromJson(wName, wPath);
    // animator->Play(wName);
}
