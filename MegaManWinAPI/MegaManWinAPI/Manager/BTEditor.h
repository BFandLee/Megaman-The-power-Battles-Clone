#pragma once
#include "Singleton.h"

class BTNode;
using CreateNodeFunc = function<BTNode*()>;

struct NodeDebugInfo
{
    NodeState lastState = NodeState::Running;
    float holdTimer = 0.0f;
};

class BTEditor : public Singleton<BTEditor>
{
    friend Singleton<BTEditor>;
private:
    BTEditor() = default;
    ~BTEditor() = default;

public:
    void Init();
    void Update();
    void Render();
    
    // 에디터 On/Off 상태를 토글
    void ToggleEditor() { _isOpen = !_isOpen; }
    bool IsOpen() const { return _isOpen; }

    void DrawNode(class BTNode* node);

    // 런타임 보스 노드가 자신의 상태를 에디터에 보고하는 함수
    void ReportNodeState(int32 nodeId, NodeState state);

private:
    bool _isOpen = false;
    vector<class BTNode*> _testNodes;
    map<string, map<string, CreateNodeFunc>> _nodeRegistry;
    int32 _nextNodeId = 1;

    // 선의 고유 ID
    vector<NodeLink> _links;
    int32 _nextLinkId = 100000;
    ImVec2 _spawnPos;
    // [NodeID -> 디버그 정보] 맵
    map<int32, NodeDebugInfo> _debugStateMap;

};
