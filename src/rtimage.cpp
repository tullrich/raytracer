#include "rtimage.h"
#include <math.h>
#include <regex>

using namespace std;

namespace raytracer {

#define IMG_EXT_REGEX ".*\\.(.*)"
#define IMG_BMP_EXT "\\.bmp"
#define IMG_JPEG_EXT "\\.jpg"
#define IMG_PNG_EXT "\\.png"
	 //(\\.jpeg)

Image::Image(int width, int height)
{
	this->width  = width;
	this->height = height;
	pixels       = new RGB[width*height]();
}

Image::~Image()
{
	delete pixels;
}

RGB* Image::getRGBForPixel(int u, int v)
{
	if(u < width && u >= 0 && v < height && v >= 0 )
		return &(pixels[v*height + u]);

	return NULL;
}

void Image::setPixelColor(int u, int v, const RGB &color)
{
	RGB *pixel = getRGBForPixel(u, v);

	if(pixel)
	{

		pixel->r = clamp(color.r, 0.0f, 1.0f);
		pixel->g = clamp(color.g, 0.0f, 1.0f);
		pixel->b = clamp(color.b, 0.0f, 1.0f);
	}
	else
	{
		std::cout << "Warning: bad (u,v) in Image::setPixelColor()" << std::cout;
	}
}

bool Image::writeOut(const std::string &filename) const
{
	image::initImageSystem();

    image::rtimage *img = image::allocateImage(width, height);

	for(int v = 0; v < height; v++)
	{
		for(int u = 0; u < width; u++)
		{
			RGBQUAD value;
			value.rgbRed = 255 * pixels[v*height + u].r;
			value.rgbGreen = 255 * pixels[v*height + u].g;
			value.rgbBlue = 255 * pixels[v*height + u].b;
			value.rgbReserved = 255; // always full alpha
			if (!FreeImage_SetPixelColor(img, u, v, &value))
			{
				std::cout << "Freeimage SetPixelColor() failed" << std::endl;
			}
		}
	}

	// FreeImage_Unload(img);

	return image::saveImage(img,  filename.c_str());
}

namespace image {


bool FreeImage_Initialised = false;

void initImageSystem() 
{	
	if(!FreeImage_Initialised)
	{
		FreeImage_Initialise(TRUE);
		FreeImage_Initialised = true;
	}
}

rtimage* allocateImage(int wx, int hx) 
{
	return FreeImage_Allocate(wx,hx,32);
}

bool saveImage(rtimage *img, const char *filename)
{
	return FreeImage_Save(FIF_PNG, img, filename, 0);
}

FREE_IMAGE_FORMAT getFreeimageFormat(const std::string &filepath)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(filepath.c_str(), 0);
	if(fif == FIF_UNKNOWN) 
	{
		fif = FreeImage_GetFIFFromFilename(filepath.c_str());
	}
	if(!FreeImage_FIFSupportsReading(fif)) 
	{
		return FIF_UNKNOWN;
	}

	return fif;
}


rtimage* loadImage(const std::string &filepath)
{
	image::initImageSystem();

	FREE_IMAGE_FORMAT format = getFreeimageFormat(filepath);
	if(format != FIF_UNKNOWN)
	{
		rtimage* img = NULL;
		if(img  = FreeImage_Load(format, filepath.c_str(), 0))
		{
			return img;
		}
	}

	return NULL;
}

} /* namespace image */
} /* namespace raytracer */
