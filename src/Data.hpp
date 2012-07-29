#ifndef DATA_HPP_
#define DATA_HPP_

#include "AString.hpp"
#include <set>
#include <map>

#include <SFML/Graphics.hpp>

class Data;

class _DataProxy {
    private:
        const Data *data;
        std::string category;
    public:
        _DataProxy(const Data *parent, const std::string& category);
        AString operator[](const std::string& field) const;
        std::set<AString> GetKeys(const std::string& field_pattern = "") const;
        bool HasField(const std::string& field) const;
};

class Data {
    friend class _DataProxy;
    private:
        std::map<AString, std::map<AString, AString> > values;
        AString current_category;
    private:
        void parse_line(AString line);
    public:
        Data(AString file_path);

        _DataProxy operator[](const std::string& category) const;

        std::set<AString> GetKeys(const std::string& category_pattern = "") const;

        bool HasCategory(const std::string& category) const;

};

#endif
