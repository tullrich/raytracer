#ifndef _RT_IMAGE_
#define _RT_IMAGE_ 

#include "FreeImage.h"

namespace raytracer {

/**
 * functions isolating all FreeImage interaction
 */
namespace image {


typedef FIBITMAP rtimage;

void initImage();
rtimage* allocateImage(int wx, int hx);
bool saveImage(rtimage *img, const char *filename);


} /* namespace image */
} /* namespace raytracer */
#endif /*_RT_IMAGE_*/
