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
        ResourceManager *resman;
    public:
        Unit *player;
    private:
        void AddLevel(Data data);
        void LoadMaps(const std::string& module_name);
    public:
        World(ResourceManager *resman, const std::string& base_path);
        Level *GetLevel(const std::string& name);

        void Load();

        ////TODO: Saving and loading savegames
        //void LoadSavegame(const std::string& save_path);
        //void SaveSavegame(const std::string& save_path);
        //void IncrementalSave();
};

#endif
