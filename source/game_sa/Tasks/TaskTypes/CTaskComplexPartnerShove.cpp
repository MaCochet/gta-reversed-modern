#include "StdInc.h"

void CTaskComplexPartnerShove::InjectHooks()
{
    HookInstall(0x6846F0, &CTaskComplexPartnerShove::Constructor);
}

CTaskComplexPartnerShove::CTaskComplexPartnerShove(const char* commandName, CPed* partner, bool leadSpeaker, float distanceMultiplier, int8_t updateDirectionCount, CVector point) :
    CTaskComplexPartner(commandName, partner, leadSpeaker, distanceMultiplier, false, updateDirectionCount, point)
{
    m_updateDirectionCount = updateDirectionCount;
    m_taskId = TASK_COMPLEX_PARTNER_SHOVE;
}

CTaskComplexPartnerShove* CTaskComplexPartnerShove::Constructor(const char* commandName, CPed* partner, bool leadSpeaker, float distanceMultiplier, int8_t updateDirectionCount, CVector point)
{
    this->CTaskComplexPartnerShove::CTaskComplexPartnerShove(commandName, partner, leadSpeaker, distanceMultiplier, updateDirectionCount, point);
    return this;
}

CTaskComplexSequence* CTaskComplexPartnerShove::GetPartnerSequence()
{
    return plugin::CallMethodAndReturn<CTaskComplexSequence*, 0x683120, CTask*>(this);
}
