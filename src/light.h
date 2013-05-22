#ifndef _LIGHT_H_
#define _LIGHT_H_ 

#include "common.h"
#include "ray.h"

namespace raytracer {

/**
 * base class for Lights that are added to the scene
 */
class Light
{
public:
	Light(const std::string &name);

	typedef std::shared_ptr<Light> light_ptr;

	void setLocation(const glm::vec3 &position, const glm::vec3 &direction);
	void setColor(const RGB &ambient, const RGB &diffuse, const RGB &specular);
	void setAttenuation(float constant, float linear, float quadratic);

	const std::string name;

	virtual void getAttenuatedRadiance(const Ray &r, RGB &out) const {};

	virtual void genShadowRay(const glm::vec3 point, Ray &r) const;
	/**
	 * Location vars
	 */
	glm::vec3 position, direction;

protected:
	/**
	 * Lighting coefficents
	 */
	RGB ambient, diffuse, specular;

	/**
	 * Attenuation vars
	 */
	float attenuationConstant, attenuationLinear, attenuationQuadratic;
	/**
	 * access to private members
	 */
	friend std::ostream& operator<<(std::ostream& o, const Light& b);
};

/**
 * Insert stream operator for mesh_data
 */
std::ostream& operator<<(std::ostream& o, const Light& b);


class PointLight : public Light
{
public:
	PointLight(const std::string &name) : Light(name) {};

	void getAttenuatedRadiance(const Ray &r, RGB &out) const;
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

class AreaLight : public Light
{
public:
	AreaLight(const std::string &name, const glm::vec3 &side_h, const glm::vec3 &side_w);
	virtual void genShadowRay(const glm::vec3 point, Ray &r) const;
	void getAttenuatedRadiance(const Ray &r, RGB &out) const;
private:
	glm::vec3 normal;
	glm::vec3 side_h, side_w;
	float area;
};


/**
 * Helper class for visiting Lights
 */
typedef cgutils::Visitor<Light::light_ptr> LightVisitor;

} /* raytracer */

#endif /* _LIGHT_H_ */
