#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <string>
#include <vector>
using namespace std;

#include "Data.hpp"

class Object {
    public:
        string id;
        string name;
        string desc;
    public:
        Object() {}
        Object(Data data);
};

#endif