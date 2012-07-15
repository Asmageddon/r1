#include <iostream>

#include <string>

#include <vector>
#include <map>

#include <cstring>
#include <iostream>

#include <SFML/Graphics.hpp>


#include "utils.hpp"

#include "ConfigManager.hpp"
#include "ResourceManager.hpp"

#include "Level.hpp"
#include "Field.hpp"
#include "LightField.hpp"

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
    argv0[last_slash+1] = '\0'; //lolololol (hope gods of programming will forgive me)
    return argv0;
}

class World {
    private:
        int seed;
        std::map<std::string, Level*> maps;
    public:
        Unit *player;
    public:
        World(int seed = 0) {
            if (seed == 0) {
                srand(time(NULL));
                this->seed = rand();
            }
            else {
                this->seed = seed;
            }
        }
        void AddLevel(const string& name, Level *level) {
            if (!contains(maps, name))
                maps[name] = level;
        }
        void DeleteLevel(const string& name) {
            if (contains(maps, name)) {
                Level *map = maps[name];
                std::set<Unit*>::iterator i = map->units.begin();
                for (; i != map->units.end(); i++) {
                    delete *i;
                }
                delete map;
            }
        }
        Level* GetLevel(const string& name) {
            if (contains(maps, name)) {
                return maps[name];
            }
            return NULL;
        }
};

class Game {
    private:
        sf::RenderWindow window;
        World *world;
        Level *current_level;
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

            world = new World();

            current_level = new Level(&resman, Vector2u(64, 64));
            world->AddLevel("test1", current_level);

            world->player = current_level->player;

            view_changed = true;

            fov = new Field();
            fov->SetRadius(12);
            fov->SetFalloff(FALLOFF_FLAT);

            light = new LightField();
            light->SetRadius(12);
            light->SetFalloff(FALLOFF_LINEAR_SMOOTH);
            light->SetColor(sf::Color(255, 200, 170));
            light->Calculate(current_level, sf::Vector2i(6, 17));

            light2 = new LightField();
            light2->SetRadius(8);
            light2->SetFalloff(FALLOFF_LINEAR_SMOOTH);
            light2->SetColor(sf::Color(150, 210, 230));

            camera_pos = Vector2i(5,5);

