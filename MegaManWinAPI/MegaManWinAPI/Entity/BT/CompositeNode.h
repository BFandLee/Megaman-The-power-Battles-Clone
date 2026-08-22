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

    virtual void ClearChildren() override
    {
        _children.clear();
    }

protected:
    std::vector<BTNode*> _children;
    int _currentNodeIndex = 0;
};