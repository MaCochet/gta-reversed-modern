/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "CPtrNode.h"

class CPtrList {
public:
    CPtrNode* pNode;
    inline CPtrNode *GetNode() {
        return pNode;
    }

    inline CPtrList() {
        pNode = nullptr;
    }

    // get elements count
    uint32 CountElements();
    
    /*
    uint32 CountElements() {
        uint32 counter;
        CPtrNode *currNode = GetNode();
        for (counter = 0; currNode; ++counter)
            currNode = currNode->pNext;
        return counter;
    }
    */

    // check if data is a member of list
    bool IsMemberOfList(void* data);

    /*
    bool IsMemberOfList(void* data) {
        CPtrNode *currNode = GetNode();
        while (currNode) {
            if (currNode->pData == data)
                return true;
            currNode = currNode->pNext;
        }
        return false;
    }
    */

};

VALIDATE_SIZE(CPtrList, 4);