#include "StdInc.h"

CTaskSimpleCarDriveTimed* CTaskSimpleCarDriveTimed::Constructor(CVehicle* pVehicle, int nTime)
{
    this->CTaskSimpleCarDriveTimed::CTaskSimpleCarDriveTimed(pVehicle, nTime);
    return this;
}

// 0x5FF940
CTaskSimpleCarDriveTimed::CTaskSimpleCarDriveTimed(CVehicle* pVehicle, int nTime) : CTaskSimpleCarDrive(pVehicle, false, false), m_nTimer()
{
    m_nTime = nTime;
}

CTaskSimpleCarDriveTimed::~CTaskSimpleCarDriveTimed()
{

}

bool CTaskSimpleCarDriveTimed::ProcessPed(CPed* ped)
{
    return plugin::CallMethodAndReturn<bool, 0x46F610, CTaskSimpleCarDriveTimed*, CPed*>(this, ped);
}

CTask* CTaskSimpleCarDriveTimed::Clone() const
{
    return new CTaskSimpleCarDriveTimed(m_pVehicle, m_nTime);
}
