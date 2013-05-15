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

        if (scene->testVisibility(point, glm::vec3(-20, 20, 0), d))
        {
            light->getAttenuatedRadiance(point, d, per_light);
            color += per_light;
        }
    }
}

void Raytracer::rayForPixel(int u, int v, Ray &r) const
{
    glm::vec3 point, point2;

    //float x_inc = tan(camera->FOV) * ((2 * u) - img.width) / img.width;
    //float x_inc = 2 * (camera->nearPlane * tan(camera->FOV)) / img.width;
    //float y_inc = tan(camera->FOV) * ((2 * v) - img.height) / img.height;
    //std::cout << "plane width " << tan(M_PI / 2 / 2) * camera->nearPlane * 2 << std::endl;
    float x_inc = tan(camera->FOV / 2) * camera->nearPlane * 2 / img.width;
    //std::cout << "unit plane width " << x_inc << std::endl;
    float y_inc = tan(camera->FOVy / 2) * camera->nearPlane * 2 / img.height;

    // 0.5*(2y+1-H)*Yinc

    glm::vec3 image_center(0);// = camera->position;// + glm::vec1(1);// + (camera->nearPlane * camera->direction);


    //std::cout << side << std::endl;
    ///0.5 * (2 * i - img.width) * x_inc
    ///0.5 * (2 * j - img.width) * y_inc
    glm::vec4 temp(u * x_inc - (img.width * x_inc / 2), camera->nearPlane, v * y_inc - (img.height * y_inc / 2), 1);

    //float sum = (0.5 * (2 *j + 1 - img.height))  + (0.5 * (2 *i + 1 - img.width) * x_inc);
    //glm::vec3 dir = vec3(camera->direction.x + sum, camera->direction.y + sum, camera->direction.z + sum);
    //glm::vec3 dir = vec3(camera->direction.x + sum, camera->direction.y + sum, camera->direction.z + sum);
    //image_center = vec3(camera->transform * temp);
    //image_center += 
    //std::cout << glm::vec3(temp) << std::endl;
    vec3 final = camera->position + glm::normalize(glm::vec3(temp));
    r = Ray(camera->position, final);
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
                //std::cout << " got a hit on mesh " << *result.mesh << std::endl;

                computeLightAt(result.tri.intersectionToPoint(result.intersection), color);

                img.setPixelColor(u, v , color);
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

std::ostream& operator<<(std::ostream& o, const Triangle& b)
{
    return  o << "Triangle {" << b.A << ", " << b.B << ", " << b.C << " }";
}

glm::vec3 Triangle::intersectionToPoint(glm::vec4 &intersection)
{
    return intersection.x * A + intersection.y * B + intersection.z * C;
}

} /* namespace raytracer */
