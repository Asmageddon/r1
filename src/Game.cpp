#include <iostream>

#include <string>

#include <vector>
#include <map>

#include <cstring>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "ConfigManager.hpp"
#include "TileType.hpp"
#include "Material.hpp"
#include "UnitType.hpp"
#include "ResourceManager.hpp"
#include "utils.hpp"

#include <cmath>

#define PI 3.14159265

static std::string base_path = ".";

std::string get_base_path(char* argv0) {
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
        u_int16_t type;
        u_int16_t material;
        u_int16_t integrity;
        u_int16_t temperature;

        u_int32_t hp;

        sf::Vector2i pos;

        //std::vector<Object> contents;

    public:
        Unit(const UnitType& prototype) { }
        Unit() {
            type = 0;
            material = 0;
            integrity = 0;
            temperature = 0;

            hp = 1;
            pos = sf::Vector2i(0,0);

        }

        //void Move(const sf::Vector2i& vec) {
            //if (world == NULL) return;

            //sf::Vector2i new_pos = pos + vec;

            //if (!world->InBounds(new_pos)) return;

            //Unit* tmp = NULL;
            //sf::Vector2i old_pos = pos;
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

        bool known;

        Unit *unit;
    public:
        Tile() {
            type = 0;
            material = 0;
            integrity = 0;
            temperature = 0;
            unit = NULL;

            known = false;
        }
};

class Level {
    private:
        ResourceManager* resman;
        Tile *data;
        Tile default_tile;
    public:
        int width, height;
        std::map<Unit*, sf::Vector2i> units;
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
        bool InBounds(const sf::Vector2i& pos) const {
            if      (pos.x < 0) return false;
            else if (pos.x >= width) return false;
            else if (pos.y < 0) return false;
            else if (pos.y >= height) return false;
            return true;
        }

        void SetKnown(const sf::Vector2i& pos, const bool& state) {
            if (InBounds(pos))
                data[pos.x + width * pos.y].known = state;
        }

        const Tile& GetTile(const sf::Vector2i& pos) const {
            if (!InBounds(pos))
                return default_tile;
            return data[pos.x + width * pos.y];
        }
        const Tile& GetTile(const int& x, const int& y) const {
            return GetTile(sf::Vector2i(x, y));
        }
        void SetTile(const sf::Vector2i& pos, const Tile& new_tile) {
            data[pos.x + width * pos.y] = new_tile;
        }

        bool IsWall(const sf::Vector2i& pos) const {
            const Tile& t = GetTile(pos);
            return (t.type == TILE_WALL);
        }
        bool IsWall(const int& x, const int& y) const { return IsWall(sf::Vector2i(x, y));  }

        bool IsFloor(const sf::Vector2i& pos) const {
            const Tile& t = GetTile(pos);
            return (t.type == TILE_FLOOR);
        }
        bool IsFloor(const int& x, const int& y) const { return IsFloor(sf::Vector2i(x, y)); }

        bool IsKnown(const sf::Vector2i& pos) const {
            const Tile& t = GetTile(pos);
            return t.known;
        }

        void SetDefaultTile(const Tile& tile) {
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
                PlaceUnit(sf::Vector2i(12,5), u);
                player = u;

                u = new Unit();
                u->type = 2;
                u->material = 1;
                PlaceUnit(sf::Vector2i(12,3), u);

            }
        }
        void PlaceUnit(const sf::Vector2i& pos, Unit* unit) {
            if (data[pos.x + width * pos.y].unit != NULL) {
                units.erase(data[pos.x + width * pos.y].unit);
                delete data[pos.x + width * pos.y].unit;
            }
            data[pos.x + width * pos.y].unit = unit;
            units[unit] = pos;
            unit->pos = pos;
        }
        void MoveUnit(Unit* unit, const sf::Vector2i& vec) {
            sf::Vector2i new_pos = unit->pos + vec;

            if (!InBounds(new_pos)) return;

            Unit* tmp = NULL;
            sf::Vector2i old_pos = unit->pos;
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

        void PrintState() {
            std::map<Unit*, sf::Vector2i>::iterator iter;

            for (iter = units.begin(); iter != units.end(); ++iter) {
                std::cout << iter->first << " => " << iter->second << std::endl;
            }
        }
};

