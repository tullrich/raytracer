#include "texture.h"

namespace raytracer {
	
Texture::Texture(const std::string filepath)
{
	this->filepath = filepath;
}

bool Texture::isLoaded() const
{
	//TODO:: return pointer = null
	return false;
}

bool Texture::load()
{

}

} /* namespace raytracer */
