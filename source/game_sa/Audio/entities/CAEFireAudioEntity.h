/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "CAEAudioEntity.h"

class FxSystem_c;

class CAEFireAudioEntity : public CAEAudioEntity {
public:
    CAESound*   field_7C;
    CAESound*   field_80;
    FxSystem_c* field_84;
};

VALIDATE_SIZE(CAEFireAudioEntity, 0x88);