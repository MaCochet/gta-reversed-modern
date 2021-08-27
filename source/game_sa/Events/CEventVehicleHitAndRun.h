#pragma once
#include "CEvent.h"
#include "CPed.h"
#include "CVehicle.h"
class CVehicle;

class CEventVehicleHitAndRun : public CEvent
{
public:
    CPed* m_victim;
    CVehicle* m_vehicle;

    static void InjectHooks();

    CEventVehicleHitAndRun(CPed* victim, CVehicle* vehicle);
    ~CEventVehicleHitAndRun();
private:
    CEventVehicleHitAndRun* Constructor(CPed* victim, CVehicle* vehicle);
public:
    eEventType GetEventType() const override { return EVENT_VEHICLE_HIT_AND_RUN; }
    int32_t GetEventPriority() const override { return 14; }
    int GetLifeTime() override { return 0; }
    CEvent* Clone() const override;
    bool AffectsPed(CPed* ped) override { return ped->IsAlive(); }
    bool IsCriminalEvent() override { return m_vehicle->m_pDriver && m_vehicle->m_pDriver->IsPlayer(); }
    void ReportCriminalEvent(CPed* ped) override;
    float GetLocalSoundLevel() override { return 100.0f; }

    CEvent* Clone_Reversed() const;
    void ReportCriminalEvent_Reversed(CPed* ped);
};

VALIDATE_SIZE(CEventVehicleHitAndRun, 0x14);
