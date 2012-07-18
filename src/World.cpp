#include "World.hpp"

#include <map>
#include <string>

#include "Data.hpp"

#include "Unit.hpp"
#include "Level.hpp"

World::World(const ResourceManager *resman, const std::string& base_path) : base_path(base_path) {
    this->resman = resman;
    save_path = "";
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
    return maps["nowhere"];
}

void World::AddLevel(Data data) {
    Level *l = new Level(resman, data);
    if (!contains(maps, l->id)) {
        maps[l->id] = l;
        std::cout << " * Loaded map: " << l->id << std::endl;
    }
}

void World::LoadMaps(const std::string& module_name) {
    //TODO: Actually care about module_name

    std::string dir = base_path + "data/world/maps/";

    std::vector<std::string> files = list_dir(dir);

    for ( unsigned int i = 0; i < files.size(); i++ ) {
        std::string filename = files[i];
        Data d(dir + filename);
        this->AddLevel(d);
    }
}

void World::Load() {
    LoadMaps("");
    std::cout << "Loaded " << maps.size() << " maps" << std::endl;
}

const ResourceManager* World::GetResman() const {
    return resman;
}
