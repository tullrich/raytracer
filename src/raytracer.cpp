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

RGB Raytracer::emittedRadiance(const TraceResult &r)
{
    return 2.0f * r.p->Le(r.intersection);
}

RGB Raytracer::directRadiance(const TraceResult &r)
{
    RGB color(0);
    RGB per_light(0);
    TraceResult result;
    Ray to_light;

    glm::vec3 point = r.biasedIntersectionPoint();

    for (Light::light_ptr light : scene->lights)
    {
        // get some light ray for this light
        light->genShadowRay(point, to_light);

        if (scene->testVisibility(to_light, result))
        {
            light->getAttenuatedRadiance(to_light, per_light);
            color += per_light * (r.p->BRDF(r.intersection) / (float) M_PI) * r.p->geometricTerm(to_light.n);
        }
    }

    return color;
}

RGB Raytracer::indirectRadiance(const TraceResult &r, int depth)
{
    RGB indirect_color(0);
    glm::vec3 point = r.biasedIntersectionPoint();


    int numIndirectRays = 1;
    for (int i = 0; i < numIndirectRays; i++)
    {
        float survive = 1.0f;
        if(depth > 0)
        {
            float inverse_pdf = 0;
            glm::vec3 rand_direction = uniformImportanceSampling(r.p->surfaceNormal(), inverse_pdf);
            Ray indirect_ray(point, point + rand_direction);
            RGB per_ray_color; 
            traceRay(indirect_ray, per_ray_color, depth - 1);
            indirect_color +=  survive * per_ray_color * r.p->BRDF(r.intersection) * r.p->geometricTerm(rand_direction) * 2.0f;
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
        color = emittedRadiance(result);
        color += directRadiance(result);
        color += indirectRadiance(result, depth);
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

    int numViewRays = 50;
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

glm::vec3 Raytracer::uniformImportanceSampling(const glm::vec3 normal, float &inverse_pdf)
{
    inverse_pdf = 2.0f * (float)M_PI;
    return uniformDirectionOnHemisphere(normal);
}

Raytracer* RaytraceBuilder::buildRaytracer()
{
    return new Raytracer(outputfile, width, height);
}

} /* namespace raytracer */
