#ifndef _AABB_H_
#define _AABB_H_ 

#include "common.h"

namespace raytracer {

using namespace glm;

/**
 * Axially aligned bounding box
 */
typedef struct 
{
	vec3 max;
	vec3 min;
} AABB;

/**
 * Grows aabb to encompass all vertices in buffer
 * @param aabb        the Axially Aligned BB to grow
 * @param numVertices number of vertices in buffer
 * @param buffer      vertex buffer
 */
void AABBContainVertices(AABB &aabb, int  numVertices, const vec3 *buffer);

	
/**
 * Insert stream operator for AABB
 */
std::ostream& operator<<(std::ostream& o, const AABB& b);

} /* raytracer */

#endif /* _AABB_H_ */
