#pragma once

class BTNode
{
public:
    BTNode() {}
    virtual ~BTNode() {}

    // 트리를 순회하며 노드의 로직을 실행하는 핵심 함수
    virtual NodeState Tick(class Blackboard* bb) = 0;
    virtual bool IsLeafNode() const { return false; }
    virtual void AddChild(BTNode* child) {}
    virtual void DrawProperty() {}
    virtual void ClearChildren() {}

    virtual void SaveProperty(json& j) {}
    virtual void LoadProperty(const json& j) {}

public:
    // 노드 식별을 위한 이름 반환
    const std::string& GetName() const { return _name; }
    void SetName(const string& name) { _name = name; }

    // 노드의 마지막 실행 결과를 반환 (시각화 시 색상 표현 등에 사용)
    NodeState GetLastState() const { return _nodeState; }
    void SetLastState(NodeState state) { _nodeState = state; }

    int32 GetNodeID() { return _nodeId; }
    void SetNodeID(int32 nodeId) { _nodeId = nodeId; }

    const string& GetType() const { return _type; }
    void SetType(const string& type) { _type = type; }



protected:
    NodeState _nodeState = NodeState::Running;
    string _name = "BTNode";
    string _type = "BTNode";
private:
    int32 _nodeId = 1;

};