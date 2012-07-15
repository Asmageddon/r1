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
    argv0[last_slash+1] = '\0'; //lolololol
    return argv0;
}

enum FOW_MODE {
    FOW_DISABLED,
    FOW_AMBIENT,
    FOW_THRESHOLD,
    FOW_THRESHOLD_FORGET
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

        FOW_MODE fow_mode;

    private:

        void start() {
            confman = ConfigManager(base_path);
            confman.Load();

            window.create(VideoMode(confman.resolution.x, confman.resolution.y), "r1");
            window.setFramerateLimit(confman.max_fps);

            resman = ResourceManager(base_path);
            resman.Load();
            level = new Level(&resman, Vector2u(64, 64));

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
            light2->SetRadius(8);
            light2->SetFalloff(FALLOFF_LINEAR_SMOOTH);
            light2->SetColor(sf::Color(150, 210, 230));

            camera_pos = Vector2i(5,5);

            level->ambient = Color(15, 5, 20);

            fow_mode = FOW_THRESHOLD;
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
            if (level->IsFloor(Vector2i(x, y))) {
                if (level->IsWall(Vector2i(x - 1, y))) {
                    if (level->IsWall(Vector2i(x, y - 1))) {
                        if (level->IsWall(Vector2i(x, y + 1))) {
                            if (level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_ALL);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_ALL_BUT_RIGHT);
                                window.draw(*shadow);
                            }
                        }
                        else {
                            if (level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_ALL_BUT_BOTTOM);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_TOP_LEFT);
                                window.draw(*shadow);
                                if (level->IsWall(Vector2i(x + 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                    }
                    else {
                        if (level->IsWall(Vector2i(x, y + 1))) {
                            if (level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_ALL_BUT_TOP);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_BOTTOM_LEFT);
                                window.draw(*shadow);
                                if (level->IsWall(Vector2i(x + 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                        else {
                            if (level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_SIDES_HORIZ);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_LEFT);
                                window.draw(*shadow);
                                if (level->IsWall(Vector2i(x + 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                    window.draw(*shadow);
                                }
                                if (level->IsWall(Vector2i(x + 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                    }
                }
                else {
                    if (level->IsWall(Vector2i(x, y - 1))) {
                        if (level->IsWall(Vector2i(x, y + 1))) {
                            if (level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_ALL_BUT_LEFT);
                                window.draw(*shadow);
                            }
                            else {
                                shadow->setSprite(SHADOW_SIDES_VERT);
                                window.draw(*shadow);
                            }
                        }
                        else {
                            if (level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_TOP_RIGHT);
                                window.draw(*shadow);
                                if (level->IsWall(Vector2i(x - 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                }
                            }
                            else {
                                shadow->setSprite(SHADOW_TOP);
                                window.draw(*shadow);
                                if (level->IsWall(Vector2i(x - 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                }
                                if (level->IsWall(Vector2i(x + 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                    }
                    else {
                        if (level->IsWall(Vector2i(x, y + 1))) {
                            if (level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_BOTTOM_RIGHT);
                                window.draw(*shadow);
                                if (level->IsWall(Vector2i(x - 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                            }
                            else {
                                shadow->setSprite(SHADOW_BOTTOM);
                                window.draw(*shadow);
                                if (level->IsWall(Vector2i(x - 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                                if (level->IsWall(Vector2i(x + 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                    window.draw(*shadow);
                                }
                            }
                        }
                        else {
                            if (level->IsWall(Vector2i(x + 1, y))) {
                                shadow->setSprite(SHADOW_RIGHT);
                                window.draw(*shadow);
                                if (level->IsWall(Vector2i(x - 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                                if (level->IsWall(Vector2i(x - 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                }
                            }
                            else {
                                if (level->IsWall(Vector2i(x - 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_LEFT);
                                    window.draw(*shadow);
                                }
                                if (level->IsWall(Vector2i(x + 1, y - 1))) {
                                    shadow->setSprite(SHADOW_CORNER_TOP_RIGHT);
                                    window.draw(*shadow);
                                }
                                if (level->IsWall(Vector2i(x - 1, y + 1))) {
                                    shadow->setSprite(SHADOW_CORNER_BOTTOM_LEFT);
                                    window.draw(*shadow);
                                }
                                if (level->IsWall(Vector2i(x + 1, y + 1))) {
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

                camera_pos = level->player->pos;
                camera_pos.x -= window.getSize().x / resman.GetTileSize().x / 2;
                camera_pos.y -= window.getSize().y / resman.GetTileSize().y / 2;

                fov->Calculate(level, level->player->pos);
                light2->Calculate(level, level->player->pos);

                int start_x = max(camera_pos.x, 0);
                int start_y = max(camera_pos.y, 0);
                int end_x = min((unsigned int)level->GetSize().x, camera_pos.x + window.getSize().x / resman.GetTileSize().x);
                int end_y = min((unsigned int)level->GetSize().y, camera_pos.y + window.getSize().y / resman.GetTileSize().y);

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

                    sf::Color light_color = level->ambient;
                    light_color += light->GetColorAt(map_pos);
                    light_color += light2->GetColorAt(map_pos);

                    float i = max(light_color.r, max(light_color.g, light_color.b)) / 255.0;

                    //In threshold mode, we're marking tiles below certain brightness as not visible
                    switch (fow_mode) {
                        case FOW_THRESHOLD:
                            tile_visible = tile_visible && (i >= fow_brightness);
                            break;
                        case FOW_THRESHOLD_FORGET:
                            tile_visible = tile_visible && (i >= fow_brightness);
                            break;
                        default:
                            break;
                    }

                    bool shade = false;
                    bool known = level->IsKnown(map_pos);

                    if ( !tile_visible ) {
                        if ( !known )
                            continue; //Tile is unknown, not visible, skip rendering
                        else {
                            //Tile is not visible but known
                            switch (fow_mode) {
                                case FOW_DISABLED:
                                    continue;
                                default:
                                    shade = true;
                                    break;
                            }
                        }
                    }


                    //Handle revealing and hiding tiles behind Fog of War
                    switch (fow_mode) {
                        case FOW_DISABLED:
                            break;
                        case FOW_AMBIENT:
                            if (tile_visible)
                                level->SetKnown(map_pos, true);
                            break;
                        case FOW_THRESHOLD:
                            if (tile_visible) {
                                if (i >= fow_brightness) {
                                    level->SetKnown(map_pos, true);
                                }
                            }
                            break;
                        case FOW_THRESHOLD_FORGET:
                            if (tile_visible) {
                                if (i >= fow_brightness) {
                                    level->SetKnown(map_pos, true);
                                }
                                else {
                                    level->SetKnown(map_pos, false);
                                }
                            }
                            break;
                    }

                    const Tile& tile = level->GetTile(map_pos);

                    if (current_tiletype != tile.type) {
                        current_tiletype = tile.type;
                        current_sprite = resman.GetTileSprite(current_tiletype);
                    }

                    sf::Color tile_color = resman.GetMaterialColor(tile.material);
                    if (shade) {
                        int c;
                        int c1;
                        float c2;
                        sf::Color col;
                        switch (fow_mode) {
                            //In FOW_AMBIENT, we're using grayscaled ambient * tile color
                            case FOW_AMBIENT:
                                col = level->ambient;
                                c1 = (col.r + col.g + col.b) / 3;
                                c2 = (tile_color.r + tile_color.g + tile_color.b) / 3 / 255.0;
                                c = c2 * c1;
                                tile_color = sf::Color(c, c, c);
                                break;
                            default:
                                c = (tile_color.r + tile_color.g + tile_color.b) / 3 * fow_brightness;
                                tile_color = sf::Color(c, c, c);
                                break;
                        }
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
                    //Movement with numpad
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
                        level->MoveUnit(level->player, sf::Vector2i(0, -1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        level->MoveUnit(level->player, sf::Vector2i(0, 1));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Left) {
                        level->MoveUnit(level->player, sf::Vector2i(-1, 0));
                        view_changed = true;
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        level->MoveUnit(level->player, sf::Vector2i(1, 0));
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