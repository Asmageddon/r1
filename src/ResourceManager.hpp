#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

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

class ResourceManager {
    private:
        std::map<std::string, int> tiletype_map;
        std::map<std::string, int> material_map;
        std::map<std::string, int> unit_map;

        sf::Vector2i tile_size;
        std::vector<TileSprite> tile_sprites;
        sf::Texture shadow_texture;

        std::string base_path;
    public:
        std::map<std::string, sf::Texture> tilesets;
        std::vector<TileType> tiletypes;
        std::vector<Material> materials;
        std::vector<UnitType> units;
        ShadowSprite shadow;
    private:
        void LoadConfiguration(std::string dir);
        void LoadTilesets(std::string dir);
        void LoadTiletypes(std::string dir);
        void LoadMaterials(std::string dir);
        void LoadUnits(std::string dir);
        void AddTileType(Data data);
        void AddMaterial(Data data);
        void AddUnitType(Data data);

    public:
        ResourceManager();
        ResourceManager(std::string base_path);
        void Load();
        //WIP: Get rid of these, Unit/Tile should hold references to their Material and UnitType/TileType
        //As for now, I leave tiles as they are to have less to rollback if it turns out to be a bad idea
        int FindTiletype(std::string id);
        const TileSprite& GetTileSprite(const int& tiletype_n) const;

        const UnitType& GetUnitType(const string& id);
        const TileType& GetTileType(const string& id);
        const Material& GetMaterial(const string& id);

        TileSprite GetSprite(const string& tileset, const int& n);

        const sf::Vector2i& GetTileSize() const;
};

#endif