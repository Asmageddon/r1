#include "Data.hpp"

#include <fstream>
#include <istream>

#include "utils.hpp"


_DataProxy::_DataProxy(const Data *parent, const std::string& category) : data(parent), category(category) { }

AString _DataProxy::operator[](const std::string& field) const {
    if (HasField(field)) {
        return const_access(const_access(data->values, category), field);
    }
    else {
        return AString("");
    }
}

std::set<AString> _DataProxy::GetKeys(const std::string& field_pattern) const {
    std::set<AString> result;

    const std::map<AString, AString>& submap = const_access(data->values, category);
    std::map<AString, AString>::const_iterator it;

    for ( it=submap.begin() ; it != submap.end(); it++ ) {

        if (it->first.startswith(field_pattern)) {
            result.insert(it->first);
        }
    }

    return result;
}

bool _DataProxy::HasField(const std::string& field) const {
    return contains(const_access(data->values, category), field);
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

void Data::parse_line(AString line) {
    line = line.strip(" \t");
    if (line.startswith("#")) return;

    if (line.contains('=')) {
        std::vector<AString> vec = line.split("=");
        vec[0] = vec[0].strip(" ");
        if (vec[1].contains('#'))
            vec[1] = vec[1].lpartition("#")[0];
        vec[1] = vec[1].strip(" ");


        values[current_category][vec[0]] = vec[1];
    }
    else {
        if (line.startswith("[")) {
            current_category = line.strip("[]");
        }
    }
    //if (line.length() == 0) return;

    //int stage = 0;
    //AString field;
    //AString value;
    //AString category;

    //for (unsigned int i = 0; i < line.length(); i++) {
        //char c = line[i];
        //if (stage == 0) {
            //if ( (c == ' ') || (c == '\t') )
                //continue;
            //else if (c == '=')
                //stage = 1;
            //else if (c == '#')
                //return;
            //else if (c == '[')
                //stage = 3;
            //else
                //field += c;
        //}
        //else if (stage == 1) {
            //if ( (c == ' ') || (c == '\t') )
                //continue;
            //else {
                //stage = 2;
                //value += c;
            //}
        //}
        //else if (stage == 2) {
            //value += c;
        //}
        //else if (stage == 3) {
            //if ( c == ']' ) {
                //current_category = category;
                //return;
            //}
            //else
                //category += c;
        //}
    //}
    //this->values[current_category][field] = value;
}

_DataProxy Data::operator[](const std::string& category) const {
    if (category == "")
        return _DataProxy(this, "default");
    else
        return _DataProxy(this, category);
}

bool Data::HasCategory(const std::string& category) const {
    return contains(values, category);
}


std::set<AString> Data::GetKeys(const std::string& category_pattern) const {
    std::set<AString> result;

    std::map<AString, std::map<AString, AString> >::const_iterator it;

    for ( it=values.begin() ; it != values.end(); it++ ) {

        if (it->first.startswith(category_pattern)) {
            result.insert(it->first);
        }
    }

    return result;
}
