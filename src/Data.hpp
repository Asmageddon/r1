#ifndef DATA_HPP_
#define DATA_HPP_

#include <iostream>
#include <fstream>

#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "utils.hpp"

#include <SFML/Graphics.hpp>

class Data {
    private:
        std::map<std::string, std::map<std::string, std::string> > values;
        std::string current_category;
    private:
        void parse_line(std::string line);
    public:
        Data(std::string file_path);

        std::string              as_string     (const std::string& category, const std::string& field, bool allow_empty=false);
        int                      as_int        (const std::string& category, const std::string& field, bool allow_empty=false);
        sf::Color                as_Color      (const std::string& category, const std::string& field, bool allow_empty=false);
        sf::Vector2i             as_Vector2i   (const std::string& category, const std::string& field, bool allow_empty=false);
        std::vector<std::string> as_str_vector (const std::string& category, const std::string& field, bool allow_empty=false);
        std::vector<int>         as_int_vector (const std::string& category, const std::string& field, bool allow_empty=false);

        bool HasCategory(const std::string& category);
        bool HasField(const std::string& category, const std::string& field);

        void Print();
};

#endif