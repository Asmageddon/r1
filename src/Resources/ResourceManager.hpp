#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include "../AString.hpp"
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
        std::map<AString, TileType> tiletypes;
        std::map<AString, Material> materials;
        std::map<AString, UnitType> unittypes;
        std::map<AString, MapType> maptypes;

        sf::Vector2i tile_size;
        sf::Texture shadow_texture;

        AString base_path;
    public:
        std::map<AString, sf::Texture> tilesets;
        TileSprite shadow;
    private:
        void LoadConfiguration(const std::string& dir);
        void LoadTilesets(const std::string& dir);
        void LoadTiletypes(const std::string& dir);
        void LoadMaterials(const std::string& dir);
        void LoadUnitTypes(const std::string& dir);
        void LoadMapTypes(const std::string& dir);
        void AddTileType(Data data);
        void AddMaterial(Data data);
        void AddUnitType(Data data);
        void AddMapType(Data data);

    public:
        ResourceManager();
        ResourceManager(AString base_path);
        void Load();

        const UnitType& GetUnitType(const std::string& id) const;
        const TileType& GetTileType(const std::string& id) const;
        const Material& GetMaterial(const std::string& id) const;
        const MapType& GetMapType(const std::string& id) const;

        const std::map<AString, TileType>& GetTileTypeMap() const;
        const std::map<AString, UnitType>& GetUnitTypeMap() const;
        const std::map<AString, Material>& GetMaterialMap() const;
        const std::map<AString, MapType>& GetMapTypeMap() const;

        TileSprite GetSprite(const std::string& tileset, const int& n);

        const sf::Vector2i& GetTileSize() const;
};

#endif
