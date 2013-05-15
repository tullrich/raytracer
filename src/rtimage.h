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


} /* namespace image */
} /* namespace raytracer */
#endif /*_RT_IMAGE_*/
