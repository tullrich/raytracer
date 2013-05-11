#include "rtimage.h"

namespace raytracer {
namespace image {

void initImage() 
{
	FreeImage_Initialise(TRUE);
}

rtimage* allocateImage(int wx, int hx) 
{
	return FreeImage_Allocate(wx,hx,3);
}

bool saveImage(rtimage *img, const char *filename)
{
	return FreeImage_Save(FIF_JPEG, img, filename, 0);
}

} /* namespace image */
} /* namespace raytracer */
