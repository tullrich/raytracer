#ifndef _COMMON_H_
#define _COMMON_H_ 

#include <iostream>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"
#include "cgutils/cgutils.hpp"

namespace raytracer {

/* Default input values */
#define DEFAULT_IMG_WIDTH 100	// default output image height value
#define DEFAULT_IMG_HEIGHT 100	// default output image height value
#define DEFAULT_OUT_PATH "traceimage.png"	// default output image filepath


typedef glm::vec3 RGB;

class Triangle 
{
public:
	Triangle() : A(0), B(0), C(0) {};
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) : A(a), B(b), C(c) {}; 
	
	glm::vec3 A;
	glm::vec3 B;
	glm::vec3 C;

	/**
	 * converts a vec3 containing barycentric coordinates relative to this triangle
	 * into a point on the surface of this
	 * @param  intersection barycentric coordinates, |intersection| = 1.0
	 * @return              intersection point on the surface of this triangle
	 */
	glm::vec3 intersectionToPoint(glm::vec4 &intersection);
};

std::ostream& operator<<(std::ostream& o, const glm::vec3& b);
std::ostream& operator<<(std::ostream& o, const Triangle& b);

} /* raytracer */

#endif /* _COMMON_H_ */

