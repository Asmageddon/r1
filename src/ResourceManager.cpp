#include "ResourceManager.hpp"

void ResourceManager::LoadConfiguration(std::string dir) {
    Data d(base_path + "data/config");

    std::vector<std::string> s = split(d["data"]["tile_size"], ' ');
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
        cout << " * Loaded tileset: " << filename << endl;
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
        this->AddTiletype(d);
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
    UnitType u;
    units.push_back(u);
    unit_map[u.id] = units.size() - 1;
    TileSprite s(tilesets[u.tileset], tile_size, u.image);
    unit_sprites.push_back(s);

    //If no specific path given, use default one
    // (I should probably so it differently but meh)
    if (dir == "")
        dir = base_path + "data/objects/units/";

    std::vector<std::string> files = list_dir(dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        std::string filename = files[i];
        Data d(dir + filename);
        this->AddUnit(d);
    }
}
void ResourceManager::AddTiletype(Data data) {
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
void ResourceManager::AddUnit(Data data) {
    UnitType u(data);
    if (!contains(unit_map, u.id)) {
        units.push_back(u);
        unit_map[u.id] = units.size() - 1;

        TileSprite s(tilesets[u.tileset], tile_size, u.image);

        unit_sprites.push_back(s);
    }
}

ResourceManager::ResourceManager() {}
ResourceManager::ResourceManager(std::string base_path) : base_path(base_path) {}
void ResourceManager::Load() {
    LoadConfiguration("");
    cout << "Loaded data configuration configuration" << endl;

    LoadTilesets("");
    cout << "Loaded " << tilesets.size() << " tilesets" << endl;

    LoadTiletypes("");
    cout << "Loaded " << tiletypes.size() << " tile type definitions" << endl;

    LoadMaterials("");
    cout << "Loaded " << materials.size() << " material definitions" << endl;

    LoadUnits("");
    cout << "Loaded " << units.size() << " unit definitions" << endl;

    sf::Texture tex;
    shadow_texture.loadFromFile(base_path + "data/images/shadows.png");
    shadow = ShadowSprite(shadow_texture, tile_size);
    cout << "Shadow sprite loaded" << endl;
}
int ResourceManager::FindTiletype(std::string id) {
    if (contains(tiletype_map, id))
        return tiletype_map[id];
    return 0;
}
int ResourceManager::FindMaterial(std::string id) {
    if (contains(material_map, id))
        return material_map[id];
    return 0;
}
int ResourceManager::FindUnit(std::string id) {
    if (contains(unit_map, id))
        return unit_map[id];
    return 0;
}

const TileSprite& ResourceManager::GetTileSprite(const int& tiletype_n) const {
    return tile_sprites[tiletype_n];
}
const TileSprite& ResourceManager::GetUnitSprite(const int& unit_n) const {
    return unit_sprites[unit_n];
}

const sf::Color& ResourceManager::GetMaterialColor(const int& material_n) const {
    return materials[material_n].color;
}

const sf::Vector2i& ResourceManager::GetTileSize() const {
    return this->tile_size;
}