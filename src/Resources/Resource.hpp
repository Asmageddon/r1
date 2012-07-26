#ifndef RESOURCE_HPP_
#define RESOURCE_HPP_

#include "../AString.hpp"

#include "../Data.hpp"

class Resource {
    public:
        AString id;
        AString name;
        AString desc;
    public:
        Resource() {}
        Resource(Data data);
};

#endif
