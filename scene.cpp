#include "include/json.hpp"
#include "debug.h"
#include "scene.h"

using json = nlohmann::json;

bool Scene::LoadFromResources(std::shared_ptr<ResourcesManager> resourcesManager)
{
    std::vector<uint8_t> sceneUnparsed = resourcesManager->GetResource("/res/scene.json");

	json data;
	try {
        data = json::parse(sceneUnparsed.begin(), sceneUnparsed.end());
	} catch (const json::parse_error& e) {
        Debug::WriteLn(e.what());
        return false;
	}

	try {
		for (int i = 0; i < data["materials"].size(); i++) {
			materials.push_back(std::shared_ptr<Material> (new Material));
            std::vector<uint8_t> binaryTexture = resourcesManager->GetResource(data["materials"][i]["texture"]);
            materials[i]->texture.loadFromMemory(binaryTexture.data(), binaryTexture.size());
            // materials[i]->texture.copyToImage().saveToFile("sosi.bmp");
            // exit(10);
		}

		for (int i = 0; i < data["cells"].size(); i++) {
			cells.push_back(std::shared_ptr<Cell>(new Cell));
            cells[i]->wallMaterials = materials[data["cells"][i]["wallMaterials"] - 1];
		}

		for (int y = 0; y < data["map"].size(); y++) {
			map.push_back(std::vector< std::shared_ptr<Cell> >(data["map"][y].size()));
			for (int x = 0; x < data["map"][y].size(); x++) {
				map[y][x] = (data["map"][y][x] == 0) ? nullptr : cells[data["map"][y][x] - 1];
			}
		}
	} catch (const json::exception& e) {
        Debug::WriteLn(e.what());
        return false;
	}
    return true;
}

std::shared_ptr<Cell> Scene::GetCell(sf::Vector2i coord)
{
	return map[coord.y][coord.x];
}

sf::Vector2i Scene::GetSize()
{
	return sf::Vector2i(map[0].size(), map.size());
}
