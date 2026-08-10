#pragma once

// 팩토리의 부모가 될 추상 클래스(인터페이스)입니다.
class ActorFactory
{
public:
    virtual ~ActorFactory() = default;

    // TODO: 위치값 Vector(pos)를 매개변수로 받고, 반환형이 Actor* 인 순수 가상 함수 CreateActor를 선언하세요.
    // (힌트: = 0; 을 붙이면 순수 가상 함수가 됩니다!)
    virtual class Actor* CreateActor(Vector pos) = 0;

};