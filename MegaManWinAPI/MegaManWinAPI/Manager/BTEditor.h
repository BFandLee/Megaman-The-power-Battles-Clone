#pragma once
#include "Singleton.h"



class BTNode;
using CreateNodeFunc = function<BTNode*()>;

class BTEditor : public Singleton<BTEditor>
{
    friend Singleton<BTEditor>;
private:
    BTEditor() = default;
    ~BTEditor() = default;

public:

public:
    void Init();
    void Update();
    void Render();
    
    // 에디터 On/Off 상태를 토글
    void ToggleEditor() { _isOpen = !_isOpen; }
    bool IsOpen() const { return _isOpen; }

    void DrawNode(class BTNode* node);

private:
    bool _isOpen = false;
    vector<class BTNode*> _testNodes;
    map<string, map<string, CreateNodeFunc>> _nodeRegistry;
    int32 _nextNodeId = 1;

    // 선의 고유 ID
    vector<NodeLink> _links;
    int32 _nextLinkId = 100000;

    ImVec2 _spawnPos;
};
