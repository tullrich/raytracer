#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include <string>
#include "rtimage.h"

namespace raytracer {

using std::string;
using namespace raytracer::image;

#define DEFAULT_IMG_HEIGHT 100
#define DEFAULT_IMG_WIDTH 100
#define DEFAULT_OUT_PATH "traceimage.png"

/**
 * raytracer root
 */
class Raytracer 
{
public:
	Raytracer();
	~Raytracer() {};

	string& getOutpath() { return outpath; };
	void setOutpath(string &newpath) {  outpath = newpath; };

	void trace();

private:
    rtimage *img;
    int hx;
    int wx;
 	string outpath;
};

} /* namespace raytracer */

#endif /* _RAYTRACER_H_ */