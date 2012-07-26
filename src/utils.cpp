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
