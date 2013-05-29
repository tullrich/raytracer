#ifndef _TEXTURE_H_
#define _TEXTURE_H_ 

#include "common.h"

namespace raytracer {

class Texture
{
public:
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

protected:
	std::string filepath;
};

} /* raytracer */

#endif /* _TEXTURE_H_ */
