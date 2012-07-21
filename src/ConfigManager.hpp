#ifndef CONFIGMANAGER_HPP_
#define CONFIGMANAGER_HPP

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Data.hpp"

class ConfigManager {
    private:
        std::string base_path;
    public:
        sf::Vector2i resolution;
        int max_fps;
        sf::Color cursor_color;
    public:
        ConfigManager() : base_path(".") {}
        ConfigManager(std::string base_path) : base_path(base_path) {}
        void Load();
};

#endif
