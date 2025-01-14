/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "CCrimeBeingQd.h"
#include "CAEPoliceScannerAudioEntity.h"

#include "eCrimeType.h"

class CPed;
class CCopPed;

class CWanted {
public:
    uint32 m_nChaosLevel;
    uint32 m_nChaosLevelBeforeParole;
    uint32 m_nLastTimeWantedDecreased;
    uint32 m_nLastTimeWantedLevelChanged;
    uint32 m_nTimeOfParole;
    float  m_fMultiplier; // New crimes have their wanted level contribution multiplied by this
    uint8  m_nCopsInPursuit;
    uint8  m_nMaxCopsInPursuit;
    uint8  m_nMaxCopCarsInPursuit;
    uint8  m_nCopsBeatingSuspect;
    uint16 m_nChanceOnRoadBlock;

    union {
        struct {
            uint8 m_bPoliceBackOff : 1;       // If this is set the police will leave player alone (for cut-scenes)
            uint8 m_bPoliceBackOffGarage : 1; // If this is set the police will leave player alone (for garages)
            uint8 m_bEverybodyBackOff : 1;    // If this is set then everybody (including police) will leave the player alone (for cut-scenes)
            uint8 m_bSwatRequired : 1;        // These three booleans are needed so that the
            uint8 m_bFbiRequired : 1;         // streaming required vehicle stuff can be overrided
            uint8 m_bArmyRequired : 1;
            // uint8 m_b6 : 1;
            // uint8 m_b7 : 1;
            // uint8 m_b8 : 1;
        };
        uint8 m_nFlags;
    };

    char _pad1F;

    uint32                      m_nCurrentChaseTime;
    uint32                      m_nCurrentChaseTimeCounter;
    uint32                      m_nTimeCounting;
    uint32                      m_nWantedLevel;
    uint32                      m_nWantedLevelBeforeParole;
    CCrimeBeingQd               m_CrimesBeingQd[16];
    CCopPed*                    m_pCopsInPursuit[10];
    CAEPoliceScannerAudioEntity m_PoliceScannerAudio;
    bool                        m_bLeavePlayerAlone;
    char                        _pad299[3];

    static uint32 &MaximumWantedLevel; // default 6
    static uint32 MaximumChaosLevel;
    static bool &bUseNewsHeliInAdditionToPolice;

public:
    static void InjectHooks();

    void Initialise();
    void Reset();
    static void InitialiseStaticVariables();

    void UpdateWantedLevel();
    static void SetMaximumWantedLevel(int32 level);
    bool AreMiamiViceRequired();
    bool AreSwatRequired() const;
    bool AreFbiRequired() const;
    bool AreArmyRequired() const;
    int32 NumOfHelisRequired();
    // dummy function
    static void ResetPolicePursuit();
    void ClearQdCrimes();
    bool AddCrimeToQ(eCrimeType crimeType, int32 crimeId, const CVector& posn, bool bAlreadyReported, bool bPoliceDontReallyCare);
    void ReportCrimeNow(eCrimeType crimeType, const CVector& posn, bool bPoliceDontReallyCare);
    static void RemovePursuitCop(CCopPed* cop, CCopPed** copsArray, uint8& copsCounter);
    bool IsInPursuit(CCopPed* cop);
    static void UpdateEachFrame();
    void RegisterCrime(eCrimeType crimeType, const CVector& posn, CPed* ped, bool bPoliceDontReallyCare);
    void RegisterCrime_Immediately(eCrimeType crimeType, const CVector& posn, CPed* ped, bool bPoliceDontReallyCare);
    void SetWantedLevel(uint32 level);
    void CheatWantedLevel(uint32 level);
    void SetWantedLevelNoDrop(uint32 level);
    void ClearWantedLevelAndGoOnParole();
    static int32 WorkOutPolicePresence(CVector posn, float radius);
    void UpdateCrimesQ();
    bool IsClosestCop(CPed* ped, int32 numCopsToCheck);
    static CCopPed* ComputePursuitCopToDisplace(CCopPed* cop, CCopPed** copsArray);
    void RemovePursuitCop(CCopPed* cop);
    void RemoveExcessPursuitCops();
    void Update();
    static bool CanCopJoinPursuit(CCopPed* target, uint8 maxCopsCount, CCopPed** copsArray, uint8& copsCounter);
    bool CanCopJoinPursuit(CCopPed* cop);
    bool SetPursuitCop(CCopPed* cop);
};

VALIDATE_SIZE(CWanted, 0x29C);
