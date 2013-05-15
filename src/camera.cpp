#include "camera.h"

namespace raytracer {

Camera::Camera(const glm::vec3 &position, float focal_length)
{
	this->position = position;
	this->focal_length = focal_length;
}


} /* namespace raytracer */