---
name: Boss Behavior Tree Design
description: 3주차 보스 패턴 구현 시 비헤이비어 트리(BT) 데코레이터 패턴을 적용하기 위한 설계 규칙입니다.
---

# 보스 비헤이비어 트리(BT) 데코레이터 설계 규칙

3주차 록맨 보스의 다채로운 패턴을 BT로 구현할 때, 조건 검사 로직은 다음 규칙을 반드시 따릅니다.

## 1. Condition 노드 사용 지양
- `BTCheck_...` 와 같이 조건 검사만을 수행하는 잎(Leaf) 노드를 만들어 `Sequence` 노드 아래에 액션과 나열하는 방식을 지양합니다. 
- 이는 불필요한 Sequence 생성을 유발해 트리 구조를 깊고 복잡하게 만듭니다.

## 2. Decorator 패턴 적극 활용
- 조건을 검사할 때는 항상 `ConditionDecorator`를 상속받은 구체적인 데코레이터 클래스(`CooldownDecorator`, `ProbabilityDecorator` 등)를 생성하여 액션 노드를 직접 감싸는(Wrap) 구조를 사용합니다.

## 3. 단일 책임 원칙 (SRP)과 OCP
- `DecoratorNode` 자체에 하드코딩(enum switch 등)을 하지 않고, 쿨다운, 확률 등 조건이 생길 때마다 각각 독립된 클래스로 분리합니다.
- 워프 쿨다운, 자력 쿨다운 등 목적이 달라도 **'시간을 재는' 본질이 같다면 `CooldownDecorator` 하나만 생성**하고, 생성자에서 쿨다운 시간을 다르게 주입하여 재사용합니다.
