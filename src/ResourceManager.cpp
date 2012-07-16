#include "ResourceManager.hpp"

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
    //Create the void tiletype
    TileType t;
    tiletypes.push_back(t);
    tiletype_map[t.id] = tiletypes.size() - 1;
    TileSprite s(tilesets[t.tileset], tile_size, t.image);
    tile_sprites.push_back(s);

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
    //Create the void tiletype
    Material m;
    materials.push_back(m);
    material_map[m.id] = materials.size() - 1;

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
    //Create the MISSINGNO unit
    UnitType u(this);
    units.push_back(u);
    unit_map[u.id] = units.size() - 1;

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
    TileType t(data);
    if (!contains(tiletype_map, t.id)) {
        tiletypes.push_back(t);
        tiletype_map[t.id] = tiletypes.size() - 1;

        TileSprite s(tilesets[t.tileset], tile_size, t.image);

        tile_sprites.push_back(s);
    }
}
void ResourceManager::AddMaterial(Data data) {
    Material m(data);
    if (!contains(material_map, m.id)) {
        materials.push_back(m);
        material_map[m.id] = materials.size() - 1;
    }
}
void ResourceManager::AddUnitType(Data data) {
    UnitType u(this, data);
    if (!contains(unit_map, u.id)) {
        units.push_back(u);
        unit_map[u.id] = units.size() - 1;
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
    std::cout << "Loaded " << units.size() << " unit definitions" << std::endl;

    sf::Texture tex;
    shadow_texture.loadFromFile(base_path + "data/images/shadows.png");
    shadow = ShadowSprite(shadow_texture, tile_size);
    std::cout << "Shadow sprite loaded" << std::endl;
}

int ResourceManager::FindTiletype(std::string id) {
    if (contains(tiletype_map, id))
        return tiletype_map[id];
    return 0;
}
const TileSprite& ResourceManager::GetTileSprite(const int& tiletype_n) const {
    return tile_sprites[tiletype_n];
}

const UnitType& ResourceManager::GetUnitType(const string& id) {
    //FIXME: Make maps directly store types rather than go around
    if (contains(unit_map, id)) {
        return units[unit_map[id]];
    }
    return units[0];
}

const TileType& ResourceManager::GetTileType(const string& id) {
    if (contains(tiletype_map, id)) {
        return tiletypes[tiletype_map[id]];
    }
    return tiletypes[0];
}

const Material& ResourceManager::GetMaterial(const string& id) {
    if (contains(material_map, id)) {
        return materials[material_map[id]];
    }
    return materials[0];
}

TileSprite ResourceManager::GetSprite(const string& tileset, const int& n) {
    TileSprite result = TileSprite(tilesets[tileset], tile_size, n);
    return result;
}

const sf::Vector2i& ResourceManager::GetTileSize() const {
    return this->tile_size;
}