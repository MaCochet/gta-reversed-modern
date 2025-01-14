#include "StdInc.h"

#include "CollisionPlugin.h"

CClumpModelInfo*& CCollisionPlugin::ms_currentModel = *(CClumpModelInfo**)0x9689E0;

void CCollisionPlugin::InjectHooks() {
    ReversibleHooks::Install("CCollisionPlugin", "PluginAttach", 0x41B310, &CCollisionPlugin::PluginAttach);
    ReversibleHooks::Install("CCollisionPlugin", "SetModelInfo", 0x41B350, &CCollisionPlugin::SetModelInfo);
}

// internal
// 0x41B1A0
static void* ClumpCollisionConstructor(void* object, RwInt32 offsetInObject, RwInt32 sizeInObject) {
    return object;
}

// internal
// 0x41B1C0
static void* ClumpCollisionDestructor(void* object, RwInt32 offsetInObject, RwInt32 sizeInObject) {
    return object;
}

// internal
// 0x41B1B0
static void* ClumpCollisionCopyConstructor(void* dstObject, const void* srcObject, RwInt32 offsetInObject, RwInt32 sizeInObject) {
    return dstObject;
}

// internal
// 0x41B1D0
static RwStream* ClumpCollisionStreamRead(RwStream* stream, RwInt32 binaryLength, void* object, RwInt32 offsetInObject, RwInt32 sizeInObject) {
    return plugin::CallAndReturn<RwStream*, 0x41B1D0, RwStream*, RwInt32, void*, RwInt32, RwInt32>(stream, binaryLength, object, offsetInObject, sizeInObject);

    // incomplete
    CMemoryMgr::LockScratchPad();
    RwStreamRead(stream, &PC_Scratch, binaryLength);
    CColModel* model = new CColModel();

    switch (*(uint32*)PC_Scratch) {
    case make_fourcc4("COLL"):
        CFileLoader::LoadCollisionModel((uint8*)(&PC_Scratch[32]), *model);
        break;
    case make_fourcc4("COL2"):
        CFileLoader::LoadCollisionModelVer2((uint8*)(&PC_Scratch[32]), (PC_Scratch[4] - 24), *model, nullptr);
        break;
    case make_fourcc4("COL3"):
        CFileLoader::LoadCollisionModelVer3((uint8*)(&PC_Scratch[32]), (PC_Scratch[4] - 24), *model, nullptr);
        break;
    }

    model->MakeMultipleAlloc();
    CCollisionPlugin::ms_currentModel->SetColModel(model, true);
    CCollisionPlugin::ms_currentModel->bDontWriteZBuffer = true;
    CMemoryMgr::ReleaseScratchPad();
    return stream;
}

// internal
// 0x41B2F0
static RwStream* ClumpCollisionStreamWrite(RwStream* stream, RwInt32 binaryLength, const void* object, RwInt32 offsetInObject, RwInt32 sizeInObject) {
    return stream;
}

// internal
// 0x41B300
static RwInt32 ClumpCollisionGetSize(const void* object, RwInt32 offsetInObject, RwInt32 sizeInObject) {
    return -1;
}

// 0x41B310
bool CCollisionPlugin::PluginAttach() {
    // 0x9689DC unused
    static RwInt32 CollisionPlugin = RpClumpRegisterPlugin(
        0,
        rwID_COLLISIONPLUGIN,
        ClumpCollisionConstructor,
        ClumpCollisionDestructor,
        ClumpCollisionCopyConstructor
    );

    RpClumpRegisterPluginStream(
        rwID_COLLISIONPLUGIN,
        ClumpCollisionStreamRead,
        ClumpCollisionStreamWrite,
        ClumpCollisionGetSize
    );

    return TRUE;
}

// 0x41B350
void CCollisionPlugin::SetModelInfo(CClumpModelInfo* modelInfo) {
    ms_currentModel = modelInfo;
}
