#include "ResourceManager.hpp"

void ResourceManager::LoadConfiguration(string dir) {
    Data d(base_path + "data/config");

    vector<string> s = split(d["data"]["tile_size"], ' ');
    stringstream( s[0] ) >> tile_size.x;
    stringstream( s[1] ) >> tile_size.y;
}
void ResourceManager::LoadTilesets(string dir) {
    if (dir == "")
        dir = base_path + "data/images/tilesets/";

    vector<string> files = list_dir(dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        string filename = files[i];
        tilesets[filename].loadFromFile(dir + filename);
        cout << " * Loaded tileset: " << filename << endl;
    }
}
void ResourceManager::LoadTiletypes(string dir) {
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

    vector<string> files = list_dir(dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        string filename = files[i];
        Data d(dir + filename);
        this->AddTiletype(d);
    }
}
void ResourceManager::LoadMaterials(string dir) {
    //Create the void tiletype
    Material m;
    materials.push_back(m);
    material_map[m.id] = materials.size() - 1;

    if (dir == "")
        dir = base_path + "data/materials/";

    vector<string> files = list_dir(dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        string filename = files[i];
        Data d(dir + filename);
        this->AddMaterial(d);
    }
}
void ResourceManager::LoadUnits(string dir) {
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

    vector<string> files = list_dir(dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        string filename = files[i];
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
ResourceManager::ResourceManager(string base_path) : base_path(base_path) {}
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

    Texture tex;
    shadow_texture.loadFromFile(base_path + "data/images/shadows.png");
    shadow = ShadowSprite(shadow_texture, tile_size);
    cout << "Shadow sprite loaded" << endl;
}
int ResourceManager::find_tiletype(string id) {
    if (contains(tiletype_map, id))
        return tiletype_map[id];
    return 0;
}
int ResourceManager::find_material(string id) {
    if (contains(material_map, id))
        return material_map[id];
    return 0;
}
int ResourceManager::find_unit(string id) {
    if (contains(unit_map, id))
        return unit_map[id];
    return 0;
}

const TileSprite& ResourceManager::get_tile_sprite(const int& tiletype_n) {
    return tile_sprites[tiletype_n];
}
const TileSprite& ResourceManager::get_unit_sprite(const int& unit_n) {
    return unit_sprites[unit_n];
}

const Color& ResourceManager::get_color(const int& material_n) {
    return materials[material_n].color;
}
