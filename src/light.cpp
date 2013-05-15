#include "light.h"

namespace raytracer {


Light::Light(const std::string &name) : name(name), ambient(0), diffuse(0), specular(0), position(0), direction(0)
{}

void Light::setColor(const RGB &ambient, const RGB &diffuse, const RGB &specular)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

void Light::setLocation(const glm::vec3 &position, const glm::vec3 &direction)
{
	this->position = position;
	this->direction = direction;
}

void Light::setAttenuation(float constant, float linear, float quadratic)
{
	this->attenuationConstant = constant;
	this->attenuationLinear = linear;
	this->attenuationQuadratic = quadratic;
}

} /* namespace raytracer */
