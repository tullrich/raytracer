#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "common.h"
#include "resources.h"

namespace raytracer {


/**
 * material definition referenced by each {@link Mesh}
 */
class Material
{
public:
	Material() : name(""), diffuse(0), specular(0), ambient(0), emissive(0), shineness(0) { };
	Material(std::string name) : name(name), diffuse(0), specular(0), ambient(0), emissive(0), shineness(0) { };

	/**
	 * material name
	 */
	std::string name;

	/**
	 * diffuse, specular, & ambient coefficents for each RGB
	 */
	glm::vec3 diffuse, specular, ambient;

	/**
	 * emmisive component of this light
	 */
	glm::vec3 emissive;

	/**
	 *  specular shineness exponent 
	 */
	float shineness;

};

/**
 * Insert stream operator for Material
 */
std::ostream& operator<<(std::ostream& o, const Material& b);

/**
 * Singleton managing a global material cache
 */
class  MaterialManager : public ResourceManager<int, Material> {};

}; /* namespace raytracer */

#endif /* _MATERIAL_H */
