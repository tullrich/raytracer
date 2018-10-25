#ifndef _RT_IMAGE_
#define _RT_IMAGE_ 

#include "common.h"
#include "FreeImage.h"

namespace raytracer {

/**
 * Maintains the 2D array of RGB values and handles image
 * writing after raytracing
 */
class Image
{
public:
	Image(int width, int height);
	~Image();

	RGB* getRGBForPixel(int u, int v);
	void setPixelColor(int u, int v, const RGB &color);
	bool writeOut(const std::string &filename) const;


	int width;
	int height;
private:
	RGB *pixels;
};

/**
 * functions isolating all FreeImage interaction
 */
namespace image {


typedef FIBITMAP rtimage;

extern bool FreeImage_Initialised;

void initImageSystem();
rtimage* allocateImage(int wx, int hx);
bool saveImage(rtimage *img, const char *filename);

/**
 * get the FreeImageFormat for the extension
 * @param  filepath file path to determine the FREE_IMAGE_FORMAT for
 * @return     true if this is a supported extension, false otherwise
 */
FREE_IMAGE_FORMAT getFreeimageFormat(const std::string &filepath);

/**
 * Load an image from disk using the image subsystem
 * @param  filepath path to the image (absolute or relative)
 * @return          NULL if the image could not loaded, the fully loaded image otherwise
 */
rtimage* loadImage(const std::string &filepath);


} /* namespace image */
} /* namespace raytracer */
#endif /*_RT_IMAGE_*/
