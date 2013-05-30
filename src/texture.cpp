#include "texture.h"

namespace raytracer {

	
Texture::Texture(const std::string filepath) : img(NULL)
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
	image::rtimage *img = NULL;
	if(img = image::loadImage(filepath))
	{
		this->img = img;
		return true;
	}

	std::cout << "Error: could not load Texture from file '" << filepath  << "'" << std::endl; 
	return false;
}

} /* namespace raytracer */
