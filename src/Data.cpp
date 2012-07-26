#include "Data.hpp"

#include "utils.hpp"

void Data::parse_line(AString line) {
    if (line.length() == 0) return;

    int stage = 0;
    AString field;
    AString value;
    AString category;

    for (unsigned int i = 0; i < line.length(); i++) {
        char c = line[i];
        if (stage == 0) {
            if ( (c == ' ') || (c == '\t') )
                continue;
            else if (c == '=')
                stage = 1;
            else if (c == '#')
                return;
            else if (c == '[')
                stage = 3;
            else
                field += c;
        }
        else if (stage == 1) {
            if ( (c == ' ') || (c == '\t') )
                continue;
            else {
                stage = 2;
                value += c;
            }
        }
        else if (stage == 2) {
            value += c;
        }
        else if (stage == 3) {
            if ( c == ']' ) {
                current_category = category;
                return;
            }
            else
                category += c;
        }
    }
    this->values[current_category][field] = value;
}

Data::Data(AString file_path) {
    current_category = "default";

    std::ifstream f (file_path.c_str());
    while (f.good()) {
        AString line;
        getline(f, line);
        this->parse_line(line);
    }
}

AString Data::as_string(const std::string& category, const std::string& field) {
    AString _category = category;
    if (category == "") _category = "default";

    if (! HasField(_category, field) )
        return AString("");

    return values[_category][field];
}

bool Data::as_bool(const std::string& category, const std::string& field) {
    AString _category = category;
    if (category == "") _category = "default";

    if (! HasField(_category, field) )
        return false;

    AString v = values[_category][field];
    if (v == "true") return true;
    if (v == "yes") return true; //Oh well, why not?
    if (v == "on") return true;
    if (v == "enabled") return true;
    if (v == "1") return true;

    return false;
}

int Data::as_int(const std::string& category, const std::string& field) {
    AString _category = category;
    if (category == "") _category = "default";

    if (! HasField(_category, field) )
        return 0;

    AString v = values[_category][field];
    if (v == "") return 0;

    int i;
    std::stringstream(v) >> i;
    return i;
}

float Data::as_float(const std::string& category, const std::string& field) {
    AString _category = category;
    if (category == "") _category = "default";

    if (! HasField(_category, field) )
        return 0;

    AString v = values[_category][field];
    if (v == "") return 0.0f;

    float i;
    std::stringstream(v) >> i;
    return i;
}

sf::Color Data::as_Color(const std::string& category, const std::string& field) {
    AString _category = category;
    if (category == "") _category = "default";

    if (! HasField(_category, field) )
        return sf::Color(0,0,0);

    AString v = values[_category][field];
    std::stringstream ss(v);
    unsigned int r, g, b, a;
    ss >> r >> g >> b;

    if (!ss.eof())
        ss >> a;
    else
        a = 255;

    return sf::Color(r, g, b, a);
}

sf::Vector2i Data::as_Vector2i(const std::string& category, const std::string& field) {
    AString _category = category;
    if (category == "") _category = "default";

    if (! HasField(_category, field) )
        return sf::Vector2i(0, 0);

    AString v = values[_category][field];
    std::stringstream ss(v);
    sf::Vector2i vec;
    ss >> vec.x >> vec.y;
    return vec;
}

sf::Vector2u Data::as_Vector2u(const std::string& category, const std::string& field) {
    AString _category = category;
    if (category == "") _category = "default";

    if (! HasField(_category, field) )
        return sf::Vector2u(0, 0);

    AString v = values[_category][field];
    std::stringstream ss(v);
    sf::Vector2u vec;
    ss >> vec.x >> vec.y;
    return vec;
}

sf::Vector2f Data::as_Vector2f(const std::string& category, const std::string& field) {
    AString _category = category;
    if (category == "") _category = "default";

    if (! HasField(_category, field) )
        return sf::Vector2f(0, 0);

    AString v = values[_category][field];
    std::stringstream ss(v);
    sf::Vector2f vec;
    ss >> vec.x >> vec.y;
    return vec;
}

std::vector<AString> Data::as_str_vector(const std::string& category, const std::string& field) {
    AString _category = category;
    if (category == "") _category = "default";

    if (! HasField(_category, field) )
        return std::vector<AString>();

    return split(values[_category][field], ' ');
}

std::vector<int> Data::as_int_vector(const std::string& category, const std::string& field) {
    AString _category = category;
    if (category == "") _category = "default";

    if (! HasField(_category, field) )
        return std::vector<int>();

    std::vector<AString> s = split(values[_category][field], ' ');
    std::vector<int> result;

    for (unsigned int i=0; i < s.size(); i++) {
        int value;
        std::stringstream ss(s[i]);
        ss >> value;
        result.push_back(value);
    }
    return result;
}

std::vector<float> Data::as_float_vector(const std::string& category, const std::string& field) {
    AString _category = category;
    if (category == "") _category = "default";

    if (! HasField(_category, field) )
        return std::vector<float>();

    std::vector<AString> s = split(values[_category][field], ' ');
    std::vector<float> result;

    for (unsigned int i=0; i < s.size(); i++) {
        float value;
        std::stringstream ss(s[i]);
        ss >> value;
        result.push_back(value);
    }
    return result;
}

std::set<AString> Data::GetKeys(const std::string& category, const std::string& field_pattern) const {
    AString _category = category;
    if (category == "") _category = "default";

    if(!HasCategory(_category))
        return std::set<AString>();

    std::set<AString> result;

    std::map<AString, AString>::const_iterator it;

    const std::map<AString, AString>& submap = const_access(values, _category);

    for ( it=submap.begin() ; it != submap.end(); it++ ) {

        if (it->first.compare(0, field_pattern.length(), field_pattern) == 0) {
            result.insert(it->first);
        }
    }

    return result;
}

void Data::Print() {
    std::map<AString, std::map<AString, AString> >::iterator it;
    for ( it=values.begin() ; it != values.end(); it++ ) {
        std::cout << "[" << (*it).first << "]" << std::endl;
        std::map<AString, AString>::iterator it2;
        for ( it2=(*it).second.begin() ; it2 != (*it).second.end(); it2++ ) {
            std::cout << "    " << (*it2).first << " = " << (*it2).second << std::endl;
        }
    }
}

bool Data::HasCategory(const std::string& category) const {
    return contains(values, category);
}

bool Data::HasField(const std::string& category, const std::string& field) const {
    if ( contains(values, category) ) {
        return contains(const_access(values, category), field);
    }
    else {
        return false;
    }
}
