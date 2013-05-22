#include "raytracer.h"
#include <math.h>
#include <ctime>

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

    return color;
}

RGB Raytracer::indirectRadiance(const Material &mat, const Triangle &tri, const glm::vec4 intersection)
{
    return RGB(0);
}

bool Raytracer::computeRadiance(const Material &mat, const Triangle &tri, const glm::vec4 intersection, RGB &color)
{

    color = emittedRadiance(mat, tri, intersection);
    color += directRadiance(mat, tri, intersection);
    color += indirectRadiance(mat, tri, intersection);
}

static int once = false;

void Raytracer::rayForPixel(int x, int y, Ray &r) const
{

    glm::vec3 pixel_center(0);
    float x_variance = camera->pixelXDimension() * (randf() - 0.5f);
    float y_variance = camera->pixelYDimension() * (randf() - 0.5f);
    //std::cout << "pixelXDimension " << camera->pixelXDimension() << " x_variance " << x_variance << std::endl; 
    camera->getPixelCenter(x, y, pixel_center, x_variance, y_variance);

    r = Ray(camera->eye, pixel_center);
}

void Raytracer::lightPixel(int u, int v)
{
    RGB color;
    TraceResult result;
    Ray r;

    int numViewRays = 25;
    for(int i = 0; i < numViewRays ; i++)
    {
        RGB view_ray(0);
        rayForPixel(u, v, r);
        if(scene->traceRay(r, result))
        {
            if (!once)
            {
                std::cout << " got a hit on mesh "  << std::endl;
                once = true;
            }
            computeRadiance(*(result.material), result.tri, result.intersection, view_ray);
        }
        else
        {
            view_ray = RGB(0.1f, 0.1f, 0.1f);
        }

        color += view_ray;
    }

    color /= numViewRays;

    img.setPixelColor(u, v , color);
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
            lightPixel(u, v);
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

glm::vec3 uniformDirectionOnHemisphere(const glm::vec3 normal, float &pdf_theta)
{
    // sample 3-axis in uniform direction
    glm::vec3 rand_direction(randf() - 0.5f, randf() - 0.5f, randf() - 0.5f);

    if(glm::dot(rand_direction, normal) < 0)
    {
        rand_direction = -rand_direction;
    }

    pdf_theta = 2.0f * (float)M_PI
    return rand_direction;
}

} /* namespace raytracer */
