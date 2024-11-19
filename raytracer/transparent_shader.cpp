#include "transparent_shader.h"
#include "parse.h"
#include "ray.h"
#include "render_world.h"
#include <cmath> 

Transparent_Shader::
Transparent_Shader(const Parse* parse,std::istream& in)
{
    in>>name>>index_of_refraction>>opacity;
    shader=parse->Get_Shader(in);
    assert(index_of_refraction>=1.0);
}

// Use opacity to determine the contribution of this->shader and the Schlick
// approximation to compute the reflectivity.  This routine shades transparent
// objects such as glass.  Note that the incoming and outgoing indices of
// refraction depend on whether the ray is entering the object or leaving it.
// You may assume that the object is surrounded by air with index of refraction
// 1.
vec3 Transparent_Shader::
Shade_Surface(const Render_World& render_world,const Ray& ray,const Hit& hit,
    const vec3& intersection_point,const vec3& normal,int recursion_depth) const
{
    vec3 result_color;
    vec3 base_color = shader->Shade_Surface(render_world, ray, hit, intersection_point, normal, recursion_depth);

    // calculate rays 
    vec3 incident = -ray.direction;
    vec3 reflected_dir = 2.0 * normal * dot(normal, incident) - incident;
    Ray reflected_ray(intersection_point, reflected_dir);
    vec3 reflected_color = render_world.Cast_Ray(reflected_ray, ++recursion_depth);

    // calculate opacity and refraction
    double air_refractive_index = 1.0;
    double material_refractive_index = index_of_refraction;
    
    // calcualte angles/vectors for refraction
    vec3 view_direction = -ray.direction;
    double cos_theta_in = dot(normal, view_direction);
    double sin_theta_in = sqrt(1.0 - cos_theta_in * cos_theta_in);
    double sin_theta_out = (air_refractive_index / material_refractive_index) * sqrt(1.0 - cos_theta_in * cos_theta_in);
    double cos_theta_out = sqrt(1.0 - (air_refractive_index * air_refractive_index / (material_refractive_index * material_refractive_index)) * (1.0 - cos_theta_in * cos_theta_in));

    // calculate ray directions
    vec3 transverse;
    if(dot(ray.direction, normal) < 0){
        transverse = (view_direction - cos_theta_in * normal) / sin_theta_in;
    } else if (dot(ray.direction, normal) > 0){
        transverse = (view_direction - cos_theta_in * -normal) / sin_theta_in;
    }

    vec3 trans_dir = cos_theta_out * (-normal) + sin_theta_out * (-transverse);
    
    vec3 trans_color;
    Ray trans_ray(intersection_point, trans_dir);
    trans_color = render_world.Cast_Ray(trans_ray, ++recursion_depth);

    double exit_cosine = dot(normal, trans_dir);

    // Schlick's approx. for reflectance
    double reflect_at_normal = pow((air_refractive_index - material_refractive_index) / (air_refractive_index + material_refractive_index), 2.0);
    double reflect = 0.0;

    if(dot(ray.direction, normal) < 0){
        reflect = reflect_at_normal + (1.0 - reflect_at_normal) * pow((1.0 - cos_theta_in), 5.0);
    } else if (dot(ray.direction, normal) > 0){
        reflect = reflect_at_normal + (1.0 - reflect_at_normal) * pow((1.0 - exit_cosine), 5.0);
    }

    if(render_world.recursion_depth_limit > recursion_depth){
        if(sin_theta_in > 1.0) {
            reflect = 1.0;
            result_color = base_color * opacity + (1.0 - opacity) * reflect * reflected_color + (1.0 - reflect) * trans_color;
        } else {
            result_color = base_color * opacity + (1.0 - opacity) * reflect * reflected_color + (1.0 - reflect) * trans_color;
        }
    } else {
        result_color = render_world.background_shader->Shade_Surface(render_world, ray, hit, intersection_point, normal, recursion_depth);
    }

    return result_color;
}
