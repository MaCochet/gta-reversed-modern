#include "StdInc.h"

void CTaskComplexJump::InjectHooks()
{
    ReversibleHooks::Install("CTaskComplexJump", "Constructor", 0x67A030, &CTaskComplexJump::Constructor);
    ReversibleHooks::Install("CTaskComplexJump", "CreateSubTask", 0x67D980, &CTaskComplexJump::CreateSubTask);
    //VTABLE
    ReversibleHooks::Install("CTaskComplexJump", "CreateFirstSubTask", 0x67FD10, &CTaskComplexJump::CreateFirstSubTask_Reversed);
    ReversibleHooks::Install("CTaskComplexJump", "CreateNextSubTask", 0x67FC00, &CTaskComplexJump::CreateNextSubTask_Reversed);
    ReversibleHooks::Install("CTaskComplexJump", "Clone", 0x67C5A0, &CTaskComplexJump::Clone_Reversed);
    ReversibleHooks::Install("CTaskComplexJump", "MakeAbortable", 0x67A070, &CTaskComplexJump::MakeAbortable_Reversed);
}

CTaskComplexJump* CTaskComplexJump::Constructor(eComplexJumpType jumpType)
{
    this->CTaskComplexJump::CTaskComplexJump(jumpType);
    return this;
}

// 0x67A030
CTaskComplexJump::CTaskComplexJump(eComplexJumpType type)
{
    m_nType = type;
    m_bHighJump = false;
}

// 0x67C5A0
CTask* CTaskComplexJump::Clone()
{
    return Clone_Reversed();
}

// 0x67FD10
CTask* CTaskComplexJump::CreateFirstSubTask(CPed* ped)
{
    return CreateFirstSubTask_Reversed(ped);
}

// 0x67FC00
CTask* CTaskComplexJump::CreateNextSubTask(CPed* ped)
{
    return CreateNextSubTask_Reversed(ped);
}

// 0x67A070
bool CTaskComplexJump::MakeAbortable(CPed* ped, eAbortPriority priority, const CEvent* event)
{
    return MakeAbortable_Reversed(ped, priority, event);
}

CTask* CTaskComplexJump::Clone_Reversed()
{
    auto newTask = new CTaskComplexJump(m_nType);
    newTask->m_bHighJump = this->m_bHighJump;
    return newTask;
}

CTask* CTaskComplexJump::CreateFirstSubTask_Reversed(CPed* ped)
{
    return CreateSubTask(TASK_SIMPLE_JUMP, ped);
}

CTask* CTaskComplexJump::CreateNextSubTask_Reversed(CPed* ped)
{
    eTaskType subTaskType = m_pSubTask->GetTaskType();

    if (subTaskType == TASK_SIMPLE_CLIMB)
        return CreateSubTask(ped->bIsInTheAir ? TASK_COMPLEX_IN_AIR_AND_LAND : TASK_FINISHED, ped);

    if (subTaskType == TASK_SIMPLE_HIT_HEAD || subTaskType == TASK_COMPLEX_IN_AIR_AND_LAND)
    {
        ped->bIsLanding = false;
        return CreateSubTask(TASK_FINISHED, ped);
    }

    if (subTaskType == TASK_SIMPLE_JUMP)
    {
        auto pJumpTask = reinterpret_cast<CTaskSimpleJump*>(m_pSubTask);

        if (!pJumpTask->m_bLaunchAnimStarted)
        {
            ped->bIsLanding = false;
            return CreateSubTask(TASK_FINISHED, ped);
        }
        else if (pJumpTask->m_bIsJumpBlocked)
        {
            ped->bIsLanding = true;
            return CreateSubTask(TASK_SIMPLE_HIT_HEAD, ped);
        }
        else if (pJumpTask->m_pClimbEntity && m_nType != -1)
        {
            ped->bIsInTheAir = true;
            return CreateSubTask(TASK_SIMPLE_CLIMB, ped);
        }
        else
        {
            ped->bIsInTheAir = true;
            return CreateSubTask(TASK_COMPLEX_IN_AIR_AND_LAND, ped);
        }
    }

    return nullptr;
}

bool CTaskComplexJump::MakeAbortable_Reversed(CPed* ped, eAbortPriority priority, const CEvent* event)
{
    if (priority == ABORT_PRIORITY_URGENT && event)
    {
        if (event->GetEventType() == EVENT_DAMAGE)
        {
            const auto pDamageEvent = static_cast<const CEventDamage*>(event);
            if (pDamageEvent->m_weaponType == WEAPON_FALL
                && pDamageEvent->m_damageResponse.m_bHealthZero
                && pDamageEvent->m_bAddToEventGroup
                )
            {
                ped->bIsInTheAir = false;
                ped->bIsLanding = false;
                return true;
            }
        }
        else if (event->GetEventType() == EVENT_DEATH)
        {
            ped->bIsInTheAir = false;
            ped->bIsLanding = false;
            return true;
        }
    }

    if (m_pSubTask->MakeAbortable(ped, priority, event))
    {
        ped->bIsInTheAir = false;
        ped->bIsLanding = false;
        return true;
    }

    return false;
}

// 0x67D980
CTask* CTaskComplexJump::CreateSubTask(eTaskType taskType, CPed* ped)
{
    switch (taskType)
    {
    case TASK_SIMPLE_HIT_HEAD:
        return new CTaskSimpleHitHead();
    case TASK_FINISHED:
        ped->bIsInTheAir = false;
        return nullptr;
    case TASK_SIMPLE_JUMP:
    {
        auto pTask = new CTaskSimpleJump(m_nType == COMPLEX_JUMP_TYPE_CLIMB);
        if (m_bHighJump || CPedGroups::IsInPlayersGroup(ped))
            pTask->m_bHighJump = true;
        return pTask;
    }
    case TASK_SIMPLE_CLIMB:
        if (m_pSubTask && m_pSubTask->GetTaskType() == TASK_SIMPLE_JUMP)
        {
            auto pJumpTask = reinterpret_cast<CTaskSimpleJump*>(m_pSubTask);
            return new CTaskSimpleClimb(
                pJumpTask->m_pClimbEntity,
                pJumpTask->m_vecClimbPos,
                pJumpTask->m_fClimbAngle,
                pJumpTask->m_nClimbSurfaceType,
                pJumpTask->m_vecClimbPos.z - ped->GetPosition().z < CTaskSimpleClimb::ms_fMinForStretchGrab ? CLIMB_PULLUP : CLIMB_GRAB,
                m_nType == COMPLEX_JUMP_TYPE_CLIMB
            );
        }
        else
        {
            return new CTaskComplexInAirAndLand(true, false);
        }
    case TASK_COMPLEX_IN_AIR_AND_LAND:
    {
        auto pNewTask = new CTaskComplexInAirAndLand(true, false);

        if (m_pSubTask->GetTaskType() == TASK_SIMPLE_CLIMB && reinterpret_cast<CTaskSimpleClimb*>(m_pSubTask)->m_bInvalidClimb)
            pNewTask->m_bInvalidClimb = true;

        return pNewTask;
    }
    default:
        return nullptr;
    }
}
