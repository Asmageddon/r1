#include "utils.hpp"

std::ostream& std::operator<<(std::ostream& cout, sf::Color color) {
    cout << "sf::Color(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")";
    return cout;
}
std::ostream& std::operator<<(std::ostream& cout, sf::IntRect rect) {
    cout << "sf::IntRect(" << rect.left << ", " << rect.top << ", " << rect.width << ", " << rect.height << ")";
    return cout;
}

std::vector<std::string> split(const std::string& str, const char& chr) {
    std::vector<std::string> result;
    std::string current = "";
    for(unsigned int i = 0; i < str.size(); i++) {
        char c = str[i];
        if (c == chr){
            if (current.size() > 0) {
                result.push_back(current);
                current = "";
            }
        }
        else
            current += c;
    }
    if (current.size() > 0) {
        result.push_back(current);
        current = "";
    }
    return result;
}

std::vector<std::string> list_dir(std::string dir) {
    std::vector<std::string> result;
    if (!fs::exists(dir)) {
        return result;
    }
    else if (!fs::is_directory(dir)) {
        return result;
    }
    else {
        fs::directory_iterator end_iter;
        for ( fs::directory_iterator dir_itr( dir ); dir_itr != end_iter; ++dir_itr ) {
            std::string filename = dir_itr->path().filename().string();
            result.push_back(filename);
        }
    }
    return result;
}
