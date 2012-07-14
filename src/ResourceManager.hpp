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
        std::vector<TileSprite> unit_sprites;
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
        void AddTiletype(Data data);
        void AddMaterial(Data data);
        void AddUnit(Data data);

    public:
        ResourceManager();
        ResourceManager(std::string base_path);
        void Load();
        int FindTiletype(std::string id);
        int FindMaterial(std::string id);
        int FindUnit(std::string id);
        const TileSprite& GetTileSprite(const int& tiletype_n) const;
        const TileSprite& GetUnitSprite(const int& unit_n) const;
        const sf::Color& GetMaterialColor(const int& material_n) const;

        const sf::Vector2i& GetTileSize() const;
};

#endif