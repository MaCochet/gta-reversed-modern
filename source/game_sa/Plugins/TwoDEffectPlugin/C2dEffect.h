/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "CVector.h"
#include "CRGBA.h"

enum e2dEffectType : uint8 {
    EFFECT_LIGHT,
    EFFECT_PARTICLE,
    EFFECT_ATTRACTOR = 3,
    EFFECT_SUN_GLARE,
    EFFECT_FURNITUR,
    EFFECT_ENEX,
    EFFECT_ROADSIGN,
    EFFECT_SLOTMACHINE_WHEEL,
    EFFECT_COVER_POINT,
    EFFECT_ESCALATOR,
};

enum ePedAttractorType : uint8 {
    PED_ATTRACTOR_ATM            = 0, // Ped uses ATM(at day time only)
    PED_ATTRACTOR_SEAT           = 1, // Ped sits(at day time only)
    PED_ATTRACTOR_STOP           = 2, // Ped stands(at day time only)
    PED_ATTRACTOR_PIZZA          = 3, // Ped stands for few seconds
    PED_ATTRACTOR_SHELTER        = 4, // Ped goes away after spawning, but stands if weather is rainy
    PED_ATTRACTOR_TRIGGER_SCRIPT = 5, // Launches an external script
    PED_ATTRACTOR_LOOK_AT        = 6, // Ped looks at object, then goes away
    PED_ATTRACTOR_SCRIPTED       = 7, // This type is not valid
    PED_ATTRACTOR_PARK           = 8, // Ped lays(at day time only, ped goes away after 6 PM)
    PED_ATTRACTOR_STEP           = 9  // Ped sits on steps
};

// From https://gtamods.com/wiki/2d_Effect_(RW_Section)
enum e2dCoronaFlashType : uint8 {
    FLASH_DEFAULT         = 0,
    FLASH_RANDOM          = 1,
    FLASH_RANDOM_WHEN_WET = 2,
    FLASH_ANIM_SPEED_4X   = 3,
    FLASH_ANIM_SPEED_2X   = 4,
    FLASH_ANIM_SPEED_1X   = 5,
    FLASH_UNKN            = 6,
    FLASH_TRAFFICLIGHT    = 7,
    FLASH_TRAINCROSSING   = 8,
    FLASH_UNUSED          = 9,
    FLASH_ONLY_RAIN       = 10,
    FLASH_5ON_5OFF        = 11,
    FLASH_6ON_4OFF        = 12,
    FLASH_4ON_6OFF        = 13,
};

struct tEffectLight {
    RwRGBA m_color;
    float  m_fCoronaFarClip;
    float  m_fPointlightRange;
    float  m_fCoronaSize;
    float  m_fShadowSize;
    union {
        uint16 m_nFlags;
        struct {
            uint16 m_bCheckObstacles : 1;
            uint16 m_nFogType : 2;
            uint16 m_bWithoutCorona : 1;
            uint16 m_bOnlyLongDistance : 1;
            uint16 m_bAtDay : 1;
            uint16 m_bAtNight : 1;
            uint16 m_bBlinking1 : 1;

            uint16 m_bOnlyFromBelow : 1;
            uint16 m_bBlinking2 : 1;
            uint16 m_bUpdateHeightAboveGround : 1;
            uint16 m_bCheckDirection : 1;
            uint16 m_bBlinking3 : 1;
        };
    };
    e2dCoronaFlashType m_nCoronaFlashType;
    bool               m_bCoronaEnableReflection;
    uint8      m_nCoronaFlareType;
    uint8      m_nShadowColorMultiplier;
    char               m_nShadowZDistance;
    char               offsetX;
    char               offsetY;
    char               offsetZ;
    // char               _pad2E[2];
    RwTexture*         m_pCoronaTex;
    RwTexture*         m_pShadowTex;
    int32                field_38;
    int32                field_3C;
};
VALIDATE_SIZE(tEffectLight, 0x30);

struct tEffectParticle {
    char m_szName[24];
};
VALIDATE_SIZE(tEffectParticle, 0x18);

struct tEffectPedAttractor {
    RwV3d             m_vecQueueDir;
    RwV3d             m_vecUseDir;
    RwV3d             m_vecForwardDir;
    ePedAttractorType m_nAttractorType;
    uint8     m_nPedExistingProbability;
    uint8     field_36;
    uint8     m_nFlags;
    char              m_szScriptName[8];
};
VALIDATE_SIZE(tEffectPedAttractor, 0x30);

