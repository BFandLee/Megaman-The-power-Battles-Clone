#pragma once
#include "UIElement.h"

// 대형 시트 텍스트 및 기호
const D2D1_RECT_F BIG_CONTINUE_TEXT_RECT = D2D1::RectF(0.0f, 0.0f, 237.0f, 64.0f); // "CONTINUE"
const D2D1_RECT_F BIG_QUESTION_RECT = D2D1::RectF(237.0f, 0.0f, 301.0f, 64.0f); // "?"

// 대형 시트 숫자 0 ~ 9 영역 테이블
const D2D1_RECT_F BIG_NUMBER_RECTS[10] = {
    D2D1::RectF(301.0f, 0.0f, 367.0f, 65.0f), // 숫자 0 (가로 66, 세로 65)
    D2D1::RectF(367.0f, 0.0f, 431.0f, 67.0f), // 숫자 1 (가로 64, 세로 67)
    D2D1::RectF(431.0f, 0.0f, 495.0f, 64.0f), // 숫자 2 (가로 64, 세로 64)
    D2D1::RectF(495.0f, 0.0f, 559.0f, 66.0f), // 숫자 3 (가로 64, 세로 66)
    D2D1::RectF(559.0f, 0.0f, 623.0f, 64.0f), // 숫자 4 (가로 64, 세로 64)
    D2D1::RectF(623.0f, 0.0f, 687.0f, 66.0f), // 숫자 5 (가로 64, 세로 66)
    D2D1::RectF(687.0f, 0.0f, 751.0f, 66.0f), // 숫자 6 (가로 64, 세로 66)
    D2D1::RectF(751.0f, 0.0f, 815.0f, 67.0f), // 숫자 7 (가로 64, 세로 67)
    D2D1::RectF(815.0f, 0.0f, 879.0f, 66.0f), // 숫자 8 (가로 64, 세로 66)
    D2D1::RectF(879.0f, 0.0f, 943.0f, 68.0f)  // 숫자 9 (가로 64, 세로 68)
};

class Texture;

class ContinueUI : public UIElement
{
public:
    ContinueUI() = default;
    virtual ~ContinueUI() = default;

    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual void Render(ID2D1RenderTarget* renderTarget) override;

    // 카운트다운 시작
    void StartCountdown(int startCount = 9);

    // 콜백 등록
    void SetOnContinueCallback(std::function<void()> callback) { _onContinue = callback; }
    void SetOnGameOverCallback(std::function<void()> callback) { _onGameOver = callback; }

private:
    ContinueState _state = ContinueState::Inactive;
    int _currentCount = 9;
    float _timer = 0.0f;           // 1초 단위 측정을 위한 타이머

    // 텍스처 포인터 (Raw Pointer 사용, 초기값 nullptr)
    Texture* _bigSheetTexture = nullptr;
    Texture* _smallSheetTexture = nullptr; // 또는 0~9 개별 텍스처 배열

    std::function<void()> _onContinue = nullptr;
    std::function<void()> _onGameOver = nullptr;
};