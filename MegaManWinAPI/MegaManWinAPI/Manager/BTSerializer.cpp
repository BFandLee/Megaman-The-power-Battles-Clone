#include "pch.h"
#include "BTSerializer.h"
#include "BTNode.h"
#include "BTNodeFactory.h"

void BTSerializer::SaveToJSON(const std::string& filepath, const std::vector<BTNode*>& nodes, const std::vector<struct NodeLink>& links)
{
    json j;
    j["Nodes"] = json::array(); // Nodes라는 이름의 배열 생성

    for (auto node : nodes)
    {
        json n;
        n["ID"] = node->GetNodeID();
        n["Type"] = node->GetName(); // 팩토리를 위해 타입 이름 저장
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

BTNode* BTSerializer::LoadFromJSON(const std::string& filepath)
{
    map<int32, BTNode*> nodeMap;
    std::ifstream file(filepath);
    if (!file.is_open()) return nullptr;

    json j;
    file >> j;

    unordered_set<int> childNodeIDs;

    if (j.contains("Nodes") && j["Nodes"].is_array())
    {
        for (const auto& nodeData : j["Nodes"])
        {
            int id = nodeData["ID"];
            std::string type = nodeData["Type"];

            BTNode* createdNode = BTNodeFactory::GetInstance().CreateNode(type);

            if (createdNode != nullptr)
            {
                int id = nodeData["ID"];
                createdNode->SetNodeID(id);
                
                nodeMap[id] = createdNode;
            }

        }
    }

    if (j.contains("Links") && j["Links"].is_array())
    {
        for (const auto& linkData : j["Links"])
        {
            int startAttr = linkData["StartAttrID"];
            int endAttr = linkData["EndAttrID"];

            // 원본 노드 ID 복원
            int parentId = startAttr / 100;
            int childId = endAttr / 100;

            // map에서 부모와 자식 노드 검색
            auto parentIt = nodeMap.find(parentId);
            auto childIt = nodeMap.find(childId);

            // 두 노드 모두 성공적으로 찾았으면 연결
            if (parentIt != nodeMap.end() && childIt != nodeMap.end())
            {
                BTNode* parentNode = parentIt->second;
                BTNode* childNode = childIt->second;
                childNodeIDs.insert(childNode->GetNodeID());
            
                parentNode->AddChild(childNode);
            }
            
            
        }
    }

    for (auto& iter : nodeMap)
    {
        auto it = childNodeIDs.find(iter.first);

        if (it == childNodeIDs.end())
        {
            return iter.second;
        }
    }

    return nullptr;
}