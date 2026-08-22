#include "pch.h"
#include "BossMissile.h"
#include "BoxCollider.h"
#include "AnimatorComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "SoundManager.h"
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
    if (_isDestoryed || other == _ignoreActor) return;

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
                    _isDestoryed = true;
                    Destroy();
                    return;
                }

                Vector newDir = _dir;
                Vector spawnPos = GetPos();

                BoxCollider* otherCol = other->GetComponent<BoxCollider>();
                Vector otherPos = (otherCol != nullptr) ? otherCol->GetColliderPos() : other->GetPos();
                float otherHalfW = (otherCol != nullptr) ? otherCol->GetWidth() * 0.5f : 25.0f;
                float otherHalfH = (otherCol != nullptr) ? otherCol->GetHeight() * 0.5f : 25.0f;
                const float SAFE_OFFSET = 35.0f;
                

                // 지형 판별: 명시적 타입 or 콜라이더 가로/세로 비율 및 위치로 정확히 판별
                bool isCeiling = (otherType == ActorType::Ceiling) ||
                    (otherType == ActorType::WALL && otherHalfW > otherHalfH && otherPos.y < GWinSizeY * 0.5f);
               
                bool isGround = (otherType == ActorType::Ground) ||
                    (otherType == ActorType::WALL && otherHalfW > otherHalfH && otherPos.y >= GWinSizeY * 0.5f);


                if (isCeiling) // ⬆️ 천장 충돌
                {
                    newDir.y = abs(_dir.y); // 아래(+Y)로 반사
                    spawnPos.y = otherPos.y + otherHalfH + SAFE_OFFSET; // X는 원래 충돌 X 유지, Y만 아래로 이동
                }
                else if (isGround) // ⬇️ 바닥 충돌
                {
                    newDir.y = -abs(_dir.y); // 위(-Y)로 반사
                    spawnPos.y = otherPos.y - otherHalfH - SAFE_OFFSET; // X는 원래 충돌 X 유지, Y만 위로 이동
                }
                else // ⬅️ ➡️ 좌/우 세로 벽 충돌
                {
                    if (otherPos.x < GWinSizeX * 0.5f) // 왼쪽 벽
                    {
                        newDir.x = abs(_dir.x); // 오른쪽(+X)으로 반사
                        spawnPos.x = otherPos.x + otherHalfW + SAFE_OFFSET;
                    }
                    else // 오른쪽 벽
                    {
                        newDir.x = -abs(_dir.x); // 왼쪽(-X)으로 반사
                        spawnPos.x = otherPos.x - otherHalfW - SAFE_OFFSET;
                    }
                    // Y가 수평으로 날아가던 중이면 대각선 위로 유도
                    if (_dir.y == 0.0f) newDir.y = -1.0f;
                }
                newDir.Normalize();

                spawnPos.x = std::clamp(spawnPos.x, 30.0f, (float)GWinSizeX - 30.0f);
                spawnPos.y = std::clamp(spawnPos.y, 30.0f, (float)GWinSizeY - 80.0f);
                
                BossMissile* nextLaser = new BossMissile();
                if (nextLaser != nullptr)
                {
                    nextLaser->Init();
                    nextLaser->Fire(spawnPos, newDir, MissileType::Razer, _bounceCount + 1, other);
                    SceneManager::GetInstance().GetScene()->AddActor(nextLaser);
                }
                _isDestoryed = true;
                Destroy();
            }
            break;
        }

        default:
            break;
    }

    
}

void BossMissile::Fire(Vector startPos, Vector dir, MissileType type, int bounceCount, Actor* ignoreActor)
{
    this->SetPos(startPos);
    _dir = dir;
    _type = type;
    _bounceCount = bounceCount;
    _ignoreActor = ignoreActor;

    LoadDataFromJson(_type);

    if (_type == MissileType::Razer)
    {
        if (_bounceCount == 0)
        {
            SoundManager::GetInstance().PlaySFX(L"Gemini_Razer");
        }
        _animator->Play(L"Razer");
        float Degree = atan2f(_dir.y, _dir.x);
        float angleDeg = (Degree * (180.0f / 3.14159265f));

        _transform->SetRotation(angleDeg);
    }

    else if(_type == MissileType::Base)
    {
        SoundManager::GetInstance().PlaySFX(L"Geminiman_shoot");
        _animator->Play(L"Base");
        _transform->SetRotation(0.0f);

        Vector scale = _transform->GetScale();
        scale.x = abs(scale.x) * (-_dir.x);
        _transform->SetScale(scale);
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


