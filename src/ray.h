#ifndef _RAY_H_
#define _RAY_H_ 

#include "common.h"
#include "aabb.h"

namespace raytracer {

using namespace glm;

class Material; // forward declaration
class Entity; // forward declaration
class mesh_data; // forward declaration

/**
 * class containing the result of a success full ray cast
 */
class TraceResult
{
public:
	TraceResult() : intersection(0), material(NULL) {};
	TraceResult(const glm::vec4 &intersection, const Triangle &tri, const Material *material);

	glm::vec4 intersection; // barycentric coordinates representing the
	Triangle tri; // triangle of impact
	const Material *material; // ptr to the mesh of intersection
};

/**
 * Ray r = p + t(q), t >= 0
 */
class Ray
{
public:
	Ray() : p(0), q(0) {};
	Ray(glm::vec3 p, glm::vec3 q);

	glm::vec3 p; // ray origin point
	glm::vec3 q; // Direction of the ray is normalize (q - p);
	glm::vec3 n; // Direction of the ray is normalize (q - p);

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
