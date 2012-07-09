#include <iostream>
#include <fstream>

#include <string>

#include <sstream>

#include <vector>
#include <map>

#include <cstring>
#include <iostream>
#include <stdint.h>

using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include "Data.hpp"
#include "ConfigManager.hpp"
#include "Object.hpp"
#include "Displayable.hpp"
#include "TileSprite.hpp"
#include "TileType.hpp"
#include "Material.hpp"
#include "UnitType.hpp"
#include "ResourceManager.hpp"
#include "utils.hpp"

static string base_path = ".";

string get_base_path(char* argv0) {
    unsigned int last_slash = 0;
    for (unsigned int i=0; i < strlen(argv0); i++) {
        if (argv0[i] == '/')
            last_slash = i;
    }
    argv0[last_slash+1] = '\0'; //lolololol
    return argv0;
}

class Unit {
    public:
        Level* world;
        u_int16_t type;
        u_int16_t material;
        u_int16_t integrity;
        u_int16_t temperature;

        u_int32_t hp;

        Vector2i pos;

        //vector<Object> contents;

    public:
        Unit(const UnitType& prototype) { }
        Unit() {
            type = 0;
            material = 0;
            integrity = 0;
            temperature = 0;

            hp = 1;
            pos = Vector2i(0,0);

            world = NULL;
        }

        //void Move(const Vector2i& vec) {
            //if (world == NULL) return;

            //Vector2i new_pos = pos + vec;

            //if (!world->in_bounds(new_pos)) return;

            //Unit* tmp = NULL;
            //Vector2i old_pos = pos;
            //if (world->data[new_pos.x + width * new_pos.y].unit != NULL) {
                //tmp = world->data[new_pos.x + width * new_pos.y].unit;
                //tmp->pos = pos;
            //}
            //pos = new_pos;
            //world->data[new_pos.x + width * new_pos.y].unit = unit;
            //world->units[unit] = new_pos;

            //world->data[old_pos.x + width * old_pos.y].unit = NULL;
            //if (tmp != NULL) {
                //world->data[old_pos.x + width * old_pos.y].unit = tmp;
                //world->units[tmp] = old_pos;
            //}
        //}
};

class Tile {
    public:
        u_int16_t type;
        u_int16_t material;
        u_int8_t integrity;
        u_int8_t temperature;

        Unit *unit;
    public:
        Tile() {
            type = 0;
            material = 0;
            integrity = 0;
            temperature = 0;
            unit = NULL;
        }
};

class Level {
    private:
        ResourceManager* resman;
        Tile *data;
        Tile default_tile;
    public:
        int width, height;
        map<Unit*, Vector2i> units;
        Unit *player;
    public:
        Level(ResourceManager* resman, int width, int height) {
            this->resman = resman;
            data = new Tile[width * height];
            this->width = width;
            this->height = height;
            Generate();
            default_tile = Tile();
        }
        bool in_bounds(const Vector2i& pos) {
            if      (pos.x < 0) return false;
            else if (pos.x >= width) return false;
            else if (pos.y < 0) return false;
            else if (pos.y >= height) return false;
            return true;
        }
        const Tile& get_tile(const Vector2i& pos) {
            if (!in_bounds(pos))
                return default_tile;
            return data[pos.x + width * pos.y];
        }
        const Tile& get_tile(const int& x, const int& y) {
            return get_tile(Vector2i(x, y));
        }
        void set_tile(const Vector2i& pos, const Tile& new_tile) {
            data[pos.x + width * pos.y] = new_tile;
        }

        bool is_wall(const Vector2i& pos) {
            const Tile& t = get_tile(pos);
            return (t.type == TILE_WALL);
        }
        bool is_wall(const int& x, const int& y) { return is_wall(Vector2i(x, y));  }

        bool is_floor(const Vector2i& pos) {
            const Tile& t = get_tile(pos);
            return (t.type == TILE_FLOOR);
        }
        bool is_floor(const int& x, const int& y) { return is_floor(Vector2i(x, y)); }

