#include "raytracer.h"
#include <math.h>

using namespace cgutils;

namespace raytracer {

Raytracer::Raytracer(const string &outputfile, int width, int height) : img(width, height), outpath(outputfile)
{
    scene = NULL;
}

Raytracer::~Raytracer()
{
}

void Raytracer::computeLightAt(const glm::vec3 point, RGB &color)
{
    RGB per_light(0);

    for (Light::light_ptr light : scene->lights)
    {
        float d = 0.0f;
        //std::cout << *light << std::endl;
        if (scene->testVisibility(point, light->position, d))
        {
            light->getAttenuatedRadiance(point, d, per_light);
            //std::cout << "per light distance " << d << std::endl;
            color += per_light;
        }
    }
}

static int once = false;

void Raytracer::rayForPixel(int x, int y, Ray &r) const
{

    glm::vec3 pixel_center(0);
    camera->getPixelCenter(x, y, img.width, img.height, pixel_center);

    r = Ray(camera->eye, pixel_center);
}


void Raytracer::trace()
{
    CGUTILS_ASSERT(scene);
    CGUTILS_ASSERT(camera);

    TraceResult result;

    for (int u = 0; u < img.width; ++u)
    {
        for (int v = 0; v < img.height; ++v)
        {
            //std::cout << " handling pixel " << i << " x " << j << std::endl;
            RGB color(0);
            Ray r;
            rayForPixel(u, v, r);

            if(scene->traceRay(r, result))
            {
                if (!once)
                {
                    std::cout << " got a hit on mesh "  << std::endl;
                    once = true;
                }

                computeLightAt(result.tri.intersectionToPoint(result.intersection), color);

                color = color * result.mat->diffuse;

                img.setPixelColor(u, v , color);
            }
            else
            {
                //std::cout << " Some nonhit !"  << std::endl;
            }
        }
    }

   std::cout << "num meshes " << MeshManager::getInstance().size() << std::endl;

    if (!img.writeOut(outpath))
    {
        std::cout << "Error: FreeImage_Save()" << std::endl;
    }
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

glm::vec3 Triangle::intersectionToPoint(glm::vec4 &intersection)
{
    return intersection.x * A + intersection.y * B + intersection.z * C;
}

} /* namespace raytracer */