struct tEffectEnEx {
    float         m_fEnterAngle;
    RwV2d         m_vecRadius;
    RwV3d         m_vecExitPosn;
    float         m_fExitAngle;
    int16         m_nInteriorId;
    uint8 m_nFlags1;
    uint8 m_nSkyColor;
    char          m_szInteriorName[8];
    uint8 m_nTimeOn;
    uint8 m_nTimeOff;
    union {
        uint8 m_nFlags2;
        struct {
            uint8 bUnkn0x1 : 1;
            uint8 bUnkn0x2 : 1;
            uint8 bTimedEffect : 1; // 0x4
        };
    };
};
VALIDATE_SIZE(tEffectEnEx, 0x2C);

struct CRoadsignAttrFlags {
    uint16 m_nNumOfLines : 2;
    uint16 m_nSymbolsPerLine : 2;
    uint16 m_nTextColor : 2;
};
VALIDATE_SIZE(CRoadsignAttrFlags, 0x2);

struct tEffectRoadsign {
    RwV2d              m_vecSize;
    RwV3d              m_vecRotation;
    CRoadsignAttrFlags m_nFlags;
    // char               _pad26[2];
    char*              m_pText; // size is 64
    RpAtomic*          m_pAtomic;
};
VALIDATE_SIZE(tEffectRoadsign, 0x20);

struct tEffectCoverPoint {
    RwV2d         m_vecDirection;
    uint8 m_nType;
    // char _pad19[3];
};
VALIDATE_SIZE(tEffectCoverPoint, 0xC);

struct tEffectEscalator {
    RwV3d         m_vecBottom;
    RwV3d         m_vecTop;
    RwV3d         m_vecEnd;
    uint8 m_nDirection; // 0 - down, 1 - up
    // char          _pad35[3];
};
VALIDATE_SIZE(tEffectEscalator, 0x28);

class C2dEffect {
  public:
    CVector       m_vecPosn;
    e2dEffectType m_nType;
    char          _pad0[3];
    union {
        tEffectLight        light;
        tEffectParticle     particle;
        tEffectPedAttractor pedAttractor;
        tEffectEnEx         enEx;
        tEffectRoadsign     roadsign;
        int32        iSlotMachineIndex;
        tEffectCoverPoint   coverPoint;
        tEffectEscalator    escalator;
    };

public:
    static uint32& g2dEffectPluginOffset;
    static uint32& ms_nTxdSlot;

public:
    static void InjectHooks();

    void Shutdown();

    static int32 Roadsign_GetNumLinesFromFlags(CRoadsignAttrFlags flags);
    static int32 Roadsign_GetNumLettersFromFlags(CRoadsignAttrFlags flags);
    static int32 Roadsign_GetPaletteIDFromFlags(CRoadsignAttrFlags flags);

    static bool PluginAttach();
    static void DestroyAtomic(RpAtomic* pAtomic);
};
VALIDATE_SIZE(C2dEffect, 0x40);

// RW PLUGIN
struct t2dEffectPluginEntry {
    uint32 m_nObjCount;
    C2dEffect m_pObjects[16]; // Size not real, it's decided on runtime, 16 is written here only to see the objects in debugger without issues
};
struct t2dEffectPlugin {
    t2dEffectPluginEntry* m_pEffectEntries;
};
VALIDATE_SIZE(t2dEffectPlugin, 0x4);

#define C2DEFFECTPLG(geometry, var) \
    (RWPLUGINOFFSET(t2dEffectPlugin, geometry, C2dEffect::g2dEffectPluginOffset)->var)

#define C2DEFFECCONSTTPLG(geometry, var) \
    (RWPLUGINOFFSETCONST(t2dEffectPlugin, geometry, C2dEffect::g2dEffectPluginOffset)->var)

// Own function names, we don't seem to have symbols for those
uint32 RpGeometryGet2dFxCount(RpGeometry* pGeometry);
C2dEffect* RpGeometryGet2dFxAtIndex(RpGeometry* pGeometry, int32 iEffectInd);

void* t2dEffectPluginConstructor(void* object, RwInt32 offsetInObject, RwInt32 sizeInObject);
void* t2dEffectPluginDestructor(void* object, RwInt32 offsetInObject, RwInt32 sizeInObject);
void* t2dEffectPluginCopyConstructor(void* dstObject, const void* srcObject, RwInt32 offsetInObject, RwInt32 sizeInObject);

RwStream* Rwt2dEffectPluginDataChunkReadCallBack(RwStream* stream, RwInt32 binaryLength, void* object, RwInt32 offsetInObject, RwInt32 sizeInObject);
RwStream* Rwt2dEffectPluginDataChunkWriteCallBack(RwStream* stream, RwInt32 binaryLength, const void* object, RwInt32 offsetInObject, RwInt32 sizeInObject);
RwInt32 Rwt2dEffectPluginDataChunkGetSizeCallBack(const void* object, RwInt32 offsetInObject, RwInt32 sizeInObject);
