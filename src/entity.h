#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "glm/glm.hpp"

namespace raytracer {

using namespace::glm;

typedef struct 
{
	vec3 max;
	vec3 min;
} AABB;

class Entity
{
public:
	AABB aabb;
};


} /* namespace raytracer */

#endif /* _ENTITY_H_ */