class FieldOfView {
    private:
        bool *visibility;
        short radius;
        short width;
        Vector2i origin;
    public:
        FieldOfView() {
            radius = 1;
            visibility = new bool[3*3];
            origin = Vector2i(1,1);
        }
        void SetRadius(const short& radius) {
            this->radius = radius;
            this->width = 1 + radius * 2;
            this->origin = Vector2i(radius, radius);
            delete[] visibility;
            visibility = new bool[width * width];
        }
        const short& GetRadius() const {
            return this->radius;
        }
        void Calculate(const Level* level, const Vector2i& caster_pos) {
            /*
             * Simple raycasting
             */
            for(int x = 0; x < width; x++)
            for(int y = 0; y < width; y++) {
                visibility[x + width * y] = false;
            }
            visibility[origin.x + width * origin.y] = true;
            //int rays = PI * 2 * radius * 2;
            int rays = 360;
            for(int a = 0; a < rays; a++) {
                float ax = sin(a * PI * 2 / rays);
                float ay = cos(a * PI * 2 / rays);
                float x = origin.x;
                float y = origin.y;
                for(int z = 0; z < this->radius; z++) {
                    x += ax;
                    y += ay;
                    Vector2i pos = Vector2i(
                        (int)round(x),
                        (int)round(y)
                    );
                    if (z == radius) {
                        if (sqrt((pos.x - origin.x) * (pos.x - origin.x) + (pos.y - origin.y) * (pos.y - origin.y)) > radius) {
                            break;
                        }
                    }

                    visibility[pos.x + width * pos.y] = true;

                    Vector2i map_pos = pos + caster_pos - origin;

                    if (level->IsWall(map_pos))
                        break;
                }
            }
        }
        void CalculateAndMark(Level* level, const Vector2i& caster_pos) {
            Calculate(level, caster_pos);
            for(int x = 0; x < width; x++)
            for(int y = 0; y < width; y++) {
                int mx = x + caster_pos.x - origin.x;
                int my = y + caster_pos.y - origin.y;

                if (visibility[x + width * y])
                    level->SetKnown(Vector2i(mx, my), true);
            }
        }
        inline bool IsVisible(const Vector2i& pos) const {
            if (pos.x < -radius) return false;
            if (pos.x > radius) return false;
            if (pos.y < -radius) return false;
            if (pos.y > radius) return false;

            Vector2i _pos = pos + origin;
            return visibility[_pos.x + width * _pos.y];
        }
        inline bool IsVisible(const int& x, const int& y) const {
            return IsVisible(Vector2i(x, y));
        }
};

class Game {
    private:
        sf::RenderWindow window;
        Level* level;
        ResourceManager resman;
        ConfigManager confman;

        bool view_changed;

        FieldOfView *fov;

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

