#include "StdInc.h"

void CTaskSimpleHoldEntity::InjectHooks() {

    HookInstall(0x6913A0, (CTaskSimpleHoldEntity * (CTaskSimpleHoldEntity::*)(CEntity*, const CVector*, uint8_t, uint8_t, int, int, bool)) & CTaskSimpleHoldEntity::Constructor);
    HookInstall(0x691470, (CTaskSimpleHoldEntity * (CTaskSimpleHoldEntity::*)(CEntity*, const CVector*, uint8_t, uint8_t, const char*, const char*, int)) & CTaskSimpleHoldEntity::Constructor);
    HookInstall(0x691550, (CTaskSimpleHoldEntity * (CTaskSimpleHoldEntity::*)(CEntity*, const CVector*, uint8_t, uint8_t, CAnimBlock*, CAnimBlendHierarchy*, int)) & CTaskSimpleHoldEntity::Constructor);
    HookInstall(0x6929B0, &CTaskSimpleHoldEntity::Clone_Reversed);
    HookInstall(0x691460, &CTaskSimpleHoldEntity::GetId_Reversed);
    HookInstall(0x693BD0, &CTaskSimpleHoldEntity::MakeAbortable_Reversed);
    HookInstall(0x693C40, &CTaskSimpleHoldEntity::ProcessPed_Reversed);
    HookInstall(0x6940A0, &CTaskSimpleHoldEntity::SetPedPosition_Reversed);
    HookInstall(0x6916E0, &CTaskSimpleHoldEntity::ReleaseEntity);
    HookInstall(0x691700, &CTaskSimpleHoldEntity::CanThrowEntity);
    HookInstall(0x691720, &CTaskSimpleHoldEntity::PlayAnim);
    HookInstall(0x691740, &CTaskSimpleHoldEntity::FinishAnimHoldEntityCB);
    HookInstall(0x692FF0, &CTaskSimpleHoldEntity::StartAnim);
    HookInstall(0x6930F0, &CTaskSimpleHoldEntity::DropEntity);
    HookInstall(0x693440, &CTaskSimpleHoldEntity::ChoosePutDownHeight);
}

CTaskSimpleHoldEntity::CTaskSimpleHoldEntity(CEntity* entityToHold, const CVector* position, uint8_t boneFrameId, uint8_t boneFlags, int animId, int groupId, bool bDisAllowDroppingOnAnimEnd)
{
    m_pEntityToHold = entityToHold;
    m_vecPosition = CVector(0.0f, 0.0f, 0.0f);
    m_bBoneFrameId = boneFrameId;
    m_bBoneFlags = boneFlags;
    m_nAnimId = animId;
    m_nAnimGroupId = groupId;
    m_bDisallowDroppingOnAnimEnd = bDisAllowDroppingOnAnimEnd;
    m_pAnimBlock = nullptr;
    m_pAnimBlendHierarchy = nullptr;
    m_bEntityDropped = false;
    m_bEntityRequiresProcessing = true;
    m_pAnimBlendAssociation = nullptr;
    if (position)
        m_vecPosition = *position;
    if (m_pEntityToHold) {
        m_pEntityToHold->m_bStreamingDontDelete = true;
        m_pEntityToHold->RegisterReference(&m_pEntityToHold);
    }
}

CTaskSimpleHoldEntity::CTaskSimpleHoldEntity(CEntity* entityToHold, const CVector* position, uint8_t boneFrameId, uint8_t boneFlags, const char* animName, const char* animBlockName, int animFlags)
{
    m_pEntityToHold = entityToHold;
    m_vecPosition = CVector(0.0f, 0.0f, 0.0f);
    m_bBoneFrameId = boneFrameId;
    m_bBoneFlags = boneFlags;
    m_nAnimId = ANIM_ID_NO_ANIMATION_SET;
    m_nAnimGroupId = 0;
    m_bEntityDropped = false;
    m_bEntityRequiresProcessing = true;
    m_bDisallowDroppingOnAnimEnd = false;
    m_pAnimBlendAssociation = nullptr;
    if (position)
        m_vecPosition = *position;
    if (m_pEntityToHold) {
        m_pEntityToHold->m_bStreamingDontDelete = true;
        m_pEntityToHold->RegisterReference(&m_pEntityToHold);
    }
    m_animFlags = animFlags;
    m_pAnimBlock = CAnimManager::GetAnimationBlock(animBlockName);
    m_pAnimBlendHierarchy = CAnimManager::GetAnimation(animName, m_pAnimBlock);
    CAnimManager::AddAnimBlockRef(m_pAnimBlock - CAnimManager::ms_aAnimBlocks);
}

