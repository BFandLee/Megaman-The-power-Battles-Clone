#pragma once

class IObjectPool
{
public:
	virtual void Return(class Actor* actor) = 0;
};


template<typename T>
class ObjectPool : public IObjectPool
{
public:
	// 초기화
	void Init(int32 size)
	{
		// 메모리 할당+객체를 생성해서 추가
		_buffer.resize(size);
		_freeList.reserve(size); //capacity만 확보

		for (auto& iter : _buffer)
		{
			Actor* actor = static_cast<Actor*>(&iter);
			actor->SetPool(this);
			actor->Init();

			_freeList.push_back(&iter);
		}
	}

	// 꺼내쓰는것
	T* Acquire()
	{
		if (_freeList.empty())
		{
			assert(false && "ObjectPool exhausted: increase Init(size)");
			return nullptr;
		}
		 
		T* object = _freeList.back();
		_freeList.pop_back();

		return object;
	}

	// 반환하는것
	virtual void Return(Actor* actor) override
	{
		_freeList.push_back((T*)actor);
	}

private:
	// 메모리가 할당되어 있는 원본
	vector<T>	_buffer; 

	vector<T*>  _freeList;
};

