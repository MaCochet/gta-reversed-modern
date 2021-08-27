#include "StdInc.h"


void CTaskComplexMedicTreatInjuredPed::InjectHooks()
{
    ReversibleHooks::Install("CTaskComplexMedicTreatInjuredPed", "Constructor", 0x658BA0, &CTaskComplexMedicTreatInjuredPed::Constructor);
    ReversibleHooks::Install("CTaskComplexMedicTreatInjuredPed", "CreateSubTask", 0x658DB0, &CTaskComplexMedicTreatInjuredPed::CreateSubTask);
    ReversibleHooks::Install("CTaskComplexMedicTreatInjuredPed", "CreateDealWithNextAccidentTask", 0x65A020, &CTaskComplexMedicTreatInjuredPed::CreateDealWithNextAccidentTask);
    ReversibleHooks::Install("CTaskComplexMedicTreatInjuredPed", "FindNearestAccident", 0x658CC0, &CTaskComplexMedicTreatInjuredPed::FindNearestAccident);
    ReversibleHooks::Install("CTaskComplexMedicTreatInjuredPed", "FindAccidentPosition", 0x658D20, &CTaskComplexMedicTreatInjuredPed::FindAccidentPosition);
    //VTABLE
    ReversibleHooks::Install("CTaskComplexMedicTreatInjuredPed", "Clone", 0x659AF0, &CTaskComplexMedicTreatInjuredPed::Clone_Reversed);
    ReversibleHooks::Install("CTaskComplexMedicTreatInjuredPed", "CreateFirstSubTask", 0x659FE0, &CTaskComplexMedicTreatInjuredPed::CreateFirstSubTask_Reversed);
    ReversibleHooks::Install("CTaskComplexMedicTreatInjuredPed", "CreateNextSubTask", 0x65A990, &CTaskComplexMedicTreatInjuredPed::CreateNextSubTask_Reversed);
    ReversibleHooks::Install("CTaskComplexMedicTreatInjuredPed", "ControlSubTask", 0x65ABF0, &CTaskComplexMedicTreatInjuredPed::ControlSubTask_Reversed);
}

CTaskComplexMedicTreatInjuredPed* CTaskComplexMedicTreatInjuredPed::Constructor(CVehicle* vehicle, CPed* ped, unsigned char isDriver)
{
    this->CTaskComplexMedicTreatInjuredPed::CTaskComplexMedicTreatInjuredPed(vehicle, ped, isDriver);
    return this;
}

// 0x658BA0
CTaskComplexMedicTreatInjuredPed::CTaskComplexMedicTreatInjuredPed(CVehicle* vehicle, CPed* ped, unsigned char isDriver) : CTaskComplex()
{
    m_pVehicle = vehicle;
    m_pPartnerMedic = ped;
    m_bIsDriver = isDriver;
    m_pAccident = nullptr;
    m_bLeftCarOnce = false;

    if (m_pVehicle)
        m_pVehicle->RegisterReference(reinterpret_cast<CEntity * *>(&m_pVehicle));

    if (m_pPartnerMedic)
        m_pPartnerMedic->RegisterReference(reinterpret_cast<CEntity * *>(&m_pPartnerMedic));
}

// 0x658C30
CTaskComplexMedicTreatInjuredPed::~CTaskComplexMedicTreatInjuredPed()
{
    if (m_pAccident && m_pAccident->m_pPed && !m_pAccident->m_bIsRevived)
        m_pAccident->m_bIsTreated = false;

    if (m_pVehicle)
        m_pVehicle->CleanUpOldReference(reinterpret_cast<CEntity * *>(&m_pVehicle));

    if (m_pPartnerMedic)
        m_pPartnerMedic->CleanUpOldReference(reinterpret_cast<CEntity * *>(&m_pPartnerMedic));
}

