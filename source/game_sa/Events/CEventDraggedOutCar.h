#include "CEventEditableResponse.h"
#include "CPed.h"
#include "common.h"
#include "CPedGroup.h"

class CVehicle;

class CEventDraggedOutCar : public CEventEditableResponse {
public:
    CPed* m_carjacker;
    CVehicle* m_vehicle;
    bool m_IsDriverSeat;
    char _pad[3];

public:

    CEventDraggedOutCar(CVehicle* vehicle, CPed* carjacker, bool IsDriverSeat);
    ~CEventDraggedOutCar();
public:
    eEventType GetEventType() const override { return EVENT_DRAGGED_OUT_CAR; }
    int32_t GetEventPriority() const override { return 40; }
    int GetLifeTime() override { return 0; }
    bool AffectsPed(CPed* ped) override { return ped->IsAlive() && m_carjacker; }
    bool AffectsPedGroup(CPedGroup* pedGroup) override { return FindPlayerPed(-1) == pedGroup->m_groupMembership.GetLeader(); }
    CEntity* GetSourceEntity() const override { return m_carjacker; }
    float GetLocalSoundLevel() override { return 100.0f; }
    CEventEditableResponse* CloneEditable() const override;

private:
    friend void InjectHooksMain();
    static void InjectHooks();

    CEventDraggedOutCar* Constructor(CVehicle* vehicle, CPed* carjacker, bool IsDriverSeat);

    CEventEditableResponse* CloneEditable_Reversed() const;
};

VALIDATE_SIZE(CEventDraggedOutCar, 0x20);
