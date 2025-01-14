/*
    Plugin-SDK (Grand Theft Auto San Andreas) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/

#include "StdInc.h"

uint32& CTrain::GenTrain_Track = *(uint32*)0xC37FFC;
uint32& CTrain::GenTrain_TrainConfig = *(uint32*)0xC38000;
uint8& CTrain::GenTrain_Direction = *(uint8*)0xC38004;
uint32& CTrain::GenTrain_GenerationNode = *(uint32*)0xC38008;
uint32& CTrain::GenTrain_Status = *(uint32*)0xC3800C;
bool& CTrain::bDisableRandomTrains = *(bool*)0xC38010;
CVector* CTrain::aStationCoors = (CVector*)0x8D48F8;
uint32* NumTrackNodes = (uint32*)0xC38014;
float* arrTotalTrackLength = (float*)0xC37FEC;
CTrainNode** pTrackNodes = (CTrainNode * *)0xC38024;
float* StationDist = (float*)0xC38034;

void CTrain::InjectHooks()
{
    ReversibleHooks::Install("CTrain", "ProcessControl", 0x6F86A0, &CTrain::ProcessControl_Reversed);
}

// 0x6F6030
CTrain::CTrain(int32 modelIndex, eVehicleCreatedBy createdBy) : CVehicle(plugin::dummy) {
    plugin::CallMethod<0x6F6030, CTrain*, int32, eVehicleCreatedBy>(this, modelIndex, createdBy);
}

// 0x6F55D0
void CTrain::ReadAndInterpretTrackFile(char* filename, CTrainNode** nodes, int32* lineCount, float* totalDist, int32 skipStations) {
    ((void(__cdecl*)(char*, CTrainNode * *, int32*, float*, int32))0x6F55D0)(filename, nodes, lineCount, totalDist, skipStations);
}

// 0x6F58D0
void CTrain::Shutdown() {
    ((void(__cdecl*)())0x6F58D0)();
}

// 0x6F5900
void CTrain::UpdateTrains() {
    ((void(__cdecl*)())0x6F5900)();
}

// 0x6F5910
void ProcessTrainAnnouncements() {
    ((void(__cdecl*)())0x6F5910)();
}

// 0x6F5920
void PlayAnnouncement(uint8 arg0, uint8 arg1) {
    ((void(__cdecl*)(uint8, uint8))0x6F5920)(arg0, arg1);
}

// 0x6F59E0
void CTrain::FindCoorsFromPositionOnTrack(float railDistance, int32 trackId, CVector* outCoors) {
    ((void(__cdecl*)(float, int32, CVector*))0x6F59E0)(railDistance, trackId, outCoors);
}

// 0x6F5BA0
bool CTrain::FindMaximumSpeedToStopAtStations(float* speed) {
    return ((bool(__thiscall*)(CTrain*, float*))0x6F5BA0)(this, speed);
}

// 0x6F5CD0
uint32 CTrain::FindNumCarriagesPulled() {
    return ((uint32(__thiscall*)(CTrain*))0x6F5CD0)(this);
}

// 0x6F5CF0
void TrainHitStuff(CPtrList& ptrList, CEntity* entity) {
    ((void(__cdecl*)(CPtrList&, CEntity*))0x6F5CF0)(ptrList, entity);
}

// 0x6F5D80
void CTrain::OpenTrainDoor(float state) {
    ((void(__thiscall*)(CTrain*, float))0x6F5D80)(this, state);
}

// 0x6F5D90
void CTrain::AddPassenger(CPed* ped) {
    ((void(__thiscall*)(CTrain*, CPed*))0x6F5D90)(this, ped);
}

// 0x6F5DA0
void CTrain::RemovePassenger(CPed* ped) {
    ((void(__thiscall*)(CTrain*, CPed*))0x6F5DA0)(this, ped);
}

// 0x6F5DB0
void CTrain::DisableRandomTrains(bool disable) {
    ((void(__cdecl*)(bool))0x6F5DB0)(disable);
}

// 0x6F5DC0
void CTrain::RemoveOneMissionTrain(CTrain* train) {
    ((void(__cdecl*)(CTrain*))0x6F5DC0)(train);
}

// 0x6F5DF0
void CTrain::ReleaseOneMissionTrain(CTrain* train) {
    ((void(__cdecl*)(CTrain*))0x6F5DF0)(train);
}

// 0x6F5E20
void CTrain::SetTrainSpeed(CTrain* train, float speed) {
    ((void(__cdecl*)(CTrain*, float))0x6F5E20)(train, speed);
}

// 0x6F5E50
void CTrain::SetTrainCruiseSpeed(CTrain* arg0, float cruiseSpeed) {
    ((void(__cdecl*)(CTrain*, float))0x6F5E50)(arg0, cruiseSpeed);
}

// 0x6F5E70
CTrain* CTrain::FindCaboose(CTrain* train) {
    return ((CTrain * (__cdecl*)(CTrain*))0x6F5E70)(train);
}

// 0x6F5E90
CTrain* CTrain::FindEngine(CTrain* train) {
    return ((CTrain * (__cdecl*)(CTrain*))0x6F5E90)(train);
}

// 0x6F5EB0
CTrain* CTrain::FindCarriage(CTrain* train, uint8 carriage) {
    return ((CTrain * (__cdecl*)(CTrain*, uint8))0x6F5EB0)(train, carriage);
}

// 0x6F5EF0
bool CTrain::FindSideStationIsOn() {
    return ((bool(__thiscall*)(CTrain*))0x6F5EF0)(this);
}

// 0x6F5F00
void CTrain::FindNextStationPositionInDirection(bool clockwiseDirection, float distance, float* distanceToStation, int32* numStations) {
    ((void(__cdecl*)(bool, float, float*, int32*))0x6F5F00)(clockwiseDirection, distance, distanceToStation, numStations);
}

// 0x6F6320
bool CTrain::IsInTunnel() {
    return ((bool(__thiscall*)(CTrain*))0x6F6320)(this);
}

// 0x6F6640
void MarkSurroundingEntitiesForCollisionWithTrain(CVector arg0, float arg1, CEntity* arg2, bool bOnlyVehicles) {
    ((void(__cdecl*)(CVector, float, CEntity*, bool))0x6F6640)(arg0, arg1, arg2, bOnlyVehicles);
}

// 0x6F6850
void CTrain::RemoveRandomPassenger() {
    ((void(__thiscall*)(CTrain*))0x6F6850)(this);
}

// 0x6F6A20
void CTrain::RemoveMissionTrains() {
    ((void(__cdecl*)())0x6F6A20)();
}

// 0x6F6AA0
void CTrain::RemoveAllTrains() {
    ((void(__cdecl*)())0x6F6AA0)();
}

// 0x6F6B60
void CTrain::ReleaseMissionTrains() {
    ((void(__cdecl*)())0x6F6B60)();
}

// 0x6F6BD0
int32 CTrain::FindClosestTrackNode(CVector posn, int32* outTrackId) {
    return ((int32(__cdecl*)(CVector, int32*))0x6F6BD0)(posn, outTrackId);
}

// 0x6F6CC0
void CTrain::FindPositionOnTrackFromCoors() {
    ((void(__thiscall*)(CTrain*))0x6F6CC0)(this);
}

// 0x6F7090
CTrain* CTrain::FindNearestTrain(CVector posn, bool mustBeMainTrain) {
    return ((CTrain * (__cdecl*)(CVector, bool))0x6F7090)(posn, mustBeMainTrain);
}

// 0x6F7140
void CTrain::SetNewTrainPosition(CTrain* train, CVector posn) {
    ((void(__cdecl*)(CTrain*, CVector))0x6F7140)(train, posn);
}

// 0x6F7260
bool CTrain::IsNextStationAllowed(CTrain* train) {
    return ((bool(__cdecl*)(CTrain*))0x6F7260)(train);
}

// 0x6F72F0
void CTrain::SkipToNextAllowedStation(CTrain* train) {
    ((void(__cdecl*)(CTrain*))0x6F72F0)(train);
}

// 0x6F7440
void CTrain::InitTrains() {
    ((void(__cdecl*)())0x6F7440)();
}

// 0x6F7550
void CTrain::CreateMissionTrain(CVector posn, bool clockwiseDirection, uint32 trainType, CTrain** outFirstCarriage, CTrain** outLastCarriage, int32 nodeIndex, int32 trackId, bool isMissionTrain) {
    ((void(__cdecl*)(CVector, bool, uint32, CTrain * *, CTrain * *, int32, int32, bool))0x6F7550)(posn, clockwiseDirection, trainType, outFirstCarriage, outLastCarriage, nodeIndex, trackId, isMissionTrain);
}

// 0x6F7900
void CTrain::DoTrainGenerationAndRemoval() {
    ((void(__cdecl*)())0x6F7900)();
}

// 0x6F8170
void CTrain::AddNearbyPedAsRandomPassenger() {
    ((void(__thiscall*)(CTrain*))0x6F8170)(this);
}

void CTrain::ProcessControl()
{
#ifdef USE_DEFAULT_FUNCTIONS
    plugin::CallMethod<0x6F86A0, CTrain*>(this);
#else
    ProcessControl_Reversed();
#endif
}

void CTrain::ProcessControl_Reversed()
{
    vehicleFlags.bWarnedPeds = 0;
    m_vehicleAudio.Service();
    if (gbModelViewer)
    {
        return;
    }

    CVector vecOldTrainPosition = GetPosition();
    float fOldTrainHeading = GetHeading();

    float fTotalTrackLength = arrTotalTrackLength[m_nTrackId];
    CTrainNode* pTrainNodes = pTrackNodes[m_nTrackId];
    int32 numTrackNodes = NumTrackNodes[m_nTrackId];

    if (trainFlags.bNotOnARailRoad == 0)
    {
        if (!trainFlags.bIsFrontCarriage)
        {
            CTrain* pPrevCarriage = m_pPrevCarriage;
            if (pPrevCarriage)
            {
                m_fTrainSpeed = pPrevCarriage->m_fTrainSpeed;
                m_fCurrentRailDistance = pPrevCarriage->m_fCurrentRailDistance + m_fLength;
            }
            else
            {
                m_fTrainSpeed *= pow(0.9900000095367432f, CTimer::ms_fTimeStep);
                m_fCurrentRailDistance += m_fTrainSpeed * CTimer::ms_fTimeStep;
            }

            if (trainFlags.b01 && trainFlags.bStoppedAtStation
                && m_nModelIndex == MODEL_STREAKC
                && !trainFlags.bMissionTrain)
            {
                CPlayerPed* pLocalPlayer = FindPlayerPed(-1);
                if (m_nPassengersGenerationState == TRAIN_PASSENGERS_QUERY_NUM_PASSENGERS_TO_LEAVE)
                {
                    if (pLocalPlayer->m_pVehicle == this)
                    {
                        m_nNumPassengersToLeave = 0;
                    }
                    else
                    {
                        m_nNumPassengersToLeave = (rand() & 3) + 1; // rand(1, 4)
                    }
                    m_nPassengersGenerationState = TRAIN_PASSENGERS_TELL_PASSENGERS_TO_LEAVE;
                }

                if (m_nPassengersGenerationState == TRAIN_PASSENGERS_TELL_PASSENGERS_TO_LEAVE)
                {
                    RemoveRandomPassenger();
                    if (m_nNumPassengersToLeave == 0)
                    {
                        m_nPassengersGenerationState = TRAIN_PASSENGERS_QUERY_NUM_PASSENGERS_TO_ENTER;
                    }
                }

                if (m_nPassengersGenerationState == TRAIN_PASSENGERS_QUERY_NUM_PASSENGERS_TO_ENTER)
                {
                    if (pLocalPlayer->m_pVehicle == this)
                    {
                        m_nNumPassengersToEnter = 0;
                    }
                    else
                    {
                        m_nNumPassengersToEnter = rand() % 4 + 1; // rand(1, 4)
                    }
                    m_nPassengersGenerationState = TRAIN_PASSENGERS_TELL_PASSENGERS_TO_ENTER;
                }

                if (m_nPassengersGenerationState == TRAIN_PASSENGERS_TELL_PASSENGERS_TO_ENTER)
                {
                    if (trainFlags.bPassengersCanEnterAndLeave)
                    {
                        AddNearbyPedAsRandomPassenger();
                        if (m_nNumPassengersToLeave == m_nNumPassengersToEnter)
                        {
                            m_nPassengersGenerationState = TRAIN_PASSENGERS_GENERATION_FINISHED;
                        }
                    }
                }
            }
        }
        else
        {
            CPad* pPad = CPad::GetPad(0);
            if (m_pDriver && m_pDriver->IsPlayer())
            {
                CPlayerPed* pLocalPlayer = static_cast<CPlayerPed*>(m_pDriver);
                pPad = pLocalPlayer->GetPadFromPlayer();
            }

            uint32 numCarriagesPulled = FindNumCarriagesPulled();
            if (!trainFlags.bClockwiseDirection)
            {
                m_fTrainSpeed = -m_fTrainSpeed;
            }

            ///*
            if (m_nStatus)
            {
                bool bIsStreakModel = trainFlags.bIsStreakModel;
                float fStopAtStationSpeed = static_cast<float>(m_autoPilot.m_nCruiseSpeed);

                uint32 timeInMilliSeconds = CTimer::m_snTimeInMilliseconds;
                uint32 timeAtStation = CTimer::m_snTimeInMilliseconds - m_nTimeWhenStoppedAtStation;
                if (timeAtStation >= (bIsStreakModel ? 20000u : 10000u))
                {
                    if (timeAtStation >= (bIsStreakModel ? 28000u : 18000u))
                    {
                        if (timeAtStation >= (bIsStreakModel ? 32000u : 22000u))
                        {
                            if (trainFlags.bStopsAtStations)
                            {
                                float maxTrainSpeed = 0.0f;
                                if (FindMaximumSpeedToStopAtStations(&maxTrainSpeed))
                                {
                                    fStopAtStationSpeed = 0.0;
                                    m_nTimeWhenStoppedAtStation = timeInMilliSeconds;
                                }
                                else
                                {
                                    if (fStopAtStationSpeed >= maxTrainSpeed)
                                    {
                                        fStopAtStationSpeed = maxTrainSpeed;
                                    }
                                }
                            }
                        }
                        else if (trainFlags.bStoppedAtStation)
                        {
                            CTrain* pTrainCarriage = this;
                            do
                            {
                                trainFlags.bStoppedAtStation = false;
                                pTrainCarriage->m_nPassengersGenerationState = TRAIN_PASSENGERS_GENERATION_FINISHED;
                                pTrainCarriage = pTrainCarriage->m_pNextCarriage;
                            } while (pTrainCarriage);
                        }
                    }
                    else
                    {
                        fStopAtStationSpeed = 0.0;
                        if (trainFlags.bStoppedAtStation)
                        {
                            CTrain* pTrainCarriage = this;
                            do
                            {
                                trainFlags.bPassengersCanEnterAndLeave = false;
                                pTrainCarriage->m_nPassengersGenerationState = TRAIN_PASSENGERS_GENERATION_FINISHED;
                                pTrainCarriage = pTrainCarriage->m_pNextCarriage;
                            } while (pTrainCarriage);
                        }
                    }
                }
                else
                {
                    fStopAtStationSpeed = 0.0;
                    if (!trainFlags.bStoppedAtStation)
                    {
                        CTrain* pTrainCarriage = this;
                        do
                        {
                            trainFlags.bStoppedAtStation = true;
                            trainFlags.bPassengersCanEnterAndLeave = true;
                            pTrainCarriage->m_nPassengersGenerationState = TRAIN_PASSENGERS_QUERY_NUM_PASSENGERS_TO_LEAVE;
                            pTrainCarriage = pTrainCarriage->m_pNextCarriage;
                        } while (pTrainCarriage);
                    }
                }

                fStopAtStationSpeed = fStopAtStationSpeed * 0.02f - m_fTrainSpeed;
                if (fStopAtStationSpeed > 0.0)
                {
                    m_fTrainGas = fStopAtStationSpeed * 30.0f;
                    if (m_fTrainGas >= 1.0f)
                    {
                        m_fTrainGas = 1.0f;
                    }

                    m_fTrainGas *= 255.0f;
                    m_fTrainBrake = 0.0f;
                }
                else
                {
                    float fTrainSpeed = fStopAtStationSpeed * -30.0f;
                    m_fTrainGas = 0.0f;
                    if (fTrainSpeed >= 1.0f)
                    {
                        fTrainSpeed = 1.0f;
                    }
                    m_fTrainBrake = fTrainSpeed * 255.0f;
                }
            }
            else
            {
                float fTrainSpeed = m_fTrainSpeed;
                if (fTrainSpeed < 0.0f)
                {
                    fTrainSpeed = -fTrainSpeed;
                }

                if (fTrainSpeed < 0.001f)
                {
                    m_fTrainBrake = 0.0;
                    m_fTrainGas = static_cast<float>(pPad->GetAccelerate() - pPad->GetBrake());
                }
                else
                {
                    if (m_fTrainSpeed > 0.0)
                    {
                        m_fTrainBrake = static_cast<float>(pPad->GetBrake());
                        m_fTrainGas = static_cast<float>(pPad->GetAccelerate());
                    }
                    else
                    {
                        m_fTrainGas = static_cast<float>(-pPad->GetBrake());
                        m_fTrainBrake = static_cast<float>(pPad->GetAccelerate());
                    }
                }
            }

            if (trainFlags.bForceSlowDown)
            {
                CVector vecPoint = GetPosition();
                CVector vecDistance;
                if (CGameLogic::CalcDistanceToForbiddenTrainCrossing(vecPoint, m_vecMoveSpeed, true, &vecDistance) < 230.0f)
                {
                    if (DotProduct(GetForwardVector(), vecDistance) <= 0.0f)
                    {
                        m_fTrainGas = std::max(0.0f, m_fTrainGas);
                    }
                    else
                    {
                        m_fTrainGas = std::min(0.0f, m_fTrainGas);
                    }

                    if (CGameLogic::CalcDistanceToForbiddenTrainCrossing(vecPoint, m_vecMoveSpeed, false, &vecDistance) < 230.0f)
                    {
                        m_fTrainBrake = 512.0f;

                    }
                }
            }

            numCarriagesPulled += 3;

            m_fTrainSpeed += m_fTrainGas * 0.00390625f * CTimer::ms_fTimeStep * 0.0020000001f / numCarriagesPulled;

            if (m_fTrainBrake != 0.0f)
            {
                float fTrainSpeed = m_fTrainSpeed;
                if (m_fTrainSpeed < 0.0)
                {
                    fTrainSpeed = -fTrainSpeed;
                }
                float fBreak = m_fTrainBrake * 0.00390625f * CTimer::ms_fTimeStep * 0.0060000001f / numCarriagesPulled;
                if (fTrainSpeed >= fBreak)
                {
                    if (m_fTrainSpeed < 0.0)
                    {
                        m_fTrainSpeed += fBreak;
                    }
                    else
                    {
                        m_fTrainSpeed -= fBreak;
                    }
                }
                else
                {
                    m_fTrainSpeed = 0.0;
                }
            }

            m_fTrainSpeed *= pow(0.999750018119812f, CTimer::ms_fTimeStep);
            if (!trainFlags.bClockwiseDirection)
            {
                m_fTrainSpeed = -m_fTrainSpeed;
            }

            m_fCurrentRailDistance += CTimer::ms_fTimeStep * m_fTrainSpeed;

            if (m_nStatus == STATUS_PLAYER)
            {

                float fTheTrainSpeed = m_fTrainSpeed;
                if (fTheTrainSpeed < 0.0)
                {
                    fTheTrainSpeed = -fTheTrainSpeed;
                }
                if (fTheTrainSpeed > 1.0 * 0.94999999)
                {
                    CPad::GetPad(0)->StartShake(300, 0x46u, 0);
                    CVector vecVehiclePosition = GetPosition();
                    TheCamera.CamShake(0.1f, vecVehiclePosition.x, vecVehiclePosition.y, vecVehiclePosition.z);
                }

                fTheTrainSpeed = m_fTrainSpeed;
                if (fTheTrainSpeed < 0.0)
                {
                    fTheTrainSpeed = -fTheTrainSpeed;
                }

                if (fTheTrainSpeed > 1.0f)
                {
                    int32 nNodeIndex = m_nNodeIndex;
                    int32 previousNodeIndex = nNodeIndex - 1;
                    if (previousNodeIndex < 0)
                    {
                        previousNodeIndex = numTrackNodes;
                    }

                    int32 previousNodeIndex2 = previousNodeIndex - 1;
                    if (previousNodeIndex2 < 0)
                    {
                        previousNodeIndex2 = numTrackNodes;
                    }

                    CTrainNode* pCurrentTrainNode = &pTrainNodes[m_nNodeIndex];
                    CTrainNode* pPreviousTrainNode = &pTrainNodes[previousNodeIndex];
                    CTrainNode* pPreviousTrainNode2 = &pTrainNodes[previousNodeIndex2];

                    CVector vecDifference1 = pCurrentTrainNode->GetPosn() - pPreviousTrainNode->GetPosn();
                    CVector vecDifference2 = pPreviousTrainNode->GetPosn() - pPreviousTrainNode2->GetPosn();
                    vecDifference1.Normalise();
                    vecDifference2.Normalise();

                    if (vecDifference1.x * vecDifference2.x +
                        vecDifference1.y * vecDifference2.y +
                        vecDifference1.z * vecDifference2.z < 0.99599999f)
                    {
                        CTrain* pCarriage = this;
                        bool bIsInTunnel = false;
                        while (!bIsInTunnel)
                        {
                            bIsInTunnel = pCarriage->IsInTunnel();
                            pCarriage = pCarriage->m_pNextCarriage;
                            if (!pCarriage)
                            {
                                if (!bIsInTunnel)
                                {
                                    CTrain* pTheTrainCarriage = this;
                                    do
                                    {
                                        trainFlags.bNotOnARailRoad = true;
                                        pTheTrainCarriage->physicalFlags.bDisableCollisionForce = false;
                                        pTheTrainCarriage->physicalFlags.bDisableSimpleCollision = false;
                                        pTheTrainCarriage->SetIsStatic(false);
                                        pTheTrainCarriage = pTheTrainCarriage->m_pNextCarriage;
                                    } while (pTheTrainCarriage);

                                    CPhysical::ProcessControl();
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (m_fCurrentRailDistance < 0.0)
        {
            do
            {
                m_fCurrentRailDistance += fTotalTrackLength;
            } while (m_fCurrentRailDistance < 0.0);
        }

        if (m_fCurrentRailDistance >= fTotalTrackLength)
        {
            do
            {
                m_fCurrentRailDistance -= fTotalTrackLength;
            } while (m_fCurrentRailDistance >= fTotalTrackLength);
        }

        float fNextNodeTrackLength = 0.0f;
        int32 nextNodeIndex = m_nNodeIndex + 1;
        if (nextNodeIndex < numTrackNodes)
        {
            CTrainNode* pNextTrainNode = &pTrainNodes[nextNodeIndex];
            fNextNodeTrackLength = pNextTrainNode->m_nDistanceFromStart / 3.0f;
        }
        else
        {
            fNextNodeTrackLength = fTotalTrackLength;
            nextNodeIndex = 0;
        }

        CTrainNode* pTheTrainNode = &pTrainNodes[m_nNodeIndex];
        float fCurrentNodeTrackLength = pTheTrainNode->m_nDistanceFromStart / 3.0f;
        while (m_fCurrentRailDistance < fCurrentNodeTrackLength || fNextNodeTrackLength < m_fCurrentRailDistance)
        {
            int32 newNodeIndex = m_nNodeIndex - 1; // previous node
            if (fCurrentNodeTrackLength <= m_fCurrentRailDistance)
            {
                newNodeIndex = m_nNodeIndex + 1; // next node
            }
            m_nNodeIndex = newNodeIndex % numTrackNodes;
            m_vehicleAudio.AddAudioEvent(AE_TRAIN_CLACK, 0.0f);

            pTheTrainNode = &pTrainNodes[m_nNodeIndex];
            fCurrentNodeTrackLength = pTheTrainNode->m_nDistanceFromStart / 3.0f;

            nextNodeIndex = m_nNodeIndex + 1;
            if (nextNodeIndex < numTrackNodes)
            {
                CTrainNode* pNextTrainNode = &pTrainNodes[nextNodeIndex];
                fNextNodeTrackLength = pNextTrainNode->m_nDistanceFromStart / 3.0f;
            }
            else
            {
                fNextNodeTrackLength = fTotalTrackLength;
                nextNodeIndex = 0;
            }
        }

        CTrainNode* pNextTrainNode = &pTrainNodes[nextNodeIndex];
        fNextNodeTrackLength = pNextTrainNode->m_nDistanceFromStart / 3.0f;

        float fTrackNodeDifference = fNextNodeTrackLength - fCurrentNodeTrackLength;
        if (fTrackNodeDifference < 0.0f)
        {
            fTrackNodeDifference += fTotalTrackLength;
        }

        float fTheDistance = (m_fCurrentRailDistance - fCurrentNodeTrackLength) / fTrackNodeDifference;
        CVector vecPosition1;
        vecPosition1.x = pTheTrainNode->x * 0.125f * (1.0f - fTheDistance) + pNextTrainNode->x * 0.125f * fTheDistance;
        vecPosition1.y = pTheTrainNode->y * 0.125f * (1.0f - fTheDistance) + pNextTrainNode->y * 0.125f * fTheDistance;
        vecPosition1.z = pTheTrainNode->z * 0.125f * (1.0f - fTheDistance) + pNextTrainNode->z * 0.125f * fTheDistance;


        CColModel* pVehicleColModel = CModelInfo::ms_modelInfoPtrs[m_nModelIndex]->GetColModel();
        CBoundingBox* pBoundingBox = &pVehicleColModel->GetBoundingBox();
        float fTotalCurrentRailDistance = pBoundingBox->m_vecMax.y - pBoundingBox->m_vecMin.y + m_fCurrentRailDistance;
        if (fTotalCurrentRailDistance > fTotalTrackLength)
        {
            fTotalCurrentRailDistance -= fTotalTrackLength;
        }

        nextNodeIndex = m_nNodeIndex + 1;
        if (nextNodeIndex < numTrackNodes)
        {
            CTrainNode* pNextTrainNode = &pTrainNodes[nextNodeIndex];
            fNextNodeTrackLength = pNextTrainNode->m_nDistanceFromStart / 3.0f;
        }
        else
        {
            fNextNodeTrackLength = fTotalTrackLength;
            nextNodeIndex = 0;
        }

        int32 trainNodeIndex = m_nNodeIndex;
        while (fTotalCurrentRailDistance < fCurrentNodeTrackLength || fTotalCurrentRailDistance > fNextNodeTrackLength)
        {
            trainNodeIndex = (trainNodeIndex + 1) % numTrackNodes;

            pTheTrainNode = &pTrainNodes[trainNodeIndex];
            fCurrentNodeTrackLength = pTheTrainNode->m_nDistanceFromStart / 3.0f;

            nextNodeIndex = trainNodeIndex + 1;
            if (nextNodeIndex < numTrackNodes)
            {
                CTrainNode* pNextTrainNode = &pTrainNodes[nextNodeIndex];
                fNextNodeTrackLength = pNextTrainNode->m_nDistanceFromStart / 3.0f;
            }
            else
            {
                fNextNodeTrackLength = fTotalTrackLength;
                nextNodeIndex = 0;
            }
        }

        pNextTrainNode = &pTrainNodes[nextNodeIndex];
        fNextNodeTrackLength = pNextTrainNode->m_nDistanceFromStart / 3.0f;

        fTrackNodeDifference = fNextNodeTrackLength - fCurrentNodeTrackLength;
        if (fTrackNodeDifference < 0.0)
        {
            fTrackNodeDifference += fTotalTrackLength;
        }

        fTheDistance = (fTotalCurrentRailDistance - fCurrentNodeTrackLength) / fTrackNodeDifference;
        CVector vecPosition2;
        vecPosition2.x = pTheTrainNode->x * 0.125f * (1.0f - fTheDistance) + pNextTrainNode->x * 0.125f * fTheDistance;
        vecPosition2.y = pTheTrainNode->y * 0.125f * (1.0f - fTheDistance) + pNextTrainNode->y * 0.125f * fTheDistance;
        vecPosition2.z = pTheTrainNode->z * 0.125f * (1.0f - fTheDistance) + pNextTrainNode->z * 0.125f * fTheDistance;

        {
            CVector& vecVehiclePosition = GetPosition();
            vecVehiclePosition = (vecPosition1 + vecPosition2) * 0.5f;
            vecVehiclePosition.z += m_pHandlingData->m_fSuspensionLowerLimit - pBoundingBox->m_vecMin.z;
        }

        GetForward() = vecPosition2 - vecPosition1;
        GetForward().Normalise();
        if (!trainFlags.bClockwiseDirection)
        {
            GetForward() *= -1.0f;
        }

        CVector vecTemp(0.0f, 0.0f, 1.0f);

        CrossProduct(&GetRight(), &GetForward(), &vecTemp);
        GetRight().Normalise();

        CrossProduct(&GetUp(), &GetRight(), &GetForward());

        uint8 trainNodeLighting = pTheTrainNode->GetLightingFromCollision();;
        uint8 trainNextNodeLighting = pNextTrainNode->GetLightingFromCollision();

        float fTrainNodeLighting = static_cast<float>(ScaleLighting(trainNodeLighting, 0.5f));
        float fTrainNextNodeLighting = static_cast<float>(ScaleLighting(trainNextNodeLighting, 0.5f));

        fTrainNodeLighting += (fTrainNextNodeLighting - fTrainNodeLighting) * fTheDistance;
        m_fContactSurfaceBrightness = fTrainNodeLighting;
        m_vecMoveSpeed = (1.0f / CTimer::ms_fTimeStep) * (GetPosition() - vecOldTrainPosition);

        float fNewTrainHeading = GetHeading();
        float fHeading = fNewTrainHeading - fOldTrainHeading;
        if (fHeading <= 3.1415927f)
        {
            if (fHeading < -3.1415927f)
            {
                fHeading += 6.2831855f;
            }
        }
        else
        {
            fHeading -= 6.2831855f;
        }

        m_vecTurnSpeed = CVector(0.0f, 0.0f, fHeading / CTimer::ms_fTimeStep);

        if (trainFlags.bNotOnARailRoad)
        {
            m_vecMoveSpeed *= -1.0f;
            m_vecTurnSpeed *= -1.0f;

            ApplyMoveSpeed();

            m_vecMoveSpeed *= -1.0f;
            m_vecTurnSpeed *= -1.0f;

            CPhysical::ProcessControl();
        }
        else
        {
            float fVehicleMoveSpeedX = m_vecMoveSpeed.x;
            if (fVehicleMoveSpeedX <= -2.0f)
            {
                fVehicleMoveSpeedX = -2.0f;
            }

            if (fVehicleMoveSpeedX >= 2.0f)
            {
                fVehicleMoveSpeedX = 2.0f;
            }
            m_vecMoveSpeed.x = fVehicleMoveSpeedX;

            float fVehicleMoveSpeedY = m_vecMoveSpeed.y;
            if (fVehicleMoveSpeedY <= -2.0f)
            {
                fVehicleMoveSpeedY = -2.0f;
            }

            if (fVehicleMoveSpeedY >= 2.0f)
            {
                fVehicleMoveSpeedY = 2.0f;
            }
            m_vecMoveSpeed.y = fVehicleMoveSpeedY;

            float fVehicleMoveSpeedZ = m_vecMoveSpeed.z;
            if (fVehicleMoveSpeedZ <= -2.0f)
            {
                fVehicleMoveSpeedZ = -2.0f;
            }

            if (fVehicleMoveSpeedZ >= 2.0f)
            {
                fVehicleMoveSpeedZ = 2.0f;
            }
            m_vecMoveSpeed.z = fVehicleMoveSpeedZ;

            float fVehicleTurnSpeedX = m_vecTurnSpeed.x;
            if (fVehicleTurnSpeedX <= -0.1f)
            {
                fVehicleTurnSpeedX = -0.1f;
            }

            if (fVehicleTurnSpeedX >= 0.1f)
            {
                fVehicleTurnSpeedX = 0.1f;
            }
            m_vecTurnSpeed.x = fVehicleTurnSpeedX;

            float fVehicleTurnSpeedY = m_vecTurnSpeed.y;
            if (fVehicleTurnSpeedY <= -0.1f)
            {
                fVehicleTurnSpeedY = -0.1f;
            }

            if (fVehicleTurnSpeedY >= 0.1f)
            {
                fVehicleTurnSpeedY = 0.1f;
            }
            m_vecTurnSpeed.y = fVehicleTurnSpeedY;

            float fVehicleTurnSpeedZ = m_vecTurnSpeed.z;
            if (fVehicleTurnSpeedZ <= -0.1f)
            {
                fVehicleTurnSpeedZ = -0.1f;
            }

            if (fVehicleTurnSpeedZ >= 0.1f)
            {
                fVehicleTurnSpeedZ = 0.1f;
            }
            m_vecTurnSpeed.z = fVehicleTurnSpeedZ;
        }

        UpdateRW();
        UpdateRwFrame();
        RemoveAndAdd();

        m_bIsStuck = false;
        m_bWasPostponed = false;
        m_bIsInSafePosition = true;

        m_fMovingSpeed = (vecOldTrainPosition - GetPosition()).Magnitude();

        if (trainFlags.bIsFrontCarriage || trainFlags.bIsLastCarriage)
        {
            CVector vecPoint = pBoundingBox->m_vecMax.y * GetForward();
            vecPoint += GetPosition();
            vecPoint += CTimer::ms_fTimeStep * m_vecMoveSpeed;

            MarkSurroundingEntitiesForCollisionWithTrain(vecPoint, 3.0f, this, false);
        }

        if (!vehicleFlags.bWarnedPeds)
        {
            CCarCtrl::ScanForPedDanger(this);
        }
        return;
    }
    else
    {
        if (!m_bIsStuck)
        {
            float fMaxForce = 0.003f;
            float fMaxTorque = 0.00090000004f;
            float fMaxMovingSpeed = 0.005f;

            if (m_nStatus != STATUS_PLAYER)
            {
                fMaxForce = 0.006f;
                fMaxTorque = 0.0015f;
                fMaxMovingSpeed = 0.015f;
            }

            float fMaxForceTimeStep = (fMaxForce * CTimer::ms_fTimeStep) * (fMaxForce * CTimer::ms_fTimeStep);
            float fMaxTorqueTimeStep = (fMaxTorque * CTimer::ms_fTimeStep) * (fMaxTorque * CTimer::ms_fTimeStep);

            m_vecForce = (m_vecForce + m_vecMoveSpeed) * 0.5f;
            m_vecTorque = (m_vecTorque + m_vecTurnSpeed) * 0.5f;

            if (m_vecForce.SquaredMagnitude() > fMaxForceTimeStep
                || m_vecTorque.SquaredMagnitude() > fMaxTorqueTimeStep
                || m_fMovingSpeed >= fMaxMovingSpeed
                || m_fDamageIntensity > 0.0
                && m_pDamageEntity != 0
                && (m_pDamageEntity->m_nType == ENTITY_TYPE_PED)
                )
            {
                m_nFakePhysics = 0;
            }
            else
            {

                m_nFakePhysics += 1;
                if (m_nFakePhysics > 10)
                {

                    //if (!dummy_424100()) dummy_424100 returns 0, nothing else
                    {
                        if (m_nFakePhysics > 10)
                        {
                            m_nFakePhysics = 10;
                        }

                        m_vecMoveSpeed = CVector();
                        m_vecTurnSpeed = CVector();
                        SkipPhysics();
                        return;
                    }
                }
            }
        }

        CPhysical::ProcessControl();

        CVector vecMoveForce, vecTurnForce;
        if (mod_Buoyancy.ProcessBuoyancy(this, m_fBuoyancyConstant, &vecMoveForce, &vecTurnForce))
        {
            physicalFlags.bTouchingWater = true;

            float fTimeStep = 0.0099999998f;
            if (CTimer::ms_fTimeStep >= 0.0099999998f)
            {
                fTimeStep = CTimer::ms_fTimeStep;
            }

            float fSpeedFactor = 1.0f - vecOldTrainPosition.z / (fTimeStep * m_fMass * 0.0080000004f) * 0.050000001f;
            fSpeedFactor = pow(fSpeedFactor, CTimer::ms_fTimeStep);

            m_vecMoveSpeed *= fSpeedFactor;
            m_vecTurnSpeed *= fSpeedFactor;
            ApplyMoveForce(vecOldTrainPosition.x, vecOldTrainPosition.y, vecOldTrainPosition.z);
            ApplyTurnForce(vecTurnForce, vecMoveForce);
        }
    }
}
