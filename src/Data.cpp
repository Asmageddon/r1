#include "Data.hpp"

void Data::parse_line(string line) {
    if (line.length() == 0) return;

    int stage = 0;
    string field;
    string value;
    string category;

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

Data::Data(string file_path) {
    current_category = "default";

    ifstream f (file_path.c_str());
    while (f.good()) {
        string line;
        getline(f, line);
        this->parse_line(line);
    }
}
map<string, string> Data::operator[] (const string& idx) {
    if (idx == "")
        return this->values["default"];
    return this->values[idx];
}
string Data::as_string(const string& category, const string& field, bool allow_empty) {
    string _category = category;
    if (category == "") _category = "default";
    return values[_category][field];
}
int Data::as_int(const string& category, const string& field, bool allow_empty) {
    string _category = category;
    if (category == "") _category = "default";
    string v = values[_category][field];
    if (v == "") return 0;
    int i;
    stringstream(v) >> i;
    return i;
}
Color Data::as_Color(const string& category, const string& field, bool allow_empty) {
    string _category = category;
    if (category == "") _category = "default";
    string v = values[_category][field];
    stringstream ss(v);
    unsigned int r, g, b;
    ss >> r >> g >> b;
    return Color(r, g, b);
}
Vector2i Data::as_Vector2i(const string& category, const string& field, bool allow_empty) {
    string _category = category;
    if (category == "") _category = "default";
    string v = values[_category][field];
    stringstream ss(v);
    Vector2i vec;
    ss >> vec.x >> vec.y;
    return vec;
}

vector<string> Data::as_str_vector(const string& category, const string& field, bool allow_empty) {
    string _category = category;
    if (category == "") _category = "default";
    return split(values[_category][field], ' ');
}

vector<int> Data::as_int_vector(const string& category, const string& field, bool allow_empty) {
    string _category = category;
    if (category == "") _category = "default";
    vector<string> s = split(values[_category][field], ' ');
    vector<int> result;
    for (unsigned int i=0; i < s.size(); i++) {
        int value;
        stringstream ss(s[i]);
        ss >> value;
        result.push_back(value);
    }
    return result;
}

void Data::print() {
    map<string, map<string, string> >::iterator it;
    for ( it=values.begin() ; it != values.end(); it++ ) {
        cout << "[" << (*it).first << "]" << endl;
        map<string, string>::iterator it2;
        for ( it2=(*it).second.begin() ; it2 != (*it).second.end(); it2++ ) {
            cout << "    " << (*it2).first << " = " << (*it2).second << endl;
        }
    }
}