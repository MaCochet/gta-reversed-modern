/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

/*
    http://www.gtamodding.com/wiki/IMG_archive
*/

class CDirectory {
public:
    struct DirectoryInfo {
        uint32 m_nOffset;
        uint16 m_nStreamingSize;
        uint16 m_nSizeInArchive;
        char   m_szName[24];
    };

    DirectoryInfo* m_pEntries{};
    uint32         m_nCapacity{};
    uint32         m_nNumEntries{};
    bool           m_bOwnsEntries{};

public:
    CDirectory();
    CDirectory(size_t capacity);
    ~CDirectory();

    void Init(int32 capacity, DirectoryInfo* entries);
    void AddItem(const DirectoryInfo& dirInfo);
    void ReadDirFile(const char* filename);
    bool WriteDirFile(const char* fileName);
    DirectoryInfo* FindItem(const char* itemName);
    bool FindItem(const char* name, uint32& outOffset, uint32& outStreamingSize);
    bool FindItem(uint32 hashKey, uint32& outOffset, uint32& outStreamingSize);

private:
    friend void InjectHooksMain();
    static void InjectHooks();

    CDirectory* Destructor();
    CDirectory* Constructor();
    CDirectory* Constructor(size_t capacity);
};

VALIDATE_SIZE(CDirectory, 0x10);
VALIDATE_SIZE(CDirectory::DirectoryInfo, 0x20);
