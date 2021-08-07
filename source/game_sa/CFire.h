/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "CEntity.h"
#include "FxSystem_c.h"

class CFire {
public:
    uint8    m_bActive : 1;
    uint8    m_bCreatedByScript : 1;
    uint8    m_bMakesNoise : 1;
    uint8    m_bBeingExtinguished : 1;
    uint8    m_bFirstGeneration : 1;
    char     _pad0;
    int16    m_nScriptReferenceIndex;
    CVector  m_vecPosition;
    CEntity* m_pEntityTarget;
    CEntity* m_pEntityCreator;
    uint32   m_nTimeToBurn;
    float    m_fStrength;
    uint8    m_nNumGenerationsAllowed;
    uint8    m_nRemovalDist;
    char     _pad1[2];

public:
    FxSystem_c *m_pFxSystem;

public:
    static void InjectHooks();

    CFire();
    ~CFire();

    CFire* Constructor();
    CFire* Destructor();

    void Initialise();
    void CreateFxSysForStrength(const CVector& point, RwMatrixTag* m);
    void Extinguish();
    void ProcessFire();

private:
    // NOTSA funcs:
    auto GetFireParticleNameForStrength() const {
        if (m_fStrength > 1.0f)
            return (m_fStrength > 2.0f) ? "fire_large" : "fire_med";
        else
            return "fire";
    };

    void DestroyFx() {
        if (m_pFxSystem) {
            m_pFxSystem->Kill();
            m_pFxSystem = nullptr;
        }
    }
    bool HasTimeToBurn();
    bool IsNotInRemovalDistance();
};
VALIDATE_SIZE(CFire, 0x28);
