#include "camera.h"

namespace raytracer {

Camera::Camera(const glm::vec3 &position, const glm::vec3 &lookAt, const glm::vec3 &up, float nearPlane, float farPlane, float FOV, float aspect)
{
	this->position = position;
	this->direction = glm::normalize(lookAt - position);
	this->up = glm::normalize(up);
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	this->FOV = FOV;
	this->aspect = aspect;


	glm::vec3 side = glm::normalize(glm::cross(direction, up));
    this->transform = glm::mat4(glm::vec4(direction, 0), glm::vec4(up, 0), glm::vec4(side, 0),  glm::vec4(0, 0, 0, 1));
}

/*Camera::directionForPixel(int i, int j, int width, int height, glm::vec3 &out)
{
	float x_inc = i * (2 * tan(FOV/2)) / width;
	float y_inc = j * (2 * tan(FOV/2)) / width;
}*/


std::ostream& operator<<(std::ostream& o, const Camera& b)
{
	return o << "Camera {\n"
		<< "\tposition : " << b.position
		<< "\n\tdirection : " << b.direction
		<< "\n\tup : " << b.up
		<< "\n\tnearPlane : " << b.nearPlane
		<< "\n\tfarPlane : " << b.farPlane
		<< "\n\tFOV : " << b.FOV
		<< "\n\taspect: " << b.aspect
		<< "\n}";
}


} /* namespace raytracer */