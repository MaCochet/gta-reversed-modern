#include "CTaskComplex.h"

class CTaskComplexBeInGroup : public CTaskComplex {
public:
    int32_t m_groupId;
    bool m_isLeader;
    int8_t _pad[3];
    CPed* m_ped;
    CTask* m_mainTask;
    int32_t m_mainTaskId;
    CTask* m_secondaryTask;
    int32_t m_secondaryTaskSlot; // used as index in CTaskManager::m_aSecondaryTasks

public:
    CTaskComplexBeInGroup(int32_t groupId, bool isLeader);
    ~CTaskComplexBeInGroup() { }

    CTask* Clone() const override;
    eTaskType GetId() override { return TASK_COMPLEX_BE_IN_GROUP; }
    bool MakeAbortable(CPed* ped, eAbortPriority priority, const CEvent* event) override;
    CTask* CreateNextSubTask(CPed* ped) override;
    CTask* CreateFirstSubTask(CPed* ped) override;
    CTask* ControlSubTask(CPed* ped) override;

};

VALIDATE_SIZE(CTaskComplexBeInGroup, 0x28);
