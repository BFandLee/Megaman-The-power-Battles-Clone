#include "pch.h"
#include "BossMissile.h"
#include "BoxCollider.h"
#include "AnimatorComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"

void BossMissile::Init()
{
    Super::Init();
    _collider = AddComponent<BoxCollider>();
    _transform = GetComponent<TransformComponent>();
    _animator = AddComponent<AnimatorComponent>();
}

void BossMissile::Update(float deltaTime)
{
    Super::Update(deltaTime);
    Vector nextpos = GetPos() + (_dir * _state.speed * deltaTime);
    this->SetPos(nextpos);

    _lifeTime += deltaTime;

    if (_lifeTime > _state.maxLifeTime)
    {
        Destroy();
    }
}

void BossMissile::RenderUI()
{
    ImGui::PushID(this);
    ImGui::Text("[ Boss Bullet Spec ]");
    // 타입 표시
    const string typeStr = (_type == MissileType::Base) ? "Base" : "Razer";
    string displayName = GetName() + "[" + typeStr + "]";

    if (ImGui::TreeNode(displayName.c_str()))
    {
        for (auto& component : GetComponents())
        {
            component->RenderUI();
        }

        ImGui::Separator();     // 구분선
        ImGui::Text("[ Boss Bullet Spec ]");

        ImGui::DragFloat("Speed", &_state.speed);
        ImGui::DragFloat("Damage", &_state.damage);
        ImGui::DragFloat("MaxLifeTime", &_state.maxLifeTime);

        if (ImGui::Button("Save Bullet Data"))
        {
            SaveDataToJson(_type);
        }

        ImGui::TreePop();
    }
    ImGui::PopID();

}

void BossMissile::OnEnter(Actor* other, const HitResult& hit)
{
    ActorType otherType = other->GetActorType();
    switch (otherType)
    {
        case ActorType::Player:
        {
            other->TakeDamage(_state.damage, _dir.x);
            Destroy();
            break;
        }

        case ActorType::WALL:
        case ActorType::Ceiling:
        case ActorType::Ground:
        {
            if (_type == MissileType::Base)
            {
                Destroy();
                return;
            }

            if (_type == MissileType::Razer)
            {
                if (_bounceCount >= _maxBounceCount)
                {
                    Destroy();
                    return;
                }

                // 기존 레이저 파괴
                Destroy();

                Vector newDir = _dir;
                switch (otherType)
                {
                case ActorType::WALL:
                    if (_dir.y == 0.0f)
                        newDir = Vector(-_dir.x, -1.0f);
                    else
                        newDir.x = -_dir.x;
                    break;
                case ActorType::Ceiling:
                    newDir.y = abs(_dir.y);
                    break;
                case ActorType::Ground:
                    newDir.y = -abs(_dir.y);
                    break;
                }
                newDir.Normalize();

                BossMissile* nextLaser = new BossMissile();
                if (nextLaser != nullptr)
                {
                    nextLaser->Init();
                    nextLaser->Fire(GetPos(), newDir, MissileType::Razer, _bounceCount + 1);
                    SceneManager::GetInstance().GetScene()->AddActor(nextLaser);
                }
            }
            break;
        }

        default:
            break;
    }

    
}

void BossMissile::Fire(Vector startPos, Vector dir, MissileType type, int bounceCount)
{
    this->SetPos(startPos);
    _dir = dir;
    _type = type;
    _bounceCount = bounceCount;

    LoadDataFromJson(_type);

    if (_type == MissileType::Razer)
    {
        
        _animator->Play(L"Razer");
        float Degree = atan2f(_dir.y, _dir.x);
        float angleDeg = (Degree * (180.0f / 3.14159265f));

        _transform->SetRotation(angleDeg);
    }
    else if(_type == MissileType::Base)
    {
        _animator->Play(L"Base");
        _transform->SetRotation(0.0f);
    }
    
}

void BossMissile::SaveDataToJson(MissileType type)
{
    json j;
    std::ifstream inFile(_dataPath);
    if (inFile.is_open())
    {
        inFile >> j;
        inFile.close();
    }

    std::string key = (type == MissileType::Base) ? "Base" : "Razer";

    j[key]["speed"] = _state.speed;
    j[key]["damage"] = _state.damage;
    j[key]["maxLifeTime"] = _state.maxLifeTime;

    j[key]["scaleX"] = _transform->GetScale().x;
    j[key]["scaleY"] = _transform->GetScale().y;

    j[key]["Width"] = _collider->GetWidth();
    j[key]["Height"] = _collider->GetHeight();

    j[key]["OffsetX"] = _collider->GetOffset().x;
    j[key]["OffsetY"] = _collider->GetOffset().y;

    ofstream outFile(_dataPath);
    outFile << j.dump(4);
    
}

void BossMissile::LoadDataFromJson(MissileType type)
{
    // 파일 스트림 열기
    std::ifstream file(_dataPath);
    if (!file.is_open())
        return;

    // 2. nlohmann/json 객체로 파싱
    json j;
    file >> j;

    string key = (type == MissileType::Base) ? "Base" : "Razer";

    if (!j.contains(key))
        return;
    
    auto& data = j[key];

    _state.speed = data["speed"];
    _state.damage = data["damage"];
    _state.maxLifeTime = data["maxLifeTime"];

    _transform->SetScale(Vector(data["scaleX"], data["scaleY"]));
    _collider->SetSize(data["Width"], data["Height"]);
    _collider->SetOffset(Vector(data["OffsetX"], data["OffsetY"]));

    string path = data["animationPath"];
    string name = data["animationName"];

    wstring wPath(path.begin(), path.end());
    wstring wName(name.begin(), name.end());

    _animator->LoadAnimationFromJson(wName, wPath);
    _animator->Play(wName);
}


