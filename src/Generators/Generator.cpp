#include "Generator.hpp"

#include <vector>
#include <string>

#include "../Resources/MapType.hpp"
#include "../Data.hpp"
#include "../Level.hpp"
#include "../Tile.hpp"

#include "../utils.hpp"

//TODO: Move this to utils
template<class T> bool contains(const std::set<T>& _set, const T& v) {
    return (_set.find(v) != _set.end());
}

Generator::Generator(const MapType& type, const Data& data, const std::string& category) {
    //WIP: This v (will probably need to extend Tile class or replace sets with something else)
    std::set<AString> sset;
    std::set<AString>::const_iterator it;

    sset = data[category]["mask.blacklist"].as_string_set();
    for(it = sset.begin(); it != sset.end(); it++) {
        Tile t = const_access(type.generator_tiles, *it);
        mask_blacklist.insert(t);
    }

    sset = data[category]["mask.whitelist"].as_string_set();
    for(it = sset.begin(); it != sset.end(); it++) {
        Tile t = const_access(type.generator_tiles, *it);
        mask_whitelist.insert(t);
    }

}

bool Generator::HasMask() const {
    return ((mask_blacklist.size() > 0) | (mask_whitelist.size() > 0));
}

bool Generator::IsMasked(const Level& level, int x, int y) const {
    if (mask_blacklist.size() == 0) {
        if (mask_whitelist.size() == 0) {
            return false;
        }
        else {
            if (contains(mask_whitelist, level.GetTile(sf::Vector2i(x, y)))) {
                return false;
            }
            else {
                return true;
            }
        }
    }
    else {
        if (contains(mask_blacklist, level.GetTile(sf::Vector2i(x, y)))) {
            return true;
        }
        else {
            return false;
        }
    }
}
