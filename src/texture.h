#ifndef _TEXTURE_H_
#define _TEXTURE_H_ 

#include "common.h"
#include "rtimage.h"

namespace raytracer {

using namespace cgutils;

class Texture
{
public:

	/**
	 * Texture representing filepath on disk
	 */
	Texture(const std::string filepath);

	/**
	 * Load the image from disk using the filepath provided during construction
	 * @return TRUE if loading was successfull, FALSE otherwise
	 */
	bool load();

	/**
	 * check if this texture has been loaded from disk
	 * @return FALSE if load() has not returned successfully for this {@link Texture}
	 */
	bool isLoaded() const;


	glm::vec3 lookUp(const TexCoord &uv) const;

protected:
	image::rtimage *img;
	unsigned int img_height, img_width;
	std::string filepath;
};


class  TextureManager : public ResourceManager<std::string, Texture> {};

} /* raytracer */

#endif /* _TEXTURE_H_ */
