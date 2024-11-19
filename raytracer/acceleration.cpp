#include "acceleration.h"
#include "object.h"
#include "hit.h"

extern int acceleration_grid_size;

Acceleration::Acceleration()
{
    domain.Make_Empty();
    num_cells.fill(acceleration_grid_size);
}

void Acceleration::Add_Object(const Object* obj, int id)
{
    int objectId = id;
    for(const auto& triangle : obj->triangles){
        for(int i = 0; i < 3; ++i){
            vec3 cellIndex = Cell_Index(triangle.vertices[i]);
        }
    }
}

ivec3 Acceleration::Cell_Index(const vec3& pt) const
{
    TODO;
    return {};
}

void Acceleration::Initialize()
{
    TODO;
}

std::pair<int,Hit> Acceleration::Closest_Intersection(const Ray& ray) const
{
    TODO;
    return {};
}
