#include "World.hpp"

#include <map>
#include "AString.hpp"

#include "Data.hpp"

#include "Unit.hpp"
#include "Level.hpp"

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
    //TODO
}

const ResourceManager* World::GetResman() const {
    return resman;
}