CTaskSimpleHoldEntity::CTaskSimpleHoldEntity(CEntity* entityToHold, const CVector* position, uint8_t boneFrameId, uint8_t boneFlags, CAnimBlock* animBlock, CAnimBlendHierarchy* animHierarchy, int animFlags)
{
    m_pEntityToHold = entityToHold;
    m_vecPosition = CVector(0.0f, 0.0f, 0.0f);
    m_bBoneFrameId = boneFrameId;
    m_bBoneFlags = boneFlags;
    m_fRotation = 0.0f;
    m_nAnimId = ANIM_ID_NO_ANIMATION_SET;
    m_nAnimGroupId = ANIM_GROUP_DEFAULT;
    m_bEntityDropped = false;
    m_bEntityRequiresProcessing = true;
    m_bDisallowDroppingOnAnimEnd = false;
    m_pAnimBlendAssociation = nullptr;
    if (position)
        m_vecPosition = *position;
    if (m_pEntityToHold)
    {
        m_pEntityToHold->m_bStreamingDontDelete = true;
        m_pEntityToHold->RegisterReference(&m_pEntityToHold);
    }
    m_animFlags = animFlags;
    m_pAnimBlock = animBlock;
    m_pAnimBlendHierarchy = animHierarchy;
    CAnimManager::AddAnimBlockRef(animBlock - CAnimManager::ms_aAnimBlocks);
}

CTaskSimpleHoldEntity::~CTaskSimpleHoldEntity() {
    if (!m_bEntityRequiresProcessing) {
        if (m_pEntityToHold) {
            m_pEntityToHold->m_bStreamingDontDelete = false;
            m_pEntityToHold->CleanUpOldReference(&m_pEntityToHold);
            m_pEntityToHold->m_bRemoveFromWorld = true;
            m_pEntityToHold = nullptr;
        }
    }
    if (m_pAnimBlendAssociation)
        m_pAnimBlendAssociation->SetFinishCallback(CDefaultAnimCallback::DefaultAnimCB, nullptr);
    if (m_pAnimBlock)
        CAnimManager::RemoveAnimBlockRef(m_pAnimBlock - CAnimManager::ms_aAnimBlocks);
}

// 0x6913A0
CTaskSimpleHoldEntity* CTaskSimpleHoldEntity::Constructor(CEntity* entityToHold, const CVector* position, uint8_t boneFrameId, uint8_t boneFlags, int animId, int groupId, bool bDisAllowDroppingOnAnimEnd)
{
    this->CTaskSimpleHoldEntity::CTaskSimpleHoldEntity(entityToHold, position, boneFrameId, boneFlags, animId, groupId, bDisAllowDroppingOnAnimEnd);
    return this;
}

// 0x691470
CTaskSimpleHoldEntity* CTaskSimpleHoldEntity::Constructor(CEntity* entityToHold, const CVector* position, uint8_t boneFrameId, uint8_t boneFlags, const char* animName, const char* animBlockName, int animFlags)
{
    this->CTaskSimpleHoldEntity::CTaskSimpleHoldEntity(entityToHold, position, boneFrameId, boneFlags, animName, animBlockName, animFlags);
    return this;
}

// 0x691550
CTaskSimpleHoldEntity* CTaskSimpleHoldEntity::Constructor(CEntity* entityToHold, const CVector* position, uint8_t boneFrameId, uint8_t boneFlags, CAnimBlock* animBlock, CAnimBlendHierarchy* animHierarchy, int animFlags)
{
    this->CTaskSimpleHoldEntity::CTaskSimpleHoldEntity(entityToHold, position, boneFrameId, boneFlags, animBlock, animHierarchy, animFlags);
    return this;
}

// 0x6929B0
CTask* CTaskSimpleHoldEntity::Clone() const {
    return  CTaskSimpleHoldEntity::Clone_Reversed();
}

