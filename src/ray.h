#ifndef _RAY_H_
#define _RAY_H_ 

#include "common.h"

namespace raytracer {

using namespace glm;

class Material; // forward declaration

/**
 * class containing the result of a success full ray cast
 */
typedef struct
{
	glm::vec4 intersection; // barycentric coordinates representing the
	Triangle tri; // triangle of impact
	//Entity::entity_ptr entity; // ptr to the entity of intersection
	//mesh_data::mesh_ptr mesh; // ptr to the mesh of intersection
	const Material *mat; // material of the impacting triangle
} TraceResult;

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
