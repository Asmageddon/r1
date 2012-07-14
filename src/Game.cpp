#include <iostream>

#include <string>

#include <vector>
#include <map>

#include <cstring>
#include <iostream>

#include <SFML/Graphics.hpp>

#include <noise/noise.h>

#include "ConfigManager.hpp"
#include "TileType.hpp"
#include "Material.hpp"
#include "UnitType.hpp"
#include "ResourceManager.hpp"
#include "utils.hpp"

#include <cmath>

#define PI 3.14159265

static std::string base_path = ".";

sf::Color blend(const sf::Color& col1, const sf::Color& col2, float dist) {
    sf::Color result = col1;
    result.r = result.r * (1.0f - dist) + col2.r * dist;
    result.g = result.g * (1.0f - dist) + col2.g * dist;
    result.b = result.b * (1.0f - dist) + col2.b * dist;

    return result;
}

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
        Color ambient;
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
            noise::module::Perlin perlin;
            perlin.SetOctaveCount (2);
            perlin.SetFrequency (1.0);

            for(int x=0; x < width; x++)
            for(int y=0; y < height; y++) {
                double value = perlin.GetValue (0.14 * x, 0.14 * y, 0.5);

                data[x + width*y].type = 2;
                data[x + width*y].material = 1;
                data[x + width*y].integrity = 255;
                data[x + width*y].temperature = 127;

                if (value > 0.0)
                    data[x + width*y].type = 1;

                //if (x == y)
                    //data[x + width*y].type = 1;
                //if ((x * y) == 0)
                    //data[x + width*y].type = 1;
                //if (x == width - 1)
                    //data[x + width*y].type = 1;
                //if (y == height - 1)
                    //data[x + width*y].type = 1;
                //if (y == x+1)
                    //data[x + width*y].type = 1;
                //if (y == x-1)
                    //data[x + width*y].type = 1;



            }
            Unit *u = new Unit();
            u->type = 1;
            PlaceUnit(sf::Vector2i(12,5), u);
            player = u;

            u = new Unit();
            u->type = 2;
            u->material = 1;
            PlaceUnit(sf::Vector2i(12,3), u);
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

enum FALLOFF {
    FALLOFF_FLAT,
    FALLOFF_LINEAR_ROUGH,
    FALLOFF_LINEAR_SMOOTH
};

class Field {
    private:
        short radius;
        short width;
        sf::Vector2i center;
        sf::Vector2i origin;
        FALLOFF falloff;
    protected:
        float *intensity;
    public:
        Field() {
            radius = 1;
            intensity = new float[3*3];
            center = sf::Vector2i(1,1);
        }

        void SetRadius(const short& radius) {
            this->radius = radius;
            this->width = 1 + radius * 2;
            this->center = sf::Vector2i(radius, radius);
            delete[] intensity;
            intensity = new float[width * width];
        }
        const short& GetRadius() const {
            return this->radius;
        }

        void SetFalloff(const FALLOFF& falloff) {
            this->falloff = falloff;
        }
        const FALLOFF& GetFalloff() const {
            return this->falloff;
        }

        void Calculate(const Level* level, const sf::Vector2i& caster_pos) {
            /*
             * Simple raycasting
             */
            origin = caster_pos;
            for(int x = 0; x < width; x++)
            for(int y = 0; y < width; y++) {
                intensity[x + width * y] = 0.0f;
            }
            intensity[center.x + width * center.y] = 1.0f;
            //int rays = PI * 2 * radius * 2;
            int rays = 360;
            for(int a = 0; a < rays; a++) {
                float ax = sin(a * PI * 2 / rays);
                float ay = cos(a * PI * 2 / rays);
                float x = center.x;
                float y = center.y;
                for(int z = 0; z < this->radius; z++) {
                    x += ax;
                    y += ay;
                    sf::Vector2i pos = sf::Vector2i(
                        (int)round(x),
                        (int)round(y)
                    );
                    if (z == radius) {
                        if (sqrt((pos.x - center.x) * (pos.x - center.x) + (pos.y - center.y) * (pos.y - center.y)) > radius) {
                            break;
                        }
                    }
                    switch(this->falloff) {
                        float dist;
                        case FALLOFF_LINEAR_ROUGH:
                            intensity[pos.x + width * pos.y] = 1.0f - (z * 1.0f / this->radius);
                            break;
                        case FALLOFF_LINEAR_SMOOTH:
                            dist = sqrt((pos.x - center.x) * (pos.x - center.x) + (pos.y - center.y) * (pos.y - center.y));
                            if (dist > radius) dist = radius;
                            intensity[pos.x + width * pos.y] = 1.0f - (dist / this->radius);
                            break;
                        case FALLOFF_FLAT:
                            intensity[pos.x + width * pos.y] = 1.0f;
                            break;
                    }

                    sf::Vector2i map_pos = pos + caster_pos - center;

                    if (level->IsWall(map_pos))
                        break;
                }
            }
        }
        float GetIntensityAt(const sf::Vector2i& pos) const {
            sf::Vector2i _pos = pos - origin + center;
            if (_pos.x < 0) return 0.0f;
            if (_pos.x >= width) return 0.0f;
            if (_pos.y < 0) return 0.0f;
            if (_pos.y >= width) return 0.0f;

            const float& i = intensity[_pos.x + width * _pos.y];
            return i;
        }

};

