#include "light.h"
#include "parse.h"
#include "object.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"

Phong_Shader::Phong_Shader(const Parse* parse,std::istream& in)
{
    in>>name;
    color_ambient=parse->Get_Color(in);
    color_diffuse=parse->Get_Color(in);
    color_specular=parse->Get_Color(in);
    in>>specular_power;
}

vec3 Phong_Shader::
Shade_Surface(
    const Render_World& render_world,const Ray& ray,const Hit& hit,
    const vec3& intersection_point,const vec3& normal,int recursion_depth) const
{
    //determine the color
    vec3 color;
    
    // initialize reflections 
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // debugging
    Debug_Scope scope;

    // light direction
    vec3 light_dir = ray.direction;

    // solves for ambient reflection (RaLa)
    if (render_world.ambient_color != nullptr){
        ambient = color_ambient->Get_Color(hit.uv) * render_world.ambient_intensity * render_world.ambient_color->Get_Color(hit.uv);
    } else {
        // use only the ambient intensity 
        ambient = color_ambient->Get_Color(hit.uv) * render_world.ambient_intensity;
    }
    color += ambient;

    // iterates through lights in render world
    for ( const Light* light :render_world.lights) {
        //check if shadows have been enabled
        if(render_world.enable_shadows){
            Ray shadow_ray(intersection_point, light->position - intersection_point);
            std::pair<Shaded_Object, Hit> shadow_hit = render_world.Closest_Intersection(shadow_ray);\

            if(shadow_hit.second.dist > (light->position - intersection_point).magnitude() || !shadow_hit.second.Valid()){
                // solve for diffuse (RdLdmax(n.1, 0))
                diffuse = color_diffuse->Get_Color(hit.uv) * light->Emitted_Light(light->position - intersection_point);
                diffuse *= std::max(dot((light->position - intersection_point).normalized(), normal), 0.0);
                color += diffuse;

                // solve for specular
                specular = color_specular->Get_Color(hit.uv) * light->Emitted_Light(light->position - intersection_point);
                specular *= std::pow(std::max(dot((2 * dot((light->position - intersection_point), normal) * normal 
                    - (light->position - intersection_point)).normalized(), -light_dir), 0.0), specular_power); 
                color += specular;
            }
        } else {
           // solve for diffuse (RdLdmax(n.1, 0))
            diffuse = color_diffuse->Get_Color(hit.uv) * light->Emitted_Light(light->position - intersection_point);
            diffuse *= std::max(dot((light->position - intersection_point).normalized(), normal), 0.0);
            color += diffuse;

            // solve for specular
            specular = color_specular->Get_Color(hit.uv) * light->Emitted_Light(light->position - intersection_point);
            specular *= std::pow(std::max(dot((2 * dot((light->position - intersection_point), normal) * normal 
                - (light->position - intersection_point)).normalized(), -light_dir), 0.0), specular_power);
            color += specular;
        }
    }

    Pixel_Print("ambient: ", ambient);
    Pixel_Print("diffuse: ", diffuse);
    Pixel_Print("specular: ", specular);

    //color = ambient + diffuse + specular;
    Pixel_Print("color: ", color);
    return color;
}