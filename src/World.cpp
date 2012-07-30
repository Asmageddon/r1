#include "World.hpp"

#include <map>
#include "AString.hpp"

#include "Data.hpp"

#include "Unit.hpp"
#include "Level.hpp"

#include "utils.hpp"

World::World(const ResourceManager *resman, const std::string& base_path) : base_path(base_path), resman(resman) {
    save_path = "";
}

Level* World::AddLevel(const std::string& name, const std::string& type_id) {
    if (contains(maps, name)) return NULL;
    //Previous map should be deleted beforehand

    Level *new_level = new Level(this, name, type_id);
    maps[name] = new_level;

    return GetLevel(name); //Tee-hee
}

void World::DeleteLevel(const std::string& name) {
    if (contains(maps, name)) {
        delete maps[name];
        maps.erase(name);
        //TODO: At the very least check if the player isn't there.
    }
}

Level* World::GetLevel(const std::string& name) {
    if (contains(maps, name)) {
        if (!maps[name]->IsReady()) {
            //TODO: If it's saved in current save file, load it from there, otherwise generate it
            maps[name]->Create();

            maps[name]->Generate();
        }
        return maps[name];
    }
    return maps[AString("nowhere")];
}

void World::Load() {
    //WIP
    Data d(base_path + "/data/world/world");

    std::set<AString> s = d["locations"].GetKeys("");
    std::set<AString>::iterator it;
    for(it = s.begin(); it != s.end(); it++) {
        AString name(*it);
        AString type = d["locations"][name];
        AddLevel(name, type);
        std::cout << " * Added new location: " << name << " (" << type << ")" << std::endl;
    }

    std::cout << "Loaded " << maps.size() << " locations" << std::endl;

    if (!d["gameplay"].HasField("ticks_per_frame.max"))
        ticks_per_frame_max = 10;
    else
        ticks_per_frame_max = d["gameplay"]["ticks_per_frame.max"].as_int();

    if (!d["gameplay"].HasField("ticks_per_frame.min"))
        ticks_per_frame_min = 0;
    else
        ticks_per_frame_min = d["gameplay"]["ticks_per_frame.min"].as_int();

    Level *l = GetLevel("start");

    if (l != NULL) std::cout << "Successfully initialized start location" << std::endl;
    if (player != NULL) std::cout << "Player unit registered correctly" << std::endl;
}

const ResourceManager* World::GetResman() const {
    return resman;
}

void World::Simulate() {
    player->GetCurrentLevel()->Simulate(player);
}

unsigned int World::GetTicksPerFrameMin() const {
    return ticks_per_frame_min;
}
unsigned int World::GetTicksPerFrameMax() const {
    return ticks_per_frame_max;
}
