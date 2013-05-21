#include "camera.h"
#include <cmath>
#include "common.h"

namespace raytracer {

Camera::Camera()
{
	this->eye       = glm::vec3(20.0f, 20.0f, 20.0f); //  position; // 
	this->lookAt    = glm::vec3(0.0f, 2.0f, 0.0f); // lookAt; // 
	this->up        = glm::vec3(0.0f, 1.0f, 0.0f); // up; // 
	
	this->nearPlane = 1.0f;
	this->farPlane  = 100.0f;
	this->FOV       = 90.0f * M_PI / 180.0f ; // in radians
	this->aspect    = 16.0f / 9.0f;
	this->FOVy      = FOV / (aspect); // in radians
	
	glm::vec3 w     = glm::normalize(eye - lookAt);
	glm::vec3 u     = glm::normalize(glm::cross(up, w));
	glm::vec3 v     = glm::normalize(glm::cross(w, u));
	glm::vec3 t     = eye;

    this->cameraToWorld = glm::mat4(glm::vec4(u, 0.0f), glm::vec4(v, 0.0f), glm::vec4(w, 0.0f), glm::vec4(t, 1.0f));
}

Camera::Camera(const glm::vec3 &position, const glm::vec3 &lookAtPoint, const glm::vec3 &upPoint, float nearPlane, float farPlane, float FOV, float aspect)
{
	this->eye = glm::vec3(0.0f, 0.0f, 0.0f); //  position; // 
	this->lookAt =  glm::vec3(1.0f, 0.0f, 0.0f); // lookAt; // 
	this->up = glm::vec3(0.0f, 1.0f, 0.0f); // up; // 

	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	this->FOV = 45 * M_PI / 180 ; // in radians
	this->FOVy = FOV / aspect; // in radians
	this->aspect = aspect;

	glm::vec3 w = glm::normalize(eye - lookAt);
	glm::vec3 u = glm::normalize(glm::cross(up, w));
	glm::vec3 v = glm::normalize(glm::cross(w, u));
	glm::vec3 t = eye;

    this->cameraToWorld = glm::mat4(glm::vec4(u, 0.0f), glm::vec4(v, 0.0f), glm::vec4(w, 0.0f), glm::vec4(t, 1.0f));
}

void Camera::getPixelCenter(int i, int j, int x_res, int y_res, glm::vec3 &out)
{
	float near = 150.0f;
	float L_x = 2.0f * tan(FOV/2.0f) * near; // horizontal length of near plane
	float L_y = 2.0f * tan(FOV/2.0f) * near; // vertical length of near plane
	//std::cout << "L_x" << L_x << std::endl;


	float pixel_x  = L_x / x_res;// horizontal length of each pixel
	float pixel_y  = L_y / y_res;// vertical length of each pixel
	
	float center_x_i = pixel_x * ((2.0f * i + 1.0f) / (2.0f * x_res) - 0.5f);
	float center_y_j = pixel_y * ((2.0f * j + 1.0f) / (2.0f * y_res) - 0.5f);
	glm::vec4 pixel_center(center_x_i, center_y_j, -1.0f, 1.0f);

	out = glm::vec3(cameraToWorld * pixel_center);

}


std::ostream& operator<<(std::ostream& o, const Camera& b)
{
	return o << "Camera {\n"
		<< "\teye : " << b.eye
		<< "\n\tlookAt : " << b.lookAt
		<< "\n\tup : " << b.up
		<< "\n\tright : " << b.right
		<< "\n\tnearPlane : " << b.nearPlane
		<< "\n\tfarPlane : " << b.farPlane
		<< "\n\tFOV : " << b.FOV
		<< "\n\tFOVy : " << b.FOVy
		<< "\n\taspect: " << b.aspect
		<< "\n\tcameraToWorld: \n" << b.cameraToWorld
		<< "\n}";
}


} /* namespace raytracer */