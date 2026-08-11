#pragma once
#include <string>
#include "pch.h"

// BT 노드의 데이터를 저장하거나 불러오기 위한 헬퍼 클래스
class BTSerializer
{
public:
    // 에디터에서 구성한 노드 데이터(JSON 형식)를 파일로 저장
    static void SaveToJSON(const std::string& filepath, const std::vector<class BTNode*>& nodes, 
        const std::vector<struct NodeLink>& links);

    // 저장된 JSON 파일을 읽어와 노드 데이터로 파싱
    static BTNode* LoadFromJSON(const std::string& filepath);
};
