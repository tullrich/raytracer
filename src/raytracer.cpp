#include "raytracer.h"
    
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

void Raytracer::rayForPixel(int i, int j, Ray &r) const
{
    glm::vec3 point, point2;

    float x_inc = camera->FOV / img.width;
    float y_inc = camera->FOV / img.height;

    // 0.5*(2y+1-H)*Yinc

    glm::vec3 image_center = camera->position;// + glm::vec1(1);// + (camera->nearPlane * camera->direction);


    //std::cout << side << std::endl;
    glm::vec4 temp(0, 0.5 * (2 *j + 1 - img.height) * y_inc, 0.5 * (2 *i + 1 - img.width) * x_inc, 1);

    //float sum = (0.5 * (2 *j + 1 - img.height))  + (0.5 * (2 *i + 1 - img.width) * x_inc);
    //glm::vec3 dir = vec3(camera->direction.x + sum, camera->direction.y + sum, camera->direction.z + sum);
    //glm::vec3 dir = vec3(camera->direction.x + sum, camera->direction.y + sum, camera->direction.z + sum);
    image_center += vec3(camera->transform * temp);
    //image_center += 

    r = Ray(camera->position, image_center);
}


void Raytracer::trace()
{
    CGUTILS_ASSERT(scene);
    CGUTILS_ASSERT(camera);

    TraceResult result;

    for (int i = 0; i < img.width; ++i)
    {
        for (int j = 0; j < img.height; ++j)
        {
            //std::cout << " handling pixel " << i << " x " << j << std::endl;
            RGB color;
            Ray r;
            rayForPixel(i, j, r);

            if(scene->traceRay(r, result))
            {

                computeLightAt(result.tri.intersectionToPoint(result.intersection), color);

                img.setPixelColor(i, j , color);
            }
        }
    }

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
