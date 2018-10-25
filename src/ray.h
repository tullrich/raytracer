#ifndef _RAY_H_
#define _RAY_H_

#include "common.h"
#include "aabb.h"

namespace raytracer {

class Material; // forward declaration
class Entity; // forward declaration
class mesh_data; // forward declaration

/**
 * Ray r = p + t(q), t >= 0
 */
class Ray
{
public:
	Ray() : p(0), q(0), n(0), d(0) {};
	Ray(glm::vec3 p, glm::vec3 q);

	glm::vec3 p; // ray origin point
	glm::vec3 q; // ray second point;
	glm::vec3 n; // Direction of the ray is normalize (q - p);
	float d;

	/**
	 * Calculate the barycentric coordinates of the intersection of this ray with
	 * {@link Triangle} T.
	 * @param  t triangle to test intersection
	 * @param  result vec4 returning the barycentric coordinates of the intersection with respect to t
	 * @return   true if this {@link Ray} intersects t, false otherwise
	 */
	bool intersects(const Triangle &t, glm::vec4 &result) const;


	/**
	 * Intersect ray with an {@AABB link}
	 * @param  t triangle to test intersection
	 * @param  result vec3 returning the intersecting point
	 * @return   true if this {@link Ray} intersects t, false otherwise
	 */
	bool intersects(const AABB &aabb, glm::vec3 &result) const;
};

std::ostream& operator<<(std::ostream& o, const Ray& b);

} /* raytracer */

#endif /* _RAY_H_ */
