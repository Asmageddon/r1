#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <map>
#include "AString.hpp"

class Unit;
class Level;
class Data;

class ResourceManager;

class World {
    private:
        int seed;
        std::map<AString, Level*> maps;
        AString base_path;
        AString save_path;
        const ResourceManager *resman;

        unsigned int ticks_per_frame_max;
        unsigned int ticks_per_frame_min;
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

        void Simulate();

        unsigned int GetTicksPerFrameMin() const;
        unsigned int GetTicksPerFrameMax() const;
};

#endif
