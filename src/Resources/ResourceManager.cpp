#include "ResourceManager.hpp"

#include "../AString.hpp"
#include <map>
#include <vector>

#include <sstream>

#include <SFML/Graphics.hpp>

#include "../Data.hpp"
#include "Material.hpp"
#include "TileType.hpp"
#include "UnitType.hpp"
#include "MapType.hpp"

#include "../TileSprite.hpp"

#include "../utils.hpp"

//TODO: Replace data dir with module name and/or subdir name

void ResourceManager::LoadConfiguration(const std::string& dir) {
    Data d(base_path + "data/config");

    std::vector<int> s = d["data"]["tile_size"].as_int_vector();
    tile_size.x = s[0];
    tile_size.y = s[1];
}


void ResourceManager::LoadTilesets(const std::string& dir) {
    AString _dir = dir;
    if (_dir == "")
        _dir = base_path + "data/images/tilesets/";

    std::vector<AString> files = list_dir(_dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        AString filename = files[i];
        tilesets[filename].loadFromFile(_dir + filename);
        std::cout << " * Loaded tileset: " << filename << std::endl;
    }
}

void ResourceManager::LoadTiletypes(const std::string& dir) {
    //If no specific path given, use default one
    // (I should probably so it differently but meh)
    AString _dir = dir;
    if (_dir == "")
        _dir = base_path + "data/tiletypes/";

    std::vector<AString> files = list_dir(_dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        AString filename = files[i];
        Data d(_dir + filename);
        this->AddTileType(d);
    }
}
void ResourceManager::LoadMaterials(const std::string& dir) {
    AString _dir = dir;
    if (_dir == "")
        _dir = base_path + "data/materials/";

    std::vector<AString> files = list_dir(_dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        AString filename = files[i];
        Data d(_dir + filename);
        this->AddMaterial(d);
    }
}
void ResourceManager::LoadUnitTypes(const std::string& dir) {
    //If no specific path given, use default one
    // (I should probably so it differently but meh)
    AString _dir = dir;
    if (_dir == "")
        _dir = base_path + "data/objects/units/";

    std::vector<AString> files = list_dir(_dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        AString filename = files[i];
        Data d(_dir + filename);
        this->AddUnitType(d);
    }
}
void ResourceManager::LoadMapTypes(const std::string& dir) {
    //If no specific path given, use default one
    // (I should probably so it differently but meh)
    AString _dir = dir;
    if (_dir == "")
        _dir = base_path + "data/world/maps/";

    std::vector<AString> files = list_dir(_dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        AString filename = files[i];
        Data d(_dir + filename);
        this->AddMapType(d);
    }
}


void ResourceManager::AddTileType(Data data) {
    TileType t(this, data);
    if (!contains(tiletypes, t.id)) {
        tiletypes[t.id] = t;
        std::cout << " * Loaded tiletype: " << t.id << " " << std::endl;
    }
    else {
        std::cout << "ERROR: Redefinition of tile type: " << t.id << std::endl;
    }
}
void ResourceManager::AddMaterial(Data data) {
    Material m(data);
    if (!contains(materials, m.id)) {
        materials[m.id] = m;
        std::cout << " * Loaded material: " << m.id << std::endl;
    }
    else {
        std::cout << "ERROR: Redefinition of material: " << m.id << std::endl;
    }
}
void ResourceManager::AddUnitType(Data data) {
    UnitType u(this, data);
    if (!contains(unittypes, u.id)) {
        unittypes[u.id] = u;
        std::cout << " * Loaded unit type: " << u.id << std::endl;
    }
    else {
        std::cout << "ERROR: Redefinition of unit type: " << u.id << std::endl;
    }
}
void ResourceManager::AddMapType(Data data) {
    MapType m(this, data);
    if (!contains(maptypes, m.id)) {
        maptypes[m.id] = m;
        std::cout << " * Loaded map type: " << m.id << std::endl;
    }
    else {
        std::cout << "ERROR: Redefinition of map type: " << m.id << std::endl;
    }
}

ResourceManager::ResourceManager() {}
ResourceManager::ResourceManager(AString base_path) : base_path(base_path) {}
void ResourceManager::Load() {
    LoadConfiguration("");
    std::cout << "Loaded data configuration" << std::endl;

    LoadTilesets("");
    std::cout << "Loaded " << tilesets.size() << " tilesets" << std::endl;

    LoadTiletypes("");
    std::cout << "Loaded " << tiletypes.size() << " tile type definitions" << std::endl;

    LoadMaterials("");
    std::cout << "Loaded " << materials.size() << " material definitions" << std::endl;

    LoadUnitTypes("");
    std::cout << "Loaded " << unittypes.size() << " unit definitions" << std::endl;

    LoadMapTypes("");
    std::cout << "Loaded " << maptypes.size() << " map definitions" << std::endl;

    sf::Texture tex;
    shadow_texture.loadFromFile(base_path + "data/images/shadows.png");
    shadow = TileSprite(shadow_texture, tile_size);
    std::cout << "Shadow sprite loaded" << std::endl;
}

const UnitType& ResourceManager::GetUnitType(const std::string& id) const {
    if (contains(unittypes, id)) {
        return const_access(unittypes, id);
    }
    return const_access(unittypes, "missingno");
}

const TileType& ResourceManager::GetTileType(const std::string& id) const {
    if (contains(tiletypes, id)) {
        return const_access(tiletypes, id);
    }
    return const_access(tiletypes, "void");
}

const Material& ResourceManager::GetMaterial(const std::string& id) const {
    if (contains(materials, id)) {
        return const_access(materials, id);
    }
    return const_access(materials, "void");
}

const MapType& ResourceManager::GetMapType(const std::string& id) const {
    if (contains(maptypes, id)) {
        return const_access(maptypes, id);
    }
    return const_access(maptypes, "nowhere");
}

const std::map<AString, TileType>& ResourceManager::GetTileTypeMap() const {
    return tiletypes;
}
const std::map<AString, UnitType>& ResourceManager::GetUnitTypeMap() const {
    return unittypes;
}
const std::map<AString, Material>& ResourceManager::GetMaterialMap() const {
    return materials;
}
const std::map<AString, MapType>& ResourceManager::GetMapTypeMap() const {
    return maptypes;
}

TileSprite ResourceManager::GetSprite(const std::string& tileset, const int& n) {
    TileSprite result = TileSprite(tilesets[tileset], tile_size, n);
    return result;
}

const sf::Vector2i& ResourceManager::GetTileSize() const {
    return this->tile_size;
}