// 0x691460
eTaskType CTaskSimpleHoldEntity::GetId() {
    return GetId_Reversed();
}

// 0x693BD0
bool CTaskSimpleHoldEntity::MakeAbortable(class CPed* ped, eAbortPriority priority, const CEvent* event) {
    return CTaskSimpleHoldEntity::MakeAbortable_Reversed(ped, priority, event);
}

// 0x693C40
bool CTaskSimpleHoldEntity::ProcessPed(class CPed* ped) {
    return ProcessPed_Reversed(ped);
}

// 0x6940A0
bool CTaskSimpleHoldEntity::SetPedPosition(class CPed* ped) {
    return CTaskSimpleHoldEntity::SetPedPosition_Reversed(ped);
}

CTask* CTaskSimpleHoldEntity::Clone_Reversed() const {
    if (m_pAnimBlendHierarchy)
        return new CTaskSimpleHoldEntity(m_pEntityToHold, &m_vecPosition, m_bBoneFrameId, m_bBoneFlags, m_pAnimBlock, m_pAnimBlendHierarchy, m_animFlags);
    else
        return new CTaskSimpleHoldEntity(m_pEntityToHold, &m_vecPosition, m_bBoneFrameId, m_bBoneFlags, m_nAnimId, m_nAnimGroupId, false);
}

bool CTaskSimpleHoldEntity::MakeAbortable_Reversed(class CPed* ped, eAbortPriority priority, const CEvent* event) {
    if (priority == ABORT_PRIORITY_URGENT || priority == ABORT_PRIORITY_IMMEDIATE) {
        if (m_pAnimBlendAssociation) {
            m_pAnimBlendAssociation->m_fBlendDelta = -4.0f;
            m_pAnimBlendAssociation->SetFinishCallback(CDefaultAnimCallback::DefaultAnimCB, nullptr);
            m_pAnimBlendAssociation = nullptr;
        }
        DropEntity(ped, true);
        m_bEntityDropped = true;
        return true;
    }
    else {
        if (m_pAnimBlendAssociation)
            m_pAnimBlendAssociation->m_fBlendDelta = -4.0f;
        return false;
    }
}

