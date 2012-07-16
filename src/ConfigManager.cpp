#include "ConfigManager.hpp"

void ConfigManager::Load() {
    Data d(base_path + "/user/config");
    resolution = d.as_Vector2i("display", "resolution");
    max_fps = d.as_int("display", "max_fps");
    std::cout << "Successfully loaded user config" << std::endl;
}