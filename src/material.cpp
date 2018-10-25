#include "material.h"

namespace raytracer {

RGB Material::BRDF(const TexCoord *uv) const
{
	return diffuse;
}

RGB TextureMaterial::BRDF(const TexCoord *uv) const
{
	if(uv && map_diffuse)
	{
	return map_diffuse->lookUp(*uv);
	}

	std::cout << "map_diffuse " << map_diffuse << " " << uv << std::endl; 

	return Material::BRDF(uv);
}

RGB Material::Le(const TexCoord *uv) const
{
	return emissive;
}

RGB TextureMaterial::Le(const TexCoord *uv) const
{
	if(uv && map_emissive)
	{
		return map_emissive->lookUp(*uv);
	}

	std::cout << "map_emissive " << map_emissive << " " << uv << std::endl; 

	return Material::Le(uv);
}

std::ostream& operator<<(std::ostream& o, const Material& b) 
{
	return o << "Material '" << b.name << "' {\n" << \
		"\tspecular: <" << b.specular.x << ", " << b.specular.y << ", " << b.specular.z << ">\n" << \
		"\tdiffuse : <" << b.diffuse.x << ", " << b.diffuse.y << ", " << b.diffuse.z << ">\n" << \
		"\tambient : <" << b.ambient.x << ", " << b.ambient.y << ", " << b.ambient.z << ">\n" << \
		"\temissive : <" << b.emissive.x << ", " << b.emissive.y << ", " << b.emissive.z << ">\n" << \
		"\tshineness : " << b.shineness << "\n" <<
		"}";
}

}; /* namespace raytracer */
