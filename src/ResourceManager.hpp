#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include <string>
#include <map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Data.hpp"
#include "Material.hpp"
#include "TileType.hpp"
#include "UnitType.hpp"

#include "TileSprite.hpp"

class ResourceManager {
    private:
        std::map<std::string, TileType> tiletypes;
        std::map<std::string, Material> materials;
        std::map<std::string, UnitType> unittypes;

        sf::Vector2i tile_size;
        sf::Texture shadow_texture;

        std::string base_path;
    public:
        std::map<std::string, sf::Texture> tilesets;
        TileSprite shadow;
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

        const UnitType& GetUnitType(const std::string& id) const;
        const TileType& GetTileType(const std::string& id) const;
        const Material& GetMaterial(const std::string& id) const;

        TileSprite GetSprite(const std::string& tileset, const int& n);

        const sf::Vector2i& GetTileSize() const;
};

#endif
