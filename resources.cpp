#include <array>
#include <physfs.h>
#include "debug.h"
#include "resources.h"

void ResourcesManager::LoadPack(std::filesystem::path mount, std::string mountPoint)
{
    std::string s = ToOSPath(mount.string());
    int success = PHYSFS_mount(ToOSPath(mount.string()).c_str(), mountPoint.c_str(), true);
    if (!success)
        Debug::WriteLn("LoadPack error: " + std::string(PHYSFS_getLastError()));
}

void ResourcesManager::UnloadPack(std::filesystem::path mount)
{
    int success = PHYSFS_unmount(ToOSPath(mount.string()).c_str());
    if (!success)
        Debug::WriteLn("UnloadPack error: " + std::string(PHYSFS_getLastError()));
}

std::vector<std::string> ResourcesManager::GetLoadedPacks()
{
    return std::vector<std::string>();
}

void ResourcesManager::List(std::string path)
{
    char **rc = PHYSFS_enumerateFiles(path.c_str());
    char **i;
    for (i = rc; *i != NULL; i++)
        printf(" * We've got [%s].\n", *i);
    PHYSFS_freeList(rc);
}

std::string ResourcesManager::ToOSPath(std::string path)
{
#ifdef _WIN32
    std::replace(path.begin(), path.end(), '/', '\\');
    return path;
#else
    return path;
#endif
}

ResourcesManager::ResourcesManager(const char* argv0)
{
    PHYSFS_init(argv0);
    LoadPack("main.25p", "/res");
}

ResourcesManager::~ResourcesManager()
{
    PHYSFS_deinit();
}

std::vector<std::string> ResourcesManager::GetExtraPacks()
{
    std::vector<std::string> result;
    std::string path = "extra/";
    for (const auto& entry : std::filesystem::directory_iterator(path))
        result.push_back(entry.path().string());
    return result;
}

void ResourcesManager::LoadExtra(std::vector<std::string> packNames)
{

}

void ResourcesManager::UnloadExtra()
{

}

void ResourcesManager::LoadScene(std::string name)
{
    LoadPack("scenes/"+name+".25p", "/res");
    mapName = name;
}

void ResourcesManager::UnloadScene()
{
    UnloadPack("scenes/"+mapName);
}

std::vector<uint8_t> ResourcesManager::GetResource(std::string path)
{
    std::vector<uint8_t> result;

    PHYSFS_File* fHandle = PHYSFS_openRead(path.c_str());
    if (fHandle == NULL) {
        Debug::WriteLn("GetResource open error: " + std::string(PHYSFS_getLastError()));
        return result;
    }

    std::array<uint8_t, 100> buffer;
    while (!PHYSFS_eof(fHandle)) {
        int len = PHYSFS_readBytes(fHandle, buffer.data(), buffer.size());
        if (len < 0) {
            Debug::WriteLn("GetResource read error: " + std::string(PHYSFS_getLastError()));
            return result;
        }
        result.insert(result.end(), buffer.begin(), buffer.begin()+len);
    }
    return result;
}
