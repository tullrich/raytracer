#ifndef _CAMERA_H_
#define _CAMERA_H_ 

#include "common.h"

namespace raytracer {

/**
 * Represents the eye during rendering
 */
class Camera
{
public:
	Camera(const glm::vec3 &position, float focal_length);

	glm::vec3 position;
	float focal_length;
};

} /* raytracer */

#endif /* _CAMERA_H_ */
