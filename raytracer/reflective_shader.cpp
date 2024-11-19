#include "reflective_shader.h"
#include "parse.h"
#include "ray.h"
#include "render_world.h"

Reflective_Shader::Reflective_Shader(const Parse* parse,std::istream& in)
{
    in>>name;
    shader=parse->Get_Shader(in);
    in>>reflectivity;
    reflectivity=std::max(0.0,std::min(1.0,reflectivity));
}

vec3 Reflective_Shader::
Shade_Surface(const Render_World& render_world,const Ray& ray,const Hit& hit,
    const vec3& intersection_point,const vec3& normal,int recursion_depth) const
{
    vec3 color;

    color = shader->Shade_Surface(render_world, ray, hit, intersection_point, normal, recursion_depth);

    vec3 incident_dir = ray.direction;
    vec3 reflection_vec = incident_dir - 2.0 * dot(incident_dir, normal) * normal;

    Ray reflect_ray(intersection_point + 0.001 * normal, reflection_vec);

    if(recursion_depth < render_world.recursion_depth_limit){
        vec3 reflect_color = render_world.Cast_Ray(reflect_ray, recursion_depth + 1);
        color = (1.0 - reflectivity) * color + reflectivity * reflect_color;
    } else {
        color *= (1.0 - reflectivity);
    }

    return color;
}