// 0x658DB0
CTask* CTaskComplexMedicTreatInjuredPed::CreateSubTask(eTaskType taskType)
{
    switch (taskType)
    {
    case TASK_COMPLEX_TREAT_ACCIDENT:
        return new CTaskComplexTreatAccident(m_pAccident);
    case TASK_SIMPLE_STAND_STILL:
        return new CTaskSimpleStandStill(10000, false, false, 8.0F);
    case TASK_COMPLEX_ENTER_CAR_AS_PASSENGER:
        return new CTaskComplexEnterCarAsPassenger(m_pVehicle, 11, false);
    case TASK_COMPLEX_ENTER_CAR_AS_DRIVER:
        return new CTaskComplexEnterCarAsDriver(m_pVehicle);
    case TASK_COMPLEX_LEAVE_CAR:
        m_bLeftCarOnce = true;
        return new CTaskComplexLeaveCar(m_pVehicle, 0, 0, true, false);
    case TASK_SIMPLE_CAR_DRIVE:
        return new CTaskSimpleCarDrive(m_pVehicle, 0, 0);
    case TASK_COMPLEX_CAR_DRIVE_TO_POINT:
        return new CTaskComplexDriveToPoint(m_pVehicle, m_vecAccidentPosition, 30.0F, 0, -1, -1.0F, 2);
    case TASK_COMPLEX_CAR_DRIVE_WANDER:
        return new CTaskComplexCarDriveWander(m_pVehicle, 2, 30.0F);
    case TASK_COMPLEX_GO_TO_POINT_AND_STAND_STILL:
        return new CTaskComplexGoToPointAndStandStill(PEDMOVE_RUN, m_vecAccidentPosition, 0.125F, 2.0F, false, false);
    case TASK_COMPLEX_WANDER:
        return new CTaskComplexWanderMedic(PEDMOVE_WALK, CGeneral::GetRandomNumberInRange(0, 8), 1);
    default:
        return nullptr;
    }
}

// 0x65A020
CTask* CTaskComplexMedicTreatInjuredPed::CreateDealWithNextAccidentTask(CPed* ped, CAccident* accident)
{
    m_pAccident = CAccidentManager::GetInstance()->GetNearestFreeAccidentExceptThisOne(ped->GetPosition(), accident, true);
    if (m_pAccident && m_pAccident->m_pPed)
    {
        m_pAccident->m_bIsTreated = true;
        FindAccidentPosition(ped, m_pAccident->m_pPed);
        if (m_pSubTask && m_pSubTask->GetId() == TASK_COMPLEX_GO_TO_POINT_AND_STAND_STILL)
        {
            auto subTask = reinterpret_cast<CTaskComplexGoToPointAndStandStill*>(m_pSubTask);
            subTask->GoToPoint(m_vecAccidentPosition, 0.5F, 2.0F, false);
            subTask->m_moveState = PEDMOVE_RUN;
            return subTask;
        }
        else
            return CreateSubTask(TASK_COMPLEX_GO_TO_POINT_AND_STAND_STILL);
    }
    else
        return CreateSubTask(m_bIsDriver ? TASK_COMPLEX_ENTER_CAR_AS_DRIVER : TASK_COMPLEX_ENTER_CAR_AS_PASSENGER);
}

// 0x658CC0
void CTaskComplexMedicTreatInjuredPed::FindNearestAccident(CVector& posn)
{
    m_pAccident = CAccidentManager::GetInstance()->GetNearestFreeAccident(posn, true);
}

// 0x658D20
void CTaskComplexMedicTreatInjuredPed::FindAccidentPosition(CPed* ped, CPed* targetPed)
{
    CVector bonePositions[2];

    targetPed->GetBonePosition(bonePositions[0], BONE_L_THIGH, false);
    targetPed->GetBonePosition(bonePositions[1], BONE_R_THIGH, false);
    m_vecAccidentPosition = (bonePositions[0] + bonePositions[1]) * 0.5F;
}

// 0x659AF0
CTask* CTaskComplexMedicTreatInjuredPed::Clone() const
{
    return Clone_Reversed();
}

