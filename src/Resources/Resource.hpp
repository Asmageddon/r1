#ifndef RESOURCE_HPP_
#define RESOURCE_HPP_

#include <string>

#include "../Data.hpp"

class Resource {
    public:
        std::string id;
        std::string name;
        std::string desc;
    public:
        Resource() {}
        Resource(Data data);
};

#endif
