#pragma once
struct HitResult;
// Scene에 그려지는 모든 객체들은 Actor로부터 파생된다.
class Actor
{
	
public:
	Actor(string name);
	virtual ~Actor();
	
	virtual void Init();
	virtual void Destroy();
	virtual void RenderUI();
	virtual void Update(float deltaTime);
	virtual void Render(ID2D1RenderTarget* renderTarget);
	//virtual void OnHit(Actor* other) {}
	virtual json ToJson();
	virtual void FromJson(const json& j);

	// 유니티와 비슷한 충돌 3단계 함수 
	virtual void OnEnter(Actor* other, const HitResult& hit) {}
	virtual void OnStay(Actor* other, const HitResult& hit) {}
	virtual void OnExit(Actor* other) {}

	virtual void TakeDamage(float damage, float hitDirX = 0.0f) {}

	// 순수 가상함수. 모든 Actor는 반드시 본인이 그려져야하는 Layer 순서를 알려줘야한다.
	virtual RenderLayer GetRenderLayer() = 0;
	virtual ActorType GetActorType() = 0;

public:
	Vector GetPos() const;
	void SetPos(Vector pos);

	Vector GetScale() const;
	void SetScale(Vector scale);

	bool GetPendingKill() const { return _pendingKill; }

	

	// 충돌체크가 필요하다면
	class Collider* GetCollider() { return _collider; }

	// 오브젝트 풀
	class IObjectPool* GetPool() { return _pool; }
	void SetPool(class IObjectPool* pool) { _pool = pool; }

	virtual void SetActive(bool isActive) { _isActive = isActive; }
	bool GetActive() { return _isActive; }

	template<typename T>
	T* AddComponent()
	{
		T* newComponent = new T();
		newComponent->SetOwner(this);
		_components.push_back(newComponent);
		// collider를 캐싱해두자.
		cacheCollider(newComponent);

		return newComponent;
	}

	template<typename T>
	T* GetComponent()
	{
		for (auto iter : _components)
		{
			// Component* -> ColliderCircle* ? ImageRenderer*
			if (T* find = dynamic_cast<T*>(iter))
			{
				return find;
			}
		}

		return nullptr;
	}

private:
	void cacheCollider(class Component* component);

private:
	
	// N개 vector, map
	vector<class Component*> _components;

	// 부모 Actor에서 collider를 캐싱해둔다.
	class Collider* _collider = nullptr;

	bool _pendingKill = false;	// 다음 프레임에 삭제될 예정
	IObjectPool* _pool = nullptr;

	class TransformComponent* _transform = nullptr;
	string _name;

	bool _isActive = true;
};

