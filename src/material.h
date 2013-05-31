#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "common.h"
#include "resources.h"
#include "texture.h"

namespace raytracer {


/**
 * material definition referenced by each {@link Mesh}
 */
class Material
{
public:
	Material() : name(""), diffuse(0), specular(0), ambient(0), emissive(0), shineness(0) { };
	Material(const std::string name) : name(name), diffuse(0), specular(0), ambient(0), emissive(0), shineness(0) { };

	virtual glm::vec3 BRDF(const TexCoord *uv) const;
	virtual glm::vec3 Le(const TexCoord *uv) const;

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


class TextureMaterial : public Material
{
public:
	TextureMaterial(const std::string name) : Material(name) {};

	void setDiffuseTexture(const Texture *t) { map_diffuse = t; };
	void setSpecularTexture(const Texture *t) { map_specular = t; };
	void setEmissiveTexture(const Texture *t) { map_emissive = t; };
	void setNormalTexture(const Texture *t) { map_normal = t; };

	virtual glm::vec3 BRDF(const TexCoord *uv) const;
	virtual glm::vec3 Le(const TexCoord *uv) const;

protected:
	const Texture *map_diffuse, *map_specular, *map_emissive, *map_normal;
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
