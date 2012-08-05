#include "utils.hpp"

std::ostream& std::operator<<(std::ostream& cout, sf::Color color) {
    cout << "sf::Color(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")";
    return cout;
}
std::ostream& std::operator<<(std::ostream& cout, sf::IntRect rect) {
    cout << "sf::IntRect(" << rect.left << ", " << rect.top << ", " << rect.width << ", " << rect.height << ")";
    return cout;
}

std::vector<AString> split(const std::string& str, const char& chr) {
    std::vector<AString> result;
    AString current = AString("");
    for(unsigned int i = 0; i < str.size(); i++) {
        char c = str[i];
        if (c == chr){
            if (current.size() > 0) {
                result.push_back(current);
                current = AString("");
            }
        }
        else
            current += c;
    }
    if (current.size() > 0) {
        result.push_back(current);
        current = AString("");
    }
    return result;
}

std::vector<AString> list_dir(AString dir) {
    std::vector<AString> result;
    if (!fs::exists(dir)) {
        return result;
    }
    else if (!fs::is_directory(dir)) {
        return result;
    }
    else {
        fs::directory_iterator end_iter;
        for ( fs::directory_iterator dir_itr( dir ); dir_itr != end_iter; ++dir_itr ) {
            AString filename = dir_itr->path().filename().string();
            result.push_back(filename);
        }
    }
    return result;
}

sf::Color make_color(const std::string& str) {
    AString _str = str;
    std::vector<int> vec = _str.as_int_vector();
    if (vec.size() == 1)
        return sf::Color(vec[0], vec[0], vec[0]);
    else if (vec.size() == 3)
        return sf::Color(vec[0], vec[1], vec[2]);
    else if (vec.size() == 4)
        return sf::Color(vec[0], vec[1], vec[2], vec[3]);
    else
        return sf::Color(0,0,0);
}

sf::Vector2u make_vector2u(const std::string& str) {
    sf::Vector2u result;

    std::vector<int> vec = AString(str).as_int_vector();
    if (vec.size() < 2) return sf::Vector2u(0,0);

    result.x = (unsigned int)vec[0];
    result.y = (unsigned int)vec[1];
    return result;
}

sf::Vector2i make_vector2i(const std::string& str) {
    sf::Vector2i result;

    std::vector<int> vec = AString(str).as_int_vector();
    if (vec.size() < 2) return sf::Vector2i(0,0);

    result.x = vec[0];
    result.y = vec[1];
    return result;
}

sf::Vector2f make_vector2f(const std::string& str) {
    sf::Vector2f result;

    std::vector<float> vec = AString(str).as_float_vector();
    if (vec.size() < 2) return sf::Vector2f(0,0);

    result.x = vec[0];
    result.y = vec[1];
    return result;
}
