#include "StdInc.h"

#include "CFire.h"

void CFire::InjectHooks()
{
    ReversibleHooks::Install("CFire", "Constructor", 0x539D90, &CFire::Constructor);
    ReversibleHooks::Install("CFire", "Destructor", 0x538BA0, &CFire::Destructor);
    ReversibleHooks::Install("CFire", "Initialise", 0x538B30, &CFire::Initialise);
    ReversibleHooks::Install("CFire", "CreateFxSysForStrength", 0x539360, &CFire::CreateFxSysForStrength);
    ReversibleHooks::Install("CFire", "Extinguish", 0x5393F0, &CFire::Extinguish);
    ReversibleHooks::Install("CFire", "ProcessFire", 0x53A570, &CFire::ProcessFire);
}

CFire::CFire()
{
    Initialise();
}

CFire::~CFire()
{
}

CFire* CFire::Destructor()
{
    this->CFire::~CFire();
    return this;
}

CFire* CFire::Constructor()
{
    this->CFire::CFire();
    return this;
}

void CFire::Initialise()
{
    // Originally m_nFlags = (m_nFlags & 0xF4) | 0x14; - Clear 1st, 2nd, 4th and set 3rd, 5th bits (1-based numbering)
    m_bActive = false,
    m_bCreatedByScript = false,
    m_bMakesNoise = true,
    m_bBeingExtinguished = false,
    m_bFirstGeneration = true,
    m_vecPosition = CVector{};
    m_nScriptReferenceIndex = 1;
    m_nTimeToBurn = 0;
    m_pEntityTarget = nullptr;
    m_pEntityCreator = nullptr;
    m_fStrength = 1.0f;
    m_pFxSystem = nullptr;
    m_nNumGenerationsAllowed = 100;
    m_nRemovalDist = 60;
}

void CFire::CreateFxSysForStrength(const CVector& point, RwMatrixTag* matrix)
{
    DestroyFx();
    m_pFxSystem = g_fxMan.CreateFxSystem(
        GetFireParticleNameForStrength(), const_cast<CVector*>(&point), matrix, true); // TODO: Make CreateFxSys take const CVector&
    if (m_pFxSystem)
        m_pFxSystem->Play();
}

void CFire::Extinguish()
{
    if (!m_bActive)
        return;

    m_nTimeToBurn = 0;

    // Originally m_nFlags = (m_nFlags & 0xF6) | 0x10; - Clear 1st and 4th, set 5th
    m_bActive = false;
    m_bBeingExtinguished = false;
    m_bFirstGeneration = true;

    DestroyFx();

    if (m_pEntityTarget) {
        switch (m_pEntityTarget->m_nType) {
        case eEntityType::ENTITY_TYPE_PED: {
            static_cast<CPed*>(m_pEntityTarget)->m_pFire = nullptr;
            break;
        }
        case eEntityType::ENTITY_TYPE_VEHICLE: {
            static_cast<CVehicle*>(m_pEntityTarget)->m_pFire = nullptr;
            break;
        }
        }
        m_pEntityTarget->CleanUpOldReference(&m_pEntityTarget);
        m_pEntityTarget = nullptr;
    }
}

