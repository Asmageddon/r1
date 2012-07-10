#ifndef UNITTYPE_HPP_
#define UNITTYPE_HPP_

#include <string>
#include <vector>

#include "Object.hpp"
#include "Displayable.hpp"

class UnitType : public Object, public Displayable {
    public:
        std::string type;
        std::vector<std::string> categories;
        u_int32_t max_hp;
    public:
        UnitType();
        UnitType(Data data);
};

#endif