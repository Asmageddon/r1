#ifndef DATA_HPP_
#define DATA_HPP_

#include <iostream>
#include <fstream>

#include <string>
#include <map>
#include <vector>
#include <sstream>
using namespace std;

#include "utils.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

class Data {
    private:
        map<string, map<string, string> > values;
        string current_category;
    private:
        void parse_line(string line);
    public:
        Data(string file_path);

        map<string, string> operator[] (const string& idx);

        string as_string(const string& category, const string& field, bool allow_empty=false);

        int as_int(const string& category, const string& field, bool allow_empty=false);

        Color as_Color(const string& category, const string& field, bool allow_empty=false);

        Vector2i as_Vector2i(const string& category, const string& field, bool allow_empty=false);

        vector<string> as_str_vector(const string& category, const string& field, bool allow_empty=false);

        vector<int> as_int_vector(const string& category, const string& field, bool allow_empty=false);


        void print();
};

#endif