#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <map>
#include <string>

class Unit;
class Level;
class Data;

class ResourceManager;

class World {
    private:
        int seed;
        std::map<std::string, Level*> maps;
        std::string base_path;
        std::string save_path;
        const ResourceManager *resman;
    public:
        Unit *player;
    public:
        World(const ResourceManager *resman, const std::string& base_path);
        Level *AddLevel(const std::string& name, const std::string& type_id);
        void DeleteLevel(const std::string& name);
        Level *GetLevel(const std::string& name);

        //TODO: Loading list of maps and locations from a data file
        //TODO: For multiple modules and/or worlds, allow player to choose one
        void Load();

        const ResourceManager *GetResman() const;

        ////TODO: Saving and loading savegames
        //void LoadSavegame(const std::string& save_path);
        //void SaveSavegame(const std::string& save_path);
        //void IncrementalSave();
};

#endif
