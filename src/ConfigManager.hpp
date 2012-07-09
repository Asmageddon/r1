#ifndef CONFIGMANAGER_HPP_
#define CONFIGMANAGER_HPP

#include <iostream>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include "Data.hpp"

class ConfigManager {
    private:
        string base_path;
    public:
        Vector2i resolution;
        int max_fps;
    public:
        ConfigManager() : base_path(".") {}
        ConfigManager(string base_path) : base_path(base_path) {}
        void Load();
};

#endif