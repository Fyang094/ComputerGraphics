#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

extern bool enable_acceleration;

Render_World::~Render_World()
{
    for(auto a:all_objects) delete a;
    for(auto a:all_shaders) delete a;
    for(auto a:all_colors) delete a;
    for(auto a:lights) delete a;
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
std::pair<Shaded_Object,Hit> Render_World::Closest_Intersection(const Ray& ray) const
{
    Debug_Scope scope;

    Hit intersect;
    Hit intersect_current;

    float min_t = std::numeric_limits<float>::max();

    // Initialize a Shaded_Object to store closest object.
    Shaded_Object closest_object;

    // goes throught all shaded objects
    for (size_t i = 0; i < objects.size(); ++i) {
        for (int j = 0; j < objects[i].object->num_parts; ++j) {
            intersect_current = objects[i].object->Intersection(ray, j);

            // compares closest hit to small t and min t
            if (intersect_current.Valid() && intersect_current.dist <= min_t && intersect_current.dist >= small_t) {
                min_t = intersect_current.dist;
                intersect = intersect_current;
                closest_object = objects[i];
            }
            else{   
                Pixel_Print("no intersection with ", objects[i].object->name);
            }
        }
    }

    Pixel_Print("intersect test with : ", closest_object.object->name, "; hit: ", intersect);
    Pixel_Print("closest intersection; obj: ", closest_object.object->name, "; hit: ", intersect);

    return std::make_pair(closest_object, intersect);
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    // DEBUG  
    Debug_Scope scope;
    // Pixel_Print("debug pixel: -x ", pixel_index[0], " -y ", pixel_index[1]);

    // set up the initial view ray here
    Ray ray(camera.position, (camera.World_Position(pixel_index) - camera.position).normalized());
    vec3 color = Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth) const
{
    vec3 color;
    
    Pixel_Print("cast ray ",ray);

    // determine the color here
    std::pair<Shaded_Object, Hit> closest_hit = Closest_Intersection(ray);
    vec3 surface_point = ray.Point(closest_hit.second.dist);

    //if no intersect
    if (!closest_hit.second.Valid()){
        if(background_shader == nullptr){
            //color is black
            color = vec3 (0,0,0);
        } else {
            color = background_shader->Shade_Surface(*this, ray, Closest_Intersection(ray).second, ray.direction, ray.direction, recursion_depth);
        }

        Pixel_Print("flat shader; color: ", color);
        Pixel_Print("ray direction check: ", ray.direction);

    } else {
        color = closest_hit.first.shader -> Shade_Surface(*this, ray, closest_hit.second, surface_point,
             closest_hit.first.object->Normal(ray,closest_hit.second),recursion_depth);
    }

    return color;
}