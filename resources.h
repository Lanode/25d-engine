#ifndef RESOURCES_H
#define RESOURCES_H
#include <string>
#include <vector>
#include <filesystem>

class ResourcesManager
{
private:
    std::string mapName;

    void LoadPack(std::filesystem::path mount, std::string mountPoint);
    void UnloadPack(std::filesystem::path mount);
    std::vector<std::string> GetLoadedPacks();
    void List(std::string path);

    std::string ToOSPath(std::string path);
public:
    ResourcesManager(const char* argv0);
    ~ResourcesManager();
    std::vector<std::string> GetExtraPacks();
    void LoadExtra(std::vector<std::string> packNames);
    void UnloadExtra();
    void LoadScene(std::string name);
    void UnloadScene();
    std::vector<uint8_t> GetResource(std::string path);
};

#endif // RESOURCES_H
