#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <string>
#include <vector>

#include "Data.hpp"

class Object {
    public:
        std::string id;
        std::string name;
        std::string desc;
    public:
        Object() {}
        Object(Data data);
};

#endif
