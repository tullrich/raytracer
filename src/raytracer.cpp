#include "raytracer.h"
#include <math.h>
#include <ctime>
#include <boost/filesystem.hpp>

using namespace cgutils;

namespace raytracer {

Raytracer::Raytracer(const string &outputfile, int width, int height) : img(width, height), outpath(outputfile)
{
    scene = NULL;
    srand((unsigned)time(0));
}

Raytracer::~Raytracer()
{
}

RGB Raytracer::emittedRadiance(const Material &mat, const Triangle &tri, const glm::vec4 intersection)
{
    return RGB(0);
}

RGB Raytracer::directRadiance(const Material &mat, const Triangle &tri, const glm::vec4 intersection)
{
    RGB color(0);
    RGB per_light(0);
    TraceResult result;
    Ray to_light;

    glm::vec3 N = tri.normal();
    glm::vec3 point = adjustFloatingPointToward(tri.intersectionToPoint(intersection), N);

    for (Light::light_ptr light : scene->lights)
    {
        // get some light ray for this light
        light->genShadowRay(point, to_light);

        if (scene->testVisibility(to_light, result))
        {
            light->getAttenuatedRadiance(to_light, per_light);

            glm::vec3 I = to_light.n;
            color += per_light * (mat.diffuse/ (float) M_PI) * fmaxf(glm::dot(N, I), 0);
        }
    }

    return color;
}

RGB Raytracer::indirectRadiance(const Material &mat, const Triangle &tri, const glm::vec4 intersection, int depth)
{
    RGB indirect_color(0);
    glm::vec3 N = tri.normal();
    glm::vec3 point = adjustFloatingPointToward(tri.intersectionToPoint(intersection), N);


    int numIndirectRays = 1;
    for (int i = 0; i < numIndirectRays; i++)
    {
        float survive = 1.0f;
        if(depth > 0)
        {
            float inverse_pdf = 0;
            glm::vec3 rand_direction = uniformImportanceSampling(N, inverse_pdf);
            Ray indirect_ray(point, point + rand_direction);
            RGB per_ray_color; 
            traceRay(indirect_ray, per_ray_color, depth - 1);
            indirect_color +=  survive * per_ray_color * mat.diffuse * fmaxf(glm::dot(N, rand_direction), 0) * 2.0f;
        }
        
    }
    return indirect_color / (float) numIndirectRays;

    //return indirect_color / (float) numIndirectRays;
}
static int once = false;


bool Raytracer::traceRay(const Ray &r, RGB &color, int depth)
{
    TraceResult result;

    if(scene->traceRay(r, result))
    {
        if (!once)
        {
            std::cout << " got a hit on mesh "  << std::endl;
            once = true;
        }
        color = emittedRadiance(*(result.material),  result.tri,  result.intersection);
        color += directRadiance(*(result.material),  result.tri,  result.intersection);
        color += indirectRadiance(*(result.material),  result.tri,  result.intersection, depth);
        return true;
    }

    color = RGB(0.1f, 0.1f, 0.1f);
    return false;
}

static int count = 0;

void Raytracer::lightPixel(int u, int v)
{
    RGB color;
    Ray r;

    int numViewRays = 500;
    for(int i = 0; i < numViewRays ; i++)
    {
        RGB view_ray(0);
        camera->genViewingRay(u, v, r);
        traceRay(r, view_ray, 2);
        color += view_ray;
    }

    color /= numViewRays;

    img.setPixelColor(u, v , color);
}

void Raytracer::trace(int u_min, int u_max)
{
    for (int u = u_min; u < u_max; ++u)
    {
        for (int v = 0; v < img.height; ++v)
        {
            lightPixel(u, v);
            count++;
            std::cout << 100.0f * count / (float)(img.width * img.height) << "%" << std::endl;
        }
    }
}

void Raytracer::run()
{
    CGUTILS_ASSERT(scene);
    CGUTILS_ASSERT(camera);

    boost::thread_group group;

    int numThreads = 8;
    int columnsPerThread = img.width / numThreads;
    for (int i = 0; i < numThreads; i++)
    {  

        group.add_thread(new boost::thread(&Raytracer::trace, this, i * columnsPerThread, (i+1) * columnsPerThread));
    }
    group.join_all();

    if (!img.writeOut(outpath))
    {
        std::cout << "Error: FreeImage_Save()" << std::endl;
    }
}

bool Raytracer::russianRoulette(const RGB &reflectance, float &survivor)
{
    float p = fmax(reflectance[0], fmax(reflectance[1], reflectance[2]));
    survivor = 1.0/p;
    if (randf() > p)
    {
        //std::cout << "russianRoulette stop" << std::endl; 
        return true;
    }
    //std::cout << "russianRoulette keep going" << std::endl; 
    return false;
}

Raytracer* RaytraceBuilder::buildRaytracer()
{
    return new Raytracer(outputfile, width, height);
}

std::ostream& operator<<(std::ostream& o, const glm::vec3& b)
{
    return  o << "<" << b.x << ", " << b.y << ", " << b.z << ">";
}

std::ostream& operator<<(std::ostream& o, const glm::mat4& b)
{
    o << "\t[" << b[0][0] << ", " << b[1][0] << ", " << b[2][0] << ", " << b[3][0] << std::endl;
    o << "\t " << b[0][1] << ", " << b[1][1] << ", " << b[2][1] << ", " << b[3][1] << std::endl;
    o << "\t " << b[0][2] << ", " << b[1][2] << ", " << b[2][2] << ", " << b[3][2] << std::endl;
    o << "\t " << b[0][3] << ", " << b[1][3] << ", " << b[2][3] << ", " << b[3][3] << "]";

    return o;
}

std::ostream& operator<<(std::ostream& o, const Triangle& b)
{
    return  o << "Triangle {" << b.A << ", " << b.B << ", " << b.C << " }";
}

glm::vec3 Triangle::intersectionToPoint(const glm::vec4 &intersection) const
{
    return intersection.x * A + intersection.y * B + intersection.z * C;
}

glm::vec3 Triangle::normal() const
{
    return glm::normalize(glm::cross(B - A, C - A));
}


glm::vec3 adjustFloatingPointToward(const glm::vec3 point, const glm::vec3 &n)
{
    //glm::vec3 n = towards - point;

    return point + .00001f * n;
}

glm::vec3 uniformDirectionOnHemisphere(const glm::vec3 normal)
{
    // sample 3-axis in uniform direction
    glm::vec3 rand_direction(randf() - 0.5f, randf() - 0.5f, randf() - 0.5f);

    if(glm::dot(rand_direction, normal) < 0)
    {
        rand_direction = -rand_direction;
    }

    return glm::normalize(rand_direction);
}

glm::vec3 Raytracer::uniformImportanceSampling(const glm::vec3 normal, float &inverse_pdf)
{
    inverse_pdf = 2.0f * (float)M_PI;
    return uniformDirectionOnHemisphere(normal);
}

glm::vec3 Raytracer::cosineImportanceSampling(const glm::vec3 normal, float &inverse_pdf)
{
     glm::vec3 rand_direction = uniformDirectionOnHemisphere(normal);

     float cos_theta = glm::dot(rand_direction, normal);
     //std::cout << "cos_theta " << cos_theta << std::endl; 
     inverse_pdf = M_PI / cos_theta;
}

std::string parentPath(const std::string filename)
{
    std::string filename_noext;
    filename_noext = boost::filesystem::path(filename).parent_path().string();

    std::cout << "fillll " << filename << std::endl; 
    return filename_noext;
}

std::string appendFilename(const std::string parent_path, const std::string filename)
{
    boost::filesystem::path full_path(parent_path);
    full_path /= filename; // appends filename to full_path with the preferred separator

    return full_path.make_preferred().string();
}

} /* namespace raytracer */
