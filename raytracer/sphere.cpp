#include "sphere.h"
#include "ray.h"

Sphere::Sphere(const Parse* parse, std::istream& in)
{
    in>>name>>center>>radius;
}

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    Hit result;

    // initialize variables
    vec3 ec = ray.endpoint - center;

    // at^2 + bt + c
    double a = dot(ray.direction, ray.direction); 
    double b = 2 * dot(ray.direction, ec); 
    double c = dot(ec, ec) - radius * radius; 

    // discriminant
    double disc = b * b - 4 * a * c;

    if(disc >= 0){
        // calculate first intersection point
        double t1 = (-b - sqrt(disc)) / (2 * a);

        // if intersection point is a valid value, then return the point
        if (t1 > small_t){
            result.dist = t1;
            return result;
        }
    }

    return result;


    /*
    double discrim = pow(dot(ray.direction,ray.endpoint - center),2) - (dot(ray.direction, ray.direction)) * 
        (dot(ray.endpoint - center,ray.endpoint - center)- pow(radius,2));

    //if discriminant is negative then no intersection
    double t_plus = -dot(ray.direction,ray.endpoint - center);
    double t_minus = -dot(ray.direction,ray.endpoint - center);

    if (discrim < 0) {
        return {-1,part,{}};
    }
    else if (discrim >= 0) {
        t_plus = t_plus + sqrt(discrim) / dot(ray.direction, ray.direction);
        t_minus = t_minus - sqrt(discrim) / dot(ray.direction, ray.direction);

        if (t_plus > t_minus && t_plus >= small_t) {
            return {t_plus, part, {}};
        }
        else if (t_minus >= t_plus && t_minus >= small_t) {
            return {t_minus, part, {}};
        }
    } */

    return result;
}

vec3 Sphere::Normal(const Ray& ray, const Hit& hit) const
{
    vec3 normal;

    // compute the normal direction
    normal = (ray.Point(hit.dist) - center) / radius;

    return normal;
}

std::pair<Box,bool> Sphere::Bounding_Box(int part) const
{
    return {{center-radius,center+radius},false};
}
