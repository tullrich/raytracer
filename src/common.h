#ifndef _COMMON_H_
#define _COMMON_H_ 

#include <iostream>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"
#include "cgutils/cgutils.hpp"
#include "trig.h"

namespace raytracer {

/* Default input values */
#define DEFAULT_IMG_WIDTH 100	// default output image height value
#define DEFAULT_IMG_HEIGHT 100	// default output image height value
#define DEFAULT_OUT_PATH "traceimage.png"	// default output image filepath


#define CONFIG_FILEPATH "raytracer.cfg" // input configuration filepath

#define OPTION_WIDTH "w"
#define OPTION_HEIGHT "h"
#define OPTION_OUTPATH "o"
#define OPTION_ASSETPATH "asset-filepath"

#define FLOAT_ERROR 0.0001f

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
	glm::vec3 intersectionToPoint(const glm::vec4 &intersection) const;

	glm::vec3 normal()  const;
};


/**
 * A plane representing all points X s.t. dot(X, n) = d
 */
struct Plane
{
	Plane() : n(0), d(0) {};

	float d;
	glm::vec3 n;
};

glm::vec3 adjustFloatingPointToward(const glm::vec3 point, const glm::vec3 &towards);

inline float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
}

/**
 * random float between 0 and 1 inclusive
 * @return random float
 */
inline float randf()
{
	return (float)rand()/(float)RAND_MAX;
}

extern bool debugvar;

std::ostream& operator<<(std::ostream& o, const glm::vec3& b);
std::ostream& operator<<(std::ostream& o, const glm::mat4& b);
std::ostream& operator<<(std::ostream& o, const Triangle& b);

} /* raytracer */

#endif /* _COMMON_H_ */

