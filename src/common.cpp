#include "common.h"
#include <iostream>
#include <boost/filesystem.hpp>

#include "resources.h"

namespace raytracer
{

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

TexCoord UVTriangle::interpolateUV(const glm::vec4 &intersection) const
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

std::string parentPath(const std::string filename)
{
    std::string filename_noext;
    filename_noext = boost::filesystem::path(filename).parent_path().string();

    return filename_noext;
}

std::string appendFilename(const std::string parent_path, const std::string filename)
{
    boost::filesystem::path full_path(parent_path);
    full_path /= filename; // appends filename to full_path with the preferred separator

    return full_path.make_preferred().string();
}
} /* namespace raytracer */
