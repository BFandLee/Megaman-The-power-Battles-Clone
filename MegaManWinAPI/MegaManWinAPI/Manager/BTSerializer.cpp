#include "pch.h"
#include "BTSerializer.h"
#include "BTNode.h"
#include "BTNodeFactory.h"

void BTSerializer::SaveToJSON(const std::string& filepath, const std::vector<class BTNode*>& nodes,
    const std::vector<NodeLink>& links)
{
    json j;
    j["Nodes"] = json::array(); // Nodes라는 이름의 배열 생성

    for (auto node : nodes)
    {
        json n;
        n["ID"] = node->GetNodeID();
        n["Type"] = node->GetType();
        n["Name"] = node->GetName();
        n["PosX"] = ImNodes::GetNodeEditorSpacePos(node->GetNodeID()).x;
        n["PosY"] = ImNodes::GetNodeEditorSpacePos(node->GetNodeID()).y;
        node->SaveProperty(n);
        j["Nodes"].push_back(n);     // 배열에 추가
    }

    j["Links"] = json::array();

    for (auto link : links)
    {
        json n;
        n["LinkID"] = link.linkId;
        n["StartAttrID"] = link.startAttrId;
        n["EndAttrID"] = link.endAttrId;

        j["Links"].push_back(n);
    }

    std::ofstream file(filepath);
    file << j.dump(4); // 들여쓰기 4칸
}

BTNode* BTSerializer::LoadFromJSON(const std::string& filepath, vector<BTNode*>* outNodes, vector<NodeLink>* outLinks)
{
    map<int32, BTNode*> nodeMap;
    map<int32, float> nodePosYMap;
    std::ifstream file(filepath);
    if (!file.is_open()) return nullptr;

    json j;
    try
    {
        file >> j;
    }
    catch (const json::parse_error& e)
    {
        return nullptr;
    }

    

    if (j.contains("Nodes") && j["Nodes"].is_array())
    {
        for (const auto& nodeData : j["Nodes"])
        {
            int id = nodeData["ID"];
            std::string type = nodeData["Type"];

            BTNode* createdNode = BTNodeFactory::GetInstance().CreateNode(type);
            if (createdNode != nullptr)
            {
                createdNode->LoadProperty(nodeData);

                if (nodeData.contains("Name") && nodeData.contains("Type"))
                {
                    createdNode->SetName(nodeData["Name"]);
                    createdNode->SetType(nodeData["Type"]);
                }

                int id = nodeData["ID"];
                createdNode->SetNodeID(id);
                nodeMap[id] = createdNode;
                nodePosYMap[id] = nodeData.value("PosY", 0.0f);
                
                if (outNodes != nullptr)
                {
                    float posX = 0.0f;
                    float posY = 0.0f;
                    if (nodeData.contains("PosX") && nodeData.contains("PosY"))
                    {
                        posX = nodeData["PosX"];
                        posY = nodeData["PosY"];
                    }
                    
                    Vector pos = Vector(posX, posY);
                    ImNodes::SetNodeEditorSpacePos(id, ImVec2(pos.x, pos.y));
                    outNodes->push_back(createdNode);
                }
                
            }

        }
    }

    // 임시 맵
    map<BTNode*, vector<BTNode*>> parentToChildren;
    unordered_set<int> childNodeIDs;

    if (j.contains("Links") && j["Links"].is_array())
    {
        // 2. 전체 링크 관계 수집
        for (const auto& linkData : j["Links"])
        {
            int startAttr = linkData["StartAttrID"];
            int endAttr = linkData["EndAttrID"];
            int parentId = startAttr / 100;
            int childId = endAttr / 100;
            auto parentIt = nodeMap.find(parentId);
            auto childIt = nodeMap.find(childId);
            if (parentIt != nodeMap.end() && childIt != nodeMap.end())
            {
                BTNode* parentNode = parentIt->second;
                BTNode* childNode = childIt->second;
                childNodeIDs.insert(childNode->GetNodeID());
                parentToChildren[parentNode].push_back(childNode);
            }
            if (outLinks != nullptr)
            {
                NodeLink newLink;
                newLink.linkId = linkData["LinkID"];
                newLink.startAttrId = startAttr;
                newLink.endAttrId = endAttr;
                outLinks->push_back(newLink);
            }
        }
        // 3. 수집 완료 후 각 부모별 자식들을 PosY(위->아래) 순서로 정렬하여 AddChild
        for (auto& pair : parentToChildren)
        {
            BTNode* parent = pair.first;
            vector<BTNode*>& children = pair.second;
            sort(children.begin(), children.end(), [&](BTNode* nodeA, BTNode* nodeB)
                {
                    float PosY_A = nodePosYMap[nodeA->GetNodeID()];
                    float PosY_B = nodePosYMap[nodeB->GetNodeID()];
                    return PosY_A < PosY_B;
                });
            for (BTNode* child : children)
            {
                parent->AddChild(child);
            }
        }

        // 1순위: Type이 "Root"인 노드를 최우선으로 루트 반환
        for (auto& iter : nodeMap)
        {
            if (iter.second != nullptr && iter.second->GetType() == "Root")
            {
                return iter.second;
            }
        }
        // 2순위: 부모가 없는 최상위 노드 반환
        for (auto& iter : nodeMap)
        {
            if (iter.second != nullptr && childNodeIDs.find(iter.first) == childNodeIDs.end())
            {
                return iter.second;
            }
        }
        return nullptr;
    }
}