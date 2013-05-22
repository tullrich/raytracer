#ifndef _CAMERA_H_
#define _CAMERA_H_ 

#include "common.h"
#include "ray.h"

namespace raytracer {

/**
 * Represents the eye during rendering
 */
class Camera
{
public:
	Camera(int x_res, int y_res);
	Camera(const glm::vec3 &position, const glm::vec3 &lookAt, const glm::vec3 &up, float nearPlane, float farPlane, float FOV, float aspect);

	glm::vec3 eye, lookAt, up, right;
	float nearPlane, farPlane, FOV, FOVy, aspect, ;
	glm::mat4 cameraToWorld;

	float pixel_x, pixel_y;
	float x_res, y_res;

	/**
	 * Compute the worldspace vector cooresponding to the center of pixel i, j using
	 * the orientation of this camera.	
	 * @param i      horizontal pixel number
	 * @param j      vertical pixel number
	 * @param out    set to the resulting worldspace vector
	 */
	void getPixelCenter(int i, int j, glm::vec3 &out, float variance_i, float variance_j)  const;

	/**
	 * generate a random viewing ray for the near plane project pixel (i, j)
	 * @param i width pixel value
	 * @param j height pixel value
	 * @param r resultant {@link Ray}
	 */
	void genViewingRay(int i, int j, Ray &r);

	float pixelXDimension() const;
	float pixelYDimension() const;
};

/**
 * Insert stream operator for Camera
 */
std::ostream& operator<<(std::ostream& o, const Camera& b);

} /* raytracer */

#endif /* _CAMERA_H_ */
