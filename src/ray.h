#ifndef _RAY_H_
#define _RAY_H_ 

#include "common.h"

namespace raytracer {

using namespace glm;

/**
 * Ray r = p + t(q), t >= 0
 */
class Ray
{
public:
	Ray(glm::vec3 p, glm::vec3 q);

	glm::vec3 p; // ray origin point
	glm::vec3 q; // Direction of the ray is normalize (q - p);

	/**
	 * Calculate the barycentric coordinates of the intersection of this ray with
	 * {@link Triangle} T.
	 * @param  t triangle to test intersection
	 * @param  intersection empty vec3 returning the barycentric coordinates of the intersection with respect to t
	 * @return   true if this {@link Ray} intersects t, false otherwise
	 */
	bool intersects(const Triangle &t, glm::vec4 &intersection) const;
};

} /* raytracer */

#endif /* _RAY_H_ */
