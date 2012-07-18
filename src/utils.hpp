#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>
#include <map>
#include <vector>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
namespace fs = boost::filesystem;

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
bool contains(const std::map<K, V>& m, const K& key) {
    typename std::map<K, V>::const_iterator it = m.find(key);
    return it != m.end();
}

template<class K, class V, class K2>
const V& const_access(const std::map<K, V>& m, const K2& key) {
    static V v;
    typename std::map<K, V>::const_iterator it;

    it = m.find(key);

    if(it != m.end()) {
        return it->second;
    }
    return v;
}

std::vector<std::string> split(const std::string& str, const char& chr);

std::vector<std::string> list_dir(std::string dir);

#endif
