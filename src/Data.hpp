#ifndef DATA_HPP_
#define DATA_HPP_

#include <iostream>
#include <fstream>

#include "AString.hpp"
#include <set>
#include <map>
#include <vector>
#include <sstream>

#include "utils.hpp"

#include <SFML/Graphics.hpp>

class Data {
    private:
        std::map<AString, std::map<AString, AString> > values;
        AString current_category;
    private:
        void parse_line(AString line);
    public:
        Data(AString file_path);

        AString                  as_string     (const std::string& category, const std::string& field);
        bool                     as_bool       (const std::string& category, const std::string& field);
        int                      as_int        (const std::string& category, const std::string& field);
        float                    as_float      (const std::string& category, const std::string& field);
        sf::Color                as_Color      (const std::string& category, const std::string& field);
        sf::Vector2i             as_Vector2i   (const std::string& category, const std::string& field);
        sf::Vector2u             as_Vector2u   (const std::string& category, const std::string& field);
        sf::Vector2f             as_Vector2f   (const std::string& category, const std::string& field);
        std::vector<AString>     as_str_vector (const std::string& category, const std::string& field);
        std::vector<int>         as_int_vector (const std::string& category, const std::string& field);
        std::vector<float>       as_float_vector(const std::string& category, const std::string& field);

        std::set<AString> GetKeys(const std::string& category, const std::string& field_pattern) const;

        bool HasCategory(const std::string& category) const;
        bool HasField(const std::string& category, const std::string& field) const;

        void Print();
};

#endif