class LightField: public Field {
    private:
        sf::Color color;
    public:
        void SetColor(const sf::Color& col) {
            this->color = col;
        }
        const sf::Color& GetColor() const {
            return this->color;
        }
        sf::Color GetColorAt(const sf::Vector2i& pos) const {
            sf::Color result = this->color;
            const float& i = this->GetIntensityAt(pos);
            result.r *= i;
            result.g *= i;
            result.b *= i;
            return result;
        }
};

class Game {
    private:
        sf::RenderWindow window;
        Level* level;
        ResourceManager resman;
        ConfigManager confman;

        bool view_changed;

        Field *fov;
        LightField *light;
        LightField *light2;

        sf::Vector2i camera_pos;

    private:

        void start() {
            confman = ConfigManager(base_path);
            confman.Load();

            window.create(VideoMode(confman.resolution.x, confman.resolution.y), "r1");
            window.setFramerateLimit(confman.max_fps);

            resman = ResourceManager(base_path);
            resman.Load();
            level = new Level(&resman, 64, 64);

            view_changed = true;

            fov = new Field();
            fov->SetRadius(12);
            fov->SetFalloff(FALLOFF_FLAT);

            light = new LightField();
            light->SetRadius(12);
            light->SetFalloff(FALLOFF_LINEAR_SMOOTH);
            light->SetColor(sf::Color(255, 200, 170));
            light->Calculate(level, sf::Vector2i(6, 17));

            light2 = new LightField();
            light2->SetRadius(5);
            light2->SetFalloff(FALLOFF_LINEAR_SMOOTH);
            light2->SetColor(sf::Color(150, 210, 230));

            camera_pos = Vector2i(5,5);

            level->ambient = Color(15, 5, 20);
        }

        void end() {

        }

        void render_shadow(const Vector2i& map_pos, ShadowSprite* shadow) {

            sf::Vector2f screen_pos;
            screen_pos.x = map_pos.x - camera_pos.x;
            screen_pos.y = map_pos.y - camera_pos.y;

            screen_pos.x *= resman.GetTileSize().x;
            screen_pos.y *= resman.GetTileSize().y;

            int x = map_pos.x;
            int y = map_pos.y;

            shadow->setPosition(screen_pos);
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
                                shadow->setSprite(SHADOW_SIDES_HORIZ);
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
                                shadow->setSprite(SHADOW_SIDES_VERT);
                                window.draw(*shadow);
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
                                if (level->IsWall(x - 1, y - 1)) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                                if (level->IsWall(x + 1, y - 1)) {
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

                fov->Calculate(level, level->player->pos);
                light2->Calculate(level, level->player->pos);
                //camera_pos.x to max(map_size.x - camera_pos.x , camera_pos.x + screen.w / tile_size)
                //camera_pos.y to max(map_size.y - camera_pos.y , camera_pos.y + screen.h / tile_size)
                int start_x = camera_pos.x;
                int start_y = camera_pos.y;
                int end_x = min((unsigned int)level->width, camera_pos.x + window.getSize().x / resman.GetTileSize().x);
                int end_y = min((unsigned int)level->height, camera_pos.y + window.getSize().y / resman.GetTileSize().y);
                for (int x=start_x; x < end_x; x++)
                for (int y=start_y; y < end_y; y++) {
                    sf::Vector2f screen_pos = sf::Vector2f(x, y);

                    screen_pos.x -= camera_pos.x;
                    screen_pos.y -= camera_pos.y;

                    screen_pos.x *= resman.GetTileSize().x;
                    screen_pos.y *= resman.GetTileSize().y;

                    sf::Vector2i map_pos = sf::Vector2i(x, y);

                    bool shade = false;
                    if ( !fov->GetIntensityAt(map_pos) >= 0.1f ) {
                        if ( !level->IsKnown(map_pos) )
                            continue;
                        else
                            continue;
                            //shade = true;
                    }

                    sf::Color light_color = level->ambient;
                    light_color += light->GetColorAt(map_pos);
                    light_color += light2->GetColorAt(map_pos);

                    const Tile& tile = level->GetTile(map_pos);

                    if (current_tiletype != tile.type) {
                        current_tiletype = tile.type;
                        current_sprite = resman.GetTileSprite(current_tiletype);
                    }

                    sf::Color tile_color = resman.GetMaterialColor(tile.material);
                    if (shade) {
                        int c = (tile_color.r + tile_color.g + tile_color.b) / 3 * 0.75;
                        tile_color.r = c;
                        tile_color.g = c;
                        tile_color.b = c;
                    }
                    else {
                        tile_color = blend(tile_color, tile_color * light_color, 0.90);
                    }

                    current_sprite.setColor(tile_color);
                    current_sprite.setPosition(screen_pos);
                    window.draw(current_sprite);

                    render_shadow(map_pos, &resman.shadow);

                    if (!shade && (tile.unit != NULL)) {
                        TileSprite unit_sprite = resman.GetUnitSprite(tile.unit->type);
                        sf::Color color = resman.GetMaterialColor(tile.unit->material);

                        if (tile.unit != level->player) {
                            color = blend(color, color * light_color, 0.75);
                        }

                        unit_sprite.setColor(color);
                        unit_sprite.setPosition(screen_pos);
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
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::MouseButtonPressed) {
                    //event.button
                    sf::Vector2i map_pos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);

                    //map_pos /= resman.GetTileSize();
                    map_pos.x /= resman.GetTileSize().x;
                    map_pos.y /= resman.GetTileSize().y;

                    //map_pos += camera_pos;
                    map_pos.x += camera_pos.x;
                    map_pos.y += camera_pos.y;

                    light->Calculate(level, map_pos);

                    view_changed = true;
                }
                else if (event.type == sf::Event::KeyPressed) {
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