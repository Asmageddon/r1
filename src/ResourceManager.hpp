#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include <string>
#include <map>
#include <vector>

#include <sstream>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include "Data.hpp"
#include "Material.hpp"
#include "TileType.hpp"
#include "UnitType.hpp"

#include "TileSprite.hpp"
#include "ShadowSprite.hpp"

#include "utils.hpp"

class ResourceManager {
    private:
        map<string, int> tiletype_map;
        map<string, int> material_map;
        map<string, int> unit_map;

        Vector2i tile_size;
        vector<TileSprite> tile_sprites;
        vector<TileSprite> unit_sprites;
        Texture shadow_texture;

        string base_path;
    public:
        map<string, Texture> tilesets;
        vector<TileType> tiletypes;
        vector<Material> materials;
        vector<UnitType> units;
        ShadowSprite shadow;
    private:
        void LoadConfiguration(string dir);
        void LoadTilesets(string dir);
        void LoadTiletypes(string dir);
        void LoadMaterials(string dir);
        void LoadUnits(string dir);
        void AddTiletype(Data data);
        void AddMaterial(Data data);
        void AddUnit(Data data);

    public:
        ResourceManager();
        ResourceManager(string base_path);
        void Load();
        int find_tiletype(string id);
        int find_material(string id);
        int find_unit(string id);
        const TileSprite& get_tile_sprite(const int& tiletype_n);
        const TileSprite& get_unit_sprite(const int& unit_n);
        const Color& get_color(const int& material_n);
};

#endif