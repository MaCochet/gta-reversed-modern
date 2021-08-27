#pragma once

#include "CTaskSimple.h"

class CTaskSimpleSetStayInSamePlace : public CTaskSimple {
public:
    unsigned char m_bStayInSamePlace;
    unsigned char _pad_9[3];

public:
    CTaskSimpleSetStayInSamePlace(bool bStayInSamePlace);
    ~CTaskSimpleSetStayInSamePlace() override {}

    CTask* Clone() const override { return new CTaskSimpleSetStayInSamePlace(m_bStayInSamePlace); }
    eTaskType GetId() override { return TASK_SIMPLE_SET_STAY_IN_SAME_PLACE; }
    bool ProcessPed(CPed* ped) override;
    bool MakeAbortable(class CPed* ped, eAbortPriority priority, const CEvent* event) override { return true; }

private:
    friend void InjectHooksMain();
    static void InjectHooks();

    CTaskSimpleSetStayInSamePlace* Constructor(bool bStayInSamePlace);

    bool ProcessPed_Reversed(CPed* ped);
};

VALIDATE_SIZE(CTaskSimpleSetStayInSamePlace, 0xC);
