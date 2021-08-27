#include "StdInc.h"

void CEventVehicleDamageWeapon::InjectHooks()
{
    HookInstall(0x61C2E0, &CEventVehicleDamageWeapon::Constructor);
    HookInstall(0x61C330, &CEventVehicleDamageWeapon::CloneEditable_Reversed);
}

CEventVehicleDamageWeapon::CEventVehicleDamageWeapon(CVehicle* vehicle, CEntity* attacker, eWeaponType weaponType) :
    CEventVehicleDamage(vehicle, attacker, weaponType)
{
    // nothing here
}

CEventVehicleDamageWeapon::~CEventVehicleDamageWeapon()
{
    // nothing here
}

CEventVehicleDamageWeapon* CEventVehicleDamageWeapon::Constructor(CVehicle* vehicle, CEntity* attacker, eWeaponType weaponType)
{
#ifdef USE_DEFAULT_FUNCTIONS
    return plugin::CallMethodAndReturn<CEventVehicleDamageWeapon*, 0x61C2E0, CEvent*, CVehicle*, CEntity*, eWeaponType>
        (this, vehicle, attacker, weaponType);
#else
    this->CEventVehicleDamageWeapon::CEventVehicleDamageWeapon(vehicle, attacker, weaponType);
    return this;
#endif
}

CEventEditableResponse* CEventVehicleDamageWeapon::CloneEditable() const
{
#ifdef USE_DEFAULT_FUNCTIONS
    return plugin::CallMethodAndReturn<CEventEditableResponse*, 0x61C330, CEvent*>(this);
#else
    return CEventVehicleDamageWeapon::CloneEditable_Reversed();
#endif
}

CEventEditableResponse* CEventVehicleDamageWeapon::CloneEditable_Reversed() const
{
    return new CEventVehicleDamageWeapon(m_vehicle, m_attacker, m_weaponType);
}
