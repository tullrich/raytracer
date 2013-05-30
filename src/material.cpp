#include "material.h"

namespace raytracer {

void Material::BRDF()
{

}

void TextureMaterial::BRDF()
{

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