        void set_default_tile(const Tile& tile) {
            default_tile = tile;
        }
        void Generate() {
            for(int x=0; x < width; x++)
            for(int y=0; y < height; y++) {
                data[x + width*y].type = 2;
                data[x + width*y].material = 1;
                data[x + width*y].integrity = 255;
                data[x + width*y].temperature = 127;
                if (x == y)
                    data[x + width*y].type = 1;
                if ((x * y) == 0)
                    data[x + width*y].type = 1;
                if (x == width - 1)
                    data[x + width*y].type = 1;
                if (y == height - 1)
                    data[x + width*y].type = 1;
                if (y == x+1)
                    data[x + width*y].type = 1;
                if (y == x-1)
                    data[x + width*y].type = 1;

                Unit *u = new Unit();
                u->type = 1;
                placeUnit(Vector2i(12,5), u);
                player = u;

                u = new Unit();
                u->type = 2;
                u->material = 1;
                placeUnit(Vector2i(12,3), u);

            }
        }
        void placeUnit(const Vector2i& pos, Unit* unit) {
            if (data[pos.x + width * pos.y].unit != NULL) {
                units.erase(data[pos.x + width * pos.y].unit);
                delete data[pos.x + width * pos.y].unit;
            }
            data[pos.x + width * pos.y].unit = unit;
            units[unit] = pos;
            unit->pos = pos;
            unit->world = this;
        }
        void moveUnit(Unit* unit, const Vector2i& vec) {
            unit->Move(vec);
            Vector2i new_pos = unit->pos + vec;

            if (!in_bounds(new_pos)) return;

            Unit* tmp = NULL;
            Vector2i old_pos = unit->pos;
            if (data[new_pos.x + width * new_pos.y].unit != NULL) {
                tmp = data[new_pos.x + width * new_pos.y].unit;
                tmp->pos = unit->pos;
            }
            unit->pos = new_pos;
            data[new_pos.x + width * new_pos.y].unit = unit;
            units[unit] = new_pos;

            data[old_pos.x + width * old_pos.y].unit = NULL;
            if (tmp != NULL) {
                data[old_pos.x + width * old_pos.y].unit = tmp;
                units[tmp] = old_pos;
            }
        }

        void print_state() {
            map<Unit*, Vector2i>::iterator iter;

            for (iter = units.begin(); iter != units.end(); ++iter) {
                cout << iter->first << " => " << iter->second << endl;
            }
        }
};

class Game {
    private:
        RenderWindow window;
        Level* level;
        ResourceManager resman;
        ConfigManager confman;

        bool view_changed;

    private:

        void start() {
            confman = ConfigManager(base_path);
            confman.Load();

            window.create(VideoMode(confman.resolution.x, confman.resolution.y), "r1");
            window.setFramerateLimit(confman.max_fps);

            resman = ResourceManager(base_path);
            resman.Load();
            level = new Level(&resman, 32,32);

            view_changed = true;
        }

        void end() {

        }

