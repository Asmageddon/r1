#include "Resource.hpp"

#include "../AString.hpp"

#include "../Data.hpp"

Resource::Resource(Data data) {
    id = data[""]["id"];
    name = data[""]["name"];
    desc = data[""]["desc"];
}
