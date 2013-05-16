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
	Camera();
	Camera(const glm::vec3 &position, const glm::vec3 &lookAt, const glm::vec3 &up, float nearPlane, float farPlane, float FOV, float aspect);

	glm::vec3 eye, lookAt, up, right;
	float nearPlane, farPlane, FOV, FOVy, aspect, ;
	glm::mat4 cameraToWorld;

	/**
	 * Compute the worldspace vector cooresponding to the center of pixel i, j using
	 * the orientation of this camera.	
	 * @param i      horizontal pixel number
	 * @param j      vertical pixel number
	 * @param x_res  horizontal resolution
	 * @param y_res  vertical resolution
	 * @param out    set to the resulting worldspace vector
	 */
	void getPixelCenter(int i, int j, int x_res, int y_res, glm::vec3 &out);
};

/**
 * Insert stream operator for Camera
 */
std::ostream& operator<<(std::ostream& o, const Camera& b);

} /* raytracer */

#endif /* _CAMERA_H_ */