bool CTaskSimpleHoldEntity::ProcessPed_Reversed(class CPed* ped) {
    CTaskManager* pTaskManager = &ped->m_pIntelligence->m_TaskMgr;
    if (m_bEntityRequiresProcessing) {
        m_bEntityRequiresProcessing = false;
        if (GetId() == TASK_SIMPLE_PUTDOWN_ENTITY && !m_pEntityToHold && !m_pAnimBlendHierarchy && !m_nAnimGroupId) {
            auto pHoldEntityTask = (CTaskSimpleHoldEntity*)pTaskManager->GetTaskSecondary(TASK_SECONDARY_PARTIAL_ANIM);
            if (pHoldEntityTask && pHoldEntityTask->GetId() == TASK_SIMPLE_HOLD_ENTITY &&
                pHoldEntityTask->m_pEntityToHold && pHoldEntityTask->m_nAnimGroupId)
            {
                CEntity* pEntitToHold = pHoldEntityTask->m_pEntityToHold;
                m_pEntityToHold = pEntitToHold;
                pEntitToHold->RegisterReference(&m_pEntityToHold);

                m_vecPosition = pHoldEntityTask->m_vecPosition;
                m_nAnimId = ANIM_ID_PUTDWN;
                m_nAnimGroupId = pHoldEntityTask->m_nAnimGroupId;
                ChoosePutDownHeight(ped);
                pHoldEntityTask->ReleaseEntity();
            }
            else {
                m_bEntityDropped = true;
            }
        }
        CEntity* pEntitToHold = m_pEntityToHold;
        if (!pEntitToHold || m_bEntityDropped) {
            m_bEntityDropped = true;
        }
        else {
            pEntitToHold->m_bUsesCollision = false;
            if (pEntitToHold->m_nType == ENTITY_TYPE_OBJECT) {
                CObject* pObjecToHold = (CObject*)pEntitToHold;
                if (pObjecToHold->IsStatic()) {
                    pObjecToHold->SetIsStatic(false);
                    pObjecToHold->AddToMovingList();
                }
                pObjecToHold->physicalFlags.bAttachedToEntity = true;
                pObjecToHold->m_nFakePhysics = 0;
            }

            m_fRotation = pEntitToHold->GetHeading() - ped->m_fCurrentRotation;
        }
    }

    if (m_bEntityDropped)
        return true;

    if (m_nAnimId != ANIM_ID_NO_ANIMATION_SET || m_pAnimBlendHierarchy) {
        auto pAnimBlendAssoc = m_pAnimBlendAssociation;
        if (pAnimBlendAssoc) {
            if (pAnimBlendAssoc->m_fBlendDelta < 0.0f && !m_bDisallowDroppingOnAnimEnd)
                DropEntity(ped, true);
        }
        else {
            StartAnim(ped);
        }
    }

    if (m_pEntityToHold) {
        CEntity* pEntityToHold = m_pEntityToHold;
        eTaskType taskId = GetId();
        if (taskId != TASK_SIMPLE_HOLD_ENTITY) {
            if (taskId != TASK_SIMPLE_PICKUP_ENTITY) {
                auto pTaskHoldEntity = static_cast<CTaskSimpleHoldEntity*>(pTaskManager->GetTaskSecondary(TASK_SECONDARY_PARTIAL_ANIM));
                if (pTaskHoldEntity && pTaskHoldEntity->GetId() == TASK_SIMPLE_HOLD_ENTITY)
                    pTaskHoldEntity->MakeAbortable(ped, ABORT_PRIORITY_URGENT, nullptr);
            }
            else
            {
                CTaskSimplePickUpEntity* pTaskPickUpEntity = static_cast<CTaskSimplePickUpEntity*>(this);
                CAnimBlendAssociation* pAnimAssoc = pTaskPickUpEntity->m_pAnimBlendAssociation;
                if ((!pAnimAssoc || pTaskPickUpEntity->m_fMovePedUntilAnimProgress > pAnimAssoc->m_fCurrentTime)
                    && (pTaskPickUpEntity->m_vecPickupPosition.x != 0.0f || pTaskPickUpEntity->m_vecPickupPosition.y != 0.0f))
                {
                    CVector outPoint = pEntityToHold->GetMatrix() * pTaskPickUpEntity->m_vecPickupPosition;
                    outPoint -= ped->GetPosition();
                    ped->m_vecAnimMovingShiftLocal.x += DotProduct(&outPoint, &ped->GetRight()) / CTimer::ms_fTimeStep * 0.1f;
                    ped->m_vecAnimMovingShiftLocal.y += DotProduct(&outPoint, &ped->GetForward()) / CTimer::ms_fTimeStep * 0.1f ;
                    CVector direction = pEntityToHold->GetPosition() - ped->GetPosition();
                    ped->m_fAimingRotation = atan2(-direction.x, direction.y);
                }
            }
        }
        else {
            if ((m_nAnimGroupId == ANIM_GROUP_CARRY || m_nAnimGroupId == ANIM_GROUP_CARRY05 || m_nAnimGroupId == ANIM_GROUP_CARRY105) && ped->IsPlayer()) {
                CPlayerPed* pPlayer = (CPlayerPed*)ped;
                CPad* pPad = pPlayer->GetPadFromPlayer();
                if (pPad->ExitVehicleJustDown()) {
                    auto pTaskSimplePutDownEntity = new CTaskSimplePutDownEntity();
                    CEventScriptCommand eventScriptCommand(TASK_PRIMARY_PRIMARY, pTaskSimplePutDownEntity, false);
                    ped->GetEventGroup().Add(&eventScriptCommand, 0);
                }
            }
        }
    }
    return false;
}

