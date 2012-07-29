#include "ConfigManager.hpp"

#include "utils.hpp"

void ConfigManager::Load() {
    Data d(base_path + "/user/config");
    resolution = make_vector2i(d["display"]["resolution"]);
    max_fps = d["display"]["max_fps"].as_int();

    cursor_color = make_color(d["interface"]["cursor.color"]);

    std::cout << "Successfully loaded user config" << std::endl;
}
