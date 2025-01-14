/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once


#include "CTaskComplex.h"
#include "CTaskTimer.h"
#include "CTaskComplex.h"

class CTaskComplexKillPedOnFoot : public CTaskComplex {
public:
    uint8 m_bit_1 : 1;
    uint8 m_bit_2 : 1;
    uint8 m_bit_3 : 1;
    uint8 m_bit_4 : 1;
    uint8 m_bit_5 : 1;
    uint8 m_bit_6 : 1;
    uint8 m_bit_7 : 1;
    uint8 m_bit_8 : 1;
    uint8 _pad0[3];
    CPed* m_target;
    uint32 m_pedFlags;
    int32 m_actionDelay;
    int32 m_actionChance;
    uint8 field_20;
    uint8 _pad1[3];
    uint32 m_startTime;
    uint32 m_time;
    CTaskTimer m_timer;

    static void InjectHooks();

    CTaskComplexKillPedOnFoot(CPed* target, int32 time, uint32 pedFlags, int32 delay, int32 chance, int8 a7);
    ~CTaskComplexKillPedOnFoot();
private:
    CTaskComplexKillPedOnFoot* Constructor(CPed* target, int32 time, uint32 pedFlags, int32 delay, int32 chance, int8 a7);
public:
    CTask* Clone() override { return new CTaskComplexKillPedOnFoot(m_target, m_time, m_pedFlags, m_actionDelay, m_actionChance, field_20); }
    eTaskType GetTaskType() override { return TASK_COMPLEX_KILL_PED_ON_FOOT; }
    bool MakeAbortable(CPed* ped, eAbortPriority priority, const CEvent* event) override;
    CTask* CreateNextSubTask(CPed* ped) override;
    CTask* CreateFirstSubTask(CPed* ped) override;
    CTask* ControlSubTask(CPed* ped) override;
    void* CreateSubTask(int32 taskId, CPed* ped);
};

VALIDATE_SIZE(CTaskComplexKillPedOnFoot, 0x38);
