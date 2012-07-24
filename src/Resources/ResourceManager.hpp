#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include <string>
#include <map>

#include <SFML/Graphics.hpp>

#include "../Data.hpp"

#include "Material.hpp"
#include "TileType.hpp"
#include "UnitType.hpp"
#include "MapType.hpp"

#include "../TileSprite.hpp"

class ResourceManager {
    private:
        std::map<std::string, TileType> tiletypes;
        std::map<std::string, Material> materials;
        std::map<std::string, UnitType> unittypes;
        std::map<std::string, MapType> maptypes;

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
        void LoadUnitTypes(std::string dir);
        void LoadMapTypes(std::string dir);
        void AddTileType(Data data);
        void AddMaterial(Data data);
        void AddUnitType(Data data);
        void AddMapType(Data data);

    public:
        ResourceManager();
        ResourceManager(std::string base_path);
        void Load();

        const UnitType& GetUnitType(const std::string& id) const;
        const TileType& GetTileType(const std::string& id) const;
        const Material& GetMaterial(const std::string& id) const;
        const MapType& GetMapType(const std::string& id) const;

        const std::map<std::string, TileType>& GetTileTypeMap() const;
        const std::map<std::string, UnitType>& GetUnitTypeMap() const;
        const std::map<std::string, Material>& GetMaterialMap() const;
        const std::map<std::string, MapType>& GetMapTypeMap() const;

        TileSprite GetSprite(const std::string& tileset, const int& n);

        const sf::Vector2i& GetTileSize() const;
};

#endif