void CFire::ProcessFire()
{
    {
        const float fNewStrength = std::min(3.0f, m_fStrength + CTimer::ms_fTimeStep / 500.0f); // Limited to 3.0f
        if ((uint32_t)m_fStrength == (uint32_t)fNewStrength)
            m_fStrength = fNewStrength; // Not sure why they do this, probably just some hack
    }

    if (m_pEntityTarget) {
        m_vecPosition = m_pEntityTarget->GetPosition();

        switch (m_pEntityTarget->m_nType) {
        case eEntityType::ENTITY_TYPE_PED: {
            auto pTargetPed = static_cast<CPed*>(m_pEntityTarget);

            if (pTargetPed->m_pFire != this) {
                Extinguish();
                return;
            }

            switch (pTargetPed->m_nPedState) {
            case ePedState::PEDSTATE_DIE:
            case ePedState::PEDSTATE_DEAD: {
                m_vecPosition.z -= 1.0f; /* probably because ped is laying on the ground */
                break;
            }
            }

            if (auto pPedVeh = pTargetPed->GetVehicleIfInOne()) {
                if (!ModelIndices::IsFireTruck(pPedVeh->m_nModelIndex) && pPedVeh->IsAutomobile()) {
                    pPedVeh->m_fHealth = 75.0f;
                }
            } else if (!pTargetPed->IsPlayer() && !pTargetPed->IsAlive()) {
                pTargetPed->physicalFlags.bDestroyed = true;
            }

            break;
        }
        case eEntityType::ENTITY_TYPE_VEHICLE: {
            auto pTargetVeh = static_cast<CVehicle*>(m_pEntityTarget);

            if (pTargetVeh->m_pFire != this) {
                Extinguish();
                return;
            }

            if (!m_bCreatedByScript) {
                pTargetVeh->InflictDamage(m_pEntityCreator, eWeaponType::WEAPON_FLAMETHROWER, CTimer::ms_fTimeStep * 1.2f, CVector{});
            }

            if (pTargetVeh->IsAutomobile()) {
                m_vecPosition =
                    pTargetVeh->GetDummyPosition(eVehicleDummies::DUMMY_LIGHT_FRONT_MAIN) + CVector{0.0f, 0.0f, 0.15f};
            }
            break;
        }
        }
        if (m_pFxSystem) {
            auto targetPhysical = static_cast<CPhysical*>(m_pEntityTarget);
            m_pFxSystem->SetOffsetPos(m_vecPosition + CTimer::ms_fTimeStep * 2.0f * targetPhysical->m_vecMoveSpeed);
        }
    }

    CPlayerPed* player = FindPlayerPed();
    if (!m_pEntityTarget || !m_pEntityTarget->IsVehicle()) {
        // Check if we can set player's ped on fire
        if (!FindPlayerVehicle() &&
            !player->m_pFire && /* not already on fire */
            !player->physicalFlags.bFireProof &&
            !player->m_pAttachedTo
        ) {
            if ((player->GetPosition(), m_vecPosition).SquaredMagnitude() < 1.2f) { /* Note: Squared distance */
                player->DoStuffToGoOnFire();
                gFireManager.StartFire(player, m_pEntityCreator, 0.8f, true, 7000, 100);
            }
        }
    }

    if (rand() % 32 == 0) {
        for (int i = CPools::ms_pVehiclePool->GetSize() - 1; i >= 0; i--) { /* backwards loop, like original code */
            CVehicle* pVeh = CPools::ms_pVehiclePool->GetAt(i);
            if (!pVeh)
                continue;
            if (DistanceBetweenPoints(pVeh->GetPosition(), m_vecPosition) >= 2.0f)
                continue;
            if (pVeh->IsBMX()) {
                player->DoStuffToGoOnFire();
                gFireManager.StartFire(player, m_pEntityCreator, 0.8f, true, 7000, 100);
                pVeh->BurstTyre(
                    pVeh->FindTyreNearestPoint(m_vecPosition.x, m_vecPosition.y) + 13, // TODO: What's this 13?
                    false
                );
            } else {
                gFireManager.StartFire(pVeh, m_pEntityCreator, 0.8f, true, 7000, 100);
            }
        }
    }

    if (rand() % 4 == 0) {
        for (int i = CPools::ms_pObjectPool->GetSize() - 1; i >= 0; i--) { /* backwards loop, like original code */
            CObject* pObj = CPools::ms_pObjectPool->GetAt(i);
            if (!pObj)
                continue;
            if (DistanceBetweenPoints(pObj->GetPosition(), m_vecPosition) >= 3.0f)
                continue;
            pObj->ObjectFireDamage(CTimer::ms_fTimeStep * 8.0f, m_pEntityCreator);
        }
    }

    if (m_nNumGenerationsAllowed > 0 && rand() % 128 == 0) {
        if (gFireManager.GetNumOfActiveFires() < 25) {
            const CVector dir{
                CGeneral::GetRandomNumberInRange(-1.0f, 1.0f),
                CGeneral::GetRandomNumberInRange(-1.0f, 1.0f),
                0.0f
            };
            CCreepingFire::TryToStartFireAtCoors(
                m_vecPosition + dir * CGeneral::GetRandomNumberInRange(2.0f, 3.0f),
                m_nNumGenerationsAllowed,
                0,
                m_bCreatedByScript,
                10.0f
            );
        }
    }

    if (m_fStrength <= 2.0f && m_nNumGenerationsAllowed && rand() % 16 == 0) {
        CFire& nearby = gFireManager.GetRandomFire();
        if (&nearby != this &&
            nearby.m_bActive &&
            !nearby.m_bCreatedByScript &&
            nearby.m_fStrength <= 1.0f
        ) {
            if (DistanceBetweenPoints(nearby.m_vecPosition, m_vecPosition) < 3.5f) {
                nearby.m_vecPosition = nearby.m_vecPosition * 0.3f + m_vecPosition * 0.7f;
                m_fStrength += 1.0f;
                m_nTimeToBurn = std::max(m_nTimeToBurn, CTimer::m_snTimeInMilliseconds + 7000);
                CreateFxSysForStrength(m_vecPosition, nullptr);
                m_nNumGenerationsAllowed = std::max(m_nNumGenerationsAllowed, nearby.m_nNumGenerationsAllowed);
                nearby.Extinguish();
            }
        }
    }

    if (m_pFxSystem) {
        float unused;
        const float fFractPart = std::modf(m_fStrength, &unused); // R* way: m_fStrength - (float)(int)m_fStrength
        m_pFxSystem->SetConstTime(
            true,
            std::min(CTimer::m_snTimeInMilliseconds / 3500.0f, fFractPart)
        );
    }

    if (m_bCreatedByScript || (HasTimeToBurn() && IsNotInRemovalDistance())) {
        const float fColorRG = (float)(rand() % 128) / 512.0f;
        CPointLights::AddLight(
            ePointLightType::PLTYPE_POINTLIGHT, m_vecPosition, CVector{}, 8.0f, fColorRG, fColorRG, 0.0f, 0, false, nullptr);
    } else {
        if (m_fStrength <= 1.0f) {
            Extinguish();
        } else {
            m_fStrength -= 1.0f;
            m_nTimeToBurn = CTimer::m_snTimeInMilliseconds + 7000;
            CreateFxSysForStrength(m_vecPosition, nullptr);
        }
    }
}

bool CFire::HasTimeToBurn() {
    return CTimer::m_snTimeInMilliseconds < m_nTimeToBurn;
}

bool CFire::IsNotInRemovalDistance() {
    return m_nRemovalDist > (TheCamera.GetPosition() - m_vecPosition).Magnitude();
}
