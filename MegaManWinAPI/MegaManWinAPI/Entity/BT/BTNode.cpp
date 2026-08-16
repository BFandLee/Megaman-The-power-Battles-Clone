#include "pch.h"
#include "BTNode.h"
#include "BTEditor.h"

NodeState BTNode::Execute(Blackboard* bb)
{
    // 1. 실제 노드의 고유 로직(가상 함수) 실행
    _nodeState = Tick(bb);
    // 2. 에디터에 내 NodeID와 상태 보고
    BTEditor::GetInstance().ReportNodeState(_nodeId, _nodeState);
    // 3. 결과 반환
    return _nodeState;
}
