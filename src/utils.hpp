#ifndef UTILS_HPP_
#define UTILS_HPP_

#include "AString.hpp"
#include <map>
#include <vector>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
namespace fs = boost::filesystem;

//TODO: URGUENT - Split utils into multiple files because pretty much everything uses it

namespace std {
    std::ostream& operator<<(std::ostream& cout, sf::Color color);
    std::ostream& operator<<(std::ostream& cout, sf::IntRect rect);
    template<class T>
    std::ostream& operator<<(std::ostream& cout, sf::Vector2<T> vec)  {
        cout << "sf::Vector2" << typeid(T).name() << "(" << vec.x << ", " << vec.y << ")";
        return cout;
    }
    template<class T>
    std::ostream& operator<<(std::ostream& cout, sf::Vector3<T> vec)  {
        cout << "sf::Vector3" << typeid(T).name() << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return cout;
    }
}

template<class K, class V>
bool contains(const std::map<K, V>& m, const std::string& key) {
    typename std::map<K, V>::const_iterator it = m.find(key);
    return it != m.end();
}

template<class K, class V>
const V& const_access(const std::map<K, V>& m, const std::string& key) {
    static V v;
    typename std::map<K, V>::const_iterator it;

    it = m.find(key);

    if(it != m.end()) {
        return it->second;
    }
    return v;
}

std::vector<AString> split(const std::string& str, const char& chr);

std::vector<AString> list_dir(AString dir);

sf::Color make_color(const std::string& str);

sf::Vector2u make_vector2u(const std::string& str);

sf::Vector2i make_vector2i(const std::string& str);

sf::Vector2f make_vector2f(const std::string& str);

#endif
