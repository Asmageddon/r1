#include "ResourceManager.hpp"

#include <string>
#include <map>
#include <vector>

#include <sstream>

#include <SFML/Graphics.hpp>

#include "Data.hpp"
#include "Material.hpp"
#include "TileType.hpp"
#include "UnitType.hpp"

#include "TileSprite.hpp"
#include "ShadowSprite.hpp"

#include "utils.hpp"

//TODO: Make dir argument const correct
//TODO: Replace data dir with module name

void ResourceManager::LoadConfiguration(std::string dir) {
    Data d(base_path + "data/config");

    std::vector<std::string> s = d.as_str_vector("data", "tile_size");
    std::stringstream( s[0] ) >> tile_size.x;
    std::stringstream( s[1] ) >> tile_size.y;
}
void ResourceManager::LoadTilesets(std::string dir) {
    if (dir == "")
        dir = base_path + "data/images/tilesets/";

    std::vector<std::string> files = list_dir(dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        std::string filename = files[i];
        tilesets[filename].loadFromFile(dir + filename);
        std::cout << " * Loaded tileset: " << filename << std::endl;
    }
}
void ResourceManager::LoadTiletypes(std::string dir) {
    //If no specific path given, use default one
    // (I should probably so it differently but meh)
    if (dir == "")
        dir = base_path + "data/tiletypes/";

    std::vector<std::string> files = list_dir(dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        std::string filename = files[i];
        Data d(dir + filename);
        this->AddTileType(d);
    }
}
void ResourceManager::LoadMaterials(std::string dir) {
    if (dir == "")
        dir = base_path + "data/materials/";

    std::vector<std::string> files = list_dir(dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        std::string filename = files[i];
        Data d(dir + filename);
        this->AddMaterial(d);
    }
}
void ResourceManager::LoadUnits(std::string dir) {
    //If no specific path given, use default one
    // (I should probably so it differently but meh)
    if (dir == "")
        dir = base_path + "data/objects/units/";

    std::vector<std::string> files = list_dir(dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        std::string filename = files[i];
        Data d(dir + filename);
        this->AddUnitType(d);
    }
}
void ResourceManager::AddTileType(Data data) {
    TileType t(this, data);
    if (!contains(tiletypes, t.id)) {
        tiletypes[t.id] = t;
    }
}
void ResourceManager::AddMaterial(Data data) {
    Material m(data);
    if (!contains(materials, m.id)) {
        materials[m.id] = m;
    }
}
void ResourceManager::AddUnitType(Data data) {
    UnitType u(this, data);
    if (!contains(unittypes, u.id)) {
        unittypes[u.id] = u;
    }
}

ResourceManager::ResourceManager() {}
ResourceManager::ResourceManager(std::string base_path) : base_path(base_path) {}
void ResourceManager::Load() {
    LoadConfiguration("");
    std::cout << "Loaded data configuration configuration" << std::endl;

    LoadTilesets("");
    std::cout << "Loaded " << tilesets.size() << " tilesets" << std::endl;

    LoadTiletypes("");
    std::cout << "Loaded " << tiletypes.size() << " tile type definitions" << std::endl;

    LoadMaterials("");
    std::cout << "Loaded " << materials.size() << " material definitions" << std::endl;

    LoadUnits("");
    std::cout << "Loaded " << unittypes.size() << " unit definitions" << std::endl;

    sf::Texture tex;
    shadow_texture.loadFromFile(base_path + "data/images/shadows.png");
    shadow = ShadowSprite(shadow_texture, tile_size);
    std::cout << "Shadow sprite loaded" << std::endl;
}

const UnitType& ResourceManager::GetUnitType(const std::string& id) {
    //FIXME: Make maps directly store types rather than go around
    if (contains(unittypes, id)) {
        return unittypes[id];
    }
    return unittypes["missingno"];
}

const TileType& ResourceManager::GetTileType(const std::string& id) {
    if (contains(tiletypes, id)) {
        return tiletypes[id];
    }
    return tiletypes["void"];
}

const Material& ResourceManager::GetMaterial(const std::string& id) {
    if (contains(materials, id)) {
        return materials[id];
    }
    return materials["void"];
}

TileSprite ResourceManager::GetSprite(const std::string& tileset, const int& n) {
    TileSprite result = TileSprite(tilesets[tileset], tile_size, n);
    return result;
}

const sf::Vector2i& ResourceManager::GetTileSize() const {
    return this->tile_size;
}