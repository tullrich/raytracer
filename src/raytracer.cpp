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


void Raytracer::trace()
{
    CGUTILS_ASSERT(scene);
    CGUTILS_ASSERT(camera);

    RGB color(.5, 0, 0);
    glm::vec4 intersection;
    Triangle tri;

    float pixel_size = 0.01f;

    float half_width = pixel_size * img.width / 2;
    float half_height = pixel_size * img.height / 2;

    for (int i = 0; i < img.width; ++i)
    {
        for (int j = 0; j < img.height; ++j)
        {
            glm::vec3 temp = camera->position;
            temp.x += camera->focal_length;
            temp.y += (j * pixel_size) - half_height;
            temp.z += (i * pixel_size) - half_width;

            Ray r(camera->position, temp);

            if(scene->traceRay(r, intersection, tri))
            {
                img.setPixelColor(i, j ,color);
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
