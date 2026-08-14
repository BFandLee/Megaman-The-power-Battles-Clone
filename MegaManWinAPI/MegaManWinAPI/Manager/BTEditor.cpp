#include "pch.h"
#include "BTEditor.h"
#include "InputManager.h"
#include "BTSerializer.h"
#include "BTNode.h"
#include "Selector.h"
#include "Sequence.h"

// 보스 ActionNode
#include "ActionNode.h"
#include "GeminiCommonNodes.h"
#include "GeminiPhase1Nodes.h"
#include "GeminiPhase2Nodes.h"

// 보스 Decorator
#include "CooldownDecorator.h"
#include "ProbabilityDecorator.h"
#include "CheckCloneDecorator.h"

void BTEditor::Init()
{
    // Composite 노드
    _nodeRegistry["Composite"]["Selector"] = []() -> BTNode* {
        BTNode* node = new Selector();
        node->SetName("Selector");
        return node;
        };
    _nodeRegistry["Composite"]["Sequence"] = []() -> BTNode* {
        BTNode* node = new Sequence();
        node->SetName("Sequence");
        return node;
        };
    
    // Decorator Node
    _nodeRegistry["Decorator"]["Cooldown"] = []() -> BTNode* {
        BTNode* node = new CooldownDecorator();
        node->SetName("Cooldown");
        return node;
        };
    _nodeRegistry["Decorator"]["Probability"] = []() -> BTNode* {
        BTNode* node = new ProbabilityDecorator();
        node->SetName("Probability");
        return node;
        };
    _nodeRegistry["Decorator"]["CheckClone"] = []() -> BTNode* {
        BTNode* node = new CheckCloneDecorator();
        node->SetName("CheckClone");
        return node;
        };

    // ActionNode
    _nodeRegistry["Action"]["Jump"] = []() -> BTNode* {
        BTNode* node = new BTAction_Gemini_Jump();
        node->SetName("Jump");
        return node;
        };
    _nodeRegistry["Action"]["Move"] = []() -> BTNode* {
        BTNode* node = new BTAction_Gemini_Move();
        node->SetName("Move");
        return node;
        };
    _nodeRegistry["Action"]["Idle"] = []() -> BTNode* {
        BTNode* node = new BTAction_Gemini_Idle();
        node->SetName("Idle");
        return node;
        };
    _nodeRegistry["Action"]["BaseMissile"] = []() -> BTNode* {
        BTNode* node = new BTAction_Gemini_BaseMissile();
        node->SetName("BaseMissile");
        return node;
        };
    _nodeRegistry["Action"]["Clone"] = []() -> BTNode* {
        BTNode* node = new BTAction_Gemini_CloneActivate();
        node->SetName("Clone");
        return node;
        };
    


    ImNodesStyle& style = ImNodes::GetStyle();

    // 선택된 링크를 쨍한 주황색으로!
    style.Colors[ImNodesCol_LinkSelected] = IM_COL32(255, 165, 0, 255);

    // 선택된 노드의 타이틀 바(머리 부분)도 주황색으로!
    style.Colors[ImNodesCol_TitleBarSelected] = IM_COL32(255, 165, 0, 255);

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
        wstring path = FileDialog::Save(L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0");
        if (!path.empty())
        {
            // wstring을 string으로 변환
            std::string strPath(path.begin(), path.end());
            BTSerializer::SaveToJSON(strPath, _testNodes, _links);
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        std::wstring path = FileDialog::Open(L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0");
        if (!path.empty())
        {
            std::string strPath(path.begin(), path.end());
            for (auto& it : _testNodes)
            {
                it->ClearChildren();
            }

            for (auto& it : _testNodes)
            {
                delete it;
            }
            _testNodes.clear();
            _links.clear();

            BTSerializer::LoadFromJSON(strPath,&_testNodes, &_links);

            for (auto& node : _testNodes)
            {
                if (node->GetNodeID() >= _nextNodeId)
                {
                    _nextNodeId = node->GetNodeID() + 1;
                }
            }

            for (auto& link : _links)
            {
                if (link.linkId >= _nextLinkId)
                {
                    _nextLinkId = link.linkId + 1;
                }
            }
        }
    }

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1))
    {
        _spawnPos = ImGui::GetMousePos();
        ImGui::OpenPopup("NodeCreatePopup");
    }

    if (ImGui::BeginPopup("NodeCreatePopup"))
    {
        // 맵에 등록된 목록을 순회 (pair.first는 이름 문자열, pair.second는 생성 함수)
        for (auto& pairCategory : _nodeRegistry)
        {

            if(ImGui::BeginMenu(pairCategory.first.c_str()))
            {
                for (auto& pairNode : pairCategory.second)
                {
                    if (ImGui::MenuItem(pairNode.first.c_str()))
                    {
                        BTNode* newNode = pairNode.second();
                        newNode->SetType(pairNode.first);
                        newNode->SetName(pairNode.first);

                        newNode->SetNodeID(_nextNodeId);
                        ++_nextNodeId;
                        _testNodes.push_back(newNode);
                        ImNodes::SetNodeScreenSpacePos(newNode->GetNodeID(), _spawnPos);
                    }
                }
                ImGui::EndMenu();
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
    for (const auto& link : _links)
    {
        ImNodes::Link(link.linkId, link.startAttrId, link.endAttrId);
    }

    ImNodes::EndNodeEditor();

    // 삭제 기믹 수행
    int destoryedLinkId;
    if (ImNodes::IsLinkDestroyed(&destoryedLinkId))
    {
        std::erase_if(_links, [destoryedLinkId](NodeLink node) {
            return node.linkId == destoryedLinkId;
            });
    }

    // 노드 & 링크 삭제
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
        ImGui::IsKeyPressed(ImGuiKey_Delete))
    {
        // 선택된 노드 삭제
        int selectedNodeCount = ImNodes::NumSelectedNodes();
        if (selectedNodeCount > 0)
        {
            vector<int> selectedNodes;
            selectedNodes.resize(selectedNodeCount);
            ImNodes::GetSelectedNodes(selectedNodes.data());

            erase_if(_testNodes, [&](BTNode* node) {
                    
                auto it = find(selectedNodes.begin(), selectedNodes.end(), node->GetNodeID());

                if (it != selectedNodes.end())
                {
                    node->ClearChildren();
                    delete node;
                    return true;
                }
                return false;
                });

            erase_if(_links, [&](const NodeLink& link) {
                
                int startNodeId = link.startAttrId / 100;
                int endNodeId = link.endAttrId / 100;

                auto itStart = find(selectedNodes.begin(), selectedNodes.end(), startNodeId);
                auto itEnd = find(selectedNodes.begin(), selectedNodes.end(), endNodeId);

                return (itStart != selectedNodes.end() || itEnd != selectedNodes.end());
                });

        }

        // 선택된 링크 삭제
        int selectedLinkCount = ImNodes::NumSelectedLinks();
        if (selectedLinkCount > 0)
        {
            vector<int> selectedLinks;
            selectedLinks.resize(selectedLinkCount);
            ImNodes::GetSelectedLinks(selectedLinks.data());

            erase_if(_links, [&](const NodeLink& link) {

                auto it = find(selectedLinks.begin(), selectedLinks.end(), link.linkId);
                return it != selectedLinks.end();
                });
        }

        
    }
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
    ImGui::PushID(node->GetNodeID());

    char buffer[256];
    strcpy_s(buffer, sizeof(buffer), node->GetName().c_str());

    ImGui::PushItemWidth(120.0f);

    if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
    {
        node->SetName(buffer);
    }
    ImGui::PopItemWidth();
    ImGui::PopID();
    node->DrawProperty();
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
