#include "pch.h"
#include "BTEditor.h"
#include "InputManager.h"
#include "BTSerializer.h"
#include "BTNode.h"
#include "Selector.h"
#include "Sequence.h"
#include "Game.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Actor.h"
#include "Boss.h"
#include "TimeManager.h"
#include "FSMComponent.h"
#include "ResourceManager.h"

// 보스 ActionNode
#include "ActionNode.h"
#include "GeminiCommonNodes.h"
#include "GeminiPatten.h"

// 보스 Decorator
#include "CooldownDecorator.h"
#include "ProbabilityDecorator.h"
#include "CheckCloneDecorator.h"
#include "CheckPlayerAttackingDecorator.h"
#include "SucceederDecorator.h"
#include "CheckCloneDurationDecorator.h"

void BTEditor::Init()
{
    // Root 카테고리
    _nodeRegistry["Root"]["Root"] = []() -> BTNode* {
        BTNode* node = new Sequence();
        node->SetName("Root");
        return node;
        };

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
    _nodeRegistry["Decorator"]["CheckPlayerAttacking"] = []() -> BTNode* {
        BTNode* node = new CheckPlayerAttackingDecorator();
        node->SetName("Probability");
        return node;
        };
    _nodeRegistry["Decorator"]["Succeeder"] = []() -> BTNode* {
        BTNode* node = new SucceederDecorator();
        node->SetName("Succeeder");
        return node;
        };

    // 분신(Clone) 카테고리
    _nodeRegistry["Clone"]["CheckClone"] = []() -> BTNode* {
        BTNode* node = new CheckCloneDecorator();
        node->SetName("CheckClone");
        return node;
        };
    _nodeRegistry["Clone"]["CheckCloneDuration"] = []() -> BTNode* {
        BTNode* node = new CheckCloneDurationDecorator();
        node->SetName("CheckCloneDuration");
        return node;
        };
    _nodeRegistry["Clone"]["Clone"] = []() -> BTNode* {
        BTNode* node = new BTAction_Gemini_CloneActivate();
        node->SetName("Clone");
        return node;
        };
    _nodeRegistry["Clone"]["CloneDeactivate"] = []() -> BTNode* {
        BTNode* node = new BTAction_Gemini_CloneDeactivate();
        node->SetName("CloneDeactivate");
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
    _nodeRegistry["Action"]["LaserMissile"] = []() -> BTNode* {
        BTNode* node = new BTAction_Gemini_LaserMissile();
        node->SetName("LaserMissile");
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

    const int32 EDITOR_WIDTH = 768; // 에디터 패널의 가로 폭

    if (InputManager::GetInstance().GetButtonDown(KeyType::Tilde))
    {
        ToggleEditor();

        if (_isOpen)
        {
            // 에디터가 열리면 가로폭 확장 (1152 + 800 = 1952)
            Game::GetInstance().ResizeWindow(GWinSizeX + EDITOR_WIDTH, GWinSizeY);
        }
        else
        {
            // 에디터가 닫히면 기본 게임 화면 크기(1152)로 복구
            Game::GetInstance().ResizeWindow(GWinSizeX, GWinSizeY);
        }
    }

    if (!_isOpen) return;
    // 1. 에디터 윈도우 위치와 크기 강제 지정
    ImGui::SetNextWindowPos(ImVec2((float)GWinSizeX, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((float)EDITOR_WIDTH, (float)GWinSizeY), ImGuiCond_Always);

    // 2. 창 이동/크기조절/접기를 막아 완벽한 패널 형태로 고정
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("BT Live Visualizer", &_isOpen, windowFlags);

    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsWindowHovered() && io.KeyCtrl && io.MouseWheel != 0.0f)
    {
        _zoomScale += io.MouseWheel * 0.05f;
        _zoomScale = std::clamp(_zoomScale, 0.5f, 1.5f);
    }

    ImGui::SetWindowFontScale(_zoomScale);

    float dt = TimeManager::GetInstance().GetDT();
    for (auto& pair : _debugStateMap)
    {
        if (pair.second.holdTimer > 0.0f)
        {
            pair.second.holdTimer -= dt;
        }
    }

    if (ImGui::Button("Save"))
    {
        wstring path = FileDialog::Save(L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0", ResourceManager::GetInstance().GetBTDirectory().c_str());
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
        std::wstring path = FileDialog::Open(L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0", ResourceManager::GetInstance().GetBTDirectory().c_str());
        if (!path.empty())
        {
            std::string strPath(path.begin(), path.end());
            LoadBTFromFile(strPath);
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Auto Layout"))
    {
        AutoLayout();
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset View"))
    {
        ImNodes::EditorContextResetPanning(ImVec2(0.0f, 0.0f));
    }

    ImGui::SameLine();
    if (ImGui::Button("Phase 1"))
    {
        // 1) 에디터 트리 동기화
        LoadBTFromFile(ResourceManager::GetInstance().GetBTPath("Phase1Nodes.json"));
        AutoLayout();
        // 2) 현재 씬의 보스 탐색 및 상태/HP 변경
        Scene* scene = SceneManager::GetInstance().GetScene();
        if (scene)
        {
            for (Actor* actor : scene->GetActors())
            {
                if (actor && actor->GetActorType() == ActorType::Boss)
                {
                    Boss* boss = static_cast<Boss*>(actor);
                    boss->SetHP(boss->GetMaxHP());
                    boss->GetComponent<FSMComponent>()->ChangeState("Phase1");
                    break;
                }
            }
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Phase 2"))
    {
        // 1) 에디터 트리 동기화
        LoadBTFromFile(ResourceManager::GetInstance().GetBTPath("Phase2Nodes.json"));
        AutoLayout();
        // 2) 현재 씬의 보스 탐색 및 상태/HP 변경
        Scene* scene = SceneManager::GetInstance().GetScene();
        if (scene)
        {
            for (Actor* actor : scene->GetActors())
            {
                if (actor && actor->GetActorType() == ActorType::Boss)
                {
                    Boss* boss = static_cast<Boss*>(actor);
                    boss->SetHP(boss->GetMaxHP() * 0.5f);
                    boss->GetComponent<FSMComponent>()->ChangeState("Phase2");
                    break;
                }
            }
        }
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(100.0f);
    ImGui::SliderFloat("Zoom", &_zoomScale, 0.5f, 1.5f, "%.2fx");
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("Reset Zoom"))
    {
        _zoomScale = 1.0f;
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

    ImNodes::PushStyleVar(ImNodesStyleVar_NodePadding, ImVec2(8.0f * _zoomScale, 5.0f * _zoomScale));
    ImNodes::PushStyleVar(ImNodesStyleVar_PinCircleRadius, 4.0f * _zoomScale);
    ImNodes::PushStyleVar(ImNodesStyleVar_PinHoverRadius, 8.0f * _zoomScale);
    
    // imnodes 캔버스를 시작하고 종료
    ImNodes::BeginNodeEditor();
    
    std::vector<int> validPins;
    for (auto node : _testNodes)
    {
        DrawNode(node);
        validPins.push_back(node->GetNodeID() * 100); // In pin
        if (!node->IsLeafNode())
        {
            validPins.push_back(node->GetNodeID() * 100 + 1); // Out pin
        }
    }

    // Draw Link
    for (auto it = _links.begin(); it != _links.end(); )
    {
        bool startValid = std::find(validPins.begin(), validPins.end(), it->startAttrId) != validPins.end();
        bool endValid = std::find(validPins.begin(), validPins.end(), it->endAttrId) != validPins.end();
        
        if (startValid && endValid)
        {
            ImNodes::Link(it->linkId, it->startAttrId, it->endAttrId);
            ++it;
        }
        else
        {
            // 유효하지 않은 핀을 가리키는 링크는 안전하게 제거
            it = _links.erase(it);
        }
    }

    ImNodes::EndNodeEditor();

    ImNodes::PopStyleVar(3);

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
                    _debugStateMap.erase(node->GetNodeID());
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
    int32 nodeId = node->GetNodeID();
    bool hasCustomColor = false;

    // 실시간 디버그 상태 확인
    auto it = _debugStateMap.find(nodeId);
    if (it != _debugStateMap.end() && it->second.holdTimer > 0.0f)
    {
        ImU32 titleColor = 0;

        switch (it->second.lastState)
        {
        case NodeState::Running:
            titleColor = IM_COL32(230, 160, 30, 255);  // 노랑 / 주황
            break;
        case NodeState::Success:
            titleColor = IM_COL32(40, 180, 70, 255);   // 초록
            break;
        case NodeState::Failure:
            titleColor = IM_COL32(200, 50, 50, 255);   // 빨강
            break;
        }
        if (titleColor != 0)
        {
            ImNodes::PushColorStyle(ImNodesCol_TitleBar, titleColor);
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, titleColor);
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, titleColor);
            hasCustomColor = true;
        }
    }
    ImNodes::BeginNode(node->GetNodeID());
    ImNodes::BeginNodeTitleBar();

    if (_zoomScale > 0.75f)
    {
        ImGui::PushID(node->GetNodeID());

        char buffer[256];
        strcpy_s(buffer, sizeof(buffer), node->GetName().c_str());
        ImGui::PushItemWidth(120.0f * _zoomScale);
        if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
        {
            node->SetName(buffer);
        }
        ImGui::PopItemWidth();
        ImGui::PopID();
        node->DrawProperty();
    }
    else
    {
        ImGui::TextUnformatted(node->GetName().c_str());
    }
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

    // 적용했던 커스텀 색상 Pop
    if (hasCustomColor)
    {
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
    }
}

void BTEditor::LoadBTFromFile(const string& filePath)
{
    // 1. 기존 에디터 노드 및 링크 메모리 정리
    for (auto& it : _testNodes)
        it->ClearChildren();
    for (auto& it : _testNodes)
        delete it;
    _testNodes.clear();
    _links.clear();

    // 2. 새로운 JSON 파일 로드
    BTSerializer::LoadFromJSON(filePath, &_testNodes, &_links);

    // 3. ID 카운터 동기화
    for (auto& node : _testNodes)
    {
        if (!node) continue;
        if (node->GetNodeID() >= _nextNodeId) _nextNodeId = node->GetNodeID() + 1;
    }
    for (auto& link : _links)
        if (link.linkId >= _nextLinkId) _nextLinkId = link.linkId + 1;

    _zoomScale = 1.0f;
}

void BTEditor::ReportNodeState(int32 nodeId, NodeState state)
{
    // 에디터가 닫혀있으면 기록하지 않음 (성능 최적화)
    if (!_isOpen) return;

    _debugStateMap[nodeId].lastState = state;
    _debugStateMap[nodeId].holdTimer = 0.3f; // 0.3초 동안 색상 유지
}

float BTEditor::CalculateSubtreeHeight(BTNode* node, const map<BTNode*, vector<BTNode*>>& parentToChildren, map<BTNode*, float>& outHeights)
{
    if (node == nullptr) return 0.0f;

    const float NODE_ROW_HEIGHT = 130.0f; // 노드 1개가 기본 차지하는 세로 높이 (높이 80px + 여백 50px)

    auto it = parentToChildren.find(node);
    if (it == parentToChildren.end() || it->second.empty())
    {
        outHeights[node] = NODE_ROW_HEIGHT;
        return NODE_ROW_HEIGHT;
    }

    float totalHeight = 0.0f;
    for (BTNode* child : it->second)
    {
        totalHeight += CalculateSubtreeHeight(child, parentToChildren, outHeights);
    }

    outHeights[node] = totalHeight;
    return totalHeight;
}

void BTEditor::LayoutSubtree(BTNode* node, float depthX, float startY, const map<BTNode*, vector<BTNode*>>& parentToChildren, const map<BTNode*, float>& subtreeHeights, set<int32>& visited)
{
    if (node == nullptr) return;
    visited.insert(node->GetNodeID());

    const float LEVEL_WIDTH = 260.0f;    // 계층(Depth) 간 가로 간격
    const float NODE_BOX_HEIGHT = 70.0f; // 노드 자체의 세로 높이

    float currentSubtreeH = 130.0f;
    auto hIt = subtreeHeights.find(node);
    if (hIt != subtreeHeights.end())
        currentSubtreeH = hIt->second;

    // 부모 노드는 자신의 서브트리가 차지하는 Y 영역의 정중앙에 위치
    float nodeY = startY + (currentSubtreeH - NODE_BOX_HEIGHT) * 0.5f;
    ImVec2 pos(depthX, nodeY);

    ImNodes::SetNodeGridSpacePos(node->GetNodeID(), pos);

    auto it = parentToChildren.find(node);
    if (it != parentToChildren.end() && !it->second.empty())
    {
        float childY = startY;
        for (BTNode* child : it->second)
        {
            float childH = 130.0f;
            auto chIt = subtreeHeights.find(child);
            if (chIt != subtreeHeights.end())
                childH = chIt->second;

            LayoutSubtree(child, depthX + LEVEL_WIDTH, childY, parentToChildren, subtreeHeights, visited);
            childY += childH;
        }
    }
}

void BTEditor::AutoLayout()
{
    if (_testNodes.empty()) return;

    // 1. 노드 ID로 빠른 검색을 위한 맵 구축
    map<int32, BTNode*> idToNodeMap;
    for (BTNode* node : _testNodes)
    {
        if (node) idToNodeMap[node->GetNodeID()] = node;
    }

    // 2. 링크를 순회하여 parentToChildren 트리 관계 구성 및 자식 노드 ID 수집
    map<BTNode*, vector<BTNode*>> parentToChildren;
    set<int32> childNodeIDs;

    for (const auto& link : _links)
    {
        int32 parentId = link.startAttrId / 100;
        int32 childId = link.endAttrId / 100;

        auto pIt = idToNodeMap.find(parentId);
        auto cIt = idToNodeMap.find(childId);
        if (pIt != idToNodeMap.end() && cIt != idToNodeMap.end())
        {
            parentToChildren[pIt->second].push_back(cIt->second);
            childNodeIDs.insert(childId);
        }
    }

    // 3. 각 부모별 자식들의 순서를 기존 Y 좌표(위->아래) 기준으로 정렬하여 사용자 의도 순서 보존
    for (auto& pair : parentToChildren)
    {
        vector<BTNode*>& children = pair.second;
        sort(children.begin(), children.end(), [&](BTNode* a, BTNode* b) {
            float yA = ImNodes::GetNodeGridSpacePos(a->GetNodeID()).y;
            float yB = ImNodes::GetNodeGridSpacePos(b->GetNodeID()).y;
            return yA < yB;
        });
    }

    // 4. 루트 노드 탐색 (1순위: Type == "Root", 2순위: 부모가 없는 노드, 3순위: 0번 노드)
    BTNode* rootNode = nullptr;
    for (BTNode* node : _testNodes)
    {
        if (node && node->GetType() == "Root")
        {
            rootNode = node;
            break;
        }
    }
    if (!rootNode)
    {
        for (BTNode* node : _testNodes)
        {
            if (node && childNodeIDs.find(node->GetNodeID()) == childNodeIDs.end())
            {
                rootNode = node;
                break;
            }
        }
    }
    if (!rootNode && !_testNodes.empty())
    {
        rootNode = _testNodes[0];
    }

    if (!rootNode) return;

    // 5. DFS 후위 순회로 각 서브트리의 세로 높이(Height) 계산
    map<BTNode*, float> subtreeHeights;
    CalculateSubtreeHeight(rootNode, parentToChildren, subtreeHeights);

    // 6. DFS 전위 순회로 좌->우(Left-to-Right) 계층 좌표 배치
    set<int32> visited;
    const float START_X = 50.0f;
    const float START_Y = 50.0f;
    LayoutSubtree(rootNode, START_X, START_Y, parentToChildren, subtreeHeights, visited);

    // 7. 루트와 연결되지 않은 고립된 노드들(Disconnected Nodes)이 있다면 트리 하단에 나란히 배치
    float disconnectedY = START_Y + subtreeHeights[rootNode] + 60.0f;
    for (BTNode* node : _testNodes)
    {
        if (node && visited.find(node->GetNodeID()) == visited.end())
        {
            ImVec2 pos(START_X, disconnectedY);
            ImNodes::SetNodeGridSpacePos(node->GetNodeID(), pos);
            disconnectedY += 130.0f;
        }
    }

    // 8. 뷰를 Root 위치(원점)로 리셋
    ImNodes::EditorContextResetPanning(ImVec2(0.0f, 0.0f));
}
