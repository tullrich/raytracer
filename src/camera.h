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
	Camera(const glm::vec3 &position, float farPlane) : position(position), direction(1, 0, 0), up(0, 1, 0), right(0, 0, 1), nearPlane(0), farPlane(farPlane), FOV(0), FOVy(0), aspect(0), transform(0){ };
	Camera(const glm::vec3 &position, const glm::vec3 &lookAt, const glm::vec3 &up, float nearPlane, float farPlane, float FOV, float aspect);

	glm::vec3 position, direction, up, right;
	float nearPlane, farPlane, FOV, FOVy, aspect;
	glm::mat4 transform;
};

/**
 * Insert stream operator for Camera
 */
std::ostream& operator<<(std::ostream& o, const Camera& b);

} /* raytracer */

#endif /* _CAMERA_H_ */