            current_level->ambient = Color(15, 5, 20);
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
            if (current_level->IsFloor(Vector2i(x, y))) {
                if (current_level->IsWall(Vector2i(x - 1, y))) {
                    if (current_level->IsWall(Vector2i(x, y - 1))) {
                        if (current_level->IsWall(Vector2i(x, y + 1))) {
                            if (current_level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_ALL);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_ALL_BUT_RIGHT);
                                window.draw(*shadow);
                            }
                        }
                        else {
                            if (current_level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_ALL_BUT_BOTTOM);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_TOP_LEFT);
                                window.draw(*shadow);
                                if (current_level->IsWall(Vector2i(x + 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                    }
                    else {
                        if (current_level->IsWall(Vector2i(x, y + 1))) {
                            if (current_level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_ALL_BUT_TOP);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_BOTTOM_LEFT);
                                window.draw(*shadow);
                                if (current_level->IsWall(Vector2i(x + 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                        else {
                            if (current_level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_SIDES_HORIZ);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_LEFT);
                                window.draw(*shadow);
                                if (current_level->IsWall(Vector2i(x + 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(Vector2i(x + 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                    }
                }
                else {
                    if (current_level->IsWall(Vector2i(x, y - 1))) {
                        if (current_level->IsWall(Vector2i(x, y + 1))) {
                            if (current_level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_ALL_BUT_LEFT);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_SIDES_VERT);
                                window.draw(*shadow);
                            }
                        }
                        else {
                            if (current_level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_TOP_RIGHT);
                                window.draw(*shadow);
                                if (current_level->IsWall(Vector2i(x - 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                }
                            }
                            else {
                                shadow->setSprite(SHADOW_TOP);
                                window.draw(*shadow);
                                if (current_level->IsWall(Vector2i(x - 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(Vector2i(x + 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                    }
                    else {
                        if (current_level->IsWall(Vector2i(x, y + 1))) {
                            if (current_level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_BOTTOM_RIGHT);
                                window.draw(*shadow);
                                if (current_level->IsWall(Vector2i(x - 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                            }
                            else {
                                shadow->setSprite(SHADOW_BOTTOM);
                                window.draw(*shadow);
                                if (current_level->IsWall(Vector2i(x - 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(Vector2i(x + 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                        else {
                            if (current_level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_RIGHT);
                                window.draw(*shadow);
                                if (current_level->IsWall(Vector2i(x - 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(Vector2i(x - 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                }
                            }
                            else {
                                if (current_level->IsWall(Vector2i(x - 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(Vector2i(x + 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(Vector2i(x - 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(Vector2i(x + 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                    }
                }
            } // IsFloor(Vector2i(x, y))
        }

        void render() {
            if (view_changed) {
                window.clear();

                int current_tiletype = -1;
                TileSprite current_sprite;

                camera_pos = world->player->pos;
                camera_pos.x -= window.getView().getSize().x / resman.GetTileSize().x / 2;
                camera_pos.y -= window.getView().getSize().y / resman.GetTileSize().y / 2;

                fov->Calculate(current_level, world->player->pos);
                light2->Calculate(current_level, world->player->pos);

                int start_x = max(camera_pos.x, 0);
                int start_y = max(camera_pos.y, 0);
                int end_x = min((float)current_level->GetSize().x, camera_pos.x + window.getView().getSize().x / resman.GetTileSize().x);
                int end_y = min((float)current_level->GetSize().y, camera_pos.y + window.getView().getSize().y / resman.GetTileSize().y);

                for (int x=start_x; x < end_x; x++)
                for (int y=start_y; y < end_y; y++) {
                    float fow_brightness = 0.33;

                    sf::Vector2f screen_pos = sf::Vector2f(x, y);

                    screen_pos.x -= camera_pos.x;
                    screen_pos.y -= camera_pos.y;

                    screen_pos.x *= resman.GetTileSize().x;
                    screen_pos.y *= resman.GetTileSize().y;

                    sf::Vector2i map_pos = sf::Vector2i(x, y);

                    bool tile_visible = fov->GetIntensityAt(map_pos) >= 0.1f;

                    sf::Color light_color = current_level->ambient;
                    light_color += light->GetColorAt(map_pos);
                    light_color += light2->GetColorAt(map_pos);

                    float i = max(light_color.r, max(light_color.g, light_color.b)) / 255.0;

                    //We're marking tiles below certain brightness as not visible
                    tile_visible = tile_visible && (i >= fow_brightness);

                    bool shade = false;
                    bool known = current_level->IsKnown(map_pos);

                    if ( !tile_visible ) {
                        if ( !known )
                            continue; //Tile is unknown, not visible, skip rendering
                        else {
                            //Tile is not visible but known
                            shade = true;
                        }
                    }


                    //Handle revealing and hiding tiles behind Fog of War
                    if (tile_visible) {
                        //if (i >= fow_brightness) {
                            current_level->SetKnown(map_pos, true);
                        //}
                    }

                    const Tile& tile = current_level->GetTile(map_pos);

                    if (current_tiletype != tile.type) {
                        current_tiletype = tile.type;
                        current_sprite = resman.GetTileSprite(current_tiletype);
                    }

                    sf::Color tile_color = resman.GetMaterialColor(tile.material);
                    if (shade) {
                        int c = (tile_color.r + tile_color.g + tile_color.b) / 3 * fow_brightness;
                        tile_color = sf::Color(c, c, c);
                    }
                    else {
                        tile_color = tile_color * light_color;
                    }

                    current_sprite.setColor(tile_color);
                    current_sprite.setPosition(screen_pos);
                    window.draw(current_sprite);

                    render_shadow(map_pos, &resman.shadow);

                    if (!shade && (tile.unit != NULL)) {
                        TileSprite unit_sprite = resman.GetUnitSprite(tile.unit->type);
                        sf::Color color = resman.GetMaterialColor(tile.unit->material);

                        if (tile.unit != world->player) {
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
                else if (event.type == sf::Event::Resized) {
                    view_changed = true;
                }
                else if (event.type == sf::Event::MouseButtonPressed) {
                    //event.button
                    sf::Vector2i map_pos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);

                    //Screen space to view space
                    map_pos.x *= window.getView().getSize().x * 1.0 / window.getSize().x;
                    map_pos.y *= window.getView().getSize().y * 1.0 / window.getSize().y;

                    //View space to map space
                    map_pos.x /= resman.GetTileSize().x;
                    map_pos.y /= resman.GetTileSize().y;
                    map_pos.x += camera_pos.x;
                    map_pos.y += camera_pos.y;

                    if (event.mouseButton.button == sf::Mouse::Right) {
                        light->Calculate(current_level, map_pos);
                    }
                    else if (event.mouseButton.button == sf::Mouse::Left) {
                        Tile t = Tile();
                        t.type = 2;
                        t.material = 1;
                        current_level->SetTile(map_pos, t);
                        if (light->InBounds(map_pos) && (light->GetIntensityAt(map_pos) > 0.0f))
                            light->Calculate(current_level);
                    }

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
                    //Movement with numpad
                    else if (event.key.code == sf::Keyboard::Numpad7) {
                        world->player->Move(sf::Vector2i(-1, -1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad8) {
                        world->player->Move(sf::Vector2i(0, -1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad9) {
                        world->player->Move(sf::Vector2i(1, -1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad4) {
                        world->player->Move(sf::Vector2i(-1, 0));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad5) {
                        world->player->Move(sf::Vector2i(0, 0));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad6) {
                        world->player->Move(sf::Vector2i(1, 0));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad1) {
                        world->player->Move(sf::Vector2i(-1, 1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad2) {
                        world->player->Move(sf::Vector2i(0, 1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Numpad3) {
                        world->player->Move(sf::Vector2i(1, 1));
                        view_changed = true;
                    }
                    //View range
                    else if (event.key.code == sf::Keyboard::Add) {
                        fov->SetRadius(fov->GetRadius() + 1);
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Subtract) {
                        fov->SetRadius(fov->GetRadius() - 1);
                        view_changed = true;
                    }
                    //Movement with arrows
                    else if (event.key.code == sf::Keyboard::Up) {
                        world->player->Move(sf::Vector2i(0, -1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        world->player->Move(sf::Vector2i(0, 1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Left) {
                        world->player->Move(sf::Vector2i(-1, 0));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        world->player->Move(sf::Vector2i(1, 0));
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