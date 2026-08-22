// UI/ReadyGoUI.h
#pragma once
#include "UIElement.h"



class ReadyGoUI : public UIElement
{
public:
    ReadyGoUI();
    virtual ~ReadyGoUI() = default;

    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual void Render(ID2D1RenderTarget* renderTarget) override;

    // 연출이 끝났을 때 씬에서 실행할 함수(람다 등)를 등록하는 콜백
    void SetOnFinishedCallback(std::function<void()> callback) { _onFinished = callback; }

    bool IsFinished() const { return _state == ReadyGoState::Finished; }

private:
    ReadyGoState _state = ReadyGoState::Ready;
    float _timer = 0.0f;
    float _rotationAngle = 0.0f;

    class Texture* _readyTexture = nullptr;
    class Texture* _goTexture = nullptr;

    std::function<void()> _onFinished = nullptr;
};