        void render_shadow(const int& x, const int& y, ShadowSprite* shadow) {
            shadow->setPosition(Vector2f(x*16.0f, y*16.0f));
                if (level->is_floor(x, y)) {
                    if (level->is_wall(x - 1, y)) {
                        if (level->is_wall(x, y - 1)) {
                            if (level->is_wall(x, y + 1)) {
                                if (level->is_wall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_ALL);
                                    window.draw(*shadow);
                                }
                                else {
                                    shadow->setSprite(SHADOW_ALL_BUT_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                            else {
                                if (level->is_wall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_ALL_BUT_BOTTOM);
                                    window.draw(*shadow);
                                }
                                else {
                                    shadow->setSprite(SHADOW_TOP_LEFT);
                                    window.draw(*shadow);
                                    if (level->is_wall(x + 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                        }
                        else {
                            if (level->is_wall(x, y + 1)) {
                                if (level->is_wall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_ALL_BUT_TOP);
                                    window.draw(*shadow);
                                }
                                else {
                                    shadow->setSprite(SHADOW_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                    if (level->is_wall(x + 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                            else {
                                if (level->is_wall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_SIDES_VERT);
                                    window.draw(*shadow);
                                }
                                else {
                                    shadow->setSprite(SHADOW_LEFT);
                                    window.draw(*shadow);
                                    if (level->is_wall(x + 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                        window.draw(*shadow);
                                    }
                                    if (level->is_wall(x + 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                        }
                    }
                    else {
                        if (level->is_wall(x, y - 1)) {
                            if (level->is_wall(x, y + 1)) {
                                if (level->is_wall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_ALL_BUT_LEFT);
                                    window.draw(*shadow);
                                }
                                else {
                                    shadow->setSprite(SHADOW_RIGHT);
                                    window.draw(*shadow);
                                    if (level->is_wall(x - 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                        window.draw(*shadow);
                                    }
                                    if (level->is_wall(x - 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                            else {
                                if (level->is_wall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_TOP_RIGHT);
                                    window.draw(*shadow);
                                    if (level->is_wall(x - 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                        window.draw(*shadow);
                                    }
                                }
                                else {
                                    shadow->setSprite(SHADOW_TOP);
                                    window.draw(*shadow);
                                    if (level->is_wall(x - 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                        window.draw(*shadow);
                                    }
                                    if (level->is_wall(x + 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                        }
                        else {
                            if (level->is_wall(x, y + 1)) {
                                if (level->is_wall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                    if (level->is_wall(x - 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                        window.draw(*shadow);
                                    }
                                }
                                else {
                                    shadow->setSprite(SHADOW_BOTTOM);
                                    window.draw(*shadow);
                                    if (level->is_wall(x - 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                        window.draw(*shadow);
                                    }
                                    if (level->is_wall(x + 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                            else {
                                if (level->is_wall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_RIGHT);
                                    window.draw(*shadow);
                                    if (level->is_wall(x - 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                        window.draw(*shadow);
                                    }
                                    if (level->is_wall(x - 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                        window.draw(*shadow);
                                    }
                                }
                                else {
                                    if (level->is_wall(x - 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                        window.draw(*shadow);
                                    }
                                    if (level->is_wall(x + 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                        window.draw(*shadow);
                                    }
                                    if (level->is_wall(x - 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                        window.draw(*shadow);
                                    }
                                    if (level->is_wall(x + 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                        }
                    }
                } // is_floor(x, y)
        }

        void render() {
            if (view_changed) {
                window.clear();
                //window.draw(text);

                int current_tiletype = -1;
                TileSprite current_sprite;

                for (int x=0; x < level->width ; x++)
                for (int y=0; y < level->height; y++) {
                    const Tile& tile = level->get_tile(x, y);
                    if (current_tiletype != tile.type) {
                        current_tiletype = tile.type;
                        current_sprite = resman.get_tile_sprite(current_tiletype);
                    }
                    Color color = resman.get_color(tile.material);
                    current_sprite.setColor(color);
                    current_sprite.setPosition(Vector2f(x*16.0f, y*16.0f));
                    window.draw(current_sprite);

                    render_shadow(x, y, &resman.shadow);

                    if (tile.unit != NULL) {
                        TileSprite unit_sprite = resman.get_unit_sprite(tile.unit->type);
                        Color color = resman.get_color(tile.unit->material);
                        unit_sprite.setColor(color);
                        unit_sprite.setPosition(Vector2f(x*16.0f, y*16.0f));
                        window.draw(unit_sprite);
                    }

                }

                view_changed = false;
            }

            window.display();
        }

        void handle_input() {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::F12) {
                        string dir = base_path + "user/screenshots/";
                        int n = list_dir(dir).size() + 1;
                        stringstream out;
                        out << n;
                        string fname = out.str();
                        fname.insert(fname.begin(), 5 - fname.size(), '0');
                        fname = (string)"screenshot" + fname + ".png";
                        Image Screen = window.capture();
                        Screen.saveToFile(dir + fname);

                        cout << "Saved screenshot to user/screenshots/" + fname << endl;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad7) {
                        level->moveUnit(level->player, Vector2i(-1, -1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad8) {
                        level->moveUnit(level->player, Vector2i(0, -1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad9) {
                        level->moveUnit(level->player, Vector2i(1, -1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad4) {
                        level->moveUnit(level->player, Vector2i(-1, 0));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad5) {
                        level->moveUnit(level->player, Vector2i(0, 0));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad6) {
                        level->moveUnit(level->player, Vector2i(1, 0));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad1) {
                        level->moveUnit(level->player, Vector2i(-1, 1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad2) {
                        level->moveUnit(level->player, Vector2i(0, 1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad3) {
                        level->moveUnit(level->player, Vector2i(1, 1));
                        view_changed = true;
                    }
                }
            }
        }
    public:
        Game() { }
        void run() {
            start();
            while (window.isOpen()) {
                handle_input();
                render();
            }
            end();
        }
};

int main(int argc, char** argv) {
    base_path = get_base_path(argv[0]);
    cout << base_path << endl;
    Game g;
    g.run();
    return 0;
}