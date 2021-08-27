/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "CTaskSimple.h"
#include "CVehicle.h"
#include "CTaskUtilityLineUpPedWithCar.h"

class CTaskSimpleCarSetPedInAsDriver : public CTaskSimple {
public:
    bool                          m_bIsFinished;
    char                          _pad[3];
    CAnimBlendAssociation*        m_pAnim;
    CVehicle*                     m_pTargetVehicle;
    CTaskUtilityLineUpPedWithCar* m_pUtility;
    bool                          m_bWarpingInToCar;
    unsigned char                 m_nDoorFlagsToClear;
    unsigned char                 m_nNumGettingInToClear;
    char _pad2[1];

public:
    CTaskSimpleCarSetPedInAsDriver(CVehicle *pTargetVehicle, CTaskUtilityLineUpPedWithCar *pUtility);
    ~CTaskSimpleCarSetPedInAsDriver();

    CTask* Clone() const override;
    eTaskType GetId() override { return TASK_SIMPLE_CAR_SET_PED_IN_AS_DRIVER; }
    bool MakeAbortable(CPed* ped, eAbortPriority priority, const CEvent* event) override { return false; }
    bool ProcessPed(CPed* ped) override;
};

VALIDATE_SIZE(CTaskSimpleCarSetPedInAsDriver, 0x1C);
