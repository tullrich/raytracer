#include "texture.h"

namespace raytracer {

	
Texture::Texture(const std::string filepath) : img(NULL)
{
	this->filepath = filepath;
	this->img_width = 0;
	this->img_height = 0;
}

bool Texture::isLoaded() const
{
	return img != NULL;
}

bool Texture::load()
{
	image::rtimage *img = NULL;
	if(img = image::loadImage(filepath))
	{
		this->img_width = FreeImage_GetWidth(img);
		this->img_height = FreeImage_GetHeight(img);

		if(img_width > 0 && img_height > 0)
		{
			this->img = img;
			return true;
		}
	}

	std::cout << "Error: could not load Texture from file '" << filepath  << "'" << std::endl; 
	return false;
}

glm::vec3 Texture::lookUp(const TexCoord &uv) const
{
	CGUTILS_ASSERT(img);

	unsigned int pixel_x = uv.x * img_width;
	unsigned int pixel_y = uv.y * img_height;

	RGBQUAD quad;
	FreeImage_GetPixelColor(img, pixel_x, pixel_y, &quad);
	glm::vec3 color(quad.rgbRed / 255.0f, quad.rgbGreen / 255.0f, quad.rgbBlue / 255.0f);
	//std::cout << "texture lookup " << pixel_x << " " << pixel_y << " : " << color << std::endl; 
	return color;
}

} /* namespace raytracer */
