#pragma once
#include "BTNode.h"

class CompositeNode : public BTNode
{
public:
    virtual ~CompositeNode()
    {
        for (auto child : _children)
            delete child;
    }

    void AddChild(BTNode* child)
    {
        _children.push_back(child);
    }

protected:
    std::vector<BTNode*> _children;
    int _currentNodeIndex = 0;
};