bool CTaskSimpleHoldEntity::SetPedPosition_Reversed(class CPed* ped) {
    bool bUpdateEntityToHoldPosition = false;
    if (GetId() == TASK_SIMPLE_PICKUP_ENTITY)
    {
        auto pPickupEntityTask = static_cast<CTaskSimplePickUpEntity*>(this);
        if (!m_pAnimBlendAssociation || pPickupEntityTask->m_fMovePedUntilAnimProgress > m_pAnimBlendAssociation->m_fCurrentTime)
            return false;
        bUpdateEntityToHoldPosition = true;
    }
    else {
        auto pPutDownEntityTask = static_cast<CTaskSimplePutDownEntity*>(this);
        if (GetId() != TASK_SIMPLE_PUTDOWN_ENTITY || m_bEntityRequiresProcessing
            || m_pAnimBlendAssociation && pPutDownEntityTask->m_fPutDownHeightZ >= m_pAnimBlendAssociation->m_fCurrentTime)
        {
            bUpdateEntityToHoldPosition = true;
        }
    }

    if (bUpdateEntityToHoldPosition) {
        if (m_pEntityToHold) {
            m_pEntityToHold->m_bIsVisible = ped->m_bIsVisible;
            if (ped->bCalledPreRender) {
                if (m_bBoneFlags & HOLD_ENTITY_UPDATE_TRANSLATION_ONLY) {
                    CVector entityToHoldPos = m_vecPosition;
                    RpHAnimHierarchy* pHAnimHierarchy = GetAnimHierarchyFromSkinClump(ped->m_pRwClump);
                    int animIndex = RpHAnimIDGetIndex(pHAnimHierarchy, ped->m_apBones[m_bBoneFrameId]->m_nNodeId);
                    RwMatrix* pBoneMatrix = &RpHAnimHierarchyGetMatrixArray(pHAnimHierarchy)[animIndex];
                    RwV3dTransformPoints((RwV3d*)&entityToHoldPos, (RwV3d*)&entityToHoldPos, 1, pBoneMatrix);
                    m_pEntityToHold->GetMatrix().UpdateMatrix(pBoneMatrix);
                    m_pEntityToHold->SetPosn(entityToHoldPos);
                }
                else {
                    CVector entityToHoldPos = Multiply3x3(ped->GetMatrix(), m_vecPosition);
                    RpHAnimHierarchy* pHAnimHierarchy = GetAnimHierarchyFromSkinClump(ped->m_pRwClump);
                    int animIndex = RpHAnimIDGetIndex(pHAnimHierarchy, ped->m_apBones[m_bBoneFrameId]->m_nNodeId);
                    RwMatrix* pBoneMatrix = RpHAnimHierarchyGetMatrixArray(pHAnimHierarchy);
                    entityToHoldPos += *RwMatrixGetPos(&pBoneMatrix[animIndex]);
                    CMatrix rotationMatrix(ped->GetMatrix());
                    rotationMatrix.RotateZ(m_fRotation);
                    *static_cast<CMatrix*>(&m_pEntityToHold->GetMatrix()) = rotationMatrix;
                    m_pEntityToHold->SetPosn(entityToHoldPos);
                }
            }
            else {
                if (ped->m_matrix)
                    m_pEntityToHold->SetPosn(ped->GetPosition());
                else
                    m_pEntityToHold->SetPosn(ped->m_placement.m_vPosn);
            }
            m_pEntityToHold->UpdateRW();
            m_pEntityToHold->UpdateRwFrame();
            return true;
        }
        else {
            MakeAbortable(ped, ABORT_PRIORITY_URGENT, nullptr);
            return false;
        }
        return false;
    }
    if (m_pEntityToHold)
        DropEntity(ped, false);
    return false;
}

// 0x6916E0
void CTaskSimpleHoldEntity::ReleaseEntity() {
    if (m_pEntityToHold) {
        m_pEntityToHold->CleanUpOldReference(&m_pEntityToHold);
        m_pEntityToHold = nullptr;
    }
}

// 0x691700
bool CTaskSimpleHoldEntity::CanThrowEntity() {
    return m_pEntityToHold && m_bBoneFlags == HOLD_ENTITY_UPDATE_TRANSLATION_ONLY && m_bBoneFrameId == PED_NODE_RIGHT_HAND;
}

// 0x691720
void CTaskSimpleHoldEntity::PlayAnim(int groupId, int animId) {
    m_nAnimId = groupId;
    m_nAnimGroupId = animId;
}