// 0x659FE0
CTask* CTaskComplexMedicTreatInjuredPed::CreateFirstSubTask(CPed* ped)
{
    return CreateFirstSubTask_Reversed(ped);
}

// 0x65A990
CTask* CTaskComplexMedicTreatInjuredPed::CreateNextSubTask(CPed* ped)
{
    return CreateNextSubTask_Reversed(ped);
}

// 0x65ABF0
CTask* CTaskComplexMedicTreatInjuredPed::ControlSubTask(CPed* ped)
{
    return ControlSubTask_Reversed(ped);
}

CTask* CTaskComplexMedicTreatInjuredPed::Clone_Reversed() const
{
    return new CTaskComplexMedicTreatInjuredPed(m_pVehicle, m_pPartnerMedic, m_bIsDriver);
}

CTask* CTaskComplexMedicTreatInjuredPed::CreateFirstSubTask_Reversed(CPed* ped)
{
    if (!m_pVehicle)
        return CreateSubTask(TASK_COMPLEX_WANDER);

    if (m_bIsDriver)
        return CreateSubTask(TASK_COMPLEX_CAR_DRIVE_WANDER);

    return CreateSubTask(TASK_SIMPLE_CAR_DRIVE);
}

CTask* CTaskComplexMedicTreatInjuredPed::CreateNextSubTask_Reversed(CPed* ped)
{
    eTaskType subTaskId = m_pSubTask->GetId();

    if (subTaskId == TASK_COMPLEX_TREAT_ACCIDENT)
    {
        FindNearestAccident(ped->GetPosition());
        if (!m_pAccident)
            return CreateSubTask(m_bIsDriver ? TASK_COMPLEX_ENTER_CAR_AS_DRIVER : TASK_COMPLEX_ENTER_CAR_AS_PASSENGER);
        else
        {
            m_pAccident->m_bIsTreated = true;
            FindAccidentPosition(ped, m_pAccident->m_pPed);
            return CreateSubTask(TASK_COMPLEX_GO_TO_POINT_AND_STAND_STILL);
        }
    }

    if (subTaskId == TASK_SIMPLE_STAND_STILL)
        return CreateSubTask(m_bIsDriver ? TASK_COMPLEX_ENTER_CAR_AS_DRIVER : TASK_COMPLEX_ENTER_CAR_AS_PASSENGER);

    if (subTaskId == TASK_COMPLEX_ENTER_CAR_AS_DRIVER || subTaskId == TASK_COMPLEX_ENTER_CAR_AS_PASSENGER)
        return CreateSubTask(ped->bInVehicle ? TASK_SIMPLE_CAR_DRIVE : TASK_COMPLEX_WANDER);

    if (subTaskId == TASK_COMPLEX_LEAVE_CAR)
    {
        g_InterestingEvents.Add((CInterestingEvents::EType)12, ped);
        if (m_bIsDriver && m_pAccident->m_pPed && m_pAccident->m_bIsTreated)
        {
            FindAccidentPosition(ped, m_pAccident->m_pPed);
            return CreateSubTask(TASK_COMPLEX_GO_TO_POINT_AND_STAND_STILL);
        }
        else
        {
            FindNearestAccident(ped->GetPosition());
            if (m_pAccident)
            {
                m_pAccident->m_bIsTreated = true;
                FindAccidentPosition(ped, m_pAccident->m_pPed);
                return CreateSubTask(TASK_COMPLEX_GO_TO_POINT_AND_STAND_STILL);
            }
            else
            {
                return CreateSubTask(m_bIsDriver ? TASK_COMPLEX_ENTER_CAR_AS_DRIVER : TASK_SIMPLE_STAND_STILL);
            }
        }
    }

    if (subTaskId == TASK_COMPLEX_GO_TO_POINT_AND_STAND_STILL)
    {
        auto relPosn = ped->GetPosition() - m_vecAccidentPosition;
        if (relPosn.Magnitude2D() <= 1.0F && fabs(relPosn.z) <= 2.0F)
            return CreateSubTask(TASK_COMPLEX_TREAT_ACCIDENT);
        else
            return CreateDealWithNextAccidentTask(ped, m_pAccident);
    }

    return nullptr;
}

