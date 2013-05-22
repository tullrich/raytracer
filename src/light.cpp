#include "light.h"

namespace raytracer {


Light::Light(const std::string &name) : name(name), ambient(0), diffuse(0), specular(0), position(0), direction(0)
{}

void Light::genShadowRay(const glm::vec3 point, Ray &r)
{
	r = Ray(point, position);
}

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

void PointLight::getAttenuatedRadiance(const glm::vec3 &point, float d, RGB &out) const
{
	float cofactor = attenuationConstant + attenuationLinear * d + attenuationQuadratic * pow(d, 2);
	cofactor = clamp(1.0f / cofactor, 0.0, 1.0);

	//std::cout << "d " << d << " cofactor " << cofactor << " result " << diffuse * cofactor << std::endl;

	out = diffuse * cofactor;
}

AreaLight::AreaLight(const std::string &name, const glm::vec3 &side_h, const glm::vec3 &side_w) : Light(name)
{
	this->side_h = side_h;
	this->side_w = side_w;
	this->normal = glm::normalize(glm::cross(side_h, side_w));
	area = glm::length(glm::cross(side_h, side_w));
}


std::ostream& operator<<(std::ostream& o, const Light& b)
{
	return o << "Light '" << b.name << "' {\n" \
	<< "\tposition: " << b.position << "\n" \
	<< "\tattenuationConstant:" << b.attenuationConstant << "\n" \
	<< "\tattenuationLinear:" << b.attenuationLinear << "\n" \
	<< "\tattenuationQuadratic:" << b.attenuationQuadratic << "\n" \
	<< "\tdiffuse:" << b.diffuse << "\n" \
	<< "\tambient:" << b.ambient << "\n" \
	<< "\tspecular:" << b.specular << "\n" \
	<< "}";
}

} /* namespace raytracer */
