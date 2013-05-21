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

bool Raytracer::computeLightAt(const Material &mat, const Triangle &tri, const glm::vec4 intersection, RGB &color)
{
    RGB per_light(0);
    TraceResult result;
    glm::vec3 N = tri.normal();
    glm::vec3 point = adjustFloatingPointToward(tri.intersectionToPoint(intersection), N);

    for (Light::light_ptr light : scene->lights)
    {
        if (scene->testVisibility(point, light->position, result))
        {
            glm::vec3 incident_direction = glm::vec3(light->position - point);
            float d = glm::length(incident_direction);
            light->getAttenuatedRadiance(point, d, per_light);

            glm::vec3 I = glm::normalize(incident_direction);
            color += per_light * mat.diffuse * fmaxf(glm::dot(N, I), 0);
        }
    }
}

static int once = false;

void Raytracer::rayForPixel(int x, int y, Ray &r) const
{

    glm::vec3 pixel_center(0);
    camera->getPixelCenter(x, y, img.width, img.height, pixel_center);

    r = Ray(camera->eye, pixel_center);
    //r = Ray(camera->eye, glm::vec3(0, 0, 0));
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

                if(computeLightAt(*(result.material), result.tri, result.intersection, color))
                {

                    //std::cout << "distance to light " << result.-> << std::endl;
                }


                img.setPixelColor(u, v , color);
            }
            else
            {
                img.setPixelColor(u, v , RGB(0.1f, 0.1f, 0.1f));
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


} /* namespace raytracer */
