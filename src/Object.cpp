#include "Object.hpp"

Object::Object(Data data) {
    id = data.as_string("", "id");
    name = data.as_string("", "name");
    desc = data.as_string("", "desc");
}