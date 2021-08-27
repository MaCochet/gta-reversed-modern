#include "StdInc.h"

void CTaskSimpleStandStill::InjectHooks()
{
    HookInstall(0x62F310, &CTaskSimpleStandStill::Constructor);
    HookInstall(0x635CF0, &CTaskSimpleStandStill::Clone_Reversed);
    HookInstall(0x4B8690, &CTaskSimpleStandStill::MakeAbortable_Reversed);
    HookInstall(0x62F370, &CTaskSimpleStandStill::ProcessPed_Reversed);
}

CTaskSimpleStandStill::CTaskSimpleStandStill(int nTime, bool Looped, bool bUseAnimIdleStance, float fBlendData)
{
    m_nTime = nTime;
    m_fBlendData = fBlendData;
    m_bLooped = Looped;
    m_bUseAnimIdleStance = bUseAnimIdleStance;
}

CTaskSimpleStandStill::~CTaskSimpleStandStill()
{
    // nothing here
}

// 0x62F310
CTaskSimpleStandStill* CTaskSimpleStandStill::Constructor(int nTime, bool Looped, bool bUseAnimIdleStance, float fBlendData)
{
    this->CTaskSimpleStandStill::CTaskSimpleStandStill(nTime, Looped, bUseAnimIdleStance, fBlendData);
    return this;
}

// 0x635CF0
CTask* CTaskSimpleStandStill::Clone() const
{
    return CTaskSimpleStandStill::Clone_Reversed();
}

// 0x4B8690
bool CTaskSimpleStandStill::MakeAbortable(CPed* ped, eAbortPriority priority, const CEvent* event)
{
    return CTaskSimpleStandStill::MakeAbortable_Reversed(ped, priority, event);
}

// 0x62F370
bool CTaskSimpleStandStill::ProcessPed(CPed* ped)
{
    return CTaskSimpleStandStill::ProcessPed_Reversed(ped);
}


CTask* CTaskSimpleStandStill::Clone_Reversed() const
{
    return new CTaskSimpleStandStill(m_nTime, m_bLooped, m_bUseAnimIdleStance, m_fBlendData);
}

bool CTaskSimpleStandStill::MakeAbortable_Reversed(CPed* ped, eAbortPriority priority, const CEvent* event)
{
    if (priority)
        return true;

    m_timer.m_nStartTime = CTimer::m_snTimeInMilliseconds;
    m_timer.m_nInterval = -1;
    m_timer.m_bStarted = true;
    return true;
}

bool CTaskSimpleStandStill::ProcessPed_Reversed(CPed* ped)
{
    if (!m_timer.m_bStarted && m_timer.Start(m_nTime)) {
        if (!ped->bInVehicle) {
            ped->SetMoveState(PEDMOVE_STILL);
            ped->m_nSwimmingMoveState = PEDMOVE_STILL;
            if (!ped->bIsDucking || !ped->m_pIntelligence->GetTaskDuck(false)) {
                CAnimManager::BlendAnimation(ped->m_pRwClump, ped->m_nAnimGroup, ANIM_ID_IDLE, m_fBlendData);
            }
            else {
                CTaskSimpleDuck* pDuckTask = ped->m_pIntelligence->GetTaskDuck(false);
                pDuckTask->ControlDuckMove(0.0f, 0.0f);
            }
            if (ped->m_pPlayerData)
                ped->m_pPlayerData->m_fMoveBlendRatio = 0.0f;
        }
    }
    if (ped->bIsDucking && ped->m_pIntelligence->GetTaskDuck(false)) {
        CTaskSimpleDuck* pDuckTask = ped->m_pIntelligence->GetTaskDuck(false);
        pDuckTask->ControlDuckMove(0.0f, 0.0f);
    }
    else {
        ped->SetMoveState(PEDMOVE_STILL);
    }
    if (m_bUseAnimIdleStance) {
        auto pIdleAnimAssoc = RpAnimBlendClumpGetAssociation(ped->m_pRwClump, ANIM_ID_IDLE);
        if (pIdleAnimAssoc && pIdleAnimAssoc->m_fBlendAmount > 0.99f) 
            return true;
    }
    if (m_bLooped || !m_timer.Reset())
        return false;

    return m_timer.IsOutOfTime();
}