            fov = new FieldOfView();
            fov->SetRadius(12);
        }

        void end() {

        }

        void render_shadow(const int& x, const int& y, ShadowSprite* shadow) {
            shadow->setPosition(Vector2f(x*16.0f, y*16.0f));
                if (level->IsFloor(x, y)) {
                    if (level->IsWall(x - 1, y)) {
                        if (level->IsWall(x, y - 1)) {
                            if (level->IsWall(x, y + 1)) {
                                if (level->IsWall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_ALL);
                                    window.draw(*shadow);
                                }
                                else {
                                    shadow->setSprite(SHADOW_ALL_BUT_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                            else {
                                if (level->IsWall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_ALL_BUT_BOTTOM);
                                    window.draw(*shadow);
                                }
                                else {
                                    shadow->setSprite(SHADOW_TOP_LEFT);
                                    window.draw(*shadow);
                                    if (level->IsWall(x + 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                        }
                        else {
                            if (level->IsWall(x, y + 1)) {
                                if (level->IsWall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_ALL_BUT_TOP);
                                    window.draw(*shadow);
                                }
                                else {
                                    shadow->setSprite(SHADOW_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                    if (level->IsWall(x + 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                            else {
                                if (level->IsWall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_SIDES_VERT);
                                    window.draw(*shadow);
                                }
                                else {
                                    shadow->setSprite(SHADOW_LEFT);
                                    window.draw(*shadow);
                                    if (level->IsWall(x + 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                        window.draw(*shadow);
                                    }
                                    if (level->IsWall(x + 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                        }
                    }
                    else {
                        if (level->IsWall(x, y - 1)) {
                            if (level->IsWall(x, y + 1)) {
                                if (level->IsWall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_ALL_BUT_LEFT);
                                    window.draw(*shadow);
                                }
                                else {
                                    shadow->setSprite(SHADOW_RIGHT);
                                    window.draw(*shadow);
                                    if (level->IsWall(x - 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                        window.draw(*shadow);
                                    }
                                    if (level->IsWall(x - 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                            else {
                                if (level->IsWall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_TOP_RIGHT);
                                    window.draw(*shadow);
                                    if (level->IsWall(x - 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                        window.draw(*shadow);
                                    }
                                }
                                else {
                                    shadow->setSprite(SHADOW_TOP);
                                    window.draw(*shadow);
                                    if (level->IsWall(x - 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                        window.draw(*shadow);
                                    }
                                    if (level->IsWall(x + 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                        }
                        else {
                            if (level->IsWall(x, y + 1)) {
                                if (level->IsWall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                    if (level->IsWall(x - 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                        window.draw(*shadow);
                                    }
                                }
                                else {
                                    shadow->setSprite(SHADOW_BOTTOM);
                                    window.draw(*shadow);
                                    if (level->IsWall(x - 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                        window.draw(*shadow);
                                    }
                                    if (level->IsWall(x + 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                            else {
                                if (level->IsWall(x + 1, y)) {
                                    shadow->setSprite(SHADOW_RIGHT);
                                    window.draw(*shadow);
                                    if (level->IsWall(x - 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                        window.draw(*shadow);
                                    }
                                    if (level->IsWall(x - 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                        window.draw(*shadow);
                                    }
                                }
                                else {
                                    if (level->IsWall(x - 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                        window.draw(*shadow);
                                    }
                                    if (level->IsWall(x + 1, y - 1)) {
                                        shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                        window.draw(*shadow);
                                    }
                                    if (level->IsWall(x - 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                        window.draw(*shadow);
                                    }
                                    if (level->IsWall(x + 1, y + 1)) {
                                        shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                        window.draw(*shadow);
                                    }
                                }
                            }
                        }
                    }
                } // IsFloor(x, y)
        }

        void render() {
            if (view_changed) {
                window.clear();
                //window.draw(text);

                int current_tiletype = -1;
                TileSprite current_sprite;

                fov->CalculateAndMark(level, level->player->pos);

                for (int x=0; x < level->width ; x++)
                for (int y=0; y < level->height; y++) {
                    Vector2i pos = Vector2i(x, y) - level->player->pos;
                    bool shade = false;
                    if (!fov->IsVisible(pos)) {
                        if (!level->IsKnown(Vector2i(x,y)))
                            continue;
                        else
                            shade = true;
                    }
                    const Tile& tile = level->GetTile(x, y);
                    if (current_tiletype != tile.type) {
                        current_tiletype = tile.type;
                        current_sprite = resman.get_tile_sprite(current_tiletype);
                    }
                    sf::Color color = resman.get_color(tile.material);
                    if (shade) {
                        int c = (color.r + color.g + color.b) / 3 * 0.75;
                        color.r = c;
                        color.g = c;
                        color.b = c;
                    }
                    current_sprite.setColor(color);
                    current_sprite.setPosition(Vector2f(x*16.0f, y*16.0f));
                    window.draw(current_sprite);

                    render_shadow(x, y, &resman.shadow);

                    if (!shade && (tile.unit != NULL)) {
                        TileSprite unit_sprite = resman.get_unit_sprite(tile.unit->type);
                        sf::Color color = resman.get_color(tile.unit->material);
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
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::F12) {
                        std::string dir = base_path + "user/screenshots/";
                        int n = list_dir(dir).size() + 1;
                        stringstream out;
                        out << n;
                        std::string fname = out.str();
                        fname.insert(fname.begin(), 5 - fname.size(), '0');
                        fname = (std::string)"screenshot" + fname + ".png";
                        Image Screen = window.capture();
                        Screen.saveToFile(dir + fname);

                        std::cout << "Saved screenshot to user/screenshots/" + fname << std::endl;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad7) {
                        level->MoveUnit(level->player, sf::Vector2i(-1, -1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad8) {
                        level->MoveUnit(level->player, sf::Vector2i(0, -1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad9) {
                        level->MoveUnit(level->player, sf::Vector2i(1, -1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad4) {
                        level->MoveUnit(level->player, sf::Vector2i(-1, 0));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad5) {
                        level->MoveUnit(level->player, sf::Vector2i(0, 0));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad6) {
                        level->MoveUnit(level->player, sf::Vector2i(1, 0));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad1) {
                        level->MoveUnit(level->player, sf::Vector2i(-1, 1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad2) {
                        level->MoveUnit(level->player, sf::Vector2i(0, 1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad3) {
                        level->MoveUnit(level->player, sf::Vector2i(1, 1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Add) {
                        fov->SetRadius(fov->GetRadius() + 1);
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Subtract) {
                        fov->SetRadius(fov->GetRadius() - 1);
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
    std::cout << base_path << std::endl;
    Game g;
    g.run();
    return 0;
}