// 0x691740
void CTaskSimpleHoldEntity::FinishAnimHoldEntityCB(CAnimBlendAssociation* pAnimAssoc, void* pData) {
    auto pTaskHoldEntity = reinterpret_cast<CTaskSimpleHoldEntity*>(pData);
    if (pTaskHoldEntity->m_bDisallowDroppingOnAnimEnd) {
        pTaskHoldEntity->m_nAnimId = ANIM_ID_NO_ANIMATION_SET;
        pTaskHoldEntity->m_nAnimGroupId = 0;
        pTaskHoldEntity->m_pAnimBlendAssociation = nullptr;
    }
    else
    {
        if (pTaskHoldEntity->GetId() == TASK_SIMPLE_PICKUP_ENTITY && pAnimAssoc->m_fBlendAmount > 0.0f) {
            CEntity* pEntityToHold = pTaskHoldEntity->m_pEntityToHold;
            if (pEntityToHold) {
                pEntityToHold->CleanUpOldReference(&pTaskHoldEntity->m_pEntityToHold);
                pTaskHoldEntity->m_pEntityToHold = nullptr;
            }
        }
        pTaskHoldEntity->m_bEntityDropped = true;
        pTaskHoldEntity->m_pAnimBlendAssociation = nullptr;
    }
}

// 0x692FF0
void CTaskSimpleHoldEntity::StartAnim(CPed* pPed) {
    if (m_pAnimBlendHierarchy) {
        m_animFlags |= ANIM_FLAG_400 | ANIM_FLAG_FREEZE_LAST_FRAME | ANIM_FLAG_PARTIAL;
        m_pAnimBlendAssociation = CAnimManager::BlendAnimation(pPed->m_pRwClump, m_pAnimBlendHierarchy, m_animFlags, 4.0f);
    }
    else {
        if (m_nAnimGroupId && !m_pAnimBlock) {
            CAnimBlock* pAnimBlock = CAnimManager::ms_aAnimAssocGroups[m_nAnimGroupId].m_pAnimBlock;
            if (!pAnimBlock)
                pAnimBlock = CAnimManager::GetAnimationBlock(CAnimManager::GetAnimBlockName(m_nAnimGroupId));
            if (!pAnimBlock->bLoaded) {
                CStreaming::RequestModel(pAnimBlock - CAnimManager::ms_aAnimBlocks + RESOURCE_ID_IFP, STREAMING_KEEP_IN_MEMORY);
                return;
            }
            CAnimManager::AddAnimBlockRef(pAnimBlock - CAnimManager::ms_aAnimBlocks);
            m_pAnimBlock = pAnimBlock;
        }
        m_pAnimBlendAssociation = CAnimManager::BlendAnimation(pPed->m_pRwClump, m_nAnimGroupId, m_nAnimId, 4.0f);
        m_pAnimBlendAssociation->m_nFlags |= ANIM_FLAG_FREEZE_LAST_FRAME;
        if (GetId() == TASK_SIMPLE_HOLD_ENTITY)
            m_pAnimBlendAssociation->m_nFlags |= ANIM_FLAG_400;
    }

    if (GetId() == TASK_SIMPLE_PICKUP_ENTITY)
        m_pAnimBlendAssociation->SetFinishCallback(CTaskSimpleHoldEntity::FinishAnimHoldEntityCB, this);
    else
        m_pAnimBlendAssociation->SetDeleteCallback(CTaskSimpleHoldEntity::FinishAnimHoldEntityCB, this);
}

