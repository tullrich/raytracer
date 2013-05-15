#ifndef _LIGHT_H_
#define _LIGHT_H_ 

#include "common.h"
#include "common.h"

namespace raytracer {

class Light
{
public:
	Light(const std::string &name);

	typedef std::shared_ptr<Light> light_ptr;

	void setLocation(const glm::vec3 &position, const glm::vec3 &direction);
	void setColor(const RGB &ambient, const RGB &diffuse, const RGB &specular);
	void setAttenuation(float constant, float linear, float quadratic);
	
	const std::string name;

private:
	/**
	 * Lighting coefficents
	 */
	RGB ambient, diffuse, specular;

	/**
	 * Location vars
	 */
	glm::vec3 position, direction;

	/**
	 * Attenuation vars
	 */
	float attenuationConstant, attenuationLinear, attenuationQuadratic;
};


class PointLight : public Light
{
public:
	PointLight(const std::string &name) : Light(name) {};
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(const std::string &name) : Light(name) {};
};

class SpotLight : public Light
{
public:
	SpotLight(const std::string &name, float inner, float outer) : Light(name), angleInnerCone(inner), angleOuterCone(outer) {};
private:
	float angleInnerCone, angleOuterCone;
};


/**
 * Helper class for visiting Lights
 */
typedef cgutils::Visitor<Light::light_ptr> LightVisitor;

} /* raytracer */

#endif /* _LIGHT_H_ */