CTask* CTaskComplexMedicTreatInjuredPed::ControlSubTask_Reversed(CPed* ped)
{
    eTaskType subTaskId = m_pSubTask->GetId();

    if (subTaskId == TASK_SIMPLE_CAR_DRIVE)
    {
        if (ped->bInVehicle && m_bIsDriver && m_pVehicle->IsPassenger(m_pPartnerMedic))
            return CreateSubTask(TASK_COMPLEX_CAR_DRIVE_WANDER);

        if (!m_bLeftCarOnce && m_pPartnerMedic)
        {
            auto partnerTask = m_pPartnerMedic->GetTaskManager().GetActiveTask();
            if (partnerTask && partnerTask->GetId() == TASK_COMPLEX_MEDIC_TREAT_INJURED_PED)
            {
                CTaskComplexMedicTreatInjuredPed* taskTreat = reinterpret_cast<CTaskComplexMedicTreatInjuredPed*>(partnerTask);
                if (taskTreat->m_pAccident && taskTreat->m_pAccident->m_pPed)
                {
                    m_vecAccidentPosition = taskTreat->m_vecAccidentPosition;
                    if (DistanceBetweenPoints(m_pVehicle->GetPosition(), m_vecAccidentPosition) < 10.0F)
                    {
                        ped->Say(231, 0, 1.0F, false, false, false);
                        return CreateSubTask(TASK_COMPLEX_LEAVE_CAR);
                    }
                }
            }
        }
    }

    if (subTaskId == TASK_COMPLEX_CAR_DRIVE_TO_POINT)
    {
        if (ped->bInVehicle)
        {
            if (!m_pAccident || !m_pAccident->m_pPed)
                return CreateSubTask(TASK_COMPLEX_CAR_DRIVE_WANDER);

            if (DistanceBetweenPoints(m_pVehicle->GetPosition(), m_vecAccidentPosition) < 10.0F)
            {
                ped->Say(231, 0, 1.0F, false, false, false);
                return CreateSubTask(TASK_COMPLEX_LEAVE_CAR);
            }
        }
    }

    if (subTaskId == TASK_COMPLEX_CAR_DRIVE_WANDER)
    {
        if (ped->bInVehicle)
        {
            FindNearestAccident(ped->GetPosition());
            if (m_pAccident)
            {
                m_pAccident->m_bIsTreated = true;
                FindAccidentPosition(ped, m_pAccident->m_pPed);
                return CreateSubTask(TASK_COMPLEX_CAR_DRIVE_TO_POINT);
            }
        }
    }

    if (subTaskId == TASK_COMPLEX_GO_TO_POINT_AND_STAND_STILL)
    {
        if (!m_pAccident || !m_pAccident->m_pPed)
            return CreateDealWithNextAccidentTask(ped, 0);

        auto subTask = reinterpret_cast<CTaskComplexGoToPointAndStandStill*>(m_pSubTask);
        if (subTask->m_moveState != PEDMOVE_WALK && DistanceBetweenPoints(m_vecAccidentPosition, ped->GetPosition()) < 25.0F)
        {
            for (CEntity* pEntity : ped->GetIntelligence()->m_vehicleScanner.m_apEntities)
                if (pEntity && CPedGeometryAnalyser::IsEntityBlockingTarget(pEntity, m_vecAccidentPosition, 0.125F))
                    return CreateDealWithNextAccidentTask(ped, 0);

            FindAccidentPosition(ped, m_pAccident->m_pPed);
            subTask->m_moveState = PEDMOVE_WALK;
            subTask->GoToPoint(m_vecAccidentPosition, 0.5F, 2.0F, false);
        }
    }

    return m_pSubTask;
}
