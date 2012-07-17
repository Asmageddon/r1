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
        void AddLevel(const std::string& name, Level *level) {
            if (!contains(maps, name))
                maps[name] = level;
        }
        void DeleteLevel(const std::string& name) {
            if (contains(maps, name)) {
                Level *map = maps[name];
                std::set<Unit*>::iterator i = map->units.begin();
                for (; i != map->units.end(); i++) {
                    delete *i;
                }
                delete map;
            }
        }
        Level* GetLevel(const std::string& name) {
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

        LightField *light;

        sf::Vector2i camera_pos;
    private:

        void start() {
            confman = ConfigManager(base_path);
            confman.Load();

            std::cout << confman.resolution << std::endl;

            window.create(VideoMode(confman.resolution.x, confman.resolution.y), "r1");
            window.setFramerateLimit(confman.max_fps);

            resman = ResourceManager(base_path);
            resman.Load();

            world = new World();

            current_level = new Level(&resman, sf::Vector2u(64, 64));
            world->AddLevel("test1", current_level);

            world->player = current_level->player;

            light = new LightField();
            light->SetRadius(12);
            light->SetFalloff(FALLOFF_LINEAR_SMOOTH);
            light->SetColor(sf::Color(255, 200, 170));
            current_level->AttachLight(light);
            light->Calculate(current_level, sf::Vector2i(6, 17));

            camera_pos = sf::Vector2i(5,5);

            current_level->ambient = sf::Color(15, 5, 20);
        }

        void end() {

        }

        void render_shadow(const sf::Vector2i& map_pos, ShadowSprite* shadow) {

            sf::Vector2f screen_pos;
            screen_pos.x = map_pos.x - camera_pos.x;
            screen_pos.y = map_pos.y - camera_pos.y;

            screen_pos.x *= resman.GetTileSize().x;
            screen_pos.y *= resman.GetTileSize().y;

            int x = map_pos.x;
            int y = map_pos.y;

            shadow->setPosition(screen_pos);
            if (current_level->IsFloor(sf::Vector2i(x, y))) {
                if (current_level->IsWall(sf::Vector2i(x - 1, y))) {
                    if (current_level->IsWall(sf::Vector2i(x, y - 1))) {
                        if (current_level->IsWall(sf::Vector2i(x, y + 1))) {
                            if (current_level->IsWall(sf::Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_ALL);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_ALL_BUT_RIGHT);
                                window.draw(*shadow);
                            }
                        }
                        else {
                            if (current_level->IsWall(sf::Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_ALL_BUT_BOTTOM);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_TOP_LEFT);
                                window.draw(*shadow);
                                if (current_level->IsWall(sf::Vector2i(x + 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                    }
                    else {
                        if (current_level->IsWall(sf::Vector2i(x, y + 1))) {
                            if (current_level->IsWall(sf::Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_ALL_BUT_TOP);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_BOTTOM_LEFT);
                                window.draw(*shadow);
                                if (current_level->IsWall(sf::Vector2i(x + 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                        else {
                            if (current_level->IsWall(sf::Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_SIDES_HORIZ);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_LEFT);
                                window.draw(*shadow);
                                if (current_level->IsWall(sf::Vector2i(x + 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(sf::Vector2i(x + 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                    }
                }
                else {
                    if (current_level->IsWall(sf::Vector2i(x, y - 1))) {
                        if (current_level->IsWall(sf::Vector2i(x, y + 1))) {
                            if (current_level->IsWall(sf::Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_ALL_BUT_LEFT);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_SIDES_VERT);
                                window.draw(*shadow);
                            }
                        }
                        else {
                            if (current_level->IsWall(sf::Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_TOP_RIGHT);
                                window.draw(*shadow);
                                if (current_level->IsWall(sf::Vector2i(x - 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                }
                            }
                            else {
                                shadow->setSprite(SHADOW_TOP);
                                window.draw(*shadow);
                                if (current_level->IsWall(sf::Vector2i(x - 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(sf::Vector2i(x + 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                    }
                    else {
                        if (current_level->IsWall(sf::Vector2i(x, y + 1))) {
                            if (current_level->IsWall(sf::Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_BOTTOM_RIGHT);
                                window.draw(*shadow);
                                if (current_level->IsWall(sf::Vector2i(x - 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                            }
                            else {
                                shadow->setSprite(SHADOW_BOTTOM);
                                window.draw(*shadow);
                                if (current_level->IsWall(sf::Vector2i(x - 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(sf::Vector2i(x + 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                        else {
                            if (current_level->IsWall(sf::Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_RIGHT);
                                window.draw(*shadow);
                                if (current_level->IsWall(sf::Vector2i(x - 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(sf::Vector2i(x - 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                }
                            }
                            else {
                                if (current_level->IsWall(sf::Vector2i(x - 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(sf::Vector2i(x + 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(sf::Vector2i(x - 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                }
                                if (current_level->IsWall(sf::Vector2i(x + 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                    }
                }
            } // IsFloor(sf::Vector2i(x, y))
        }

        void render() {
            window.clear();

            const TileType *current_tiletype = NULL;
            TileSprite current_sprite;

            camera_pos = world->player->pos;
            camera_pos.x -= window.getView().getSize().x / resman.GetTileSize().x / 2;
            camera_pos.y -= window.getView().getSize().y / resman.GetTileSize().y / 2;

            int start_x = max(camera_pos.x, 0);
            int start_y = max(camera_pos.y, 0);
            int end_x = min((float)current_level->GetSize().x, camera_pos.x + window.getView().getSize().x / resman.GetTileSize().x);
            int end_y = min((float)current_level->GetSize().y, camera_pos.y + window.getView().getSize().y / resman.GetTileSize().y);

            for (int x=start_x; x < end_x; x++)
            for (int y=start_y; y < end_y; y++) {
                float fow_brightness = world->player->GetLightThreshold();

                sf::Vector2f screen_pos = sf::Vector2f(x, y);

                screen_pos.x -= camera_pos.x;
                screen_pos.y -= camera_pos.y;

                screen_pos.x *= resman.GetTileSize().x;
                screen_pos.y *= resman.GetTileSize().y;

                sf::Vector2i map_pos = sf::Vector2i(x, y);

                sf::Color light_color = current_level->GetLightColorAt(map_pos);
                light_color += world->player->type->vision_tint;

                bool tile_visible = world->player->CanSee(map_pos);

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
                    current_sprite = current_tiletype->sprite;
                }

                sf::Color tile_color = tile.material->color;
                if (shade) {
                    sf::Color vt = world->player->type->vision_tint;
                    int c = (tile_color.r + tile_color.g + tile_color.b) / 3 * fow_brightness;
                    c += (vt.r + vt.g + vt.b) / 3;
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
                    TileSprite unit_sprite = tile.unit->type->sprite;
                    sf::Color color = tile.unit->material->color;

                    color = blend(color, color * light_color, 0.75);

                    unit_sprite.setColor(color);
                    unit_sprite.setPosition(screen_pos);
                    window.draw(unit_sprite);
                }

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
                    //else if (event.mouseButton.button == sf::Mouse::Left) {
                        //Tile t = Tile();
                        //t.type = 2;
                        //t.material = 1;
                        //current_level->SetTile(map_pos, t);
                        //if (light->InBounds(map_pos) && (light->GetIntensityAt(map_pos) > 0.0f))
                            //light->Calculate(current_level);
                    //}
                }
                else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::F12) {
                        std::string dir = base_path + "user/screenshots/";
                        int n = list_dir(dir).size() + 1;
                        std::stringstream out;
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
                    }
                    else if (event.key.code == sf::Keyboard::Numpad8) {
                        world->player->Move(sf::Vector2i(0, -1));
                    }
                    else if (event.key.code == sf::Keyboard::Numpad9) {
                        world->player->Move(sf::Vector2i(1, -1));
                    }
                    else if (event.key.code == sf::Keyboard::Numpad4) {
                        world->player->Move(sf::Vector2i(-1, 0));
                    }
                    else if (event.key.code == sf::Keyboard::Numpad5) {
                        world->player->Move(sf::Vector2i(0, 0));
                    }
                    else if (event.key.code == sf::Keyboard::Numpad6) {
                        world->player->Move(sf::Vector2i(1, 0));
                    }
                    else if (event.key.code == sf::Keyboard::Numpad1) {
                        world->player->Move(sf::Vector2i(-1, 1));
                    }
                    else if (event.key.code == sf::Keyboard::Numpad2) {
                        world->player->Move(sf::Vector2i(0, 1));
                    }
                    else if (event.key.code == sf::Keyboard::Numpad3) {
                        world->player->Move(sf::Vector2i(1, 1));
                    }
                    //Movement with arrows
                    else if (event.key.code == sf::Keyboard::Up) {
                        world->player->Move(sf::Vector2i(0, -1));
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        world->player->Move(sf::Vector2i(0, 1));
                    }
                    else if (event.key.code == sf::Keyboard::Left) {
                        world->player->Move(sf::Vector2i(-1, 0));
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        world->player->Move(sf::Vector2i(1, 0));
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