#include "pch.h"
#include "BTEditor.h"
#include "InputManager.h"
#include "BTSerializer.h"
#include "BTNode.h"
#include "Selector.h"
#include "ActionNode.h"
#include "Sequence.h"

class TestAction : public ActionNode
{
    virtual NodeState Tick(Blackboard* bb) override { return NodeState::Success; }
};

void BTEditor::Init()
{
    // 메뉴에 띄울 이름과, 그 이름이 불렸을 때 객체를 생성해 줄 람다 함수를 맵에 등록합니다.
    _nodeRegistry["Selector"] = []() -> BTNode* {
        BTNode* node = new Selector();
        node->SetName("Selector");
        return node;
        };
    _nodeRegistry["Sequence"] = []() -> BTNode* {
        BTNode* node = new Sequence();
        node->SetName("Sequence");
        return node;
        };
    _nodeRegistry["TestAction"] = []() -> BTNode* {
        BTNode* node = new TestAction();
        node->SetName("Action");
        return node;
        };
}

void BTEditor::Update()
{
    ImGui::GetStyle().AntiAliasedLines = false;
    ImGui::GetStyle().AntiAliasedFill = false;

    if (InputManager::GetInstance().GetButtonDown(KeyType::Tilde))
    {
        ToggleEditor();
    }

    if (!_isOpen) return;

    ImGui::Begin("BT Editor", &_isOpen);
    if (ImGui::Button("Save"))
    {
        BTSerializer::SaveToJSON("BT_Test.json",_testNodes, _links);
    }
    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        BTSerializer::LoadFromJSON("BT_Test.json");
    }

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1))
    {
        ImGui::OpenPopup("NodeCreatePopup");
    }

    if (ImGui::BeginPopup("NodeCreatePopup"))
    {
        // 맵에 등록된 목록을 순회 (pair.first는 이름 문자열, pair.second는 생성 함수)
        for (auto& pair : _nodeRegistry)
        {
            if (ImGui::Selectable(pair.first.c_str()))
            {
                // 선택된 이름의 생성 함수(pair.second)를 호출()하여 객체를 동적 할당 받습니다!
                BTNode* newNode = pair.second();

                newNode->SetNodeID(_nextNodeId);
                ++_nextNodeId;
                _testNodes.push_back(newNode);
            }
        }
        ImGui::EndPopup();
    }

    // imnodes 캔버스를 시작하고 종료
    ImNodes::BeginNodeEditor();
    for (auto node : _testNodes)
    {
        DrawNode(node);
    }

    // Draw Link
    ImNodes::PushColorStyle(ImNodesCol_Link, IM_COL32(255, 0, 0, 255));
    for (const auto& link : _links)
    {
        ImNodes::Link(link.linkId, link.startAttrId, link.endAttrId);
    }
    ImNodes::PopColorStyle();

    ImNodes::EndNodeEditor();


    int startAttr, endAttr;
    if (ImNodes::IsLinkCreated(&startAttr, &endAttr))
    {
        NodeLink link;
        link.linkId = _nextLinkId;
        link.startAttrId = startAttr;
        link.endAttrId = endAttr;

        _links.push_back(link);
        _nextLinkId++;
    }

    

    // 윈도우 창 닫기
    ImGui::End();

}

void BTEditor::Render()
{
    // ImGui 렌더링은 보통 Game.cpp 의 제일 마지막에서 일괄적으로 ImGui::Render()를 호출하므로,
    // BTEditor 클래스 내부에 별도의 렌더링(Draw) 로직이 없다면 비워두셔도 됩니다.
}

void BTEditor::DrawNode(BTNode* node)
{
    ImNodes::BeginNode(node->GetNodeID());

    ImNodes::BeginNodeTitleBar();
    ImGui::Text(node->GetName().c_str());
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(node->GetNodeID() * 100);
    ImGui::Text("In");
    ImNodes::EndInputAttribute();

    if (!node->IsLeafNode())
    {
        ImNodes::BeginOutputAttribute(node->GetNodeID() * 100 + 1);
        ImGui::Text("Out");
        ImNodes::EndOutputAttribute();
    }
    ImNodes::EndNode();
}
