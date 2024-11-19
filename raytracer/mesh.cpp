#include "mesh.h"
#include <fstream>
#include <limits>
#include <string>
#include <algorithm>

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

Mesh::Mesh(const Parse* parse, std::istream& in)
{
    std::string file;
    in>>name>>file;
    Read_Obj(file.c_str());
}

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e, t;
    vec3 v;
    vec2 u;
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }

        if(sscanf(line.c_str(), "vt %lg %lg", &u[0], &u[1]) == 2)
        {
            uvs.push_back(u);
        }

        if(sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &e[0], &t[0], &e[1], &t[1], &e[2], &t[2]) == 6)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
            for(int i=0;i<3;i++) t[i]--;
            triangle_texture_index.push_back(t);
        }
    }
    num_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    Hit intersect = {-1, -1, {}};

    // tests for specified triangle part
    if (part >= 0){
        intersect = Intersect_Triangle(ray, part);
    } else if (part < 0 && !triangles.empty()){
        intersect = Intersect_Triangle(ray,0);

        // loop through the remaining traingles in mesh 
        for(size_t i = 1; i < triangles.size(); i++){
            Hit current_hit = Intersect_Triangle(ray, i);

            // tests if current intersection is closer than the previous 
            if(current_hit.Valid() && current_hit.dist < intersect.dist){
                intersect = current_hit;
                intersect.triangle = i;
            }
        }
    }

    return intersect;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const Ray& ray, const Hit& hit) const
{
    assert(hit.triangle>=0);
    
    vec3 v1 = vertices[triangles[hit.triangle][0]];
    vec3 v2 = vertices[triangles[hit.triangle][1]];
    vec3 v3 = vertices[triangles[hit.triangle][2]];
    vec3 normal = cross((v1 - v2), (v1 - v3)).normalized();

    return normal;
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
Hit Mesh::Intersect_Triangle(const Ray& ray, int tri) const
{
    // Define traingle vertices (v = vertex)
    vec3 vA = vertices[triangles[tri][0]];
    vec3 vB = vertices[triangles[tri][1]];
    vec3 vC = vertices[triangles[tri][2]];

    // Compute normal vector of triangle
    vec3 sBA = vB - vA;
    vec3 sCA = vC - vA;
    vec3 normal = cross(sBA, sCA).normalized();

    // check if ray is parallel to the triangle (thus no intersections)
    if (!dot(ray.direction, normal)){
        return {-1, -1, {}};
    }

    // compute distances 
    double distTop = dot(normal, (vA - ray.endpoint));
    double distBot = dot(normal, ray.direction);
    double dist = distTop / distBot;

    vec3 interPoint = ray.Point(dist);

    // compute barycentric coordinates 
    double alpha = (dot(cross((vB - interPoint), (vC - interPoint)), normal)) / (cross(sCA, sBA).magnitude());
    double beta = (dot(cross((interPoint - vA), (vC - vA)), normal)) / (cross(sCA, sBA).magnitude());
    double gamma = 1 - beta - alpha;

    if((alpha > -weight_tolerance) && (beta > -weight_tolerance) && (gamma > -weight_tolerance) && (dist > small_t)){
        return {dist, tri, vec2(alpha, beta)};
    }

    // if no valid intersection is found 
    return {-1, -1, {}};

}

std::pair<Box,bool> Mesh::Bounding_Box(int part) const
{
    if(part<0)
    {
        Box box;
        box.Make_Empty();
        for(const auto& v:vertices)
            box.Include_Point(v);
        return {box,false};
    }

    ivec3 e=triangles[part];
    vec3 A=vertices[e[0]];
    Box b={A,A};
    b.Include_Point(vertices[e[1]]);
    b.Include_Point(vertices[e[2]]);
    return {b,false};
}
