#include "camera.h"
#include <cmath>

namespace raytracer {

Camera::Camera(const glm::vec3 &position, const glm::vec3 &lookAt, const glm::vec3 &up, float nearPlane, float farPlane, float FOV, float aspect)
{
	this->position = position; //glm::vec3(0, -3, 0);
	this->direction = glm::vec3(0, 1, 0); //glm::normalize(lookAt); //
	this->up = glm::vec3(0, 0, 1); // glm::normalize(up); //
	this->right = glm::normalize(glm::cross(this->up, this->direction)); // glm::normalize(up); //
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	this->FOV = FOV; // in radians
	this->FOVy = FOV / aspect; // in radians
	this->aspect = aspect;

    this->transform = glm::transpose(glm::mat4(glm::vec4(this->direction, this->position.x), glm::vec4(this->up, this->position.y), glm::vec4(this->right, this->position.z),  glm::vec4(0, 0, 0, 1)));
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
		<< "\n\tright : " << b.right
		<< "\n\tnearPlane : " << b.nearPlane
		<< "\n\tfarPlane : " << b.farPlane
		<< "\n\tFOV : " << b.FOV
		<< "\n\tFOVy : " << b.FOVy
		<< "\n\taspect: " << b.aspect
		<< "\n}";
}


} /* namespace raytracer */