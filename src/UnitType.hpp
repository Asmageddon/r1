#ifndef UNITTYPE_HPP_
#define UNITTYPE_HPP_

#include <string>
#include <vector>

#include "Object.hpp"
#include "Displayable.hpp"

class ResourceManager;

class UnitType : public Object, public Displayable {
    public:
        std::string type;
        std::vector<std::string> categories;
        std::string material;
        u_int32_t max_hp;

        ResourceManager *resman;
    public:
        UnitType() { /* WARNING: This does not initialize anything */ };
        UnitType(ResourceManager *resman, Data data);
};

#endif