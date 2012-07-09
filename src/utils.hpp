#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>
#include <map>
#include <vector>
#include <sstream>
using namespace std;

#include <SFML/Graphics.hpp>

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
namespace fs = boost::filesystem;

namespace std {
    ostream& operator<<(ostream& cout, sf::Color color);
    ostream& operator<<(ostream& cout, sf::IntRect rect);
    template<class T>
    ostream& operator<<(ostream& cout, sf::Vector2<T> vec)  {
        cout << "sf::Vector2" << typeid(T).name() << "(" << vec.x << ", " << vec.y << ")";
        return cout;
    }
    template<class T>
    ostream& operator<<(ostream& cout, sf::Vector3<T> vec)  {
        cout << "sf::Vector3" << typeid(T).name() << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return cout;
    }
}

template<class K, class V>
bool contains(map<K, V> m, K key) {
    typename map<K, V>::iterator it = m.find(key);
    return it != m.end();
}
vector<string> split(const string& str, const char& chr);

vector<string> list_dir(string dir);

#endif