// 0x6930F0
void CTaskSimpleHoldEntity::DropEntity(CPed* pPed, bool bAddEventSoundQuiet) {
    bool bUpdateEntityPosition = true;
    CObject* pObjectToHold = nullptr;
    if (m_pEntityToHold) {
        m_pEntityToHold->m_bUsesCollision = true;
        if (m_pEntityToHold->m_nType != ENTITY_TYPE_OBJECT) {
            m_pEntityToHold->CleanUpOldReference(&m_pEntityToHold);
            m_pEntityToHold = nullptr;
            return;
        }
        pObjectToHold = static_cast<CObject*>(m_pEntityToHold);
        pObjectToHold->m_pEntityIgnoredCollision = pPed;
        if (pObjectToHold->physicalFlags.bDisableCollisionForce && bAddEventSoundQuiet) {
            if (!pObjectToHold->objectFlags.bIsLiftable) {
                unsigned char objectType = pObjectToHold->m_nObjectType;
                if (objectType != OBJECT_MISSION && objectType != OBJECT_MISSION2) {
                    if (objectType != OBJECT_TEMPORARY)
                        ++CObject::nNoTempObjects;
                    pObjectToHold->m_nObjectType = OBJECT_TEMPORARY;
                    pObjectToHold->m_dwRemovalTime = 0;
                    pObjectToHold->m_bUsesCollision = false;
                    pObjectToHold->m_bIsVisible = false;
                    bUpdateEntityPosition = false;
                }
            }
        }
        else {
            pObjectToHold->physicalFlags.bAttachedToEntity = false;
            if (!bAddEventSoundQuiet) {
                bUpdateEntityPosition = false;
            }
            else {
                if (pObjectToHold->IsStatic()) {
                    pObjectToHold->SetIsStatic(false);
                    pObjectToHold->AddToMovingList();
                }
                CPhysical* pPhysicalEntity = static_cast<CPhysical*>(m_pEntityToHold);
                float randomSpeedUp = rand() * 4.6566e-10f * 0.03f + 0.03f;
                float randomSpeedRight = rand() * 4.6566e-10f * 0.06f - 0.03f;
                pPhysicalEntity->m_vecMoveSpeed += randomSpeedUp * pPed->GetForward();
                pPhysicalEntity->m_vecMoveSpeed += randomSpeedRight * pPed->GetRight();
                pPhysicalEntity->m_vecMoveSpeed += 0.01f * pPed->GetUp();
                if (!pObjectToHold->TestCollision(false))
                    bUpdateEntityPosition = false;
            }
        }

        CVector objectToHoldPosition(0.0f, 0.0f, 0.0f);
        if (pObjectToHold->objectFlags.bIsLiftable && pPed->m_pPlayerData && bAddEventSoundQuiet) {
            CEventSoundQuiet eventSoundQuiet(pPed, 60.0f, -1, objectToHoldPosition);
            CEventGroup* pEventGroup = GetEventGlobalGroup();
            pEventGroup->Add(&eventSoundQuiet, false);
        }

        if (bUpdateEntityPosition) {
            pObjectToHold->m_vecMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
            pObjectToHold->SetIsStatic(true);
            pObjectToHold->RemoveFromMovingList();
            CColModel* pEntityToHoldColModel = pObjectToHold->GetColModel();
            objectToHoldPosition = pPed->GetPosition();
            objectToHoldPosition.z = objectToHoldPosition.z - 1.0f - pEntityToHoldColModel->m_boundBox.m_vecMin.z;
            pObjectToHold->SetPosn(objectToHoldPosition);
            pObjectToHold->UpdateRW();
            pObjectToHold->UpdateRwFrame();
        }
        m_pEntityToHold->CleanUpOldReference(&m_pEntityToHold);
        m_pEntityToHold = nullptr;
        return;
    }
}

// 0x693440
void CTaskSimpleHoldEntity::ChoosePutDownHeight(CPed* pPed) {
    CVector origin = (pPed->GetForward() * 0.65f) + pPed->GetPosition();
    origin.z += 0.2f;
    float distance = origin.z - 1.5f;
    CEntity* outEntity = nullptr;
    CColPoint colPoint;
    if (!CWorld::ProcessVerticalLine(origin, distance, colPoint, outEntity, true, false, false, true, false, false, nullptr)) {
        m_nAnimGroupId = ANIM_GROUP_CARRY;
    }
    else {
        if (pPed->GetPosition().z - 0.2f <= colPoint.m_vecPoint.z) {
            m_nAnimGroupId = ANIM_GROUP_CARRY105;
        }
        else {
            if (pPed->GetPosition().z - 0.7f <= colPoint.m_vecPoint.z)
                m_nAnimGroupId = ANIM_GROUP_CARRY05;
            else
                m_nAnimGroupId = ANIM_GROUP_CARRY;
        }
    }
    if (GetId() == TASK_SIMPLE_PUTDOWN_ENTITY) {
        auto pTaskPutDownEntity = static_cast<CTaskSimplePutDownEntity*>(this);
        if (m_nAnimGroupId == ANIM_GROUP_CARRY105)
            pTaskPutDownEntity->m_fPutDownHeightZ = 0.433333f;
        else if (m_nAnimGroupId == ANIM_GROUP_CARRY05)
            pTaskPutDownEntity->m_fPutDownHeightZ = 0.366667f;
        else
            pTaskPutDownEntity->m_fPutDownHeightZ = 0.6f;
    }
}
