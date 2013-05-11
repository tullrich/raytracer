#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include <string>
#include "rtimage.h"

// forward declaration so we dont need to include boost/program_options.hpp
class boost::program_options::variables_map;
typedef boost::program_options::variables_map options_map;

namespace raytracer {

using std::string;
using namespace raytracer::image;

/* Default input values */
#define DEFAULT_IMG_WIDTH 100	// default output image height value
#define DEFAULT_IMG_HEIGHT 100	// default output image height value
#define DEFAULT_OUT_PATH "traceimage.png"	// default output image filepath

#define CONFIG_FILEPATH "raytracer.cfg" // input configuration filepath

/**
 * raytracer root
 */
class Raytracer 
{
public:
	Raytracer(const string &outputfile, int height, int width);
	~Raytracer() {};

	string& getOutpath() { return outpath; };
	void setOutpath(const string &newpath) {  outpath = newpath; };

	void trace();
	void updateOptions(options_map &om);

private:
    rtimage *img;
 	string outpath;
};

} /* namespace raytracer */

#endif /* _RAYTRACER_H_ */