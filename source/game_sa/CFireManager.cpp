#include "StdInc.h"

CFireManager& gFireManager = *reinterpret_cast<CFireManager*>(0xB71F80);

CFire* CFireManager::FindNearestFire(CVector const& position, bool notBeingExtinguished, bool notScript) {
    return plugin::CallMethodAndReturn<CFire*, 0x538F40, CFireManager*, CVector const&, bool, bool>(this, position, notBeingExtinguished, notScript);
}

CFire* CFireManager::StartFire(CEntity* entity, CEntity* playerPed, float size, uint8 unused, uint32 lifetime, int8 numGenerations) {
    return plugin::CallMethodAndReturn<CFire*, 0x53A050, CFireManager*, CEntity*, CEntity*, float, uint8, uint32, int8>(this, entity, playerPed, size, unused, lifetime, numGenerations);
}

size_t CFireManager::GetNumOfActiveFires() {
    size_t count = 0;
    for (auto& fire : m_aFires) {
        if (fire.m_bActive)
            count++;
    }
    return count;
}

CFire& CFireManager::GetRandomFire() {
    return m_aFires[CGeneral::GetRandomNumberInRange(0, std::size(m_aFires))];
}
